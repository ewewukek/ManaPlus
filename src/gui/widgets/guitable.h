/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_WIDGETS_GUITABLE_H
#define GUI_WIDGETS_GUITABLE_H

#include "localconsts.h"

#include "enums/simpletypes/opaque.h"

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"
#include "listeners/tablemodellistener.h"

#include "gui/widgets/widget.h"

class GuiTableActionListener;
class TableModel;

/**
 * A table, with rows and columns made out of sub-widgets. Largely inspired by
 * (and can be thought of as a generalisation of) the guichan listbox
 * implementation.
 *
 * Normally you want this within a ScrollArea.
 *
 * \ingroup GUI
 */
class GuiTable final : public Widget,
                       public MouseListener,
                       public KeyListener,
                       public TableModelListener
{
    // so that the action listener can call distributeActionEvent
    friend class GuiTableActionListener;

    public:
        GuiTable(const Widget2 *const widget,
                 TableModel *const initial_model,
                 const Opaque opacity);

        A_DELETE_COPY(GuiTable)

        ~GuiTable() override final;

        /**
         * Sets the table model
         *
         * Note that actions issued by widgets returned from the model will
         * update the table selection, but only AFTER any event handlers
         * installed within the widget have been triggered. To be notified
         * after such an update, add an action listener to the table instead.
         */
        void setModel(TableModel *const m);

        /**
         * Retrieves the active table model
         */
        const TableModel *getModel() const A_WARN_UNUSED RETURNS_NONNULL;

        void setSelected(const int row, const int column);

        int getSelectedRow() const A_WARN_UNUSED;

        int getSelectedColumn() const A_WARN_UNUSED;

        void setSelectedRow(const int selected);

        void setSelectedColumn(const int selected);

        bool isWrappingEnabled() const noexcept2 A_WARN_UNUSED
        { return mWrappingEnabled; }

        void setWrappingEnabled(bool wrappingEnabled)
        { mWrappingEnabled = wrappingEnabled; }

        Rect getChildrenArea() override final A_WARN_UNUSED;

        /**
         * Toggle whether to use linewise selection mode, in which the table
         * selects an entire line at a time, rather than a single cell.
         *
         * Note that column information is tracked even in linewise selection
         * mode;
         *
         * this mode therefore only affects visualisation.
         *
         * Disabled by default.
         *
         * \param linewise: Whether to enable linewise selection mode
         */
        void setLinewiseSelection(bool linewise)
        { mLinewiseMode = linewise; }

        // Inherited from Widget
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        Widget *getWidgetAt(int x, int y) override final A_WARN_UNUSED;

        void moveToTop(Widget *const widget) override final;

        void moveToBottom(Widget *const widget) override final;

        void setFocusHandler(FocusHandler *const focusHandler) override final;

        // Inherited from KeyListener
        void keyPressed(KeyEvent& event) override final;

        /**
         * Sets the table to be opaque, that is sets the table
         * to display its background.
         *
         * @param opaque True if the table should be opaque, false otherwise.
         */
        void setOpaque(Opaque opaque)
        { mOpaque = opaque; }

        /**
         * Checks if the table is opaque, that is if the table area displays
         * its background.
         *
         * @return True if the table is opaque, false otherwise.
         */
        bool isOpaque() const noexcept2 A_WARN_UNUSED
        { return mOpaque == Opaque_true; }

        // Inherited from MouseListener
        void mousePressed(MouseEvent& event) override final;

        void mouseWheelMovedUp(MouseEvent& event) override final;

        void mouseWheelMovedDown(MouseEvent& event) override final;

        void mouseDragged(MouseEvent& event) override final;

        // Constraints inherited from TableModelListener
        void modelUpdated(const bool completed) override final;

        void requestFocus() override;

        void setSelectableGui(bool b)
        { mSelectableGui = b; }

    protected:
        /** Frees all action listeners on inner widgets. */
        void uninstallActionListeners();

        /** Installs all action listeners on inner widgets. */
        void installActionListeners();

        int getRowHeight() const A_WARN_UNUSED;

        int getColumnWidth(const int i) const A_WARN_UNUSED;

    private:
        int getRowForY(const int y) const A_WARN_UNUSED;     // -1 on error

        int getColumnForX(const int x) const A_WARN_UNUSED;  // -1 on error

        void recomputeDimensions();

        static float mAlpha;

        TableModel *mModel A_NONNULLPOINTER;

        /** If someone moves a fresh widget to the top, we must display it. */
        Widget *mTopWidget;

        /** Vector for compactness; used as a list in practice. */
        STD_VECTOR<GuiTableActionListener *> mActionListeners2;

        /**
         * Holds the background color of the table.
         */
        Color mHighlightColor;

        int mSelectedRow;
        int mSelectedColumn;

        bool mLinewiseMode;
        bool mWrappingEnabled;
        Opaque mOpaque;
        bool mSelectableGui;
};

#endif  // GUI_WIDGETS_GUITABLE_H
