#ifndef ABSTRACTTABLEVIEW_H
#define ABSTRACTTABLEVIEW_H

#include <QScrollBar>
#include <QAbstractScrollArea>
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include "StringUtil.h"
#include "Configuration.h"
#include "MenuBuilder.h"
#include "QActionLambda.h"
#include "CachedFontMetrics.h"
#include "MiscUtil.h"

//Hacky class that fixes a really annoying cursor problem
class AbstractTableScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    AbstractTableScrollBar(QScrollBar* scrollbar)
    {
        setOrientation(scrollbar->orientation());
        setParent(scrollbar->parentWidget());
    }

    void enterEvent(QEvent* event)
    {
        Q_UNUSED(event);
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }

    void leaveEvent(QEvent* event)
    {
        Q_UNUSED(event);
        QApplication::restoreOverrideCursor();
    }
};

class AbstractTableView : public QAbstractScrollArea
{
    Q_OBJECT

public:
    enum GuiState_t {NoState, ReadyToResize, ResizeColumnState, HeaderButtonPressed, HeaderButtonReordering};

    // Constructor
    explicit AbstractTableView(QWidget* parent = 0);
    ~AbstractTableView();

    // Configuration
    virtual void Initialize();
    virtual void updateColors();
    virtual void updateFonts();
    virtual void updateShortcuts();

    // Pure Virtual Methods
    virtual QString paintContent(QPainter* painter, dsint rowBase, int rowOffset, int col, int x, int y, int w, int h) = 0;

    // Painting Stuff
    void paintEvent(QPaintEvent* event);

    // Mouse Management
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);

    // ScrollBar Management
    virtual dsint sliderMovedHook(int type, dsint value, dsint delta);
    int scaleFromUint64ToScrollBarRange(dsint value);
    dsint scaleFromScrollBarRangeToUint64(int value);
    void updateScrollBarRange(dsint range);


    // Coordinates Utils
    int getIndexOffsetFromY(int y);
    int getColumnIndexFromX(int x);
    int getColumnPosition(int index);
    int transY(int y);
    int getViewableRowsCount();
    virtual int getLineToPrintcount();

    // New Columns/New Size
    virtual void addColumnAt(int width, const QString & title, bool isClickable);
    virtual void setRowCount(dsint count);
    virtual void deleteAllColumns();
    void setColTitle(int index, const QString & title);
    QString getColTitle(int index);

    // Getter & Setter
    dsint getRowCount();
    int getColumnCount();
    int getRowHeight();
    int getColumnWidth(int index);
    void setColumnWidth(int index, int width);
    int getHeaderHeight();
    int getTableHeigth();
    int getGuiState();
    int getNbrOfLineToPrint();
    void setNbrOfLineToPrint(int parNbrOfLineToPrint);
    void setShowHeader(bool show);
    int getCharWidth();
    bool getColumnHidden(int col);
    void setColumnHidden(int col, bool hidden);

    // UI customization
    void loadColumnFromConfig(const QString & viewName);
    void saveColumnToConfig();
    static void setupColumnConfigDefaultValue(QMap<QString, duint> & map, const QString & viewName, int columnCount);

    // Content drawing control
    bool getDrawDebugOnly();
    void setDrawDebugOnly(bool value);

    // Table offset management
    dsint getTableOffset();
    void setTableOffset(dsint val);

    // Update/Reload/Refresh/Repaint
    virtual void prepareData();

signals:
    void enterPressedSignal();
    void headerButtonPressed(int col);
    void headerButtonReleased(int col);
    void tableOffsetChanged(dsint i);
    void viewableRows(int rows);
    void repainted();

public slots:
    // Configuration
    void slot_updateColors();
    void slot_updateFonts();
    void slot_updateShortcuts();

    // Update/Reload/Refresh/Repaint
    virtual void reloadData();
    void updateViewport();

    // ScrollBar Management
    void vertSliderActionSlot(int action);

private:
    typedef struct _ColumnResizingData_t
    {
        bool splitHandle;
        int index;
        int lastPosX;
    } ColumnResizingData_t;

    typedef struct _HeaderButton_t
    {
        bool isClickable;
        bool isPressed;
        bool isMouseOver;
    } HeaderButton_t;

    typedef struct _Column_t
    {
        int width;
        bool hidden;
        HeaderButton_t header;
        QString title;
    } Column_t;

    typedef struct _Header_t
    {
        bool isVisible;
        int height;
        int activeButtonIndex;
    } Header_t;

    typedef struct _ScrollBar64_t
    {
        bool is64;
        int rightShiftCount;
    } ScrollBar64_t;

    GuiState_t mGuiState;

    ColumnResizingData_t mColResizeData;

    QPushButton mHeaderButtonSytle;

    QList<Column_t> mColumnList;

    QList<int> mColumnOrder;
    int mReorderStartX;
    int mHoveredColumnDisplayIndex;

    dsint mRowCount;

    int mMouseWheelScrollDelta;

    dsint mTableOffset;
    Header_t mHeader;

    int mNbrOfLineToPrint;

    dsint mPrevTableOffset;

    bool mShouldReload;

    ScrollBar64_t mScrollBarAttributes;

    int getColumnDisplayIndexFromX(int x);
    friend class ColumnReorderDialog;
protected:
    bool mAllowPainting;
    bool mDrawDebugOnly;

    // Configuration
    QColor backgroundColor;
    QColor textColor;
    QColor separatorColor;
    QColor headerTextColor;
    QColor selectionColor;
    QString mViewName;

    // Font metrics
    CachedFontMetrics* mFontMetrics;
    void invalidateCachedFont();

    //action helpers
protected:
#include "ActionHelpers.h"
};

#endif // ABSTRACTTABLEVIEW_H
