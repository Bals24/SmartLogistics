#pragma once

#include <QString>

namespace SmartLogisticsStyle
{
    inline QString mainStyle()
    {
        return R"(
            QMainWindow {
                background-color: #f5f7fb;
            }

            QWidget {
                font-family: "Segoe UI";
                font-size: 14px;
                color: #1e293b;
            }

            #sidebar {
                background-color: #111827;
            }

            #logo {
                color: white;
                font-size: 21px;
                font-weight: 700;
            }

            #sidebarSubtitle {
                color: #94a3b8;
                font-size: 12px;
            }

            #menuButton {
                background-color: transparent;
                color: #cbd5e1;
                border: none;
                border-radius: 8px;
                text-align: left;
                padding: 0 14px;
                font-size: 14px;
            }

            #menuButton:hover {
                background-color: #1e293b;
                color: white;
            }

            #menuButton[selected="true"] {
                background-color: #2563eb;
                color: white;
                font-weight: 600;
            }

            #menuButton[selected="true"]:hover {
                background-color: #1d4ed8;
            }

            #systemStatus {
                color: #4ade80;
                font-size: 12px;
                padding: 8px;
            }

            #topBar {
                background-color: white;
                border-bottom: 1px solid #e2e8f0;
            }

            #pageTitle {
                color: #0f172a;
                font-size: 24px;
                font-weight: 700;
            }

            #pageSubtitle {
                color: #64748b;
                font-size: 13px;
            }

            #userLabel {
                background-color: #f1f5f9;
                color: #475569;
                border-radius: 18px;
                padding: 9px 16px;
                font-weight: 600;
            }

            #sectionTitle {
                color: #0f172a;
                font-size: 18px;
                font-weight: 700;
            }

            #statCard {
                background-color: white;
                border: 1px solid #e2e8f0;
                border-radius: 12px;
                min-height: 125px;
            }

            #statCard:hover {
                border: 1px solid #cbd5e1;
            }

            #cardTitle {
                color: #64748b;
                font-size: 13px;
                font-weight: 600;
            }

            #cardValue {
                color: #0f172a;
                font-size: 28px;
                font-weight: 700;
            }

            #cardDescription {
                color: #94a3b8;
                font-size: 12px;
            }

            #dashboardTable {
                background-color: white;
                border: 1px solid #e2e8f0;
                border-radius: 10px;
                gridline-color: #e2e8f0;
            }

            #dashboardTable QHeaderView::section {
                background-color: #f8fafc;
                color: #475569;
                border: none;
                border-bottom: 1px solid #e2e8f0;
                padding: 12px;
                font-weight: 600;
            }

            #dashboardTable QTableWidgetItem {
                padding: 8px;
            }

            #placeholderTitle {
                color: #0f172a;
                font-size: 26px;
                font-weight: 700;
            }

            #placeholderText {
                color: #64748b;
                font-size: 14px;
            }

            QScrollBar:vertical {
                background: #f1f5f9;
                width: 10px;
                margin: 0;
            }

            QScrollBar::handle:vertical {
                background: #cbd5e1;
                border-radius: 5px;
                min-height: 30px;
            }

            QScrollBar::handle:vertical:hover {
                background: #94a3b8;
            }
        )";
    }
}