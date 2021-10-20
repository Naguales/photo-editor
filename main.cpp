#include "photoeditorwindow.h"
#include "constants.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Software change pixel font size, some controls font size doesn't change automatically on high DPI.
    QFont font = a.font();
    QFontMetrics fontMetrics = a.fontMetrics();
    font.setPixelSize(qRound(Constants::APP_FONT_SIZE_PX * fontMetrics.fontDpi() / Constants::LOGICAL_DPI_REF_VALUE));
    a.setFont(font);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PhotoEditor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    PhotoEditorWindow w;
    w.show();
    return a.exec();
}
