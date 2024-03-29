#include "FileDiffHighlighter.h"

#include <Colors.h>

#include <QSettings>
#include <QTextDocument>

FileDiffHighlighter::FileDiffHighlighter(QTextDocument *document)
   : QSyntaxHighlighter(document)
{
   const auto colorScheme = QSettings().value("colorSchema", 0).toInt();
   mShadowGreen = colorScheme == 0 ? editorGreenShadowDark : editorGreenShadowBright;
   mShadowRed = colorScheme == 0 ? editorRedShadowDark : editorRedShadowBright;
}

void FileDiffHighlighter::highlightBlock(const QString &text)
{
   setCurrentBlockState(previousBlockState() + 1);

   QTextBlockFormat myFormat;
   QTextCharFormat format;
   const auto currentLine = currentBlock().blockNumber() + 1;

   if (!mFileDiffInfo.isEmpty())
   {
      for (const auto &diff : std::as_const(mFileDiffInfo))
      {
         if (diff.startLine <= currentLine && currentLine <= diff.endLine)
         {
            if (diff.addition)
               myFormat.setBackground(mShadowGreen);
            else
               myFormat.setBackground(mShadowRed);
         }
      }
   }
   else if (!text.isEmpty())
   {
      switch (text.at(0).toLatin1())
      {
         case '@':
            myFormat.setBackground(graphOrange);
            format.setFontWeight(QFont::ExtraBold);
            break;
         case '+':
            myFormat.setBackground(mShadowGreen);
            break;
         case '-':
            myFormat.setBackground(mShadowRed);
            break;
         default:
            break;
      }
   }

   if (myFormat.isValid())
   {
      QTextCursor(currentBlock()).setBlockFormat(myFormat);
      setFormat(0, currentBlock().length(), format);
   }
}
