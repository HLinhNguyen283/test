/*! \file PrintConditionGUIDef.h
 *  \brief define information of printcondition GUI.
 *  \author GCS Dieu Dinh
 *  \date 2013/12/12 GCS Dieu Dinh Created
*/
#if !defined(_PRINT_CONDITION_GUI_DEF_H_)
#define _PRINT_CONDITION_GUI_DEF_H_

//! Anchor point information structure
typedef struct _DOTGAIN_ANCHOR {
	POINT POSITION;					//!< Position
	long EDIT_TYPE;					//!< Anchor point type
	long KIND;						//!< Anchor point kind
}DOTGAIN_ANCHOR;

typedef struct _COLORINFO
{
	long CORLOR_NUM;			//!< Color ID
	char CBUF_NAME[32];			//!< Color Name
	long INK_HEAD_COUNT;		//!< Number of head module per color
	COLORREF COLOR;				//!< Display color
} COLORINFO, *PCOLORINFO, *COLORINFOLIST;
//! Define anchor point kind
enum
{
	ANCHOR_KIND_DISPLAY = 0,		//!< Display only
	ANCHOR_KIND_CURVEDRAW,			//!< Curve drawing
	ANCHOR_KIND_MAX
};

//! Define anchor point type
enum
{
	ANCHOR_EDITTYPE_DEFAULT = 0,	//!< Default.
	ANCHOR_EDITTYPE_EDIT,			//!< Edit
	ANCHOR_EDITTYPE_MAX
};

#define DOTGAIN_CURVE_PT_CNT	256		//!< Number of points of tone curve
#define DEF_ANCHOR_PT_UNIT	1000		//!< Unit of the anchor point
#define MAX_DOTGAIN_CURVE_PT_X	255		//!< Maximum value of dot gain curve (X coordinate)
#define MAX_DOTGAIN_CURVE_PT_Y	255		//!< Maximum value of dot gain curve (Y coordinate)
#define MIN_DOTGAIN_CURVE_PT_X	0		//!< Minimum value of dot gain curve (X coordinate)
#define MIN_DOTGAIN_CURVE_PT_Y	0		//!< Minimum value of dot gain curve (Y coordinate)

#define DEF_PRECOAT_MAX			3		//!< Maximum value of Precoat
#define DEF_PRECOAT_MIN			0		//!< Minimum value of Precoat
#define DEF_OVERCOAT_MAX		5		//!< Maximum value of Overcoat
#define DEF_OVERCOAT_MIN		0		//!< Minimum value of Overcoat

#define DEF_COLOR_RED	RGB(255, 0, 0)		//!< Red color

#define DEF_PRINTER_1				1
#define DEF_PRINTER_2				2
#define DEF_MAX_COLOR_NUMBER		8

#endif // !defined(_PRINT_CONDITION_GUI_DEF_H_)