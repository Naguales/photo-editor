#ifndef PHOTOEDITORWINDOW_H
#define PHOTOEDITORWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QVBoxLayout>

class PhotoEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum DrawTools {
        PencilDrawTool = 0,
        ArrowDrawTool,
        BoxDrawTool,
        EllipseDrawTool,
        TriangleDrawTool,
        StarDrawTool
    };

    PhotoEditorWindow(QWidget *parent = nullptr);
    ~PhotoEditorWindow();

private:
    void init();
    void createWidgets();
    void createLayout();
    void createConnections();

    QString titleIconToolButtonStyleSheet();
    QString titleToolButtonStyleSheet();
    QString systemToolButtonStyleSheet();
    QString closeSystemToolButtonStyleSheet();
    QString toolButtonStyleSheet();
    QString pushButtonStyleSheet(const QString& normalIconUrl,
                                 const QString& hoverIconUrl = QString(),
                                 const QString& pressedIconUrl = QString(),
                                 const QString& disabledIconUrl = QString());
    QString checkableDrawToolButtonStyleSheet(const QString& normalIconUrl,
                                              const QString& pressedIconUrl = QString());

    // --------------------------------------------------------------------------
    // Title toolbar

    QToolBar* m_titleToolBar { nullptr };
    QLabel* m_titleLabel { nullptr };
    QToolButton* m_titleIconButton { nullptr };
    QToolButton* m_settingsButton { nullptr };
    QToolButton* m_helpButton { nullptr };
    QToolButton* m_minimizeButton { nullptr };
    QToolButton* m_maximizeButton { nullptr };
    QToolButton* m_closeButton { nullptr };

    // --------------------------------------------------------------------------
    // Header toolbar

    QToolBar* m_headerToolBar { nullptr };
    QToolButton* m_undoButton { nullptr };
    QToolButton* m_redoButton { nullptr };
    QToolButton* m_resetButton { nullptr };
    QPushButton* m_copyButton { nullptr };

    // --------------------------------------------------------------------------
    // Draw Tools toolbar

    QWidget* m_drawToolsPanel { nullptr };
    QLabel* m_drawToolsLabel { nullptr };
    QButtonGroup* m_drawToolsButtonGroup { nullptr };
    QToolBar* m_drawToolsBar { nullptr };
    QToolButton* m_pencilDrawToolButton { nullptr };
    QToolButton* m_arrowDrawToolButton { nullptr };
    QToolButton* m_boxDrawToolButton { nullptr };
    QToolButton* m_ellipseDrawToolButton { nullptr };
    QToolButton* m_triangleDrawToolButton { nullptr };
    QToolButton* m_starDrawToolButton { nullptr };

    // --------------------------------------------------------------------------
    // Photo Editor window

    QWidget* m_centralWidget { nullptr };
    QVBoxLayout* m_mainLayout { nullptr };
    double m_scaleFactor { 1.0 };
};

#endif // PHOTOEDITORWINDOW_H
