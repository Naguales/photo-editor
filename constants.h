#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QFont>

#include <QString>

namespace Constants {

    // --------------------------------------------------------------------------
    // Application

    // The horizontal resolution of the device in dots per inch corresponding to the 100% DPI setting.
    inline double LOGICAL_DPI_REF_VALUE { 96.0 };

    inline const QString APP_FONT_FAMILY { QStringLiteral("Roboto") };
    inline const int APP_FONT_SIZE_PX { 14 };
    inline const int APP_FONT_WEIGHT { QFont::Normal };
    inline const int HEADER_FONT_SIZE_PX { 16 };
    inline const int HEADER_FONT_WEIGHT { QFont::Bold };
    inline const QString APP_BACKGROUND_COLOR { QStringLiteral("#1D1D1E") };
    inline const QString DELIMITER_LINE_COLOR { QStringLiteral("#292A2C") };
    inline const int DELIMITER_LINE_THICKNESS_PX { 1 };

    // --------------------------------------------------------------------------
    // Title toolbar

    inline const int TITLE_BAR_HEIGHT_PX { 40 };
    inline const int TITLE_TOOL_BUTTON_SIZE_PX { 40 };
    inline const int TITLE_TOOL_BUTTON_MARGIN_X_PX { 1 };
    inline const int TITLE_TOOL_BUTTON_PADDING_PX { 10 };
    inline const QString CLOSE_SYSTEM_BUTTON_HOVER_COLOR { QStringLiteral("#E72525") };
    inline const QString CLOSE_SYSTEM_BUTTON_PRESSED_COLOR { QStringLiteral("#C75050") };

    // --------------------------------------------------------------------------
    // Header toolbar

    inline const int HEADER_TOOL_BAR_HEIGHT_PX { 64 };
    inline const int HEADER_TOOL_BAR_SIDE_MARGIN_PX { 20 };
    inline const int TOOL_BUTTON_SIZE_PX { 40 };
    inline const int TOOL_BUTTON_PADDING_PX { 8 };
    inline const int TOOL_BUTTON_MARGIN_X_PX { 4 };
    inline const int TOOL_BUTTON_BORDER_RADIUS_PX { 4 };
    inline const QString TOOL_BAR_COLOR { QStringLiteral("#1D1D1E") };
    inline const QString TOOL_BUTTON_REST_COLOR { QStringLiteral("#2A2B2C") };
    inline const QString TOOL_BUTTON_HOVER_COLOR { QStringLiteral("#3C3D3F") };
    inline const QString TOOL_BUTTON_PRESSED_COLOR { QStringLiteral("#585A5E") };
    inline const QString TOOL_BUTTON_DISABLED_COLOR { QStringLiteral("#2A2B2C") };

    inline const int PUSH_BUTTON_HEIGHT_PX { 40 };
    inline const int PUSH_BUTTON_WIDTH_PX { 120 };
    inline const int PUSH_BUTTON_ICON_SIZE_PX { 24 };
    inline const int PUSH_BUTTON_PADDING_PX { 8 };
    inline const int PUSH_BUTTON_MARGIN_X_PX { 4 };
    inline const double PUSH_BUTTON_BORDER_PX { 1.5 };
    inline const int PUSH_BUTTON_BORDER_RADIUS_PX { 4 };
    inline const int PUSH_BUTTON_DISABLED_OPACITY { 102 };
    inline const QString PUSH_BUTTON_REST_COLOR { QStringLiteral("#68AB25") };
    inline const QString PUSH_BUTTON_HOVER_COLOR { QStringLiteral("#84CC3C") };
    inline const QString PUSH_BUTTON_PRESSED_COLOR { QStringLiteral("#51861C") };
    inline const QString PUSH_BUTTON_DISABLED_COLOR { QStringLiteral("#68AB25") };

    // --------------------------------------------------------------------------
    // Draw Tools bar

    inline const int DRAW_TOOLS_SIDE_PANEL_WIDTH_PX { 400 };
    inline const QString DRAW_TOOL_BUTTON_PRESSED_COLOR { QStringLiteral("rgba(140, 228, 52, 38)") };
    inline const int DRAW_TOOLS_PANEL_HEIGHT_PX { 112 };
    inline const int DRAW_TOOLS_PANEL_MARGIN_SIDE_PX { 24 };
    inline const int DRAW_TOOLS_PANEL_MARGIN_TOP_PX { 16 };

    // --------------------------------------------------------------------------
    // Draw Tools Settings bar

    inline const int SLIDER_MAX_VALUE { 100 };
    inline const int OPACITY_LINE_EDIT_WIDTH_PX { 52 };
    inline const int OPACITY_LINE_EDIT_BORDER_PX { 1 };
    inline const int OPACITY_LINE_EDIT_BORDER_RADIUS_PX { 4 };
    inline const QString OPACITY_LINE_EDIT_BORDER_COLOR { QStringLiteral("#3C3D3F") };

    inline const int OPACITY_SLIDER_GROOVE_HEIGHT_PX { 6 };
    inline const int OPACITY_SLIDER_GROOVE_BORDER_RADIUS_PX { 2 };
    inline const int OPACITY_SLIDER_HANDLE_WIDTH_PX { 8 };
    inline const int OPACITY_SLIDER_HANDLE_HEIGHT_PX { 12 };
    inline const int OPACITY_SLIDER_HANDLE_MARGIN_PX { -3 };
    inline const int OPACITY_SLIDER_HANDLE_BORDER_PX { 2 };
    inline const int OPACITY_SLIDER_HANDLE_BORDER_RADIUS_PX { 6 };
    inline const QString OPACITY_SLIDER_GROOVE_COLOR { QStringLiteral("#585A5E") };
    inline const QString OPACITY_SLIDER_HANDLE_COLOR { QStringLiteral("#FFFFFF") };

    inline const int ROUND_TOOL_BUTTON_PADDING_PX { 0 };
    inline const int ROUND_TOOL_BUTTON_BORDER_RADIUS_PX { 16 };
    inline const int ROUND_TOOL_BUTTON_ICON_SIZE_PX { 24 };

    inline const int ROUND_COMBO_BOX_WIDTH_PX { 52 };
    inline const int ROUND_COMBO_BOX_HEIGHT_PX { 40 };
    inline const int ROUND_COMBO_BOX_ICON_SIZE_PX { 32 };
    inline const int ROUND_COMBO_BOX_DOWN_ARROW_WIDTH_PX { 14 };
    inline const int ROUND_COMBO_BOX_DOWN_ARROW_HEIGHT_PX { 9 };
    inline const int ROUND_COMBO_BOX_DOWN_ARROW_LEFT_SHIFT_PX { -10 };

    // --------------------------------------------------------------------------
    // Photo zone

    inline const int PHOTO_ZONE_MARGIN_PX { 50 };
    inline const QString PHOTO_ZONE_COLOR { QStringLiteral("#141415") };

}

#endif // CONSTANTS_H
