#include "photoeditorwindow.h"
#include "constants.h"

#include <QHBoxLayout>
#include <QFrame>
#include <QApplication>
#include <QDesktopWidget>
#include <QValidator>
#include <QRegExp>
#include <QPainter>
#include <QSignalBlocker>

PhotoEditorWindow::PhotoEditorWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_scaleFactor = logicalDpiX() / Constants::LOGICAL_DPI_REF_VALUE;

    init();
}

PhotoEditorWindow::~PhotoEditorWindow()
{
}

void PhotoEditorWindow::init()
{
    QFont appFont = font();
    appFont.setFamily(Constants::APP_FONT_FAMILY);
    appFont.setPixelSize(qRound(Constants::APP_FONT_SIZE_PX * m_scaleFactor));
    appFont.setWeight(Constants::APP_FONT_WEIGHT);
    setFont(appFont);

    QPalette palette = QApplication::palette();
    m_defaultSystemPalette = palette;
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    QApplication::setPalette(palette);

    createWidgets();
    createLayout();
    createConnections();

    setCentralWidget(m_centralWidget);

    setWindowIcon(QIcon(":/resources/svg/pe"));
    setWindowTitle(tr("Photo Editor 1.0"));
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}
void PhotoEditorWindow::createWidgets()
{
    const int delimiterLineThickness = qRound(Constants::DELIMITER_LINE_THICKNESS_PX * m_scaleFactor);
    const QString toolBarStyleSheet = QString("QToolBar { background-color: %1; border-top: %2px solid %3; border-bottom: %2px solid %3; }")
            .arg(Constants::TOOL_BAR_COLOR).arg(delimiterLineThickness).arg(Constants::DELIMITER_LINE_COLOR),
            titleToolBarStyleSheet = QString("QToolBar { background-color: %1; }").arg(Constants::TOOL_BAR_COLOR);

    const QString sToolButtonStyleSheet = toolButtonStyleSheet();

    // --------------------------------------------------------------------------
    // Title toolbar

    m_titleToolBar = new QToolBar(m_centralWidget);
    m_titleToolBar->setMovable(false);
    m_titleToolBar->setFixedHeight(qRound(Constants::TITLE_BAR_HEIGHT_PX * m_scaleFactor));
    m_titleToolBar->setStyleSheet(titleToolBarStyleSheet);

    const QString sTitleIconToolButtonStyleSheet = titleIconToolButtonStyleSheet(),
            sTitleToolButtonStyleSheet = titleToolButtonStyleSheet(),
            sSystemToolButtonStyleSheet = systemToolButtonStyleSheet(),
            sCloseSystemToolButtonStyleSheet = closeSystemToolButtonStyleSheet(),
            sTitleLabelStyleSheet = "QLabel { color: white; }";

    m_titleIconButton = new QToolButton(m_titleToolBar);
    m_titleIconButton->setIcon(QIcon(":/resources/svg/pe"));
    m_titleIconButton->setStyleSheet(sTitleIconToolButtonStyleSheet);

    m_titleLabel = new QLabel(tr("Photo Editor 1.0"), m_titleToolBar);
    m_titleLabel->setStyleSheet(sTitleLabelStyleSheet);

    m_settingsButton = new QToolButton(m_titleToolBar);
    m_settingsButton->setIcon(QIcon(":/resources/svg/settings"));
    m_settingsButton->setStyleSheet(sTitleToolButtonStyleSheet);

    m_helpButton = new QToolButton(m_titleToolBar);
    m_helpButton->setIcon(QIcon(":/resources/svg/help"));
    m_helpButton->setStyleSheet(sTitleToolButtonStyleSheet);

    m_minimizeButton = new QToolButton(m_titleToolBar);
    m_minimizeButton->setIcon(QIcon(":/resources/svg/minimize"));
    m_minimizeButton->setStyleSheet(sSystemToolButtonStyleSheet);
    m_minimizeButton->setToolTip(tr("Minimize"));

    m_maximizeButton = new QToolButton(m_titleToolBar);
    m_maximizeButton->setIcon(QIcon(":/resources/svg/maximize"));
    m_maximizeButton->setStyleSheet(sSystemToolButtonStyleSheet);
    m_maximizeButton->setToolTip(tr("Maximize"));

    m_closeButton = new QToolButton(m_titleToolBar);
    m_closeButton->setIcon(QIcon(":/resources/svg/close"));
    m_closeButton->setStyleSheet(sCloseSystemToolButtonStyleSheet);
    m_closeButton->setToolTip(tr("Close"));

    QWidget* titleSpacer = new QWidget(m_titleToolBar);
    titleSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_titleToolBar->addWidget(m_titleIconButton);
    m_titleToolBar->addWidget(m_titleLabel);
    m_titleToolBar->addWidget(titleSpacer);
    m_titleToolBar->addWidget(m_settingsButton);
    m_titleToolBar->addWidget(m_helpButton);
    m_titleToolBar->addWidget(m_minimizeButton);
    m_titleToolBar->addWidget(m_maximizeButton);
    m_titleToolBar->addWidget(m_closeButton);

    m_centralWidget = new QWidget(this);
    m_centralWidget->setStyleSheet(QString("QWidget { background-color: %1; }").arg(Constants::APP_BACKGROUND_COLOR));

    // --------------------------------------------------------------------------
    // Header toolbar

    m_headerToolBar = new QToolBar(m_centralWidget);
    m_headerToolBar->setMovable(false);
    m_headerToolBar->setFixedHeight(qRound(Constants::HEADER_TOOL_BAR_HEIGHT_PX * m_scaleFactor));
    m_headerToolBar->setStyleSheet(toolBarStyleSheet);

    m_openFileAction = new QAction(tr("Open file"), m_headerToolBar);
    m_openFileAction->setShortcuts(QKeySequence::Open);
    m_saveFileAction = new QAction(tr("Save"), m_headerToolBar);
    m_saveFileAction->setShortcuts(QKeySequence::Save);
    m_saveAsFileAction = new QAction(tr("Save as..."), m_headerToolBar);
    m_saveAsFileAction->setShortcuts(QKeySequence::SaveAs);
    m_printAction = new QAction(tr("Print"), m_headerToolBar);
    m_printAction->setShortcuts(QKeySequence::Print);

    m_fileMenu = new QMenu(tr("File"), m_headerToolBar);
    m_fileMenu->addAction(m_openFileAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveFileAction);
    m_fileMenu->addAction(m_saveAsFileAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printAction);

    m_menuBar = new QMenuBar(m_headerToolBar);
    m_menuBar->addMenu(m_fileMenu);

    m_undoButton = new QToolButton(m_headerToolBar);
    m_undoButton->setIcon(QIcon(":/resources/svg/undo"));
    m_undoButton->setStyleSheet(sToolButtonStyleSheet);

    m_redoButton = new QToolButton(m_headerToolBar);
    m_redoButton->setIcon(QIcon(":/resources/svg/redo"));
    m_redoButton->setStyleSheet(sToolButtonStyleSheet);

    m_resetButton = new QToolButton(m_headerToolBar);
    m_resetButton->setIcon(QIcon(":/resources/svg/reset"));
    m_resetButton->setStyleSheet(sToolButtonStyleSheet);

    m_copyButton = new QPushButton(tr("Copy"), m_headerToolBar);
    m_copyButton->setStyleSheet(pushButtonStyleSheet(":/resources/svg/copy-rest", ":/resources/svg/copy-hover",
                                                     ":/resources/svg/copy-pressed", ":/resources/svg/copy-disabled"));

    QWidget* headerSpacer = new QWidget(m_headerToolBar);
    headerSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    auto menuBarVBoxLayout = new QVBoxLayout;
    menuBarVBoxLayout->setMenuBar(m_menuBar);
    auto menuBarWidget = new QWidget(m_headerToolBar);
    menuBarWidget->setLayout(menuBarVBoxLayout);
    m_headerToolBar->addWidget(menuBarWidget);
    m_headerToolBar->addWidget(m_undoButton);
    m_headerToolBar->addWidget(m_redoButton);
    m_headerToolBar->addWidget(m_resetButton);
    m_headerToolBar->addWidget(headerSpacer);
    m_headerToolBar->addWidget(m_copyButton);

    // --------------------------------------------------------------------------
    // Draw Tools toolbar

    QString drawToolsPanelStyleSheet = QString("QToolBar { background-color: %1; border-left: %2px solid %3; }")
            .arg(Constants::TOOL_BAR_COLOR).arg(delimiterLineThickness).arg(Constants::DELIMITER_LINE_COLOR);
    m_drawToolsPanel = new QWidget(this);
    m_drawToolsPanel->setFixedHeight(qRound(Constants::DRAW_TOOLS_PANEL_HEIGHT_PX * m_scaleFactor));
    m_drawToolsPanel->setStyleSheet(drawToolsPanelStyleSheet);

    m_drawToolsLabel = new QLabel(tr("Draw Tools"), m_drawToolsPanel);
    auto drawToolsLabelFont = font();
    drawToolsLabelFont.setPixelSize(qRound(Constants::HEADER_FONT_SIZE_PX * m_scaleFactor));
    drawToolsLabelFont.setWeight(Constants::HEADER_FONT_WEIGHT);
    m_drawToolsLabel->setFont(drawToolsLabelFont);

    m_drawToolsButtonGroup = new QButtonGroup(m_drawToolsPanel);
    m_drawToolsButtonGroup->setExclusive(true);

    m_drawToolsBar = new QToolBar(m_drawToolsPanel);

    m_pencilDrawToolButton = new QToolButton();
    m_pencilDrawToolButton->setCheckable(true);
    m_pencilDrawToolButton->setChecked(true);
    m_pencilDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/pencil", ":/resources/svg/pencil-checked"));

    m_arrowDrawToolButton = new QToolButton();
    m_arrowDrawToolButton->setCheckable(true);
    m_arrowDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/arrow", ":/resources/svg/arrow-checked"));

    m_boxDrawToolButton = new QToolButton();
    m_boxDrawToolButton->setCheckable(true);
    m_boxDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/box", ":/resources/svg/box-checked"));

    m_ellipseDrawToolButton = new QToolButton();
    m_ellipseDrawToolButton->setCheckable(true);
    m_ellipseDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/ellipse", ":/resources/svg/ellipse-checked"));

    m_triangleDrawToolButton = new QToolButton();
    m_triangleDrawToolButton->setCheckable(true);
    m_triangleDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/triangle", ":/resources/svg/triangle-checked"));

    m_starDrawToolButton = new QToolButton(m_drawToolsPanel);
    m_starDrawToolButton->setCheckable(true);
    m_starDrawToolButton->setStyleSheet(checkableDrawToolButtonStyleSheet(":/resources/svg/star", ":/resources/svg/star-checked"));

    m_drawToolsButtonGroup->addButton(m_pencilDrawToolButton, PencilDrawTool);
    m_drawToolsButtonGroup->addButton(m_arrowDrawToolButton, ArrowDrawTool);
    m_drawToolsButtonGroup->addButton(m_boxDrawToolButton, BoxDrawTool);
    m_drawToolsButtonGroup->addButton(m_ellipseDrawToolButton, EllipseDrawTool);
    m_drawToolsButtonGroup->addButton(m_triangleDrawToolButton, TriangleDrawTool);
    m_drawToolsButtonGroup->addButton(m_starDrawToolButton, StarDrawTool);

    auto drawToolsBarSpacerRight = new QWidget(m_drawToolsBar);
    drawToolsBarSpacerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_drawToolsBar->addWidget(m_pencilDrawToolButton);
    m_drawToolsBar->addWidget(m_arrowDrawToolButton);
    m_drawToolsBar->addWidget(m_boxDrawToolButton);
    m_drawToolsBar->addWidget(m_ellipseDrawToolButton);
    m_drawToolsBar->addWidget(m_triangleDrawToolButton);
    m_drawToolsBar->addWidget(m_starDrawToolButton);
    m_drawToolsBar->addWidget(drawToolsBarSpacerRight);

    // --------------------------------------------------------------------------
    // Draw Tools Settings toolbar

    const QString sOpacityLineEditStyleSheet = opacityLineEditStyleSheet(),
            sOpacitySliderStyleSheet = opacitySliderStyleSheet(),
            sRoundToolButtonStyleSheet = roundToolButtonStyleSheet(),
            sRoundComboboxStyleSheet = roundComboboxStyleSheet();

    m_drawToolsSettingsPanel = new QWidget(m_centralWidget);

    m_opacityLabel = new QLabel(tr("Opacity image"), m_drawToolsSettingsPanel);

    m_opacitySlider = new QSlider(Qt::Horizontal, m_drawToolsSettingsPanel);
    m_opacitySlider->setRange(0, Constants::SLIDER_MAX_VALUE);
    m_opacitySlider->setValue(Constants::SLIDER_MAX_VALUE);
    m_opacitySlider->setStyleSheet(sOpacitySliderStyleSheet);

    m_opacityLineEdit = new QLineEdit(m_drawToolsSettingsPanel);
    m_opacityLineEdit->setFixedWidth(qRound(Constants::OPACITY_LINE_EDIT_WIDTH_PX * m_scaleFactor));
    m_opacityLineEdit->setStyleSheet(sOpacityLineEditStyleSheet);
    QRegExp rx("^([1-9][0-9]{0,1}|100)$");
    auto* opacityVaidator = new QRegExpValidator(rx, m_opacityLineEdit);
    m_opacityLineEdit->setValidator(opacityVaidator);
    m_opacityLineEdit->setText(QString::number(Constants::SLIDER_MAX_VALUE));

    m_outlineColorLabel = new QLabel(tr("Outline color"), m_drawToolsSettingsPanel);

    m_pipetteToolButton = new QToolButton(m_drawToolsSettingsPanel);
    m_pipetteToolButton->setIcon(QIcon(":/resources/svg/pipette"));
    m_pipetteToolButton->setStyleSheet(sRoundToolButtonStyleSheet);

    m_colorDialog = new QColorDialog(this);
    auto colorDialogPalette = m_defaultSystemPalette;
    colorDialogPalette.setColor(QPalette::WindowText, Qt::black);
    colorDialogPalette.setColor(QPalette::Text, Qt::black);
    m_colorDialog->setPalette(colorDialogPalette);

    m_colorCombobox = new QComboBox(m_drawToolsSettingsPanel);
    m_colorCombobox->setStyleSheet(sRoundComboboxStyleSheet);

    // --------------------------------------------------------------------------
    // Footer toolbar

    m_footerToolBar = new QToolBar(m_centralWidget);
    m_footerToolBar->setMovable(false);
    m_footerToolBar->setFixedHeight(qRound(Constants::HEADER_TOOL_BAR_HEIGHT_PX * m_scaleFactor));
    m_footerToolBar->setStyleSheet(toolBarStyleSheet);

    // --------------------------------------------------------------------------
    // Photo zone

    m_photoScene = new QGraphicsScene(this);
    m_photoView = new QGraphicsView(m_photoScene);
    m_photoView->setStyleSheet(QString("QGraphicsView { margin: 50px; }"));
    m_photoView->show();
}

void PhotoEditorWindow::createLayout()
{
    m_mainLayout = new QVBoxLayout(m_centralWidget);

    auto createHorizontalLine = [&](QWidget* parent){
        auto horizontalLine = new QFrame(parent);
        horizontalLine->setFrameStyle(QFrame::HLine | QFrame::Plain);
        horizontalLine->setLineWidth(qRound(Constants::DELIMITER_LINE_THICKNESS_PX * m_scaleFactor));
        horizontalLine->setStyleSheet(QString("QFrame { color: %1; } ").arg(Constants::DELIMITER_LINE_COLOR));
        return horizontalLine;
    };

    auto horizontalLineDrawTools = createHorizontalLine(m_drawToolsPanel);
    auto drawToolsVBoxLayout = new QVBoxLayout;
    drawToolsVBoxLayout->addWidget(m_drawToolsLabel);
    drawToolsVBoxLayout->addWidget(m_drawToolsBar);
    const int drawToolsPanelMarginSide = qRound(Constants::DRAW_TOOLS_PANEL_MARGIN_SIDE_PX * m_scaleFactor),
            drawToolsPanelMarginTop = qRound(Constants::DRAW_TOOLS_PANEL_MARGIN_TOP_PX * m_scaleFactor);
    drawToolsVBoxLayout->setContentsMargins(drawToolsPanelMarginSide, drawToolsPanelMarginTop, drawToolsPanelMarginSide, drawToolsPanelMarginTop);
    m_drawToolsPanel->setLayout(drawToolsVBoxLayout);

    auto horizontalLineDrawToolsSettings = createHorizontalLine(m_drawToolsSettingsPanel);
    auto opacityHBoxLayout = new QHBoxLayout;
    opacityHBoxLayout->addWidget(m_opacitySlider);
    opacityHBoxLayout->addWidget(m_opacityLineEdit);

    auto opacityVBoxLayout = new QVBoxLayout;
    opacityVBoxLayout->addWidget(m_opacityLabel);
    opacityVBoxLayout->addLayout(opacityHBoxLayout);

    auto outlineHBoxLayout = new QHBoxLayout;
    outlineHBoxLayout->addWidget(m_outlineColorLabel);
    outlineHBoxLayout->addStretch(1);
    outlineHBoxLayout->addWidget(m_pipetteToolButton);
    outlineHBoxLayout->addWidget(m_colorCombobox);

    auto drawToolsSettingsVBoxLayout = new QVBoxLayout;
    drawToolsSettingsVBoxLayout->addLayout(opacityVBoxLayout);
    drawToolsSettingsVBoxLayout->addLayout(outlineHBoxLayout);
    drawToolsSettingsVBoxLayout->addWidget(horizontalLineDrawToolsSettings);

    m_drawToolsSettingsPanel->setLayout(drawToolsSettingsVBoxLayout);

    m_mainLayout->addWidget(m_titleToolBar);
    m_mainLayout->addWidget(m_headerToolBar);
    m_mainLayout->addWidget(m_drawToolsPanel);
    m_mainLayout->addWidget(horizontalLineDrawTools);
    m_mainLayout->addWidget(m_drawToolsSettingsPanel);
    m_mainLayout->addWidget(m_footerToolBar);

    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    m_centralWidget->setLayout(m_mainLayout);
}

void PhotoEditorWindow::createConnections()
{
    connect(m_minimizeButton, &QToolButton::clicked, [&]() {
        showMinimized();
    });
    connect(m_maximizeButton, &QToolButton::clicked, [&]() {
        if (isMaximized()) {
            m_maximizeButton->setIcon(QIcon(":/resources/svg/maximize"));
            m_maximizeButton->setToolTip(tr("Maximize"));
            showNormal();
        } else {
            m_maximizeButton->setIcon(QIcon(":/resources/svg/restore-down"));
            m_maximizeButton->setToolTip(tr("Restore Down"));
            showMaximized();
        }
    });
    connect(m_closeButton, &QToolButton::clicked, [&]() {
        close();
    });
    connect(m_drawToolsButtonGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
        [=](QAbstractButton *button, bool checked){
        button->setChecked(checked);
    });
    connect(m_opacitySlider, &QSlider::valueChanged, [&](int value) {
        QSignalBlocker blocker(m_opacityLineEdit);
        m_opacityLineEdit->setText(QString::number(value));
    });
    connect(m_opacityLineEdit, &QLineEdit::textChanged, [&](const QString& value) {
        QSignalBlocker blocker(m_opacitySlider);
        m_opacitySlider->setValue(value.toInt());
    });
    connect(m_pipetteToolButton, &QToolButton::clicked, [&]() {
        m_colorDialog->show();
    });
    connect(m_colorDialog, &QColorDialog::colorSelected, [&](const QColor& color) {
        const int iconSize = m_colorCombobox->style()->pixelMetric(QStyle::PM_LargeIconSize) * 2;
        QPixmap pixmap(iconSize, iconSize);
        pixmap.fill(QColor(255, 0, 0, 0));
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QBrush brush(color);
        painter.setBrush(brush);
        painter.drawEllipse(pixmap.rect());
        QIcon icon(pixmap);
        const int itemsCount = m_colorCombobox->count();
        m_colorCombobox->addItem(icon, "");
        m_colorCombobox->setItemData(itemsCount, Qt::AlignCenter, Qt::TextAlignmentRole);
        m_colorCombobox->setCurrentIndex(itemsCount);
    });
}

QString PhotoEditorWindow::titleIconToolButtonStyleSheet()
{
    const int toolButtonSize = qRound(Constants::TITLE_TOOL_BUTTON_SIZE_PX * m_scaleFactor),
            toolButtonMarginX = qRound(Constants::TITLE_TOOL_BUTTON_MARGIN_X_PX * m_scaleFactor);

    QString toolButtonStyleSheet = QString("QToolButton { width: %1px; height: %1px; background-color: transparent; margin: 0 %2px; border: none; }")
            .arg(toolButtonSize).arg(toolButtonMarginX);
    return toolButtonStyleSheet;
}

QString PhotoEditorWindow::titleToolButtonStyleSheet()
{
    const int toolButtonSize = qRound(Constants::TITLE_TOOL_BUTTON_SIZE_PX * m_scaleFactor),
            toolButtonMarginX = qRound(Constants::TITLE_TOOL_BUTTON_MARGIN_X_PX * m_scaleFactor);

    QString toolButtonStyleSheet = QString("QToolButton { width: %1px; height: %1px; background-color: %2; margin: 0 %3px; border: 1px solid %2; border-radius: 0px; }")
            .arg(toolButtonSize).arg(Constants::TOOL_BAR_COLOR).arg(toolButtonMarginX);
    toolButtonStyleSheet.append(QString("QToolButton:hover { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_HOVER_COLOR));
    toolButtonStyleSheet.append(QString("QToolButton:pressed { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_PRESSED_COLOR));
    toolButtonStyleSheet.append(QString("QToolButton:disabled { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_DISABLED_COLOR));
    return toolButtonStyleSheet;
}

QString PhotoEditorWindow::systemToolButtonStyleSheet()
{
    const int toolButtonPadding = qRound(Constants::TITLE_TOOL_BUTTON_PADDING_PX * m_scaleFactor);

    QString systemToolButtonStyleSheet = titleToolButtonStyleSheet();
    systemToolButtonStyleSheet.append(QString("QToolButton { padding-top: %1px; padding-bottom: %1px; }").arg(toolButtonPadding));
    return systemToolButtonStyleSheet;
}

QString PhotoEditorWindow::closeSystemToolButtonStyleSheet()
{
    QString closeSystemToolButtonStyleSheet = systemToolButtonStyleSheet();
    closeSystemToolButtonStyleSheet.append(QString("QToolButton:hover { background-color: %1; border-color: %1; }")
                                           .arg(Constants::CLOSE_SYSTEM_BUTTON_HOVER_COLOR));
    closeSystemToolButtonStyleSheet.append(QString("QToolButton:pressed { background-color: %1; border-color: %1; }")
                                           .arg(Constants::CLOSE_SYSTEM_BUTTON_PRESSED_COLOR));
    return closeSystemToolButtonStyleSheet;
}

QString PhotoEditorWindow::toolButtonStyleSheet()
{
    const int toolButtonSize = qRound(Constants::TOOL_BUTTON_SIZE_PX * m_scaleFactor),
            toolButtonMarginX = qRound(Constants::TOOL_BUTTON_MARGIN_X_PX * m_scaleFactor),
            toolButtonBorderRadius = qRound(Constants::TOOL_BUTTON_BORDER_RADIUS_PX * m_scaleFactor);

    QString toolButtonStyleSheet = QString("QToolButton { width: %1px; height: %1px; background-color: %2; margin: 0 %3px; border: 1px solid %2; border-radius: %4px; }")
            .arg(toolButtonSize).arg(Constants::TOOL_BUTTON_REST_COLOR).arg(toolButtonMarginX).arg(toolButtonBorderRadius);
    toolButtonStyleSheet.append(QString("QToolButton:hover { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_HOVER_COLOR));
    toolButtonStyleSheet.append(QString("QToolButton:pressed { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_PRESSED_COLOR));
    toolButtonStyleSheet.append(QString("QToolButton:disabled { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_DISABLED_COLOR));
    return toolButtonStyleSheet;
}

QString PhotoEditorWindow::pushButtonStyleSheet(const QString& normalIconUrl, const QString& hoverIconUrl,
                                                const QString& pressedIconUrl, const QString& disabledIconUrl)
{
    const int pushButtonHeight = qRound(Constants::PUSH_BUTTON_HEIGHT_PX * m_scaleFactor),
            pushButtonWidth = qRound(Constants::PUSH_BUTTON_WIDTH_PX * m_scaleFactor),
            pushButtonMarginX = qRound(Constants::PUSH_BUTTON_MARGIN_X_PX * m_scaleFactor),
            pushButtonBorder = qRound(Constants::PUSH_BUTTON_BORDER_PX * m_scaleFactor),
            pushButtonBorderRadius = qRound(Constants::PUSH_BUTTON_BORDER_RADIUS_PX * m_scaleFactor);

    QString pushButtonStyleSheet = QString("QPushButton { qproperty-icon: url(%1); width: %2px; height: %3px; background-color: transparent; color: %4; margin: 0 %5px; border: %6px solid %4; border-radius: %7px; }")
            .arg(normalIconUrl).arg(pushButtonWidth).arg(pushButtonHeight).arg(Constants::PUSH_BUTTON_REST_COLOR)
            .arg(pushButtonMarginX).arg(pushButtonBorder).arg(pushButtonBorderRadius);
    pushButtonStyleSheet.append(QString("QPushButton:hover { qproperty-icon: url(%1); border-color: %2; color: %2; }")
                                .arg(hoverIconUrl, Constants::PUSH_BUTTON_HOVER_COLOR));
    pushButtonStyleSheet.append(QString("QPushButton:pressed { qproperty-icon: url(%1); border-color: %2; color: %2; }")
                                .arg(pressedIconUrl, Constants::PUSH_BUTTON_PRESSED_COLOR));
    pushButtonStyleSheet.append(QString("QPushButton:disabled { qproperty-icon: url(%1); border-color: %2; color: %2; opacity: %3; }")
                                .arg(disabledIconUrl, Constants::PUSH_BUTTON_DISABLED_COLOR).arg(Constants::PUSH_BUTTON_DISABLED_OPACITY));
    return pushButtonStyleSheet;
}

QString PhotoEditorWindow::checkableDrawToolButtonStyleSheet(const QString& normalIconUrl, const QString& pressedIconUrl)
{
    const int toolButtonSize = qRound(Constants::TOOL_BUTTON_SIZE_PX * m_scaleFactor),
            toolButtonMarginX = qRound(Constants::TOOL_BUTTON_MARGIN_X_PX * m_scaleFactor),
            toolButtonBorderRadius = qRound(Constants::TOOL_BUTTON_BORDER_RADIUS_PX * m_scaleFactor);

    QString checkableDrawToolButtonStyleSheet = QString("QToolButton { qproperty-icon: url(%1); width: %2px; height: %2px; background-color: %3; margin: 0 %4px; border: 1px solid %3; border-radius: %5px; }")
            .arg(normalIconUrl).arg(toolButtonSize).arg(Constants::TOOL_BUTTON_REST_COLOR).arg(toolButtonMarginX).arg(toolButtonBorderRadius);
    checkableDrawToolButtonStyleSheet.append(QString("QToolButton:checked { qproperty-icon: url(%1); background-color: %2; border-color: %2; }")
                                             .arg(pressedIconUrl, Constants::DRAW_TOOL_BUTTON_PRESSED_COLOR));
    checkableDrawToolButtonStyleSheet.append(QString("QToolButton:hover { qproperty-icon: url(%1); background-color: %2; border-color: %2; }")
                                             .arg(pressedIconUrl, Constants::DRAW_TOOL_BUTTON_PRESSED_COLOR));
    return checkableDrawToolButtonStyleSheet;
}

QString PhotoEditorWindow::opacityLineEditStyleSheet()
{
    const int opacityLineEditBorder = qRound(Constants::OPACITY_LINE_EDIT_BORDER_PX * m_scaleFactor),
            opacityLineEditBorderRadius = qRound(Constants::OPACITY_LINE_EDIT_BORDER_RADIUS_PX * m_scaleFactor);

    QString opacityLineEditStyleSheet = QString("QLineEdit { border: %1px solid %2; border-radius: %3; }")
            .arg(opacityLineEditBorder).arg(Constants::OPACITY_LINE_EDIT_BORDER_COLOR).arg(opacityLineEditBorderRadius);
    return opacityLineEditStyleSheet;
}

QString PhotoEditorWindow::opacitySliderStyleSheet()
{
    const int opacitySliderGrooveHeight = qRound(Constants::OPACITY_SLIDER_GROOVE_HEIGHT_PX * m_scaleFactor),
            opacitySliderGrooveBorderRadius = qRound(Constants::OPACITY_SLIDER_GROOVE_BORDER_RADIUS_PX * m_scaleFactor),
            opacitySliderHandleBorderRadius = qRound(Constants::OPACITY_SLIDER_HANDLE_BORDER_RADIUS_PX * m_scaleFactor),
            opacitySliderHandleWidth = qRound(Constants::OPACITY_SLIDER_HANDLE_WIDTH_PX * m_scaleFactor),
            opacitySliderHandleHeight = qRound(Constants::OPACITY_SLIDER_HANDLE_HEIGHT_PX * m_scaleFactor),
            opacitySliderHandleBorder = qRound(Constants::OPACITY_SLIDER_HANDLE_BORDER_PX * m_scaleFactor),
            opacitySliderHandleMargin = -qRound(opacitySliderHandleBorderRadius * 0.5);

    QString opacitySliderStyleSheet = QString("QSlider::groove:horizontal { background-color: %1; height: %2px; border-radius: %3px; }")
            .arg(Constants::OPACITY_SLIDER_GROOVE_COLOR).arg(opacitySliderGrooveHeight).arg(opacitySliderGrooveBorderRadius);
    opacitySliderStyleSheet.append(QString("QSlider::handle:horizontal { background-color: %1; border: %2px solid %1; width: %3px; height: %4px; line-height: %4px; margin-top: %5px; margin-bottom: %5px; border-radius: %6px; }")
            .arg(Constants::OPACITY_SLIDER_HANDLE_COLOR).arg(opacitySliderHandleBorder).arg(opacitySliderHandleWidth).arg(opacitySliderHandleHeight)
            .arg(opacitySliderHandleMargin).arg(opacitySliderHandleBorderRadius));
    opacitySliderStyleSheet.append(QString("QSlider::handle:horizontal:hover { border-radius: %1px; }").arg(opacitySliderHandleBorderRadius));
    return opacitySliderStyleSheet;
}

QString PhotoEditorWindow::roundToolButtonStyleSheet()
{
    const int roundToolButtonBorderRadius = qRound(Constants::ROUND_TOOL_BUTTON_BORDER_RADIUS_PX * m_scaleFactor);

    QString roundToolButtonStyleSheet = toolButtonStyleSheet();
    roundToolButtonStyleSheet.append(QString("QToolButton { border-radius: %1px;}").arg(roundToolButtonBorderRadius));
    return roundToolButtonStyleSheet;
}

QString PhotoEditorWindow::roundComboboxStyleSheet()
{
    const int toolButtonSize = qRound(Constants::TOOL_BUTTON_SIZE_PX * m_scaleFactor),
            toolButtonBorderRadius = qRound(Constants::ROUND_TOOL_BUTTON_BORDER_RADIUS_PX * m_scaleFactor),
            roundComboboxDownArrowSize = qRound(Constants::ROUND_COMBO_BOX_DOWN_ARROW_SIZE * m_scaleFactor);

    QString roundComboboxStyleSheet = QString("QComboBox { width: %1px; height: %1px; background-color: %2; border: 1px solid %2; border-radius: %3px; }")
            .arg(toolButtonSize).arg(Constants::TOOL_BUTTON_REST_COLOR).arg(toolButtonBorderRadius);
    roundComboboxStyleSheet.append(QString("QComboBox:hover { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_HOVER_COLOR));
    roundComboboxStyleSheet.append(QString("QComboBox:pressed { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_PRESSED_COLOR));
    roundComboboxStyleSheet.append(QString("QComboBox:disabled { background-color: %1; border-color: %1; }")
                                .arg(Constants::TOOL_BUTTON_DISABLED_COLOR));
    roundComboboxStyleSheet.append(QString("QComboBox:down-arrow { image: url(%1); width: %2px; height: %2px; }")
                                .arg(":/resources/svg/down-arrow").arg(roundComboboxDownArrowSize));
    return roundComboboxStyleSheet;
}
