{******************************************************************}
{ GDI+ Class                                                       }
{                                                                  }
{ home page : http://www.progdigy.com                              }
{ email     : hgourvest@progdigy.com                               }
{                                                                  }
{ date      : 15-02-2002                                           }
{                                                                  }
{ The contents of this file are used with permission, subject to   }
{ the Mozilla Public License Version 1.1 (the "License"); you may  }
{ not use this file except in compliance with the License. You may }
{ obtain a copy of the License at                                  }
{ http://www.mozilla.org/MPL/MPL-1.1.html                          }
{                                                                  }
{ Software distributed under the License is distributed on an      }
{ "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or   }
{ implied. See the License for the specific language governing     }
{ rights and limitations under the License.                        }
{                                                                  }
{ *****************************************************************}
      
{**********************************************************************
 Package pl_Win_GDI.pkg
 CodeTyphon Studio (http://www.pilotlogic.com/)
***********************************************************************}

{$WEAKPACKAGEUNIT}	  
unit GDIPAPI;

{$ALIGN ON}
{$MINENUMSIZE 4}

{$IFDEF CONDITIONALEXPRESSIONS}
  {$DEFINE DELPHI6_UP}
{$ENDIF} 

interface

uses
  Windows,
  ActiveX, GDICONST,
  DirectDraw,
  Math;
type
  INT16   = type Smallint;
  UINT16  = type Word;
  PUINT16 = ^UINT16;
  UINT32  = type Cardinal;
  TSingleDynArray = array of Single;

const WINGDIPDLL = 'gdiplus.dll';

//----------------------------------------------------------------------------
// Memory Allocation APIs
//----------------------------------------------------------------------------

var
GdipAlloc: function(size: ULONG): pointer; stdcall;
GdipFree: procedure(ptr: pointer); stdcall;  

type
  TGdiplusBase = class
  public
    class function NewInstance: TObject; override;
    procedure FreeInstance; override;
  end;

//--------------------------------------------------------------------------
// Default bezier flattening tolerance in device pixels.
//--------------------------------------------------------------------------

const
  FlatnessDefault = 0.25;

//--------------------------------------------------------------------------
// Graphics and Container State cookies
//--------------------------------------------------------------------------
type
  GraphicsState     = UINT;
  GraphicsContainer = UINT;

//--------------------------------------------------------------------------
// Fill mode constants
//--------------------------------------------------------------------------

  FillMode = (
    FillModeAlternate,        // 0
    FillModeWinding           // 1
  );
  TFillMode = FillMode;

//--------------------------------------------------------------------------
// Quality mode constants
//--------------------------------------------------------------------------

{$IFDEF DELPHI6_UP}
  QualityMode = (
    QualityModeInvalid   = -1,
    QualityModeDefault   =  0,
    QualityModeLow       =  1, // Best performance
    QualityModeHigh      =  2  // Best rendering quality
  );
  TQualityMode = QualityMode;
{$ELSE}
  QualityMode = Integer;
  const
    QualityModeInvalid   = -1;
    QualityModeDefault   =  0;
    QualityModeLow       =  1; // Best performance
    QualityModeHigh      =  2; // Best rendering quality
{$ENDIF}

//--------------------------------------------------------------------------
// Alpha Compositing mode constants
//--------------------------------------------------------------------------
type
  CompositingMode = (
    CompositingModeSourceOver,    // 0
    CompositingModeSourceCopy     // 1
  );
  TCompositingMode = CompositingMode;

//--------------------------------------------------------------------------
// Alpha Compositing quality constants
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  CompositingQuality = (
    CompositingQualityInvalid          = ord(QualityModeInvalid),
    CompositingQualityDefault          = ord(QualityModeDefault),
    CompositingQualityHighSpeed        = ord(QualityModeLow),
    CompositingQualityHighQuality      = ord(QualityModeHigh),
    CompositingQualityGammaCorrected,
    CompositingQualityAssumeLinear
  );
  TCompositingQuality = CompositingQuality;
{$ELSE}
  CompositingQuality = Integer;
  const
    CompositingQualityInvalid          = QualityModeInvalid;
    CompositingQualityDefault          = QualityModeDefault;
    CompositingQualityHighSpeed        = QualityModeLow;
    CompositingQualityHighQuality      = QualityModeHigh;
    CompositingQualityGammaCorrected   = 3;
    CompositingQualityAssumeLinear     = 4;

type
  TCompositingQuality = CompositingQuality;
{$ENDIF}

//--------------------------------------------------------------------------
// Unit constants
//--------------------------------------------------------------------------

  Unit_ = (
    UnitWorld,      // 0 -- World coordinate (non-physical unit)
    UnitDisplay,    // 1 -- Variable -- for PageTransform only
    UnitPixel,      // 2 -- Each unit is one device pixel.
    UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
    UnitInch,       // 4 -- Each unit is 1 inch.
    UnitDocument,   // 5 -- Each unit is 1/300 inch.
    UnitMillimeter  // 6 -- Each unit is 1 millimeter.
  );
  TUnit = Unit_;

//--------------------------------------------------------------------------
// MetafileFrameUnit
//
// The frameRect for creating a metafile can be specified in any of these
// units.  There is an extra frame unit value (MetafileFrameUnitGdi) so
// that units can be supplied in the same units that GDI expects for
// frame rects -- these units are in .01 (1/100ths) millimeter units
// as defined by GDI.
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  MetafileFrameUnit = (
    MetafileFrameUnitPixel      = ord(UnitPixel),
    MetafileFrameUnitPoint      = ord(UnitPoint),
    MetafileFrameUnitInch       = ord(UnitInch),
    MetafileFrameUnitDocument   = ord(UnitDocument),
    MetafileFrameUnitMillimeter = ord(UnitMillimeter),
    MetafileFrameUnitGdi        // GDI compatible .01 MM units
  );
  TMetafileFrameUnit = MetafileFrameUnit;
{$ELSE}
  MetafileFrameUnit = Integer;
  const
    MetafileFrameUnitPixel      = 2;
    MetafileFrameUnitPoint      = 3;
    MetafileFrameUnitInch       = 4;
    MetafileFrameUnitDocument   = 5;
    MetafileFrameUnitMillimeter = 6;
    MetafileFrameUnitGdi        = 7; // GDI compatible .01 MM units

type
  TMetafileFrameUnit = MetafileFrameUnit;
{$ENDIF}
//--------------------------------------------------------------------------
// Coordinate space identifiers
//--------------------------------------------------------------------------

  CoordinateSpace = (
    CoordinateSpaceWorld,     // 0
    CoordinateSpacePage,      // 1
    CoordinateSpaceDevice     // 2
  );
  TCoordinateSpace = CoordinateSpace;

//--------------------------------------------------------------------------
// Various wrap modes for brushes
//--------------------------------------------------------------------------

  WrapMode = (
    WrapModeTile,        // 0
    WrapModeTileFlipX,   // 1
    WrapModeTileFlipY,   // 2
    WrapModeTileFlipXY,  // 3
    WrapModeClamp        // 4
  );
  TWrapMode = WrapMode;

//--------------------------------------------------------------------------
// Various hatch styles
//--------------------------------------------------------------------------

  HatchStyle = (
    HatchStyleHorizontal,                  // = 0,
    HatchStyleVertical,                    // = 1,
    HatchStyleForwardDiagonal,             // = 2,
    HatchStyleBackwardDiagonal,            // = 3,
    HatchStyleCross,                       // = 4,
    HatchStyleDiagonalCross,               // = 5,
    HatchStyle05Percent,                   // = 6,
    HatchStyle10Percent,                   // = 7,
    HatchStyle20Percent,                   // = 8,
    HatchStyle25Percent,                   // = 9,
    HatchStyle30Percent,                   // = 10,
    HatchStyle40Percent,                   // = 11,
    HatchStyle50Percent,                   // = 12,
    HatchStyle60Percent,                   // = 13,
    HatchStyle70Percent,                   // = 14,
    HatchStyle75Percent,                   // = 15,
    HatchStyle80Percent,                   // = 16,
    HatchStyle90Percent,                   // = 17,
    HatchStyleLightDownwardDiagonal,       // = 18,
    HatchStyleLightUpwardDiagonal,         // = 19,
    HatchStyleDarkDownwardDiagonal,        // = 20,
    HatchStyleDarkUpwardDiagonal,          // = 21,
    HatchStyleWideDownwardDiagonal,        // = 22,
    HatchStyleWideUpwardDiagonal,          // = 23,
    HatchStyleLightVertical,               // = 24,
    HatchStyleLightHorizontal,             // = 25,
    HatchStyleNarrowVertical,              // = 26,
    HatchStyleNarrowHorizontal,            // = 27,
    HatchStyleDarkVertical,                // = 28,
    HatchStyleDarkHorizontal,              // = 29,
    HatchStyleDashedDownwardDiagonal,      // = 30,
    HatchStyleDashedUpwardDiagonal,        // = 31,
    HatchStyleDashedHorizontal,            // = 32,
    HatchStyleDashedVertical,              // = 33,
    HatchStyleSmallConfetti,               // = 34,
    HatchStyleLargeConfetti,               // = 35,
    HatchStyleZigZag,                      // = 36,
    HatchStyleWave,                        // = 37,
    HatchStyleDiagonalBrick,               // = 38,
    HatchStyleHorizontalBrick,             // = 39,
    HatchStyleWeave,                       // = 40,
    HatchStylePlaid,                       // = 41,
    HatchStyleDivot,                       // = 42,
    HatchStyleDottedGrid,                  // = 43,
    HatchStyleDottedDiamond,               // = 44,
    HatchStyleShingle,                     // = 45,
    HatchStyleTrellis,                     // = 46,
    HatchStyleSphere,                      // = 47,
    HatchStyleSmallGrid,                   // = 48,
    HatchStyleSmallCheckerBoard,           // = 49,
    HatchStyleLargeCheckerBoard,           // = 50,
    HatchStyleOutlinedDiamond,             // = 51,
    HatchStyleSolidDiamond,                // = 52,

    HatchStyleTotal                        // = 53,
  );

  const
    HatchStyleLargeGrid = HatchStyleCross; // 4
    HatchStyleMin       = HatchStyleHorizontal;
    HatchStyleMax       = HatchStyleSolidDiamond;

type
  THatchStyle = HatchStyle;

//--------------------------------------------------------------------------
// Dash style constants
//--------------------------------------------------------------------------

  DashStyle = (
    DashStyleSolid,          // 0
    DashStyleDash,           // 1
    DashStyleDot,            // 2
    DashStyleDashDot,        // 3
    DashStyleDashDotDot,     // 4
    DashStyleCustom          // 5
  );
  TDashStyle = DashStyle;

//--------------------------------------------------------------------------
// Dash cap constants
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  DashCap = (
    DashCapFlat             = 0,
    DashCapRound            = 2,
    DashCapTriangle         = 3
  );
  TDashCap = DashCap;
{$ELSE}
  DashCap = Integer;
  const
    DashCapFlat             = 0;
    DashCapRound            = 2;
    DashCapTriangle         = 3;

type
  TDashCap = DashCap;
{$ENDIF}

//--------------------------------------------------------------------------
// Line cap constants (only the lowest 8 bits are used).
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  LineCap = (
    LineCapFlat             = 0,
    LineCapSquare           = 1,
    LineCapRound            = 2,
    LineCapTriangle         = 3,

    LineCapNoAnchor         = $10, // corresponds to flat cap
    LineCapSquareAnchor     = $11, // corresponds to square cap
    LineCapRoundAnchor      = $12, // corresponds to round cap
    LineCapDiamondAnchor    = $13, // corresponds to triangle cap
    LineCapArrowAnchor      = $14, // no correspondence

    LineCapCustom           = $ff, // custom cap

    LineCapAnchorMask       = $f0  // mask to check for anchor or not.
  );
  TLineCap = LineCap;
{$ELSE}
  LineCap = Integer;
  const
    LineCapFlat             = 0;
    LineCapSquare           = 1;
    LineCapRound            = 2;
    LineCapTriangle         = 3;

    LineCapNoAnchor         = $10; // corresponds to flat cap
    LineCapSquareAnchor     = $11; // corresponds to square cap
    LineCapRoundAnchor      = $12; // corresponds to round cap
    LineCapDiamondAnchor    = $13; // corresponds to triangle cap
    LineCapArrowAnchor      = $14; // no correspondence

    LineCapCustom           = $ff; // custom cap

    LineCapAnchorMask       = $f0; // mask to check for anchor or not.

type
  TLineCap = LineCap;
{$ENDIF}

//--------------------------------------------------------------------------
// Custom Line cap type constants
//--------------------------------------------------------------------------

  CustomLineCapType = (
    CustomLineCapTypeDefault,
    CustomLineCapTypeAdjustableArrow
  );
  TCustomLineCapType = CustomLineCapType;

//--------------------------------------------------------------------------
// Line join constants
//--------------------------------------------------------------------------

  LineJoin = (
    LineJoinMiter,
    LineJoinBevel,
    LineJoinRound,
    LineJoinMiterClipped
  );
  TLineJoin = LineJoin;

//--------------------------------------------------------------------------
// Path point types (only the lowest 8 bits are used.)
//  The lowest 3 bits are interpreted as point type
//  The higher 5 bits are reserved for flags.
//--------------------------------------------------------------------------

{$IFDEF DELPHI6_UP}
  {$Z1}
  PathPointType = (
    PathPointTypeStart           = $00, // move
    PathPointTypeLine            = $01, // line
    PathPointTypeBezier          = $03, // default Bezier (= cubic Bezier)
    PathPointTypePathTypeMask    = $07, // type mask (lowest 3 bits).
    PathPointTypeDashMode        = $10, // currently in dash mode.
    PathPointTypePathMarker      = $20, // a marker for the path.
    PathPointTypeCloseSubpath    = $80, // closed flag

    // Path types used for advanced path.
    PathPointTypeBezier3         = $03  // cubic Bezier
  );
  TPathPointType = PathPointType;
  {$Z4}
{$ELSE}
  PathPointType = Byte;
  const
    PathPointTypeStart          : Byte = $00; // move
    PathPointTypeLine           : Byte = $01; // line
    PathPointTypeBezier         : Byte = $03; // default Bezier (= cubic Bezier)
    PathPointTypePathTypeMask   : Byte = $07; // type mask (lowest 3 bits).
    PathPointTypeDashMode       : Byte = $10; // currently in dash mode.
    PathPointTypePathMarker     : Byte = $20; // a marker for the path.
    PathPointTypeCloseSubpath   : Byte = $80; // closed flag

    // Path types used for advanced path.
    PathPointTypeBezier3        : Byte = $03;  // cubic Bezier

type
  TPathPointType = PathPointType;
{$ENDIF}

//--------------------------------------------------------------------------
// WarpMode constants
//--------------------------------------------------------------------------

  WarpMode = (
    WarpModePerspective,    // 0
    WarpModeBilinear        // 1
  );
  TWarpMode = WarpMode;

//--------------------------------------------------------------------------
// LineGradient Mode
//--------------------------------------------------------------------------

  LinearGradientMode = (
    LinearGradientModeHorizontal,         // 0
    LinearGradientModeVertical,           // 1
    LinearGradientModeForwardDiagonal,    // 2
    LinearGradientModeBackwardDiagonal    // 3
  );
  TLinearGradientMode = LinearGradientMode;

//--------------------------------------------------------------------------
// Region Comine Modes
//--------------------------------------------------------------------------

  CombineMode = (
    CombineModeReplace,     // 0
    CombineModeIntersect,   // 1
    CombineModeUnion,       // 2
    CombineModeXor,         // 3
    CombineModeExclude,     // 4
    CombineModeComplement   // 5 (Exclude From)
  );
  TCombineMode = CombineMode;

//--------------------------------------------------------------------------
 // Image types
//--------------------------------------------------------------------------

  ImageType = (
    ImageTypeUnknown,   // 0
    ImageTypeBitmap,    // 1
    ImageTypeMetafile   // 2
  );
  TImageType = ImageType;

//--------------------------------------------------------------------------
// Interpolation modes
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  InterpolationMode = (
    InterpolationModeInvalid          = ord(QualityModeInvalid),
    InterpolationModeDefault          = ord(QualityModeDefault),
    InterpolationModeLowQuality       = ord(QualityModeLow),
    InterpolationModeHighQuality      = ord(QualityModeHigh),
    InterpolationModeBilinear,
    InterpolationModeBicubic,
    InterpolationModeNearestNeighbor,
    InterpolationModeHighQualityBilinear,
    InterpolationModeHighQualityBicubic
  );
  TInterpolationMode = InterpolationMode;
{$ELSE}
  InterpolationMode = Integer;
  const
    InterpolationModeInvalid             = QualityModeInvalid;
    InterpolationModeDefault             = QualityModeDefault;
    InterpolationModeLowQuality          = QualityModeLow;
    InterpolationModeHighQuality         = QualityModeHigh;
    InterpolationModeBilinear            = 3;
    InterpolationModeBicubic             = 4;
    InterpolationModeNearestNeighbor     = 5;
    InterpolationModeHighQualityBilinear = 6;
    InterpolationModeHighQualityBicubic  = 7;

type
  TInterpolationMode = InterpolationMode;
{$ENDIF}

//--------------------------------------------------------------------------
// Pen types
//--------------------------------------------------------------------------

  PenAlignment = (
    PenAlignmentCenter,
    PenAlignmentInset
  );
  TPenAlignment = PenAlignment;

//--------------------------------------------------------------------------
// Brush types
//--------------------------------------------------------------------------

  BrushType = (
   BrushTypeSolidColor,
   BrushTypeHatchFill,
   BrushTypeTextureFill,
   BrushTypePathGradient,
   BrushTypeLinearGradient 
  );
  TBrushType = BrushType;

//--------------------------------------------------------------------------
// Pen's Fill types
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  PenType = (
   PenTypeSolidColor       =  ord(BrushTypeSolidColor),
   PenTypeHatchFill        =  ord(BrushTypeHatchFill),
   PenTypeTextureFill      =  ord(BrushTypeTextureFill),
   PenTypePathGradient     =  ord(BrushTypePathGradient),
   PenTypeLinearGradient   =  ord(BrushTypeLinearGradient),
   PenTypeUnknown          = -1
  );
  TPenType = PenType;
{$ELSE}
  PenType = Integer;
  const
    PenTypeSolidColor       =  0;
    PenTypeHatchFill        =  1;
    PenTypeTextureFill      =  2;
    PenTypePathGradient     =  3;
    PenTypeLinearGradient   =  4;
    PenTypeUnknown          = -1;

type
  TPenType = PenType;
{$ENDIF}

//--------------------------------------------------------------------------
// Matrix Order
//--------------------------------------------------------------------------

  MatrixOrder = (
    MatrixOrderPrepend,
    MatrixOrderAppend
  );
  TMatrixOrder = MatrixOrder;

//--------------------------------------------------------------------------
// Generic font families
//--------------------------------------------------------------------------

  GenericFontFamily = (
    GenericFontFamilySerif,
    GenericFontFamilySansSerif,
    GenericFontFamilyMonospace
  );
  TGenericFontFamily = GenericFontFamily;

//--------------------------------------------------------------------------
// FontStyle: face types and common styles
//--------------------------------------------------------------------------
type
  FontStyle = Integer;
  const
    FontStyleRegular    = Integer(0);
    FontStyleBold       = Integer(1);
    FontStyleItalic     = Integer(2);
    FontStyleBoldItalic = Integer(3);
    FontStyleUnderline  = Integer(4);
    FontStyleStrikeout  = Integer(8);
  Type
  TFontStyle = FontStyle;

//---------------------------------------------------------------------------
// Smoothing Mode
//---------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  SmoothingMode = (
    SmoothingModeInvalid     = ord(QualityModeInvalid),
    SmoothingModeDefault     = ord(QualityModeDefault),
    SmoothingModeHighSpeed   = ord(QualityModeLow),
    SmoothingModeHighQuality = ord(QualityModeHigh),
    SmoothingModeNone,
    SmoothingModeAntiAlias
  );
  TSmoothingMode = SmoothingMode;
{$ELSE}
  SmoothingMode = Integer;
  const
    SmoothingModeInvalid     = QualityModeInvalid;
    SmoothingModeDefault     = QualityModeDefault;
    SmoothingModeHighSpeed   = QualityModeLow;
    SmoothingModeHighQuality = QualityModeHigh;
    SmoothingModeNone        = 3;
    SmoothingModeAntiAlias   = 4;

type
  TSmoothingMode = SmoothingMode;
{$ENDIF}

//---------------------------------------------------------------------------
// Pixel Format Mode
//---------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  PixelOffsetMode = (
    PixelOffsetModeInvalid     = Ord(QualityModeInvalid),
    PixelOffsetModeDefault     = Ord(QualityModeDefault),
    PixelOffsetModeHighSpeed   = Ord(QualityModeLow),
    PixelOffsetModeHighQuality = Ord(QualityModeHigh),
    PixelOffsetModeNone,    // No pixel offset
    PixelOffsetModeHalf     // Offset by -0.5, -0.5 for fast anti-alias perf
  );
  TPixelOffsetMode = PixelOffsetMode;
{$ELSE}
  PixelOffsetMode = Integer;
  const
    PixelOffsetModeInvalid     = QualityModeInvalid;
    PixelOffsetModeDefault     = QualityModeDefault;
    PixelOffsetModeHighSpeed   = QualityModeLow;
    PixelOffsetModeHighQuality = QualityModeHigh;
    PixelOffsetModeNone        = 3;    // No pixel offset
    PixelOffsetModeHalf        = 4;    // Offset by -0.5, -0.5 for fast anti-alias perf

type
  TPixelOffsetMode = PixelOffsetMode;
{$ENDIF}

//---------------------------------------------------------------------------
// Text Rendering Hint
//---------------------------------------------------------------------------

  TextRenderingHint = (
    TextRenderingHintSystemDefault,                // Glyph with system default rendering hint
    TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
    TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
    TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
    TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
    TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
  );
  TTextRenderingHint = TextRenderingHint;

//---------------------------------------------------------------------------
// Metafile Types
//---------------------------------------------------------------------------

  MetafileType = (
    MetafileTypeInvalid,            // Invalid metafile
    MetafileTypeWmf,                // Standard WMF
    MetafileTypeWmfPlaceable,       // Placeable WMF
    MetafileTypeEmf,                // EMF (not EMF+)
    MetafileTypeEmfPlusOnly,        // EMF+ without dual, down-level records
    MetafileTypeEmfPlusDual         // EMF+ with dual, down-level records
  );
  TMetafileType = MetafileType;

//---------------------------------------------------------------------------
// Specifies the type of EMF to record
//---------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  EmfType = (
    EmfTypeEmfOnly     = Ord(MetafileTypeEmf),          // no EMF+, only EMF
    EmfTypeEmfPlusOnly = Ord(MetafileTypeEmfPlusOnly),  // no EMF, only EMF+
    EmfTypeEmfPlusDual = Ord(MetafileTypeEmfPlusDual)   // both EMF+ and EMF
  );
  TEmfType = EmfType;
{$ELSE}
  EmfType = Integer;
  const
    EmfTypeEmfOnly     = Ord(MetafileTypeEmf);          // no EMF+, only EMF
    EmfTypeEmfPlusOnly = Ord(MetafileTypeEmfPlusOnly);  // no EMF, only EMF+
    EmfTypeEmfPlusDual = Ord(MetafileTypeEmfPlusDual);   // both EMF+ and EMF

type
  TEmfType = EmfType;
{$ENDIF}

//---------------------------------------------------------------------------
// EMF+ Persistent object types
//---------------------------------------------------------------------------

  ObjectType = (
    ObjectTypeInvalid,
    ObjectTypeBrush,
    ObjectTypePen,
    ObjectTypePath,
    ObjectTypeRegion,
    ObjectTypeImage,
    ObjectTypeFont,
    ObjectTypeStringFormat,
    ObjectTypeImageAttributes,
    ObjectTypeCustomLineCap
  );
  TObjectType = ObjectType;

const
  ObjectTypeMax = ObjectTypeCustomLineCap;
  ObjectTypeMin = ObjectTypeBrush;

//---------------------------------------------------------------------------
// EMF+ Records
//---------------------------------------------------------------------------

  // We have to change the WMF record numbers so that they don't conflict with
  // the EMF and EMF+ record numbers.

const
  GDIP_EMFPLUS_RECORD_BASE      = $00004000;
  GDIP_WMF_RECORD_BASE          = $00010000;

{$IFDEF DELPHI6_UP}
type
  EmfPlusRecordType = (
   // Since we have to enumerate GDI records right along with GDI+ records,
   // We list all the GDI records here so that they can be part of the
   // same enumeration type which is used in the enumeration callback.

    WmfRecordTypeSetBkColor              = (META_SETBKCOLOR or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetBkMode               = (META_SETBKMODE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetMapMode              = (META_SETMAPMODE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetROP2                 = (META_SETROP2 or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetRelAbs               = (META_SETRELABS or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetPolyFillMode         = (META_SETPOLYFILLMODE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetStretchBltMode       = (META_SETSTRETCHBLTMODE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetTextCharExtra        = (META_SETTEXTCHAREXTRA or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetTextColor            = (META_SETTEXTCOLOR or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetTextJustification    = (META_SETTEXTJUSTIFICATION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetWindowOrg            = (META_SETWINDOWORG or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetWindowExt            = (META_SETWINDOWEXT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetViewportOrg          = (META_SETVIEWPORTORG or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetViewportExt          = (META_SETVIEWPORTEXT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeOffsetWindowOrg         = (META_OFFSETWINDOWORG or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeScaleWindowExt          = (META_SCALEWINDOWEXT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeOffsetViewportOrg       = (META_OFFSETVIEWPORTORG or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeScaleViewportExt        = (META_SCALEVIEWPORTEXT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeLineTo                  = (META_LINETO or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeMoveTo                  = (META_MOVETO or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeExcludeClipRect         = (META_EXCLUDECLIPRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeIntersectClipRect       = (META_INTERSECTCLIPRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeArc                     = (META_ARC or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeEllipse                 = (META_ELLIPSE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeFloodFill               = (META_FLOODFILL or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePie                     = (META_PIE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeRectangle               = (META_RECTANGLE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeRoundRect               = (META_ROUNDRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePatBlt                  = (META_PATBLT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSaveDC                  = (META_SAVEDC or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetPixel                = (META_SETPIXEL or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeOffsetClipRgn           = (META_OFFSETCLIPRGN or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeTextOut                 = (META_TEXTOUT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeBitBlt                  = (META_BITBLT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeStretchBlt              = (META_STRETCHBLT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePolygon                 = (META_POLYGON or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePolyline                = (META_POLYLINE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeEscape                  = (META_ESCAPE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeRestoreDC               = (META_RESTOREDC or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeFillRegion              = (META_FILLREGION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeFrameRegion             = (META_FRAMEREGION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeInvertRegion            = (META_INVERTREGION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePaintRegion             = (META_PAINTREGION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSelectClipRegion        = (META_SELECTCLIPREGION or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSelectObject            = (META_SELECTOBJECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetTextAlign            = (META_SETTEXTALIGN or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeDrawText                = ($062F or GDIP_WMF_RECORD_BASE),  // META_DRAWTEXT
    WmfRecordTypeChord                   = (META_CHORD or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetMapperFlags          = (META_SETMAPPERFLAGS or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeExtTextOut              = (META_EXTTEXTOUT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetDIBToDev             = (META_SETDIBTODEV or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSelectPalette           = (META_SELECTPALETTE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeRealizePalette          = (META_REALIZEPALETTE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeAnimatePalette          = (META_ANIMATEPALETTE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetPalEntries           = (META_SETPALENTRIES or GDIP_WMF_RECORD_BASE),
    WmfRecordTypePolyPolygon             = (META_POLYPOLYGON or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeResizePalette           = (META_RESIZEPALETTE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeDIBBitBlt               = (META_DIBBITBLT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeDIBStretchBlt           = (META_DIBSTRETCHBLT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeDIBCreatePatternBrush   = (META_DIBCREATEPATTERNBRUSH or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeStretchDIB              = (META_STRETCHDIB or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeExtFloodFill            = (META_EXTFLOODFILL or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeSetLayout               = ($0149 or GDIP_WMF_RECORD_BASE),  // META_SETLAYOUT
    WmfRecordTypeResetDC                 = ($014C or GDIP_WMF_RECORD_BASE),  // META_RESETDC
    WmfRecordTypeStartDoc                = ($014D or GDIP_WMF_RECORD_BASE),  // META_STARTDOC
    WmfRecordTypeStartPage               = ($004F or GDIP_WMF_RECORD_BASE),  // META_STARTPAGE
    WmfRecordTypeEndPage                 = ($0050 or GDIP_WMF_RECORD_BASE),  // META_ENDPAGE
    WmfRecordTypeAbortDoc                = ($0052 or GDIP_WMF_RECORD_BASE),  // META_ABORTDOC
    WmfRecordTypeEndDoc                  = ($005E or GDIP_WMF_RECORD_BASE),  // META_ENDDOC
    WmfRecordTypeDeleteObject            = (META_DELETEOBJECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreatePalette           = (META_CREATEPALETTE or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreateBrush             = ($00F8 or GDIP_WMF_RECORD_BASE),  // META_CREATEBRUSH
    WmfRecordTypeCreatePatternBrush      = (META_CREATEPATTERNBRUSH or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreatePenIndirect       = (META_CREATEPENINDIRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreateFontIndirect      = (META_CREATEFONTINDIRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreateBrushIndirect     = (META_CREATEBRUSHINDIRECT or GDIP_WMF_RECORD_BASE),
    WmfRecordTypeCreateBitmapIndirect    = ($02FD or GDIP_WMF_RECORD_BASE),  // META_CREATEBITMAPINDIRECT
    WmfRecordTypeCreateBitmap            = ($06FE or GDIP_WMF_RECORD_BASE),  // META_CREATEBITMAP
    WmfRecordTypeCreateRegion            = (META_CREATEREGION or GDIP_WMF_RECORD_BASE),

    EmfRecordTypeHeader                  = EMR_HEADER,
    EmfRecordTypePolyBezier              = EMR_POLYBEZIER,
    EmfRecordTypePolygon                 = EMR_POLYGON,
    EmfRecordTypePolyline                = EMR_POLYLINE,
    EmfRecordTypePolyBezierTo            = EMR_POLYBEZIERTO,
    EmfRecordTypePolyLineTo              = EMR_POLYLINETO,
    EmfRecordTypePolyPolyline            = EMR_POLYPOLYLINE,
    EmfRecordTypePolyPolygon             = EMR_POLYPOLYGON,
    EmfRecordTypeSetWindowExtEx          = EMR_SETWINDOWEXTEX,
    EmfRecordTypeSetWindowOrgEx          = EMR_SETWINDOWORGEX,
    EmfRecordTypeSetViewportExtEx        = EMR_SETVIEWPORTEXTEX,
    EmfRecordTypeSetViewportOrgEx        = EMR_SETVIEWPORTORGEX,
    EmfRecordTypeSetBrushOrgEx           = EMR_SETBRUSHORGEX,
    EmfRecordTypeEOF                     = EMR_EOF,
    EmfRecordTypeSetPixelV               = EMR_SETPIXELV,
    EmfRecordTypeSetMapperFlags          = EMR_SETMAPPERFLAGS,
    EmfRecordTypeSetMapMode              = EMR_SETMAPMODE,
    EmfRecordTypeSetBkMode               = EMR_SETBKMODE,
    EmfRecordTypeSetPolyFillMode         = EMR_SETPOLYFILLMODE,
    EmfRecordTypeSetROP2                 = EMR_SETROP2,
    EmfRecordTypeSetStretchBltMode       = EMR_SETSTRETCHBLTMODE,
    EmfRecordTypeSetTextAlign            = EMR_SETTEXTALIGN,
    EmfRecordTypeSetColorAdjustment      = EMR_SETCOLORADJUSTMENT,
    EmfRecordTypeSetTextColor            = EMR_SETTEXTCOLOR,
    EmfRecordTypeSetBkColor              = EMR_SETBKCOLOR,
    EmfRecordTypeOffsetClipRgn           = EMR_OFFSETCLIPRGN,
    EmfRecordTypeMoveToEx                = EMR_MOVETOEX,
    EmfRecordTypeSetMetaRgn              = EMR_SETMETARGN,
    EmfRecordTypeExcludeClipRect         = EMR_EXCLUDECLIPRECT,
    EmfRecordTypeIntersectClipRect       = EMR_INTERSECTCLIPRECT,
    EmfRecordTypeScaleViewportExtEx      = EMR_SCALEVIEWPORTEXTEX,
    EmfRecordTypeScaleWindowExtEx        = EMR_SCALEWINDOWEXTEX,
    EmfRecordTypeSaveDC                  = EMR_SAVEDC,
    EmfRecordTypeRestoreDC               = EMR_RESTOREDC,
    EmfRecordTypeSetWorldTransform       = EMR_SETWORLDTRANSFORM,
    EmfRecordTypeModifyWorldTransform    = EMR_MODIFYWORLDTRANSFORM,
    EmfRecordTypeSelectObject            = EMR_SELECTOBJECT,
    EmfRecordTypeCreatePen               = EMR_CREATEPEN,
    EmfRecordTypeCreateBrushIndirect     = EMR_CREATEBRUSHINDIRECT,
    EmfRecordTypeDeleteObject            = EMR_DELETEOBJECT,
    EmfRecordTypeAngleArc                = EMR_ANGLEARC,
    EmfRecordTypeEllipse                 = EMR_ELLIPSE,
    EmfRecordTypeRectangle               = EMR_RECTANGLE,
    EmfRecordTypeRoundRect               = EMR_ROUNDRECT,
    EmfRecordTypeArc                     = EMR_ARC,
    EmfRecordTypeChord                   = EMR_CHORD,
    EmfRecordTypePie                     = EMR_PIE,
    EmfRecordTypeSelectPalette           = EMR_SELECTPALETTE,
    EmfRecordTypeCreatePalette           = EMR_CREATEPALETTE,
    EmfRecordTypeSetPaletteEntries       = EMR_SETPALETTEENTRIES,
    EmfRecordTypeResizePalette           = EMR_RESIZEPALETTE,
    EmfRecordTypeRealizePalette          = EMR_REALIZEPALETTE,
    EmfRecordTypeExtFloodFill            = EMR_EXTFLOODFILL,
    EmfRecordTypeLineTo                  = EMR_LINETO,
    EmfRecordTypeArcTo                   = EMR_ARCTO,
    EmfRecordTypePolyDraw                = EMR_POLYDRAW,
    EmfRecordTypeSetArcDirection         = EMR_SETARCDIRECTION,
    EmfRecordTypeSetMiterLimit           = EMR_SETMITERLIMIT,
    EmfRecordTypeBeginPath               = EMR_BEGINPATH,
    EmfRecordTypeEndPath                 = EMR_ENDPATH,
    EmfRecordTypeCloseFigure             = EMR_CLOSEFIGURE,
    EmfRecordTypeFillPath                = EMR_FILLPATH,
    EmfRecordTypeStrokeAndFillPath       = EMR_STROKEANDFILLPATH,
    EmfRecordTypeStrokePath              = EMR_STROKEPATH,
    EmfRecordTypeFlattenPath             = EMR_FLATTENPATH,
    EmfRecordTypeWidenPath               = EMR_WIDENPATH,
    EmfRecordTypeSelectClipPath          = EMR_SELECTCLIPPATH,
    EmfRecordTypeAbortPath               = EMR_ABORTPATH,
    EmfRecordTypeReserved_069            = 69,  // Not Used
    EmfRecordTypeGdiComment              = EMR_GDICOMMENT,
    EmfRecordTypeFillRgn                 = EMR_FILLRGN,
    EmfRecordTypeFrameRgn                = EMR_FRAMERGN,
    EmfRecordTypeInvertRgn               = EMR_INVERTRGN,
    EmfRecordTypePaintRgn                = EMR_PAINTRGN,
    EmfRecordTypeExtSelectClipRgn        = EMR_EXTSELECTCLIPRGN,
    EmfRecordTypeBitBlt                  = EMR_BITBLT,
    EmfRecordTypeStretchBlt              = EMR_STRETCHBLT,
    EmfRecordTypeMaskBlt                 = EMR_MASKBLT,
    EmfRecordTypePlgBlt                  = EMR_PLGBLT,
    EmfRecordTypeSetDIBitsToDevice       = EMR_SETDIBITSTODEVICE,
    EmfRecordTypeStretchDIBits           = EMR_STRETCHDIBITS,
    EmfRecordTypeExtCreateFontIndirect   = EMR_EXTCREATEFONTINDIRECTW,
    EmfRecordTypeExtTextOutA             = EMR_EXTTEXTOUTA,
    EmfRecordTypeExtTextOutW             = EMR_EXTTEXTOUTW,
    EmfRecordTypePolyBezier16            = EMR_POLYBEZIER16,
    EmfRecordTypePolygon16               = EMR_POLYGON16,
    EmfRecordTypePolyline16              = EMR_POLYLINE16,
    EmfRecordTypePolyBezierTo16          = EMR_POLYBEZIERTO16,
    EmfRecordTypePolylineTo16            = EMR_POLYLINETO16,
    EmfRecordTypePolyPolyline16          = EMR_POLYPOLYLINE16,
    EmfRecordTypePolyPolygon16           = EMR_POLYPOLYGON16,
    EmfRecordTypePolyDraw16              = EMR_POLYDRAW16,
    EmfRecordTypeCreateMonoBrush         = EMR_CREATEMONOBRUSH,
    EmfRecordTypeCreateDIBPatternBrushPt = EMR_CREATEDIBPATTERNBRUSHPT,
    EmfRecordTypeExtCreatePen            = EMR_EXTCREATEPEN,
    EmfRecordTypePolyTextOutA            = EMR_POLYTEXTOUTA,
    EmfRecordTypePolyTextOutW            = EMR_POLYTEXTOUTW,
    EmfRecordTypeSetICMMode              = 98,  // EMR_SETICMMODE,
    EmfRecordTypeCreateColorSpace        = 99,  // EMR_CREATECOLORSPACE,
    EmfRecordTypeSetColorSpace           = 100, // EMR_SETCOLORSPACE,
    EmfRecordTypeDeleteColorSpace        = 101, // EMR_DELETECOLORSPACE,
    EmfRecordTypeGLSRecord               = 102, // EMR_GLSRECORD,
    EmfRecordTypeGLSBoundedRecord        = 103, // EMR_GLSBOUNDEDRECORD,
    EmfRecordTypePixelFormat             = 104, // EMR_PIXELFORMAT,
    EmfRecordTypeDrawEscape              = 105, // EMR_RESERVED_105,
    EmfRecordTypeExtEscape               = 106, // EMR_RESERVED_106,
    EmfRecordTypeStartDoc                = 107, // EMR_RESERVED_107,
    EmfRecordTypeSmallTextOut            = 108, // EMR_RESERVED_108,
    EmfRecordTypeForceUFIMapping         = 109, // EMR_RESERVED_109,
    EmfRecordTypeNamedEscape             = 110, // EMR_RESERVED_110,
    EmfRecordTypeColorCorrectPalette     = 111, // EMR_COLORCORRECTPALETTE,
    EmfRecordTypeSetICMProfileA          = 112, // EMR_SETICMPROFILEA,
    EmfRecordTypeSetICMProfileW          = 113, // EMR_SETICMPROFILEW,
    EmfRecordTypeAlphaBlend              = 114, // EMR_ALPHABLEND,
    EmfRecordTypeSetLayout               = 115, // EMR_SETLAYOUT,
    EmfRecordTypeTransparentBlt          = 116, // EMR_TRANSPARENTBLT,
    EmfRecordTypeReserved_117            = 117, // Not Used
    EmfRecordTypeGradientFill            = 118, // EMR_GRADIENTFILL,
    EmfRecordTypeSetLinkedUFIs           = 119, // EMR_RESERVED_119,
    EmfRecordTypeSetTextJustification    = 120, // EMR_RESERVED_120,
    EmfRecordTypeColorMatchToTargetW     = 121, // EMR_COLORMATCHTOTARGETW,
    EmfRecordTypeCreateColorSpaceW       = 122, // EMR_CREATECOLORSPACEW,
    EmfRecordTypeMax                     = 122,
    EmfRecordTypeMin                     = 1,

    // That is the END of the GDI EMF records.

    // Now we start the list of EMF+ records.  We leave quite
    // a bit of room here for the addition of any new GDI
    // records that may be added later.

    EmfPlusRecordTypeInvalid = GDIP_EMFPLUS_RECORD_BASE,
    EmfPlusRecordTypeHeader,
    EmfPlusRecordTypeEndOfFile,

    EmfPlusRecordTypeComment,

    EmfPlusRecordTypeGetDC,

    EmfPlusRecordTypeMultiFormatStart,
    EmfPlusRecordTypeMultiFormatSection,
    EmfPlusRecordTypeMultiFormatEnd,

    // For all persistent objects

    EmfPlusRecordTypeObject,

    // Drawing Records

    EmfPlusRecordTypeClear,
    EmfPlusRecordTypeFillRects,
    EmfPlusRecordTypeDrawRects,
    EmfPlusRecordTypeFillPolygon,
    EmfPlusRecordTypeDrawLines,
    EmfPlusRecordTypeFillEllipse,
    EmfPlusRecordTypeDrawEllipse,
    EmfPlusRecordTypeFillPie,
    EmfPlusRecordTypeDrawPie,
    EmfPlusRecordTypeDrawArc,
    EmfPlusRecordTypeFillRegion,
    EmfPlusRecordTypeFillPath,
    EmfPlusRecordTypeDrawPath,
    EmfPlusRecordTypeFillClosedCurve,
    EmfPlusRecordTypeDrawClosedCurve,
    EmfPlusRecordTypeDrawCurve,
    EmfPlusRecordTypeDrawBeziers,
    EmfPlusRecordTypeDrawImage,
    EmfPlusRecordTypeDrawImagePoints,
    EmfPlusRecordTypeDrawString,

    // Graphics State Records

    EmfPlusRecordTypeSetRenderingOrigin,
    EmfPlusRecordTypeSetAntiAliasMode,
    EmfPlusRecordTypeSetTextRenderingHint,
    EmfPlusRecordTypeSetTextContrast,
    EmfPlusRecordTypeSetInterpolationMode,
    EmfPlusRecordTypeSetPixelOffsetMode,
    EmfPlusRecordTypeSetCompositingMode,
    EmfPlusRecordTypeSetCompositingQuality,
    EmfPlusRecordTypeSave,
    EmfPlusRecordTypeRestore,
    EmfPlusRecordTypeBeginContainer,
    EmfPlusRecordTypeBeginContainerNoParams,
    EmfPlusRecordTypeEndContainer,
    EmfPlusRecordTypeSetWorldTransform,
    EmfPlusRecordTypeResetWorldTransform,
    EmfPlusRecordTypeMultiplyWorldTransform,
    EmfPlusRecordTypeTranslateWorldTransform,
    EmfPlusRecordTypeScaleWorldTransform,
    EmfPlusRecordTypeRotateWorldTransform,
    EmfPlusRecordTypeSetPageTransform,
    EmfPlusRecordTypeResetClip,
    EmfPlusRecordTypeSetClipRect,
    EmfPlusRecordTypeSetClipPath,
    EmfPlusRecordTypeSetClipRegion,
    EmfPlusRecordTypeOffsetClip,

    EmfPlusRecordTypeDrawDriverString,

    EmfPlusRecordTotal,

    EmfPlusRecordTypeMax = EmfPlusRecordTotal-1,
    EmfPlusRecordTypeMin = EmfPlusRecordTypeHeader
  );
  TEmfPlusRecordType = EmfPlusRecordType;
{$ELSE}
type
  EmfPlusRecordType = Integer;
  // Since we have to enumerate GDI records right along with GDI+ records,
  // We list all the GDI records here so that they can be part of the
  // same enumeration type which is used in the enumeration callback.
(*  const
    WmfRecordTypeSetBkColor              = (META_SETBKCOLOR or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetBkMode               = (META_SETBKMODE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetMapMode              = (META_SETMAPMODE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetROP2                 = (META_SETROP2 or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetRelAbs               = (META_SETRELABS or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetPolyFillMode         = (META_SETPOLYFILLMODE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetStretchBltMode       = (META_SETSTRETCHBLTMODE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetTextCharExtra        = (META_SETTEXTCHAREXTRA or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetTextColor            = (META_SETTEXTCOLOR or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetTextJustification    = (META_SETTEXTJUSTIFICATION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetWindowOrg            = (META_SETWINDOWORG or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetWindowExt            = (META_SETWINDOWEXT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetViewportOrg          = (META_SETVIEWPORTORG or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetViewportExt          = (META_SETVIEWPORTEXT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeOffsetWindowOrg         = (META_OFFSETWINDOWORG or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeScaleWindowExt          = (META_SCALEWINDOWEXT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeOffsetViewportOrg       = (META_OFFSETVIEWPORTORG or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeScaleViewportExt        = (META_SCALEVIEWPORTEXT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeLineTo                  = (META_LINETO or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeMoveTo                  = (META_MOVETO or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeExcludeClipRect         = (META_EXCLUDECLIPRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeIntersectClipRect       = (META_INTERSECTCLIPRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeArc                     = (META_ARC or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeEllipse                 = (META_ELLIPSE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeFloodFill               = (META_FLOODFILL or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePie                     = (META_PIE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeRectangle               = (META_RECTANGLE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeRoundRect               = (META_ROUNDRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePatBlt                  = (META_PATBLT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSaveDC                  = (META_SAVEDC or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetPixel                = (META_SETPIXEL or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeOffsetClipRgn           = (META_OFFSETCLIPRGN or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeTextOut                 = (META_TEXTOUT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeBitBlt                  = (META_BITBLT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeStretchBlt              = (META_STRETCHBLT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePolygon                 = (META_POLYGON or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePolyline                = (META_POLYLINE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeEscape                  = (META_ESCAPE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeRestoreDC               = (META_RESTOREDC or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeFillRegion              = (META_FILLREGION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeFrameRegion             = (META_FRAMEREGION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeInvertRegion            = (META_INVERTREGION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePaintRegion             = (META_PAINTREGION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSelectClipRegion        = (META_SELECTCLIPREGION or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSelectObject            = (META_SELECTOBJECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetTextAlign            = (META_SETTEXTALIGN or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeDrawText                = ($062F or GDIP_WMF_RECORD_BASE);  // META_DRAWTEXT
    WmfRecordTypeChord                   = (META_CHORD or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetMapperFlags          = (META_SETMAPPERFLAGS or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeExtTextOut              = (META_EXTTEXTOUT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetDIBToDev             = (META_SETDIBTODEV or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSelectPalette           = (META_SELECTPALETTE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeRealizePalette          = (META_REALIZEPALETTE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeAnimatePalette          = (META_ANIMATEPALETTE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetPalEntries           = (META_SETPALENTRIES or GDIP_WMF_RECORD_BASE);
    WmfRecordTypePolyPolygon             = (META_POLYPOLYGON or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeResizePalette           = (META_RESIZEPALETTE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeDIBBitBlt               = (META_DIBBITBLT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeDIBStretchBlt           = (META_DIBSTRETCHBLT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeDIBCreatePatternBrush   = (META_DIBCREATEPATTERNBRUSH or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeStretchDIB              = (META_STRETCHDIB or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeExtFloodFill            = (META_EXTFLOODFILL or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeSetLayout               = ($0149 or GDIP_WMF_RECORD_BASE);  // META_SETLAYOUT
    WmfRecordTypeResetDC                 = ($014C or GDIP_WMF_RECORD_BASE);  // META_RESETDC
    WmfRecordTypeStartDoc                = ($014D or GDIP_WMF_RECORD_BASE);  // META_STARTDOC
    WmfRecordTypeStartPage               = ($004F or GDIP_WMF_RECORD_BASE);  // META_STARTPAGE
    WmfRecordTypeEndPage                 = ($0050 or GDIP_WMF_RECORD_BASE);  // META_ENDPAGE
    WmfRecordTypeAbortDoc                = ($0052 or GDIP_WMF_RECORD_BASE);  // META_ABORTDOC
    WmfRecordTypeEndDoc                  = ($005E or GDIP_WMF_RECORD_BASE);  // META_ENDDOC
    WmfRecordTypeDeleteObject            = (META_DELETEOBJECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreatePalette           = (META_CREATEPALETTE or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreateBrush             = ($00F8 or GDIP_WMF_RECORD_BASE);  // META_CREATEBRUSH
    WmfRecordTypeCreatePatternBrush      = (META_CREATEPATTERNBRUSH or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreatePenIndirect       = (META_CREATEPENINDIRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreateFontIndirect      = (META_CREATEFONTINDIRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreateBrushIndirect     = (META_CREATEBRUSHINDIRECT or GDIP_WMF_RECORD_BASE);
    WmfRecordTypeCreateBitmapIndirect    = ($02FD or GDIP_WMF_RECORD_BASE);  // META_CREATEBITMAPINDIRECT
    WmfRecordTypeCreateBitmap            = ($06FE or GDIP_WMF_RECORD_BASE);  // META_CREATEBITMAP
    WmfRecordTypeCreateRegion            = (META_CREATEREGION or GDIP_WMF_RECORD_BASE);

    EmfRecordTypeHeader                  = EMR_HEADER;
    EmfRecordTypePolyBezier              = EMR_POLYBEZIER;
    EmfRecordTypePolygon                 = EMR_POLYGON;
    EmfRecordTypePolyline                = EMR_POLYLINE;
    EmfRecordTypePolyBezierTo            = EMR_POLYBEZIERTO;
    EmfRecordTypePolyLineTo              = EMR_POLYLINETO;
    EmfRecordTypePolyPolyline            = EMR_POLYPOLYLINE;
    EmfRecordTypePolyPolygon             = EMR_POLYPOLYGON;
    EmfRecordTypeSetWindowExtEx          = EMR_SETWINDOWEXTEX;
    EmfRecordTypeSetWindowOrgEx          = EMR_SETWINDOWORGEX;
    EmfRecordTypeSetViewportExtEx        = EMR_SETVIEWPORTEXTEX;
    EmfRecordTypeSetViewportOrgEx        = EMR_SETVIEWPORTORGEX;
    EmfRecordTypeSetBrushOrgEx           = EMR_SETBRUSHORGEX;
    EmfRecordTypeEOF                     = EMR_EOF;
    EmfRecordTypeSetPixelV               = EMR_SETPIXELV;
    EmfRecordTypeSetMapperFlags          = EMR_SETMAPPERFLAGS;
    EmfRecordTypeSetMapMode              = EMR_SETMAPMODE;
    EmfRecordTypeSetBkMode               = EMR_SETBKMODE;
    EmfRecordTypeSetPolyFillMode         = EMR_SETPOLYFILLMODE;
    EmfRecordTypeSetROP2                 = EMR_SETROP2;
    EmfRecordTypeSetStretchBltMode       = EMR_SETSTRETCHBLTMODE;
    EmfRecordTypeSetTextAlign            = EMR_SETTEXTALIGN;
    EmfRecordTypeSetColorAdjustment      = EMR_SETCOLORADJUSTMENT;
    EmfRecordTypeSetTextColor            = EMR_SETTEXTCOLOR;
    EmfRecordTypeSetBkColor              = EMR_SETBKCOLOR;
    EmfRecordTypeOffsetClipRgn           = EMR_OFFSETCLIPRGN;
    EmfRecordTypeMoveToEx                = EMR_MOVETOEX;
    EmfRecordTypeSetMetaRgn              = EMR_SETMETARGN;
    EmfRecordTypeExcludeClipRect         = EMR_EXCLUDECLIPRECT;
    EmfRecordTypeIntersectClipRect       = EMR_INTERSECTCLIPRECT;
    EmfRecordTypeScaleViewportExtEx      = EMR_SCALEVIEWPORTEXTEX;
    EmfRecordTypeScaleWindowExtEx        = EMR_SCALEWINDOWEXTEX;
    EmfRecordTypeSaveDC                  = EMR_SAVEDC;
    EmfRecordTypeRestoreDC               = EMR_RESTOREDC;
    EmfRecordTypeSetWorldTransform       = EMR_SETWORLDTRANSFORM;
    EmfRecordTypeModifyWorldTransform    = EMR_MODIFYWORLDTRANSFORM;
    EmfRecordTypeSelectObject            = EMR_SELECTOBJECT;
    EmfRecordTypeCreatePen               = EMR_CREATEPEN;
    EmfRecordTypeCreateBrushIndirect     = EMR_CREATEBRUSHINDIRECT;
    EmfRecordTypeDeleteObject            = EMR_DELETEOBJECT;
    EmfRecordTypeAngleArc                = EMR_ANGLEARC;
    EmfRecordTypeEllipse                 = EMR_ELLIPSE;
    EmfRecordTypeRectangle               = EMR_RECTANGLE;
    EmfRecordTypeRoundRect               = EMR_ROUNDRECT;
    EmfRecordTypeArc                     = EMR_ARC;
    EmfRecordTypeChord                   = EMR_CHORD;
    EmfRecordTypePie                     = EMR_PIE;
    EmfRecordTypeSelectPalette           = EMR_SELECTPALETTE;
    EmfRecordTypeCreatePalette           = EMR_CREATEPALETTE;
    EmfRecordTypeSetPaletteEntries       = EMR_SETPALETTEENTRIES;
    EmfRecordTypeResizePalette           = EMR_RESIZEPALETTE;
    EmfRecordTypeRealizePalette          = EMR_REALIZEPALETTE;
    EmfRecordTypeExtFloodFill            = EMR_EXTFLOODFILL;
    EmfRecordTypeLineTo                  = EMR_LINETO;
    EmfRecordTypeArcTo                   = EMR_ARCTO;
    EmfRecordTypePolyDraw                = EMR_POLYDRAW;
    EmfRecordTypeSetArcDirection         = EMR_SETARCDIRECTION;
    EmfRecordTypeSetMiterLimit           = EMR_SETMITERLIMIT;
    EmfRecordTypeBeginPath               = EMR_BEGINPATH;
    EmfRecordTypeEndPath                 = EMR_ENDPATH;
    EmfRecordTypeCloseFigure             = EMR_CLOSEFIGURE;
    EmfRecordTypeFillPath                = EMR_FILLPATH;
    EmfRecordTypeStrokeAndFillPath       = EMR_STROKEANDFILLPATH;
    EmfRecordTypeStrokePath              = EMR_STROKEPATH;
    EmfRecordTypeFlattenPath             = EMR_FLATTENPATH;
    EmfRecordTypeWidenPath               = EMR_WIDENPATH;
    EmfRecordTypeSelectClipPath          = EMR_SELECTCLIPPATH;
    EmfRecordTypeAbortPath               = EMR_ABORTPATH;
    EmfRecordTypeReserved_069            = 69;  // Not Used
    EmfRecordTypeGdiComment              = EMR_GDICOMMENT;
    EmfRecordTypeFillRgn                 = EMR_FILLRGN;
    EmfRecordTypeFrameRgn                = EMR_FRAMERGN;
    EmfRecordTypeInvertRgn               = EMR_INVERTRGN;
    EmfRecordTypePaintRgn                = EMR_PAINTRGN;
    EmfRecordTypeExtSelectClipRgn        = EMR_EXTSELECTCLIPRGN;
    EmfRecordTypeBitBlt                  = EMR_BITBLT;
    EmfRecordTypeStretchBlt              = EMR_STRETCHBLT;
    EmfRecordTypeMaskBlt                 = EMR_MASKBLT;
    EmfRecordTypePlgBlt                  = EMR_PLGBLT;
    EmfRecordTypeSetDIBitsToDevice       = EMR_SETDIBITSTODEVICE;
    EmfRecordTypeStretchDIBits           = EMR_STRETCHDIBITS;
    EmfRecordTypeExtCreateFontIndirect   = EMR_EXTCREATEFONTINDIRECTW;
    EmfRecordTypeExtTextOutA             = EMR_EXTTEXTOUTA;
    EmfRecordTypeExtTextOutW             = EMR_EXTTEXTOUTW;
    EmfRecordTypePolyBezier16            = EMR_POLYBEZIER16;
    EmfRecordTypePolygon16               = EMR_POLYGON16;
    EmfRecordTypePolyline16              = EMR_POLYLINE16;
    EmfRecordTypePolyBezierTo16          = EMR_POLYBEZIERTO16;
    EmfRecordTypePolylineTo16            = EMR_POLYLINETO16;
    EmfRecordTypePolyPolyline16          = EMR_POLYPOLYLINE16;
    EmfRecordTypePolyPolygon16           = EMR_POLYPOLYGON16;
    EmfRecordTypePolyDraw16              = EMR_POLYDRAW16;
    EmfRecordTypeCreateMonoBrush         = EMR_CREATEMONOBRUSH;
    EmfRecordTypeCreateDIBPatternBrushPt = EMR_CREATEDIBPATTERNBRUSHPT;
    EmfRecordTypeExtCreatePen            = EMR_EXTCREATEPEN;
    EmfRecordTypePolyTextOutA            = EMR_POLYTEXTOUTA;
    EmfRecordTypePolyTextOutW            = EMR_POLYTEXTOUTW;
    EmfRecordTypeSetICMMode              = 98;  // EMR_SETICMMODE,
    EmfRecordTypeCreateColorSpace        = 99;  // EMR_CREATECOLORSPACE,
    EmfRecordTypeSetColorSpace           = 100; // EMR_SETCOLORSPACE,
    EmfRecordTypeDeleteColorSpace        = 101; // EMR_DELETECOLORSPACE,
    EmfRecordTypeGLSRecord               = 102; // EMR_GLSRECORD,
    EmfRecordTypeGLSBoundedRecord        = 103; // EMR_GLSBOUNDEDRECORD,
    EmfRecordTypePixelFormat             = 104; // EMR_PIXELFORMAT,
    EmfRecordTypeDrawEscape              = 105; // EMR_RESERVED_105,
    EmfRecordTypeExtEscape               = 106; // EMR_RESERVED_106,
    EmfRecordTypeStartDoc                = 107; // EMR_RESERVED_107,
    EmfRecordTypeSmallTextOut            = 108; // EMR_RESERVED_108,
    EmfRecordTypeForceUFIMapping         = 109; // EMR_RESERVED_109,
    EmfRecordTypeNamedEscape             = 110; // EMR_RESERVED_110,
    EmfRecordTypeColorCorrectPalette     = 111; // EMR_COLORCORRECTPALETTE,
    EmfRecordTypeSetICMProfileA          = 112; // EMR_SETICMPROFILEA,
    EmfRecordTypeSetICMProfileW          = 113; // EMR_SETICMPROFILEW,
    EmfRecordTypeAlphaBlend              = 114; // EMR_ALPHABLEND,
    EmfRecordTypeSetLayout               = 115; // EMR_SETLAYOUT,
    EmfRecordTypeTransparentBlt          = 116; // EMR_TRANSPARENTBLT,
    EmfRecordTypeReserved_117            = 117; // Not Used
    EmfRecordTypeGradientFill            = 118; // EMR_GRADIENTFILL,
    EmfRecordTypeSetLinkedUFIs           = 119; // EMR_RESERVED_119,
    EmfRecordTypeSetTextJustification    = 120; // EMR_RESERVED_120,
    EmfRecordTypeColorMatchToTargetW     = 121; // EMR_COLORMATCHTOTARGETW,
    EmfRecordTypeCreateColorSpaceW       = 122; // EMR_CREATECOLORSPACEW,
    EmfRecordTypeMax                     = 122;
    EmfRecordTypeMin                     = 1;

    // That is the END of the GDI EMF records.

    // Now we start the list of EMF+ records.  We leave quite
    // a bit of room here for the addition of any new GDI
    // records that may be added later.

    EmfPlusRecordTypeInvalid   = GDIP_EMFPLUS_RECORD_BASE;
    EmfPlusRecordTypeHeader    = GDIP_EMFPLUS_RECORD_BASE + 1;
    EmfPlusRecordTypeEndOfFile = GDIP_EMFPLUS_RECORD_BASE + 2;

    EmfPlusRecordTypeComment   = GDIP_EMFPLUS_RECORD_BASE + 3;

    EmfPlusRecordTypeGetDC     = GDIP_EMFPLUS_RECORD_BASE + 4;

    EmfPlusRecordTypeMultiFormatStart   = GDIP_EMFPLUS_RECORD_BASE + 5;
    EmfPlusRecordTypeMultiFormatSection = GDIP_EMFPLUS_RECORD_BASE + 6;
    EmfPlusRecordTypeMultiFormatEnd     = GDIP_EMFPLUS_RECORD_BASE + 7;

    // For all persistent objects

    EmfPlusRecordTypeObject = GDIP_EMFPLUS_RECORD_BASE + 8;

    // Drawing Records

    EmfPlusRecordTypeClear           = GDIP_EMFPLUS_RECORD_BASE + 9;
    EmfPlusRecordTypeFillRects       = GDIP_EMFPLUS_RECORD_BASE + 10;
    EmfPlusRecordTypeDrawRects       = GDIP_EMFPLUS_RECORD_BASE + 11;
    EmfPlusRecordTypeFillPolygon     = GDIP_EMFPLUS_RECORD_BASE + 12;
    EmfPlusRecordTypeDrawLines       = GDIP_EMFPLUS_RECORD_BASE + 13;
    EmfPlusRecordTypeFillEllipse     = GDIP_EMFPLUS_RECORD_BASE + 14;
    EmfPlusRecordTypeDrawEllipse     = GDIP_EMFPLUS_RECORD_BASE + 15;
    EmfPlusRecordTypeFillPie         = GDIP_EMFPLUS_RECORD_BASE + 16;
    EmfPlusRecordTypeDrawPie         = GDIP_EMFPLUS_RECORD_BASE + 17;
    EmfPlusRecordTypeDrawArc         = GDIP_EMFPLUS_RECORD_BASE + 18;
    EmfPlusRecordTypeFillRegion      = GDIP_EMFPLUS_RECORD_BASE + 19;
    EmfPlusRecordTypeFillPath        = GDIP_EMFPLUS_RECORD_BASE + 20;
    EmfPlusRecordTypeDrawPath        = GDIP_EMFPLUS_RECORD_BASE + 21;
    EmfPlusRecordTypeFillClosedCurve = GDIP_EMFPLUS_RECORD_BASE + 22;
    EmfPlusRecordTypeDrawClosedCurve = GDIP_EMFPLUS_RECORD_BASE + 23;
    EmfPlusRecordTypeDrawCurve       = GDIP_EMFPLUS_RECORD_BASE + 24;
    EmfPlusRecordTypeDrawBeziers     = GDIP_EMFPLUS_RECORD_BASE + 25;
    EmfPlusRecordTypeDrawImage       = GDIP_EMFPLUS_RECORD_BASE + 26;
    EmfPlusRecordTypeDrawImagePoints = GDIP_EMFPLUS_RECORD_BASE + 27;
    EmfPlusRecordTypeDrawString      = GDIP_EMFPLUS_RECORD_BASE + 28;

    // Graphics State Records

    EmfPlusRecordTypeSetRenderingOrigin      = GDIP_EMFPLUS_RECORD_BASE + 29;
    EmfPlusRecordTypeSetAntiAliasMode        = GDIP_EMFPLUS_RECORD_BASE + 30;
    EmfPlusRecordTypeSetTextRenderingHint    = GDIP_EMFPLUS_RECORD_BASE + 31;
    EmfPlusRecordTypeSetTextContrast         = GDIP_EMFPLUS_RECORD_BASE + 32;
    EmfPlusRecordTypeSetInterpolationMode    = GDIP_EMFPLUS_RECORD_BASE + 33;
    EmfPlusRecordTypeSetPixelOffsetMode      = GDIP_EMFPLUS_RECORD_BASE + 34;
    EmfPlusRecordTypeSetCompositingMode      = GDIP_EMFPLUS_RECORD_BASE + 35;
    EmfPlusRecordTypeSetCompositingQuality   = GDIP_EMFPLUS_RECORD_BASE + 36;
    EmfPlusRecordTypeSave                    = GDIP_EMFPLUS_RECORD_BASE + 37;
    EmfPlusRecordTypeRestore                 = GDIP_EMFPLUS_RECORD_BASE + 38;
    EmfPlusRecordTypeBeginContainer          = GDIP_EMFPLUS_RECORD_BASE + 39;
    EmfPlusRecordTypeBeginContainerNoParams  = GDIP_EMFPLUS_RECORD_BASE + 40;
    EmfPlusRecordTypeEndContainer            = GDIP_EMFPLUS_RECORD_BASE + 41;
    EmfPlusRecordTypeSetWorldTransform       = GDIP_EMFPLUS_RECORD_BASE + 42;
    EmfPlusRecordTypeResetWorldTransform     = GDIP_EMFPLUS_RECORD_BASE + 43;
    EmfPlusRecordTypeMultiplyWorldTransform  = GDIP_EMFPLUS_RECORD_BASE + 44;
    EmfPlusRecordTypeTranslateWorldTransform = GDIP_EMFPLUS_RECORD_BASE + 45;
    EmfPlusRecordTypeScaleWorldTransform     = GDIP_EMFPLUS_RECORD_BASE + 46;
    EmfPlusRecordTypeRotateWorldTransform    = GDIP_EMFPLUS_RECORD_BASE + 47;
    EmfPlusRecordTypeSetPageTransform        = GDIP_EMFPLUS_RECORD_BASE + 48;
    EmfPlusRecordTypeResetClip               = GDIP_EMFPLUS_RECORD_BASE + 49;
    EmfPlusRecordTypeSetClipRect             = GDIP_EMFPLUS_RECORD_BASE + 50;
    EmfPlusRecordTypeSetClipPath             = GDIP_EMFPLUS_RECORD_BASE + 51;
    EmfPlusRecordTypeSetClipRegion           = GDIP_EMFPLUS_RECORD_BASE + 52;
    EmfPlusRecordTypeOffsetClip              = GDIP_EMFPLUS_RECORD_BASE + 53;

    EmfPlusRecordTypeDrawDriverString        = GDIP_EMFPLUS_RECORD_BASE + 54;

    EmfPlusRecordTotal                       = GDIP_EMFPLUS_RECORD_BASE + 55;

    EmfPlusRecordTypeMax = EmfPlusRecordTotal-1;
    EmfPlusRecordTypeMin = EmfPlusRecordTypeHeader;*)

type
  TEmfPlusRecordType = EmfPlusRecordType;
{$ENDIF}
//---------------------------------------------------------------------------
// StringFormatFlags
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// String format flags
//
//  DirectionRightToLeft          - For horizontal text, the reading order is
//                                  right to left. This value is called
//                                  the base embedding level by the Unicode
//                                  bidirectional engine.
//                                  For vertical text, columns are read from
//                                  right to left.
//                                  By default, horizontal or vertical text is
//                                  read from left to right.
//
//  DirectionVertical             - Individual lines of text are vertical. In
//                                  each line, characters progress from top to
//                                  bottom.
//                                  By default, lines of text are horizontal,
//                                  each new line below the previous line.
//
//  NoFitBlackBox                 - Allows parts of glyphs to overhang the
//                                  bounding rectangle.
//                                  By default glyphs are first aligned
//                                  inside the margines, then any glyphs which
//                                  still overhang the bounding box are
//                                  repositioned to avoid any overhang.
//                                  For example when an italic
//                                  lower case letter f in a font such as
//                                  Garamond is aligned at the far left of a
//                                  rectangle, the lower part of the f will
//                                  reach slightly further left than the left
//                                  edge of the rectangle. Setting this flag
//                                  will ensure the character aligns visually
//                                  with the lines above and below, but may
//                                  cause some pixels outside the formatting
//                                  rectangle to be clipped or painted.
//
//  DisplayFormatControl          - Causes control characters such as the
//                                  left-to-right mark to be shown in the
//                                  output with a representative glyph.
//
//  NoFontFallback                - Disables fallback to alternate fonts for
//                                  characters not supported in the requested
//                                  font. Any missing characters will be
//                                  be displayed with the fonts missing glyph,
//                                  usually an open square.
//
//  NoWrap                        - Disables wrapping of text between lines
//                                  when formatting within a rectangle.
//                                  NoWrap is implied when a point is passed
//                                  instead of a rectangle, or when the
//                                  specified rectangle has a zero line length.
//
//  NoClip                        - By default text is clipped to the
//                                  formatting rectangle. Setting NoClip
//                                  allows overhanging pixels to affect the
//                                  device outside the formatting rectangle.
//                                  Pixels at the end of the line may be
//                                  affected if the glyphs overhang their
//                                  cells, and either the NoFitBlackBox flag
//                                  has been set, or the glyph extends to far
//                                  to be fitted.
//                                  Pixels above/before the first line or
//                                  below/after the last line may be affected
//                                  if the glyphs extend beyond their cell
//                                  ascent / descent. This can occur rarely
//                                  with unusual diacritic mark combinations.

//---------------------------------------------------------------------------

  StringFormatFlags = Integer;
  const
    StringFormatFlagsDirectionRightToLeft        = $00000001;
    StringFormatFlagsDirectionVertical           = $00000002;
    StringFormatFlagsNoFitBlackBox               = $00000004;
    StringFormatFlagsDisplayFormatControl        = $00000020;
    StringFormatFlagsNoFontFallback              = $00000400;
    StringFormatFlagsMeasureTrailingSpaces       = $00000800;
    StringFormatFlagsNoWrap                      = $00001000;
    StringFormatFlagsLineLimit                   = $00002000;

    StringFormatFlagsNoClip                      = $00004000;

Type
  TStringFormatFlags = StringFormatFlags;

//---------------------------------------------------------------------------
// StringTrimming
//---------------------------------------------------------------------------

  StringTrimming = (
    StringTrimmingNone,
    StringTrimmingCharacter,
    StringTrimmingWord,
    StringTrimmingEllipsisCharacter,
    StringTrimmingEllipsisWord,
    StringTrimmingEllipsisPath
  );
  TStringTrimming = StringTrimming;

//---------------------------------------------------------------------------
// National language digit substitution
//---------------------------------------------------------------------------

  StringDigitSubstitute = (
    StringDigitSubstituteUser,          // As NLS setting
    StringDigitSubstituteNone,
    StringDigitSubstituteNational,
    StringDigitSubstituteTraditional
  );
  TStringDigitSubstitute = StringDigitSubstitute;
  PStringDigitSubstitute = ^TStringDigitSubstitute;

//---------------------------------------------------------------------------
// Hotkey prefix interpretation
//---------------------------------------------------------------------------

  HotkeyPrefix = (
    HotkeyPrefixNone,
    HotkeyPrefixShow,
    HotkeyPrefixHide
  );
  THotkeyPrefix = HotkeyPrefix;

//---------------------------------------------------------------------------
// String alignment flags
//---------------------------------------------------------------------------

  StringAlignment = (
    // Left edge for left-to-right text,
    // right for right-to-left text,
    // and top for vertical
    StringAlignmentNear,
    StringAlignmentCenter,
    StringAlignmentFar
  );
  TStringAlignment = StringAlignment;

//---------------------------------------------------------------------------
// DriverStringOptions
//---------------------------------------------------------------------------

  DriverStringOptions = Integer;
  const
    DriverStringOptionsCmapLookup             = 1;
    DriverStringOptionsVertical               = 2;
    DriverStringOptionsRealizedAdvance        = 4;
    DriverStringOptionsLimitSubpixel          = 8;

type
  TDriverStringOptions = DriverStringOptions;

//---------------------------------------------------------------------------
// Flush Intention flags
//---------------------------------------------------------------------------

  FlushIntention = (
    FlushIntentionFlush,  // Flush all batched rendering operations
    FlushIntentionSync    // Flush all batched rendering operations
                          // and wait for them to complete
  );
  TFlushIntention = FlushIntention;

//---------------------------------------------------------------------------
// Image encoder parameter related types
//---------------------------------------------------------------------------

  EncoderParameterValueType = Integer;
  const
    EncoderParameterValueTypeByte          : Integer = 1;    // 8-bit unsigned int
    EncoderParameterValueTypeASCII         : Integer = 2;    // 8-bit byte containing one 7-bit ASCII
                                                             // code. NULL terminated.
    EncoderParameterValueTypeShort         : Integer = 3;    // 16-bit unsigned int
    EncoderParameterValueTypeLong          : Integer = 4;    // 32-bit unsigned int
    EncoderParameterValueTypeRational      : Integer = 5;    // Two Longs. The first Long is the
                                                             // numerator, the second Long expresses the
                                                             // denomintor.
    EncoderParameterValueTypeLongRange     : Integer = 6;    // Two longs which specify a range of
                                                             // integer values. The first Long specifies
                                                             // the lower end and the second one
                                                             // specifies the higher end. All values
                                                             // are inclusive at both ends
    EncoderParameterValueTypeUndefined     : Integer = 7;    // 8-bit byte that can take any value
                                                             // depending on field definition
    EncoderParameterValueTypeRationalRange : Integer = 8;    // Two Rationals. The first Rational
                                                             // specifies the lower end and the second
                                                             // specifies the higher end. All values
                                                             // are inclusive at both ends
type
  TEncoderParameterValueType = EncoderParameterValueType;

//---------------------------------------------------------------------------
// Image encoder value types
//---------------------------------------------------------------------------

  EncoderValue = (
    EncoderValueColorTypeCMYK,
    EncoderValueColorTypeYCCK,
    EncoderValueCompressionLZW,
    EncoderValueCompressionCCITT3,
    EncoderValueCompressionCCITT4,
    EncoderValueCompressionRle,
    EncoderValueCompressionNone,
    EncoderValueScanMethodInterlaced,
    EncoderValueScanMethodNonInterlaced,
    EncoderValueVersionGif87,
    EncoderValueVersionGif89,
    EncoderValueRenderProgressive,
    EncoderValueRenderNonProgressive,
    EncoderValueTransformRotate90,
    EncoderValueTransformRotate180,
    EncoderValueTransformRotate270,
    EncoderValueTransformFlipHorizontal,
    EncoderValueTransformFlipVertical,
    EncoderValueMultiFrame,
    EncoderValueLastFrame,
    EncoderValueFlush,
    EncoderValueFrameDimensionTime,
    EncoderValueFrameDimensionResolution,
    EncoderValueFrameDimensionPage
  );
  TEncoderValue = EncoderValue;

//---------------------------------------------------------------------------
// Conversion of Emf To WMF Bits flags
//---------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  EmfToWmfBitsFlags = (
    EmfToWmfBitsFlagsDefault          = $00000000,
    EmfToWmfBitsFlagsEmbedEmf         = $00000001,
    EmfToWmfBitsFlagsIncludePlaceable = $00000002,
    EmfToWmfBitsFlagsNoXORClip        = $00000004
  );
  TEmfToWmfBitsFlags = EmfToWmfBitsFlags;
{$ELSE}
  EmfToWmfBitsFlags = Integer;
  const
    EmfToWmfBitsFlagsDefault          = $00000000;
    EmfToWmfBitsFlagsEmbedEmf         = $00000001;
    EmfToWmfBitsFlagsIncludePlaceable = $00000002;
    EmfToWmfBitsFlagsNoXORClip        = $00000004;
    
type
  TEmfToWmfBitsFlags = EmfToWmfBitsFlags;
{$ENDIF}

//--------------------------------------------------------------------------
// Callback functions
//--------------------------------------------------------------------------

  ImageAbort = function: BOOL; stdcall;
  DrawImageAbort         = ImageAbort;
  GetThumbnailImageAbort = ImageAbort;


  // Callback for EnumerateMetafile methods.  The parameters are:

  //      recordType      WMF, EMF, or EMF+ record type
  //      flags           (always 0 for WMF/EMF records)
  //      dataSize        size of the record data (in bytes), or 0 if no data
  //      data            pointer to the record data, or NULL if no data
  //      callbackData    pointer to callbackData, if any

  // This method can then call Metafile::PlayRecord to play the
  // record that was just enumerated.  If this method  returns
  // FALSE, the enumeration process is aborted.  Otherwise, it continues.

  EnumerateMetafileProc = function(recordType: EmfPlusRecordType; flags: UINT;
    dataSize: UINT; data: PBYTE; callbackData: pointer): BOOL; stdcall;

//--------------------------------------------------------------------------
// Primitive data types
//
// NOTE:
//  Types already defined in standard header files:
//      INT8
//      UINT8
//      INT16
//      UINT16
//      INT32
//      UINT32
//      INT64
//      UINT64
//
//  Avoid using the following types:
//      LONG - use INT
//      ULONG - use UINT
//      DWORD - use UINT32
//--------------------------------------------------------------------------

const
  { from float.h }
  FLT_MAX =  3.402823466e+38; // max value
  FLT_MIN =  1.175494351e-38; // min positive value

  REAL_MAX           = FLT_MAX;
  REAL_MIN           = FLT_MIN;
  REAL_TOLERANCE     = (FLT_MIN * 100);
  REAL_EPSILON       = 1.192092896e-07;        // FLT_EPSILON

//--------------------------------------------------------------------------
// Status return values from GDI+ methods
//--------------------------------------------------------------------------
type
  Status = (
    Ok,
    GenericError,
    InvalidParameter,
    OutOfMemory,
    ObjectBusy,
    InsufficientBuffer,
    NotImplemented,
    Win32Error,
    WrongState,
    Aborted,
    FileNotFound,
    ValueOverflow,
    AccessDenied,
    UnknownImageFormat,
    FontFamilyNotFound,
    FontStyleNotFound,
    NotTrueTypeFont,
    UnsupportedGdiplusVersion,
    GdiplusNotInitialized,
    PropertyNotFound,
    PropertyNotSupported
  );
  TStatus = Status;

//--------------------------------------------------------------------------
// Represents a dimension in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

type
  PGPSizeF = ^TGPSizeF;
  TGPSizeF = packed record
    Width  : Single;
    Height : Single;
  end;

//--------------------------------------------------------------------------
// Represents a dimension in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

type
  PGPSize = ^TGPSize;
  TGPSize = packed record
    Width  : Integer;
    Height : Integer;
  end;

//--------------------------------------------------------------------------
// Represents a location in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

type
  PGPPointF = ^TGPPointF;
  TGPPointF = packed record
    X : Single;
    Y : Single;
  end;
  TPointFDynArray = array of TGPPointF;

//--------------------------------------------------------------------------
// Represents a location in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

type
  PGPPoint = ^TGPPoint;
  TGPPoint = packed record
    X : Integer;
    Y : Integer;
  end;
  TPointDynArray = array of TGPPoint;

//--------------------------------------------------------------------------
// Represents a rectangle in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

type
  PGPRectF = ^TGPRectF;
  TGPRectF = packed record
    X     : Single;
    Y     : Single;
    Width : Single;
    Height: Single;
  end;
  TRectFDynArray = array of TGPRectF;

type
  PGPRect = ^TGPRect;
  TGPRect = packed record
    X     : Integer;
    Y     : Integer;
    Width : Integer;
    Height: Integer;
  end;
  TRectDynArray = array of TGPRect;

type
  TPathData = packed class
  public
    Count  : Integer;
    Points : PGPPointF;
    Types  : PBYTE;
    constructor Create;
    destructor destroy; override;
  end;

  PCharacterRange = ^TCharacterRange;
  TCharacterRange = packed record
    First  : Integer;
    Length : Integer;
  end;

(**************************************************************************
*
*   GDI+ Startup and Shutdown APIs
*
**************************************************************************)
type
  DebugEventLevel = (
    DebugEventLevelFatal,
    DebugEventLevelWarning
  );
  TDebugEventLevel = DebugEventLevel;

  // Callback function that GDI+ can call, on debug builds, for assertions
  // and warnings.

  DebugEventProc = procedure(level: DebugEventLevel; message: PChar); stdcall;

  // Notification functions which the user must call appropriately if
  // "SuppressBackgroundThread" (below) is set.

  NotificationHookProc = function(out token: ULONG): Status; stdcall;
  NotificationUnhookProc = procedure(token: ULONG); stdcall;

  // Input structure for GdiplusStartup

  GdiplusStartupInput = packed record
    GdiplusVersion          : Cardinal;       // Must be 1
    DebugEventCallback      : DebugEventProc; // Ignored on free builds
    SuppressBackgroundThread: BOOL;           // FALSE unless you're prepared to call
                                              // the hook/unhook functions properly
    SuppressExternalCodecs  : BOOL;           // FALSE unless you want GDI+ only to use
  end;                                        // its internal image codecs.
  TGdiplusStartupInput = GdiplusStartupInput;
  PGdiplusStartupInput = ^TGdiplusStartupInput;

  // Output structure for GdiplusStartup()

  GdiplusStartupOutput = packed record
    // The following 2 fields are NULL if SuppressBackgroundThread is FALSE.
    // Otherwise, they are functions which must be called appropriately to
    // replace the background thread.
    //
    // These should be called on the application's main message loop - i.e.
    // a message loop which is active for the lifetime of GDI+.
    // "NotificationHook" should be called before starting the loop,
    // and "NotificationUnhook" should be called after the loop ends.

    NotificationHook  : NotificationHookProc;
    NotificationUnhook: NotificationUnhookProc;
  end;
  TGdiplusStartupOutput = GdiplusStartupOutput;
  PGdiplusStartupOutput = ^TGdiplusStartupOutput;

  // GDI+ initialization. Must not be called from DllMain - can cause deadlock.
  //
  // Must be called before GDI+ API's or constructors are used.
  //
  // token  - may not be NULL - accepts a token to be passed in the corresponding
  //          GdiplusShutdown call.
  // input  - may not be NULL
  // output - may be NULL only if input->SuppressBackgroundThread is FALSE.

var
GdiplusStartup: function(out token: ULONG; input: PGdiplusStartupInput;
   output: PGdiplusStartupOutput): Status; stdcall;

  // GDI+ termination. Must be called before GDI+ is unloaded.
  // Must not be called from DllMain - can cause deadlock.
  //
  // GDI+ API's may not be called after GdiplusShutdown. Pay careful attention
  // to GDI+ object destructors.

GdiplusShutdown: procedure(token: ULONG); stdcall;  


type
  PARGB  = ^ARGB;
  ARGB   = DWORD;
  ARGB64 = Int64;

const
  ALPHA_SHIFT = 24;
  RED_SHIFT   = 16;
  GREEN_SHIFT = 8;
  BLUE_SHIFT  = 0;
  ALPHA_MASK  = (ARGB($ff) shl ALPHA_SHIFT);

  // In-memory pixel data formats:
  // bits 0-7 = format index
  // bits 8-15 = pixel size (in bits)
  // bits 16-23 = flags
  // bits 24-31 = reserved

type
  PixelFormat = Integer;
  TPixelFormat = PixelFormat;

const
  PixelFormatIndexed     = $00010000; // Indexes into a palette
  PixelFormatGDI         = $00020000; // Is a GDI-supported format
  PixelFormatAlpha       = $00040000; // Has an alpha component
  PixelFormatPAlpha      = $00080000; // Pre-multiplied alpha
  PixelFormatExtended    = $00100000; // Extended color 16 bits/channel
  PixelFormatCanonical   = $00200000;

  PixelFormatUndefined      = 0;
  PixelFormatDontCare       = 0;

  PixelFormat1bppIndexed    = (1  or ( 1 shl 8) or PixelFormatIndexed or PixelFormatGDI);
  PixelFormat4bppIndexed    = (2  or ( 4 shl 8) or PixelFormatIndexed or PixelFormatGDI);
  PixelFormat8bppIndexed    = (3  or ( 8 shl 8) or PixelFormatIndexed or PixelFormatGDI);
  PixelFormat16bppGrayScale = (4  or (16 shl 8) or PixelFormatExtended);
  PixelFormat16bppRGB555    = (5  or (16 shl 8) or PixelFormatGDI);
  PixelFormat16bppRGB565    = (6  or (16 shl 8) or PixelFormatGDI);
  PixelFormat16bppARGB1555  = (7  or (16 shl 8) or PixelFormatAlpha or PixelFormatGDI);
  PixelFormat24bppRGB       = (8  or (24 shl 8) or PixelFormatGDI);
  PixelFormat32bppRGB       = (9  or (32 shl 8) or PixelFormatGDI);
  PixelFormat32bppARGB      = (10 or (32 shl 8) or PixelFormatAlpha or PixelFormatGDI or PixelFormatCanonical);
  PixelFormat32bppPARGB     = (11 or (32 shl 8) or PixelFormatAlpha or PixelFormatPAlpha or PixelFormatGDI);
  PixelFormat48bppRGB       = (12 or (48 shl 8) or PixelFormatExtended);
  PixelFormat64bppARGB      = (13 or (64 shl 8) or PixelFormatAlpha  or PixelFormatCanonical or PixelFormatExtended);
  PixelFormat64bppPARGB     = (14 or (64 shl 8) or PixelFormatAlpha  or PixelFormatPAlpha or PixelFormatExtended);
  PixelFormatMax            = 15;

//--------------------------------------------------------------------------
// Determine if the Pixel Format is Canonical format:
//   PixelFormat32bppARGB
//   PixelFormat32bppPARGB
//   PixelFormat64bppARGB
//   PixelFormat64bppPARGB
//--------------------------------------------------------------------------

{$IFDEF DELPHI6_UP}
type
  PaletteFlags = (
    PaletteFlagsHasAlpha    = $0001,
    PaletteFlagsGrayScale   = $0002,
    PaletteFlagsHalftone    = $0004
  );
  TPaletteFlags = PaletteFlags;
{$ELSE}
type
  PaletteFlags = Integer;
  const
    PaletteFlagsHasAlpha    = $0001;
    PaletteFlagsGrayScale   = $0002;
    PaletteFlagsHalftone    = $0004;

type
  TPaletteFlags = PaletteFlags;
{$ENDIF}

  ColorPalette = packed record
    Flags  : UINT ;                 // Palette flags
    Count  : UINT ;                 // Number of color entries
    Entries: array [0..0] of ARGB ; // Palette color entries
  end;

  TColorPalette = ColorPalette;
  PColorPalette = ^TColorPalette;

//----------------------------------------------------------------------------
// Color mode
//----------------------------------------------------------------------------

  ColorMode = (
    ColorModeARGB32,
    ColorModeARGB64
  );
  TColorMode = ColorMode;

//----------------------------------------------------------------------------
// Color Channel flags 
//----------------------------------------------------------------------------

  ColorChannelFlags = (
    ColorChannelFlagsC,
    ColorChannelFlagsM,
    ColorChannelFlagsY,
    ColorChannelFlagsK,
    ColorChannelFlagsLast
  );
  TColorChannelFlags = ColorChannelFlags;

//----------------------------------------------------------------------------
// Color
//----------------------------------------------------------------------------

  // Common color constants
const
  aclAliceBlue            = $FFF0F8FF;
  aclAntiqueWhite         = $FFFAEBD7;
  aclAqua                 = $FF00FFFF;
  aclAquamarine           = $FF7FFFD4;
  aclAzure                = $FFF0FFFF;
  aclBeige                = $FFF5F5DC;
  aclBisque               = $FFFFE4C4;
  aclBlack                = $FF000000;
  aclBlanchedAlmond       = $FFFFEBCD;
  aclBlue                 = $FF0000FF;
  aclBlueViolet           = $FF8A2BE2;
  aclBrown                = $FFA52A2A;
  aclBurlyWood            = $FFDEB887;
  aclCadetBlue            = $FF5F9EA0;
  aclChartreuse           = $FF7FFF00;
  aclChocolate            = $FFD2691E;
  aclCoral                = $FFFF7F50;
  aclCornflowerBlue       = $FF6495ED;
  aclCornsilk             = $FFFFF8DC;
  aclCrimson              = $FFDC143C;
  aclCyan                 = $FF00FFFF;
  aclDarkBlue             = $FF00008B;
  aclDarkCyan             = $FF008B8B;
  aclDarkGoldenrod        = $FFB8860B;
  aclDarkGray             = $FFA9A9A9;
  aclDarkGreen            = $FF006400;
  aclDarkKhaki            = $FFBDB76B;
  aclDarkMagenta          = $FF8B008B;
  aclDarkOliveGreen       = $FF556B2F;
  aclDarkOrange           = $FFFF8C00;
  aclDarkOrchid           = $FF9932CC;
  aclDarkRed              = $FF8B0000;
  aclDarkSalmon           = $FFE9967A;
  aclDarkSeaGreen         = $FF8FBC8B;
  aclDarkSlateBlue        = $FF483D8B;
  aclDarkSlateGray        = $FF2F4F4F;
  aclDarkTurquoise        = $FF00CED1;
  aclDarkViolet           = $FF9400D3;
  aclDeepPink             = $FFFF1493;
  aclDeepSkyBlue          = $FF00BFFF;
  aclDimGray              = $FF696969;
  aclDodgerBlue           = $FF1E90FF;
  aclFirebrick            = $FFB22222;
  aclFloralWhite          = $FFFFFAF0;
  aclForestGreen          = $FF228B22;
  aclFuchsia              = $FFFF00FF;
  aclGainsboro            = $FFDCDCDC;
  aclGhostWhite           = $FFF8F8FF;
  aclGold                 = $FFFFD700;
  aclGoldenrod            = $FFDAA520;
  aclGray                 = $FF808080;
  aclGreen                = $FF008000;
  aclGreenYellow          = $FFADFF2F;
  aclHoneydew             = $FFF0FFF0;
  aclHotPink              = $FFFF69B4;
  aclIndianRed            = $FFCD5C5C;
  aclIndigo               = $FF4B0082;
  aclIvory                = $FFFFFFF0;
  aclKhaki                = $FFF0E68C;
  aclLavender             = $FFE6E6FA;
  aclLavenderBlush        = $FFFFF0F5;
  aclLawnGreen            = $FF7CFC00;
  aclLemonChiffon         = $FFFFFACD;
  aclLightBlue            = $FFADD8E6;
  aclLightCoral           = $FFF08080;
  aclLightCyan            = $FFE0FFFF;
  aclLightGoldenrodYellow = $FFFAFAD2;
  aclLightGray            = $FFD3D3D3;
  aclLightGreen           = $FF90EE90;
  aclLightPink            = $FFFFB6C1;
  aclLightSalmon          = $FFFFA07A;
  aclLightSeaGreen        = $FF20B2AA;
  aclLightSkyBlue         = $FF87CEFA;
  aclLightSlateGray       = $FF778899;
  aclLightSteelBlue       = $FFB0C4DE;
  aclLightYellow          = $FFFFFFE0;
  aclLime                 = $FF00FF00;
  aclLimeGreen            = $FF32CD32;
  aclLinen                = $FFFAF0E6;
  aclMagenta              = $FFFF00FF;
  aclMaroon               = $FF800000;
  aclMediumAquamarine     = $FF66CDAA;
  aclMediumBlue           = $FF0000CD;
  aclMediumOrchid         = $FFBA55D3;
  aclMediumPurple         = $FF9370DB;
  aclMediumSeaGreen       = $FF3CB371;
  aclMediumSlateBlue      = $FF7B68EE;
  aclMediumSpringGreen    = $FF00FA9A;
  aclMediumTurquoise      = $FF48D1CC;
  aclMediumVioletRed      = $FFC71585;
  aclMidnightBlue         = $FF191970;
  aclMintCream            = $FFF5FFFA;
  aclMistyRose            = $FFFFE4E1;
  aclMoccasin             = $FFFFE4B5;
  aclNavajoWhite          = $FFFFDEAD;
  aclNavy                 = $FF000080;
  aclOldLace              = $FFFDF5E6;
  aclOlive                = $FF808000;
  aclOliveDrab            = $FF6B8E23;
  aclOrange               = $FFFFA500;
  aclOrangeRed            = $FFFF4500;
  aclOrchid               = $FFDA70D6;
  aclPaleGoldenrod        = $FFEEE8AA;
  aclPaleGreen            = $FF98FB98;
  aclPaleTurquoise        = $FFAFEEEE;
  aclPaleVioletRed        = $FFDB7093;
  aclPapayaWhip           = $FFFFEFD5;
  aclPeachPuff            = $FFFFDAB9;
  aclPeru                 = $FFCD853F;
  aclPink                 = $FFFFC0CB;
  aclPlum                 = $FFDDA0DD;
  aclPowderBlue           = $FFB0E0E6;
  aclPurple               = $FF800080;
  aclRed                  = $FFFF0000;
  aclRosyBrown            = $FFBC8F8F;
  aclRoyalBlue            = $FF4169E1;
  aclSaddleBrown          = $FF8B4513;
  aclSalmon               = $FFFA8072;
  aclSandyBrown           = $FFF4A460;
  aclSeaGreen             = $FF2E8B57;
  aclSeaShell             = $FFFFF5EE;
  aclSienna               = $FFA0522D;
  aclSilver               = $FFC0C0C0;
  aclSkyBlue              = $FF87CEEB;
  aclSlateBlue            = $FF6A5ACD;
  aclSlateGray            = $FF708090;
  aclSnow                 = $FFFFFAFA;
  aclSpringGreen          = $FF00FF7F;
  aclSteelBlue            = $FF4682B4;
  aclTan                  = $FFD2B48C;
  aclTeal                 = $FF008080;
  aclThistle              = $FFD8BFD8;
  aclTomato               = $FFFF6347;
  aclTransparent          = $00FFFFFF;
  aclTurquoise            = $FF40E0D0;
  aclViolet               = $FFEE82EE;
  aclWheat                = $FFF5DEB3;
  aclWhite                = $FFFFFFFF;
  aclWhiteSmoke           = $FFF5F5F5;
  aclYellow               = $FFFFFF00;
  aclYellowGreen          = $FF9ACD32;

  // Shift count and bit mask for A, R, G, B components
  AlphaShift  = 24;
  RedShift    = 16;
  GreenShift  = 8;
  BlueShift   = 0;

  AlphaMask   = $ff000000;
  RedMask     = $00ff0000;
  GreenMask   = $0000ff00;
  BlueMask    = $000000ff;


type
{  TGPColor = class
  protected
     Argb: ARGB;
  public
    constructor Create; overload;
    constructor Create(r, g, b: Byte); overload;
    constructor Create(a, r, g, b: Byte); overload;
    constructor Create(Value: ARGB); overload;
    function GetAlpha: BYTE;
    function GetA: BYTE;
    function GetRed: BYTE;
    function GetR: BYTE;
    function GetGreen: Byte;
    function GetG: Byte;
    function GetBlue: Byte;
    function GetB: Byte;
    function GetValue: ARGB;
    procedure SetValue(Value: ARGB);
    procedure SetFromCOLORREF(rgb: COLORREF);
    function ToCOLORREF: COLORREF;
    function MakeARGB(a, r, g, b: Byte): ARGB;
  end;  }

  PGPColor = ^TGPColor;
  TGPColor = ARGB;
  TColorDynArray = array of TGPColor;

type
  RECTL = Windows.TRect;
  SIZEL = Windows.TSize;

  ENHMETAHEADER3 = packed record
    iType          : DWORD;  // Record type EMR_HEADER
    nSize          : DWORD;  // Record size in bytes.  This may be greater
                             // than the sizeof(ENHMETAHEADER).
    rclBounds      : RECTL;  // Inclusive-inclusive bounds in device units
    rclFrame       : RECTL;  // Inclusive-inclusive Picture Frame .01mm unit
    dSignature     : DWORD;  // Signature.  Must be ENHMETA_SIGNATURE.
    nVersion       : DWORD;  // Version number
    nBytes         : DWORD;  // Size of the metafile in bytes
    nRecords       : DWORD;  // Number of records in the metafile
    nHandles       : WORD;   // Number of handles in the handle table
                             // Handle index zero is reserved.
    sReserved      : WORD;   // Reserved.  Must be zero.
    nDescription   : DWORD;  // Number of chars in the unicode desc string
                             // This is 0 if there is no description string
    offDescription : DWORD;  // Offset to the metafile description record.
                             // This is 0 if there is no description string
    nPalEntries    : DWORD;  // Number of entries in the metafile palette.
    szlDevice      : SIZEL;  // Size of the reference device in pels
    szlMillimeters : SIZEL;  // Size of the reference device in millimeters
  end;
  TENHMETAHEADER3 = ENHMETAHEADER3;
  PENHMETAHEADER3 = ^TENHMETAHEADER3;

  PWMFRect16 = packed record
    Left   : INT16;
    Top    : INT16;
    Right  : INT16;
    Bottom : INT16;
  end;
  TPWMFRect16 = PWMFRect16;
  PPWMFRect16 = ^TPWMFRect16;

  WmfPlaceableFileHeader = packed record
    Key         : UINT32;      // GDIP_WMF_PLACEABLEKEY
    Hmf         : INT16;       // Metafile HANDLE number (always 0)
    BoundingBox : PWMFRect16;  // Coordinates in metafile units
    Inch        : INT16;       // Number of metafile units per inch
    Reserved    : UINT32;      // Reserved (always 0)
    Checksum    : INT16;       // Checksum value for previous 10 WORDs
  end;
  TWmfPlaceableFileHeader = WmfPlaceableFileHeader;
  PWmfPlaceableFileHeader = ^TWmfPlaceableFileHeader;

const
  GDIP_EMFPLUSFLAGS_DISPLAY      = $00000001;

type
  TMetafileHeader = packed class
  public
    Type_        : TMetafileType;
    Size         : UINT;           // Size of the metafile (in bytes)
    Version      : UINT;           // EMF+, EMF, or WMF version
    EmfPlusFlags : UINT;
    DpiX         : Single;
    DpiY         : Single;
    X            : Integer;        // Bounds in device units
    Y            : Integer;
    Width        : Integer;
    Height       : Integer;
    WmfHeader: TMETAHEADER;
    EmfHeader: TENHMETAHEADER3;
    EmfPlusHeaderSize : Integer; // size of the EMF+ header in file
    LogicalDpiX       : Integer; // Logical Dpi of reference Hdc
    LogicalDpiY       : Integer; // usually valid only for EMF+
  public
    property GetType: TMetafileType read Type_;
    property GetMetafileSize: UINT read Size;
    // If IsEmfPlus, this is the EMF+ version; else it is the WMF or EMF ver
    property GetVersion: UINT read Version;
     // Get the EMF+ flags associated with the metafile
    property GetEmfPlusFlags: UINT read EmfPlusFlags;
    property GetDpiX: Single read DpiX;
    property GetDpiY: Single read DpiY;
    procedure GetBounds(out Rect: TGPRect);
    // Is it any type of WMF (standard or Placeable Metafile)?
    function IsWmf: BOOL;
    // Is this an Placeable Metafile?
    function IsWmfPlaceable: BOOL;
    // Is this an EMF (not an EMF+)?
    function IsEmf: BOOL;
    // Is this an EMF or EMF+ file?
    function IsEmfOrEmfPlus: BOOL;
    // Is this an EMF+ file?
    function IsEmfPlus: BOOL;
    // Is this an EMF+ dual (has dual, down-level records) file?
    function IsEmfPlusDual: BOOL;
    // Is this an EMF+ only (no dual records) file?
    function IsEmfPlusOnly: BOOL;
    // If it's an EMF+ file, was it recorded against a display Hdc?
    function IsDisplay: BOOL;
    // Get the WMF header of the metafile (if it is a WMF)
    function GetWmfHeader: PMetaHeader;
    // Get the EMF header of the metafile (if it is an EMF)
    function GetEmfHeader: PENHMETAHEADER3;
  end;

//---------------------------------------------------------------------------
// Image file format identifiers
//---------------------------------------------------------------------------

const
  ImageFormatUndefined : TGUID = '{b96b3ca9-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatMemoryBMP : TGUID = '{b96b3caa-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatBMP       : TGUID = '{b96b3cab-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatEMF       : TGUID = '{b96b3cac-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatWMF       : TGUID = '{b96b3cad-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatJPEG      : TGUID = '{b96b3cae-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatPNG       : TGUID = '{b96b3caf-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatGIF       : TGUID = '{b96b3cb0-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatTIFF      : TGUID = '{b96b3cb1-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatEXIF      : TGUID = '{b96b3cb2-0728-11d3-9d7b-0000f81ef32e}';
  ImageFormatIcon      : TGUID = '{b96b3cb5-0728-11d3-9d7b-0000f81ef32e}';

//---------------------------------------------------------------------------
// Predefined multi-frame dimension IDs
//---------------------------------------------------------------------------

  FrameDimensionTime       : TGUID = '{6aedbd6d-3fb5-418a-83a6-7f45229dc872}';
  FrameDimensionResolution : TGUID = '{84236f7b-3bd3-428f-8dab-4ea1439ca315}';
  FrameDimensionPage       : TGUID = '{7462dc86-6180-4c7e-8e3f-ee7333a7a483}';

//---------------------------------------------------------------------------
// Property sets
//---------------------------------------------------------------------------

  FormatIDImageInformation : TGUID = '{e5836cbe-5eef-4f1d-acde-ae4c43b608ce}';
  FormatIDJpegAppHeaders   : TGUID = '{1c4afdcd-6177-43cf-abc7-5f51af39ee85}';

//---------------------------------------------------------------------------
// Encoder parameter sets
//---------------------------------------------------------------------------

  EncoderCompression      : TGUID = '{e09d739d-ccd4-44ee-8eba-3fbf8be4fc58}';
  EncoderColorDepth       : TGUID = '{66087055-ad66-4c7c-9a18-38a2310b8337}';
  EncoderScanMethod       : TGUID = '{3a4e2661-3109-4e56-8536-42c156e7dcfa}';
  EncoderVersion          : TGUID = '{24d18c76-814a-41a4-bf53-1c219cccf797}';
  EncoderRenderMethod     : TGUID = '{6d42c53a-229a-4825-8bb7-5c99e2b9a8b8}';
  EncoderQuality          : TGUID = '{1d5be4b5-fa4a-452d-9cdd-5db35105e7eb}';
  EncoderTransformation   : TGUID = '{8d0eb2d1-a58e-4ea8-aa14-108074b7b6f9}';
  EncoderLuminanceTable   : TGUID = '{edb33bce-0266-4a77-b904-27216099e717}';
  EncoderChrominanceTable : TGUID = '{f2e455dc-09b3-4316-8260-676ada32481c}';
  EncoderSaveFlag         : TGUID = '{292266fc-ac40-47bf-8cfc-a85b89a655de}';

  CodecIImageBytes : TGUID = '{025d1823-6c7d-447b-bbdb-a3cbc3dfa2fc}';

type
  IImageBytes = Interface(IUnknown)
    ['{025D1823-6C7D-447B-BBDB-A3CBC3DFA2FC}']
    // Return total number of bytes in the IStream
    function CountBytes(out pcb: UINT): HRESULT; stdcall;
    // Locks "cb" bytes, starting from "ulOffset" in the stream, and returns the
    // pointer to the beginning of the locked memory chunk in "ppvBytes"
    function LockBytes(cb: UINT; ulOffset: ULONG; out ppvBytes: pointer): HRESULT; stdcall;
    // Unlocks "cb" bytes, pointed by "pvBytes", starting from "ulOffset" in the
    // stream
    function UnlockBytes(pvBytes: pointer; cb: UINT; ulOffset: ULONG): HRESULT; stdcall;
  end;

//--------------------------------------------------------------------------
// ImageCodecInfo structure
//--------------------------------------------------------------------------

  ImageCodecInfo = packed record
    Clsid             : TGUID;
    FormatID          : TGUID;
    CodecName         : PWCHAR;
    DllName           : PWCHAR;
    FormatDescription : PWCHAR;
    FilenameExtension : PWCHAR;
    MimeType          : PWCHAR;
    Flags             : DWORD;
    Version           : DWORD;
    SigCount          : DWORD;
    SigSize           : DWORD;
    SigPattern        : PBYTE;
    SigMask           : PBYTE;
  end;
  TImageCodecInfo = ImageCodecInfo;
  PImageCodecInfo = ^TImageCodecInfo;

//--------------------------------------------------------------------------
// Information flags about image codecs
//--------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  ImageCodecFlags = (
    ImageCodecFlagsEncoder            = $00000001,
    ImageCodecFlagsDecoder            = $00000002,
    ImageCodecFlagsSupportBitmap      = $00000004,
    ImageCodecFlagsSupportVector      = $00000008,
    ImageCodecFlagsSeekableEncode     = $00000010,
    ImageCodecFlagsBlockingDecode     = $00000020,

    ImageCodecFlagsBuiltin            = $00010000,
    ImageCodecFlagsSystem             = $00020000,
    ImageCodecFlagsUser               = $00040000
  );
  TImageCodecFlags = ImageCodecFlags;
{$ELSE}
  ImageCodecFlags = Integer;
  const
    ImageCodecFlagsEncoder            = $00000001;
    ImageCodecFlagsDecoder            = $00000002;
    ImageCodecFlagsSupportBitmap      = $00000004;
    ImageCodecFlagsSupportVector      = $00000008;
    ImageCodecFlagsSeekableEncode     = $00000010;
    ImageCodecFlagsBlockingDecode     = $00000020;

    ImageCodecFlagsBuiltin            = $00010000;
    ImageCodecFlagsSystem             = $00020000;
    ImageCodecFlagsUser               = $00040000;

type
  TImageCodecFlags = ImageCodecFlags;
{$ENDIF}
//---------------------------------------------------------------------------
// Access modes used when calling Image::LockBits
//---------------------------------------------------------------------------

  ImageLockMode = Integer;
  const
    ImageLockModeRead         = $0001;
    ImageLockModeWrite        = $0002;
    ImageLockModeUserInputBuf = $0004;
type
  TImageLockMode = ImageLockMode;

//---------------------------------------------------------------------------
// Information about image pixel data
//---------------------------------------------------------------------------

  BitmapData = packed record
    Width       : UINT;
    Height      : UINT;
    Stride      : Integer;
    PixelFormat : PixelFormat;
    Scan0       : Pointer;
    Reserved    : UINT;
  end;
  TBitmapData = BitmapData;
  PBitmapData = ^TBitmapData;

//---------------------------------------------------------------------------
// Image flags
//---------------------------------------------------------------------------
{$IFDEF DELPHI6_UP}
  ImageFlags = (
    ImageFlagsNone                = 0,

    // Low-word: shared with SINKFLAG_x

    ImageFlagsScalable            = $0001,
    ImageFlagsHasAlpha            = $0002,
    ImageFlagsHasTranslucent      = $0004,
    ImageFlagsPartiallyScalable   = $0008,

    // Low-word: color space definition

    ImageFlagsColorSpaceRGB       = $0010,
    ImageFlagsColorSpaceCMYK      = $0020,
    ImageFlagsColorSpaceGRAY      = $0040,
    ImageFlagsColorSpaceYCBCR     = $0080,
    ImageFlagsColorSpaceYCCK      = $0100,

    // Low-word: image size info

    ImageFlagsHasRealDPI          = $1000,
    ImageFlagsHasRealPixelSize    = $2000,

    // High-word

    ImageFlagsReadOnly            = $00010000,
    ImageFlagsCaching             = $00020000
  );
  TImageFlags = ImageFlags;
{$ELSE}
  ImageFlags = Integer;
  const
    ImageFlagsNone                = 0;

    // Low-word: shared with SINKFLAG_x

    ImageFlagsScalable            = $0001;
    ImageFlagsHasAlpha            = $0002;
    ImageFlagsHasTranslucent      = $0004;
    ImageFlagsPartiallyScalable   = $0008;

    // Low-word: color space definition

    ImageFlagsColorSpaceRGB       = $0010;
    ImageFlagsColorSpaceCMYK      = $0020;
    ImageFlagsColorSpaceGRAY      = $0040;
    ImageFlagsColorSpaceYCBCR     = $0080;
    ImageFlagsColorSpaceYCCK      = $0100;

    // Low-word: image size info

    ImageFlagsHasRealDPI          = $1000;
    ImageFlagsHasRealPixelSize    = $2000;

    // High-word

    ImageFlagsReadOnly            = $00010000;
    ImageFlagsCaching             = $00020000;

type
  TImageFlags = ImageFlags;
{$ENDIF}


{$IFDEF DELPHI6_UP}
  RotateFlipType = (
    RotateNoneFlipNone = 0,
    Rotate90FlipNone   = 1,
    Rotate180FlipNone  = 2,
    Rotate270FlipNone  = 3,

    RotateNoneFlipX    = 4,
    Rotate90FlipX      = 5,
    Rotate180FlipX     = 6,
    Rotate270FlipX     = 7,

    RotateNoneFlipY    = Rotate180FlipX,
    Rotate90FlipY      = Rotate270FlipX,
    Rotate180FlipY     = RotateNoneFlipX,
    Rotate270FlipY     = Rotate90FlipX,

    RotateNoneFlipXY   = Rotate180FlipNone,
    Rotate90FlipXY     = Rotate270FlipNone,
    Rotate180FlipXY    = RotateNoneFlipNone,
    Rotate270FlipXY    = Rotate90FlipNone
  );
  TRotateFlipType = RotateFlipType;
{$ELSE}
  RotateFlipType = (
    RotateNoneFlipNone, // = 0,
    Rotate90FlipNone,   // = 1,
    Rotate180FlipNone,  // = 2,
    Rotate270FlipNone,  // = 3,

    RotateNoneFlipX,    // = 4,
    Rotate90FlipX,      // = 5,
    Rotate180FlipX,     // = 6,
    Rotate270FlipX      // = 7,
  );
  const
    RotateNoneFlipY    = Rotate180FlipX;
    Rotate90FlipY      = Rotate270FlipX;
    Rotate180FlipY     = RotateNoneFlipX;
    Rotate270FlipY     = Rotate90FlipX;

    RotateNoneFlipXY   = Rotate180FlipNone;
    Rotate90FlipXY     = Rotate270FlipNone;
    Rotate180FlipXY    = RotateNoneFlipNone;
    Rotate270FlipXY    = Rotate90FlipNone;

type
  TRotateFlipType = RotateFlipType;
{$ENDIF}

//---------------------------------------------------------------------------
// Encoder Parameter structure
//---------------------------------------------------------------------------

  EncoderParameter = packed record
    Guid           : TGUID;   // GUID of the parameter
    NumberOfValues : ULONG;   // Number of the parameter values
    Type_          : ULONG;   // Value type, like ValueTypeLONG  etc.
    Value          : Pointer; // A pointer to the parameter values
  end;
  TEncoderParameter = EncoderParameter;
  PEncoderParameter = ^TEncoderParameter;

//---------------------------------------------------------------------------
// Encoder Parameters structure
//---------------------------------------------------------------------------

  EncoderParameters = packed record
    Count     : UINT;               // Number of parameters in this structure
    Parameter : array[0..10] of TEncoderParameter;  // Parameter values
  end;
  TEncoderParameters = EncoderParameters;
  PEncoderParameters = ^TEncoderParameters;

//---------------------------------------------------------------------------
// Property Item
//---------------------------------------------------------------------------

  PropertyItem = record // NOT PACKED !!
    id       : PROPID;  // ID of this property
    length   : ULONG;   // Length of the property value, in bytes
    type_    : WORD;    // Type of the value, as one of TAG_TYPE_XXX
    value    : Pointer; // property value
  end;
  TPropertyItem = PropertyItem;
  PPropertyItem = ^TPropertyItem;

//---------------------------------------------------------------------------
// Image property types
//---------------------------------------------------------------------------

const
  PropertyTagTypeByte      : Integer =  1;
  PropertyTagTypeASCII     : Integer =  2;
  PropertyTagTypeShort     : Integer =  3;
  PropertyTagTypeLong      : Integer =  4;
  PropertyTagTypeRational  : Integer =  5;
  PropertyTagTypeUndefined : Integer =  7;
  PropertyTagTypeSLONG     : Integer =  9;
  PropertyTagTypeSRational : Integer = 10;

//---------------------------------------------------------------------------
// Image property ID tags
//---------------------------------------------------------------------------

  PropertyTagExifIFD            = $8769;
  PropertyTagGpsIFD             = $8825;

  PropertyTagNewSubfileType     = $00FE;
  PropertyTagSubfileType        = $00FF;
  PropertyTagImageWidth         = $0100;
  PropertyTagImageHeight        = $0101;
  PropertyTagBitsPerSample      = $0102;
  PropertyTagCompression        = $0103;
  PropertyTagPhotometricInterp  = $0106;
  PropertyTagThreshHolding      = $0107;
  PropertyTagCellWidth          = $0108;
  PropertyTagCellHeight         = $0109;
  PropertyTagFillOrder          = $010A;
  PropertyTagDocumentName       = $010D;
  PropertyTagImageDescription   = $010E;
  PropertyTagEquipMake          = $010F;
  PropertyTagEquipModel         = $0110;
  PropertyTagStripOffsets       = $0111;
  PropertyTagOrientation        = $0112;
  PropertyTagSamplesPerPixel    = $0115;
  PropertyTagRowsPerStrip       = $0116;
  PropertyTagStripBytesCount    = $0117;
  PropertyTagMinSampleValue     = $0118;
  PropertyTagMaxSampleValue     = $0119;
  PropertyTagXResolution        = $011A;   // Image resolution in width direction
  PropertyTagYResolution        = $011B;   // Image resolution in height direction
  PropertyTagPlanarConfig       = $011C;   // Image data arrangement
  PropertyTagPageName           = $011D;
  PropertyTagXPosition          = $011E;
  PropertyTagYPosition          = $011F;
  PropertyTagFreeOffset         = $0120;
  PropertyTagFreeByteCounts     = $0121;
  PropertyTagGrayResponseUnit   = $0122;
  PropertyTagGrayResponseCurve  = $0123;
  PropertyTagT4Option           = $0124;
  PropertyTagT6Option           = $0125;
  PropertyTagResolutionUnit     = $0128;   // Unit of X and Y resolution
  PropertyTagPageNumber         = $0129;
  PropertyTagTransferFuncition  = $012D;
  PropertyTagSoftwareUsed       = $0131;
  PropertyTagDateTime           = $0132;
  PropertyTagArtist             = $013B;
  PropertyTagHostComputer       = $013C;
  PropertyTagPredictor          = $013D;
  PropertyTagWhitePoint         = $013E;
  PropertyTagPrimaryChromaticities = $013F;
  PropertyTagColorMap           = $0140;
  PropertyTagHalftoneHints      = $0141;
  PropertyTagTileWidth          = $0142;
  PropertyTagTileLength         = $0143;
  PropertyTagTileOffset         = $0144;
  PropertyTagTileByteCounts     = $0145;
  PropertyTagInkSet             = $014C;
  PropertyTagInkNames           = $014D;
  PropertyTagNumberOfInks       = $014E;
  PropertyTagDotRange           = $0150;
  PropertyTagTargetPrinter      = $0151;
  PropertyTagExtraSamples       = $0152;
  PropertyTagSampleFormat       = $0153;
  PropertyTagSMinSampleValue    = $0154;
  PropertyTagSMaxSampleValue    = $0155;
  PropertyTagTransferRange      = $0156;

  PropertyTagJPEGProc               = $0200;
  PropertyTagJPEGInterFormat        = $0201;
  PropertyTagJPEGInterLength        = $0202;
  PropertyTagJPEGRestartInterval    = $0203;
  PropertyTagJPEGLosslessPredictors = $0205;
  PropertyTagJPEGPointTransforms    = $0206;
  PropertyTagJPEGQTables            = $0207;
  PropertyTagJPEGDCTables           = $0208;
  PropertyTagJPEGACTables           = $0209;

  PropertyTagYCbCrCoefficients  = $0211;
  PropertyTagYCbCrSubsampling   = $0212;
  PropertyTagYCbCrPositioning   = $0213;
  PropertyTagREFBlackWhite      = $0214;

  PropertyTagICCProfile         = $8773;   // This TAG is defined by ICC
                                           // for embedded ICC in TIFF
  PropertyTagGamma                = $0301;
  PropertyTagICCProfileDescriptor = $0302;
  PropertyTagSRGBRenderingIntent  = $0303;

  PropertyTagImageTitle         = $0320;
  PropertyTagCopyright          = $8298;

// Extra TAGs (Like Adobe Image Information tags etc.)

  PropertyTagResolutionXUnit           = $5001;
  PropertyTagResolutionYUnit           = $5002;
  PropertyTagResolutionXLengthUnit     = $5003;
  PropertyTagResolutionYLengthUnit     = $5004;
  PropertyTagPrintFlags                = $5005;
  PropertyTagPrintFlagsVersion         = $5006;
  PropertyTagPrintFlagsCrop            = $5007;
  PropertyTagPrintFlagsBleedWidth      = $5008;
  PropertyTagPrintFlagsBleedWidthScale = $5009;
  PropertyTagHalftoneLPI               = $500A;
  PropertyTagHalftoneLPIUnit           = $500B;
  PropertyTagHalftoneDegree            = $500C;
  PropertyTagHalftoneShape             = $500D;
  PropertyTagHalftoneMisc              = $500E;
  PropertyTagHalftoneScreen            = $500F;
  PropertyTagJPEGQuality               = $5010;
  PropertyTagGridSize                  = $5011;
  PropertyTagThumbnailFormat           = $5012;  // 1 = JPEG, 0 = RAW RGB
  PropertyTagThumbnailWidth            = $5013;
  PropertyTagThumbnailHeight           = $5014;
  PropertyTagThumbnailColorDepth       = $5015;
  PropertyTagThumbnailPlanes           = $5016;
  PropertyTagThumbnailRawBytes         = $5017;
  PropertyTagThumbnailSize             = $5018;
  PropertyTagThumbnailCompressedSize   = $5019;
  PropertyTagColorTransferFunction     = $501A;
  PropertyTagThumbnailData             = $501B;    // RAW thumbnail bits in
                                                   // JPEG format or RGB format
                                                   // depends on
                                                   // PropertyTagThumbnailFormat

  // Thumbnail related TAGs

  PropertyTagThumbnailImageWidth        = $5020;   // Thumbnail width
  PropertyTagThumbnailImageHeight       = $5021;   // Thumbnail height
  PropertyTagThumbnailBitsPerSample     = $5022;   // Number of bits per
                                                   // component
  PropertyTagThumbnailCompression       = $5023;   // Compression Scheme
  PropertyTagThumbnailPhotometricInterp = $5024;   // Pixel composition
  PropertyTagThumbnailImageDescription  = $5025;   // Image Tile
  PropertyTagThumbnailEquipMake         = $5026;   // Manufacturer of Image
                                                   // Input equipment
  PropertyTagThumbnailEquipModel        = $5027;   // Model of Image input
                                                   // equipment
  PropertyTagThumbnailStripOffsets    = $5028;  // Image data location
  PropertyTagThumbnailOrientation     = $5029;  // Orientation of image
  PropertyTagThumbnailSamplesPerPixel = $502A;  // Number of components
  PropertyTagThumbnailRowsPerStrip    = $502B;  // Number of rows per strip
  PropertyTagThumbnailStripBytesCount = $502C;  // Bytes per compressed
                                                // strip
  PropertyTagThumbnailResolutionX     = $502D;  // Resolution in width
                                                // direction
  PropertyTagThumbnailResolutionY     = $502E;  // Resolution in height
                                                // direction
  PropertyTagThumbnailPlanarConfig    = $502F;  // Image data arrangement
  PropertyTagThumbnailResolutionUnit  = $5030;  // Unit of X and Y
                                                // Resolution
  PropertyTagThumbnailTransferFunction = $5031;  // Transfer function
  PropertyTagThumbnailSoftwareUsed     = $5032;  // Software used
  PropertyTagThumbnailDateTime         = $5033;  // File change date and
                                                 // time
  PropertyTagThumbnailArtist          = $5034;  // Person who created the
                                                // image
  PropertyTagThumbnailWhitePoint      = $5035;  // White point chromaticity
  PropertyTagThumbnailPrimaryChromaticities = $5036;
                                                    // Chromaticities of
                                                    // primaries
  PropertyTagThumbnailYCbCrCoefficients = $5037; // Color space transforma-
                                                 // tion coefficients
  PropertyTagThumbnailYCbCrSubsampling = $5038;  // Subsampling ratio of Y
                                                 // to C
  PropertyTagThumbnailYCbCrPositioning = $5039;  // Y and C position
  PropertyTagThumbnailRefBlackWhite    = $503A;  // Pair of black and white
                                                 // reference values
  PropertyTagThumbnailCopyRight       = $503B;   // CopyRight holder

  PropertyTagLuminanceTable           = $5090;
  PropertyTagChrominanceTable         = $5091;

  PropertyTagFrameDelay               = $5100;
  PropertyTagLoopCount                = $5101;

  PropertyTagPixelUnit         = $5110;  // Unit specifier for pixel/unit
  PropertyTagPixelPerUnitX     = $5111;  // Pixels per unit in X
  PropertyTagPixelPerUnitY     = $5112;  // Pixels per unit in Y
  PropertyTagPaletteHistogram  = $5113;  // Palette histogram

  // EXIF specific tag

  PropertyTagExifExposureTime  = $829A;
  PropertyTagExifFNumber       = $829D;

  PropertyTagExifExposureProg  = $8822;
  PropertyTagExifSpectralSense = $8824;
  PropertyTagExifISOSpeed      = $8827;
  PropertyTagExifOECF          = $8828;

  PropertyTagExifVer           = $9000;
  PropertyTagExifDTOrig        = $9003; // Date & time of original
  PropertyTagExifDTDigitized   = $9004; // Date & time of digital data generation

  PropertyTagExifCompConfig    = $9101;
  PropertyTagExifCompBPP       = $9102;

  PropertyTagExifShutterSpeed  = $9201;
  PropertyTagExifAperture      = $9202;
  PropertyTagExifBrightness    = $9203;
  PropertyTagExifExposureBias  = $9204;
  PropertyTagExifMaxAperture   = $9205;
  PropertyTagExifSubjectDist   = $9206;
  PropertyTagExifMeteringMode  = $9207;
  PropertyTagExifLightSource   = $9208;
  PropertyTagExifFlash         = $9209;
  PropertyTagExifFocalLength   = $920A;
  PropertyTagExifMakerNote     = $927C;
  PropertyTagExifUserComment   = $9286;
  PropertyTagExifDTSubsec      = $9290;  // Date & Time subseconds
  PropertyTagExifDTOrigSS      = $9291;  // Date & Time original subseconds
  PropertyTagExifDTDigSS       = $9292;  // Date & TIme digitized subseconds

  PropertyTagExifFPXVer        = $A000;
  PropertyTagExifColorSpace    = $A001;
  PropertyTagExifPixXDim       = $A002;
  PropertyTagExifPixYDim       = $A003;
  PropertyTagExifRelatedWav    = $A004;  // related sound file
  PropertyTagExifInterop       = $A005;
  PropertyTagExifFlashEnergy   = $A20B;
  PropertyTagExifSpatialFR     = $A20C;  // Spatial Frequency Response
  PropertyTagExifFocalXRes     = $A20E;  // Focal Plane X Resolution
  PropertyTagExifFocalYRes     = $A20F;  // Focal Plane Y Resolution
  PropertyTagExifFocalResUnit  = $A210;  // Focal Plane Resolution Unit
  PropertyTagExifSubjectLoc    = $A214;
  PropertyTagExifExposureIndex = $A215;
  PropertyTagExifSensingMethod = $A217;
  PropertyTagExifFileSource    = $A300;
  PropertyTagExifSceneType     = $A301;
  PropertyTagExifCfaPattern    = $A302;

  PropertyTagGpsVer            = $0000;
  PropertyTagGpsLatitudeRef    = $0001;
  PropertyTagGpsLatitude       = $0002;
  PropertyTagGpsLongitudeRef   = $0003;
  PropertyTagGpsLongitude      = $0004;
  PropertyTagGpsAltitudeRef    = $0005;
  PropertyTagGpsAltitude       = $0006;
  PropertyTagGpsGpsTime        = $0007;
  PropertyTagGpsGpsSatellites  = $0008;
  PropertyTagGpsGpsStatus      = $0009;
  PropertyTagGpsGpsMeasureMode = $00A;
  PropertyTagGpsGpsDop         = $000B;  // Measurement precision
  PropertyTagGpsSpeedRef       = $000C;
  PropertyTagGpsSpeed          = $000D;
  PropertyTagGpsTrackRef       = $000E;
  PropertyTagGpsTrack          = $000F;
  PropertyTagGpsImgDirRef      = $0010;
  PropertyTagGpsImgDir         = $0011;
  PropertyTagGpsMapDatum       = $0012;
  PropertyTagGpsDestLatRef     = $0013;
  PropertyTagGpsDestLat        = $0014;
  PropertyTagGpsDestLongRef    = $0015;
  PropertyTagGpsDestLong       = $0016;
  PropertyTagGpsDestBearRef    = $0017;
  PropertyTagGpsDestBear       = $0018;
  PropertyTagGpsDestDistRef    = $0019;
  PropertyTagGpsDestDist       = $001A;

//----------------------------------------------------------------------------
// Color matrix
//----------------------------------------------------------------------------

type
  ColorMatrix = packed array[0..4, 0..4] of Single;
  TColorMatrix = ColorMatrix;
  PColorMatrix = ^TColorMatrix;

//----------------------------------------------------------------------------
// Color Matrix flags
//----------------------------------------------------------------------------

  ColorMatrixFlags = (
    ColorMatrixFlagsDefault,
    ColorMatrixFlagsSkipGrays,
    ColorMatrixFlagsAltGray
  );
  TColorMatrixFlags = ColorMatrixFlags;

//----------------------------------------------------------------------------
// Color Adjust Type
//----------------------------------------------------------------------------

  ColorAdjustType = (
    ColorAdjustTypeDefault,
    ColorAdjustTypeBitmap,
    ColorAdjustTypeBrush,
    ColorAdjustTypePen,
    ColorAdjustTypeText,
    ColorAdjustTypeCount,
    ColorAdjustTypeAny      // Reserved
  );
  TColorAdjustType = ColorAdjustType;

//----------------------------------------------------------------------------
// Color Map
//----------------------------------------------------------------------------

  ColorMap = packed record
    oldColor: TGPColor;
    newColor: TGPColor;
  end;
  TColorMap = ColorMap;
  PColorMap = ^TColorMap;

//---------------------------------------------------------------------------
// Private GDI+ classes for internal type checking
//---------------------------------------------------------------------------

  GpGraphics = Pointer;

  GpBrush = Pointer;
  GpTexture = Pointer;
  GpSolidFill = Pointer;
  GpLineGradient = Pointer;
  GpPathGradient = Pointer;
  GpHatch =  Pointer;

  GpPen = Pointer;
  GpCustomLineCap = Pointer;
  GpAdjustableArrowCap = Pointer;

  GpImage = Pointer;
  GpBitmap = Pointer;
  GpMetafile = Pointer;
  GpImageAttributes = Pointer;

  GpPath = Pointer;
  GpRegion = Pointer;
  GpPathIterator = Pointer;

  GpFontFamily = Pointer;
  GpFont = Pointer;
  GpStringFormat = Pointer;
  GpFontCollection = Pointer;
  GpCachedBitmap = Pointer;

  GpStatus          = TStatus;
  GpFillMode        = TFillMode;
  GpWrapMode        = TWrapMode;
  GpUnit            = TUnit;
  GpCoordinateSpace = TCoordinateSpace;
  GpPointF          = PGPPointF;
  GpPoint           = PGPPoint;
  GpRectF           = PGPRectF;
  GpRect            = PGPRect;
  GpSizeF           = PGPSizeF;
  GpHatchStyle      = THatchStyle;
  GpDashStyle       = TDashStyle;
  GpLineCap         = TLineCap;
  GpDashCap         = TDashCap;

  GpPenAlignment    = TPenAlignment;

  GpLineJoin        = TLineJoin;
  GpPenType         = TPenType;

  GpMatrix          = Pointer; 
  GpBrushType       = TBrushType;
  GpMatrixOrder     = TMatrixOrder;
  GpFlushIntention  = TFlushIntention;
  GpPathData        = TPathData;

var

GdipCreatePath: function(brushMode: GPFILLMODE;
    out path: GPPATH): GPSTATUS; stdcall;

GdipCreatePath2: function(v1: GPPOINTF; v2: PBYTE; v3: Integer; v4: GPFILLMODE;  
    out path: GPPATH): GPSTATUS; stdcall;

GdipCreatePath2I: function(v1: GPPOINT; v2: PBYTE; v3: Integer; v4: GPFILLMODE;  
    out path: GPPATH): GPSTATUS; stdcall;

GdipClonePath: function(path: GPPATH;  
    out clonePath: GPPATH): GPSTATUS; stdcall;

GdipDeletePath: function(path: GPPATH): GPSTATUS; stdcall;  

GdipResetPath: function(path: GPPATH): GPSTATUS; stdcall;  

GdipGetPointCount: function(path: GPPATH;
    out count: Integer): GPSTATUS; stdcall;

GdipGetPathTypes: function(path: GPPATH; types: PBYTE;  
    count: Integer): GPSTATUS; stdcall;

GdipGetPathPoints: function(v1: GPPATH; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipGetPathPointsI: function(v1: GPPATH; points: GPPOINT;  
             count: Integer): GPSTATUS; stdcall;

GdipGetPathFillMode: function(path: GPPATH;  
    var fillmode: GPFILLMODE): GPSTATUS; stdcall;

GdipSetPathFillMode: function(path: GPPATH;
    fillmode: GPFILLMODE): GPSTATUS; stdcall;

GdipGetPathData: function(path: GPPATH;  
    pathData: Pointer): GPSTATUS; stdcall;

GdipStartPathFigure: function(path: GPPATH): GPSTATUS; stdcall;  

GdipClosePathFigure: function(path: GPPATH): GPSTATUS; stdcall;  

GdipClosePathFigures: function(path: GPPATH): GPSTATUS; stdcall;  

GdipSetPathMarker: function(path: GPPATH): GPSTATUS; stdcall;  

GdipClearPathMarkers: function(path: GPPATH): GPSTATUS; stdcall;  

GdipReversePath: function(path: GPPATH): GPSTATUS; stdcall;  

GdipGetPathLastPoint: function(path: GPPATH;  
    lastPoint: GPPOINTF): GPSTATUS; stdcall;

GdipAddPathLine: function(path: GPPATH;  
    x1, y1, x2, y2: Single): GPSTATUS; stdcall;

GdipAddPathLine2: function(path: GPPATH; points: GPPOINTF;
    count: Integer): GPSTATUS; stdcall;

GdipAddPathArc: function(path: GPPATH; x, y, width, height, startAngle,  
    sweepAngle: Single): GPSTATUS; stdcall;

GdipAddPathBezier: function(path: GPPATH;
    x1, y1, x2, y2, x3, y3, x4, y4: Single): GPSTATUS; stdcall;

GdipAddPathBeziers: function(path: GPPATH; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathCurve: function(path: GPPATH; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathCurve2: function(path: GPPATH; points: GPPOINTF; count: Integer;  
    tension: Single): GPSTATUS; stdcall;

GdipAddPathCurve3: function(path: GPPATH; points: GPPOINTF; count: Integer;  
    offset: Integer; numberOfSegments: Integer;
    tension: Single): GPSTATUS; stdcall;

GdipAddPathClosedCurve: function(path: GPPATH; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathClosedCurve2: function(path: GPPATH; points: GPPOINTF;  
    count: Integer; tension: Single): GPSTATUS; stdcall;

GdipAddPathRectangle: function(path: GPPATH; x: Single; y: Single;  
    width: Single; height: Single): GPSTATUS; stdcall;

GdipAddPathRectangles: function(path: GPPATH; rects: GPRECTF;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathEllipse: function(path: GPPATH;  x: Single; y: Single;  
    width: Single; height: Single): GPSTATUS; stdcall;

GdipAddPathPie: function(path: GPPATH; x: Single; y: Single; width: Single;  
    height: Single; startAngle: Single; sweepAngle: Single): GPSTATUS; stdcall;

GdipAddPathPolygon: function(path: GPPATH; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathPath: function(path: GPPATH; addingPath: GPPATH;
    connect: Bool): GPSTATUS; stdcall;

GdipAddPathString: function(path: GPPATH; string_: PWCHAR; length: Integer;  
    family: GPFONTFAMILY; style: Integer; emSize: Single; layoutRect: PGPRectF;
    format: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipAddPathStringI: function(path: GPPATH; string_: PWCHAR; length: Integer;  
    family: GPFONTFAMILY; style: Integer; emSize: Single; layoutRect: PGPRect;
    format: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipAddPathLineI: function(path: GPPATH; x1: Integer; y1: Integer; x2: Integer;  
    y2: Integer): GPSTATUS; stdcall;

GdipAddPathLine2I: function(path: GPPATH; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathArcI: function(path: GPPATH; x: Integer; y: Integer; width: Integer;  
    height: Integer; startAngle: Single; sweepAngle: Single): GPSTATUS; stdcall;

GdipAddPathBezierI: function(path: GPPATH; x1: Integer; y1: Integer;  
    x2: Integer; y2: Integer; x3: Integer; y3: Integer; x4: Integer;
    y4: Integer): GPSTATUS; stdcall;

GdipAddPathBeziersI: function(path: GPPATH; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathCurveI: function(path: GPPATH; points: GPPOINT;
    count: Integer): GPSTATUS; stdcall;

GdipAddPathCurve2I: function(path: GPPATH; points: GPPOINT; count: Integer;  
    tension: Single): GPSTATUS; stdcall;

GdipAddPathCurve3I: function(path: GPPATH; points: GPPOINT; count: Integer;  
    offset: Integer; numberOfSegments: Integer;
    tension: Single): GPSTATUS; stdcall;

GdipAddPathClosedCurveI: function(path: GPPATH; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathClosedCurve2I: function(path: GPPATH; points: GPPOINT;  
    count: Integer; tension: Single): GPSTATUS; stdcall;

GdipAddPathRectangleI: function(path: GPPATH; x: Integer; y: Integer;  
    width: Integer; height: Integer): GPSTATUS; stdcall;

GdipAddPathRectanglesI: function(path: GPPATH; rects: GPRECT;  
    count: Integer): GPSTATUS; stdcall;

GdipAddPathEllipseI: function(path: GPPATH; x: Integer; y: Integer;  
    width: Integer; height: Integer): GPSTATUS; stdcall;

GdipAddPathPieI: function(path: GPPATH; x: Integer; y: Integer; width: Integer;  
    height: Integer; startAngle: Single; sweepAngle: Single): GPSTATUS; stdcall;

GdipAddPathPolygonI: function(path: GPPATH; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipFlattenPath: function(path: GPPATH; matrix: GPMATRIX;  
    flatness: Single): GPSTATUS; stdcall;

GdipWindingModeOutline: function(path: GPPATH; matrix: GPMATRIX;  
    flatness: Single): GPSTATUS; stdcall;

GdipWidenPath: function(nativePath: GPPATH; pen: GPPEN; matrix: GPMATRIX;  
    flatness: Single): GPSTATUS; stdcall;

GdipWarpPath: function(path: GPPATH; matrix: GPMATRIX; points: GPPOINTF;  
    count: Integer; srcx: Single; srcy: Single; srcwidth: Single;
    srcheight: Single; warpMode: WARPMODE; flatness: Single): GPSTATUS; stdcall;

GdipTransformPath: function(path: GPPATH; matrix: GPMATRIX): GPSTATUS; stdcall;  

GdipGetPathWorldBounds: function(path: GPPATH; bounds: GPRECTF;  
    matrix: GPMATRIX; pen: GPPEN): GPSTATUS; stdcall;

GdipGetPathWorldBoundsI: function(path: GPPATH; bounds: GPRECT;  
    matrix: GPMATRIX; pen: GPPEN): GPSTATUS; stdcall;

GdipIsVisiblePathPoint: function(path: GPPATH; x: Single; y: Single;  
    graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipIsVisiblePathPointI: function(path: GPPATH; x: Integer; y: Integer;  
    graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipIsOutlineVisiblePathPoint: function(path: GPPATH; x: Single; y: Single;  
    pen: GPPEN; graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipIsOutlineVisiblePathPointI: function(path: GPPATH; x: Integer; y: Integer;  
    pen: GPPEN; graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// PathIterator APIs 
//----------------------------------------------------------------------------

GdipCreatePathIter: function(out iterator: GPPATHITERATOR;  
    path: GPPATH): GPSTATUS; stdcall;

GdipDeletePathIter: function(iterator: GPPATHITERATOR): GPSTATUS; stdcall;  

GdipPathIterNextSubpath: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; var startIndex: Integer; var endIndex: Integer;
    out isClosed: Bool): GPSTATUS; stdcall;

GdipPathIterNextSubpathPath: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; path: GPPATH;
    out isClosed: Bool): GPSTATUS; stdcall;

GdipPathIterNextPathType: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; pathType: PBYTE; var startIndex: Integer;
    var endIndex: Integer): GPSTATUS; stdcall;

GdipPathIterNextMarker: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; var startIndex: Integer;
    var endIndex: Integer): GPSTATUS; stdcall;

GdipPathIterNextMarkerPath: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; path: GPPATH): GPSTATUS; stdcall;

GdipPathIterGetCount: function(iterator: GPPATHITERATOR;  
    out count: Integer): GPSTATUS; stdcall;

GdipPathIterGetSubpathCount: function(iterator: GPPATHITERATOR;
    out count: Integer): GPSTATUS; stdcall;

GdipPathIterIsValid: function(iterator: GPPATHITERATOR;  
    out valid: Bool): GPSTATUS; stdcall;

GdipPathIterHasCurve: function(iterator: GPPATHITERATOR;  
    out hasCurve: Bool): GPSTATUS; stdcall;

GdipPathIterRewind: function(iterator: GPPATHITERATOR): GPSTATUS; stdcall;  

GdipPathIterEnumerate: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; points: GPPOINTF; types: PBYTE;
    count: Integer): GPSTATUS; stdcall;

GdipPathIterCopyData: function(iterator: GPPATHITERATOR;  
    var resultCount: Integer; points: GPPOINTF; types: PBYTE;
    startIndex: Integer; endIndex: Integer): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Matrix APIs
//----------------------------------------------------------------------------

GdipCreateMatrix: function(out matrix: GPMATRIX): GPSTATUS; stdcall;  

GdipCreateMatrix2: function(m11: Single; m12: Single; m21: Single; m22: Single;  
    dx: Single; dy: Single; out matrix: GPMATRIX): GPSTATUS; stdcall;

GdipCreateMatrix3: function(rect: GPRECTF; dstplg: GPPOINTF;  
    out matrix: GPMATRIX): GPSTATUS; stdcall;

GdipCreateMatrix3I: function(rect: GPRECT; dstplg: GPPOINT;  
    out matrix: GPMATRIX): GPSTATUS; stdcall;

GdipCloneMatrix: function(matrix: GPMATRIX;
    out cloneMatrix: GPMATRIX): GPSTATUS; stdcall;

GdipDeleteMatrix: function(matrix: GPMATRIX): GPSTATUS; stdcall;  

GdipSetMatrixElements: function(matrix: GPMATRIX; m11: Single; m12: Single;  
    m21: Single; m22: Single; dx: Single; dy: Single): GPSTATUS; stdcall;

GdipMultiplyMatrix: function(matrix: GPMATRIX; matrix2: GPMATRIX;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslateMatrix: function(matrix: GPMATRIX; offsetX: Single;  
    offsetY: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScaleMatrix: function(matrix: GPMATRIX; scaleX: Single; scaleY: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotateMatrix: function(matrix: GPMATRIX; angle: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipShearMatrix: function(matrix: GPMATRIX; shearX: Single; shearY: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipInvertMatrix: function(matrix: GPMATRIX): GPSTATUS; stdcall;  

GdipTransformMatrixPoints: function(matrix: GPMATRIX; pts: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipTransformMatrixPointsI: function(matrix: GPMATRIX; pts: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipVectorTransformMatrixPoints: function(matrix: GPMATRIX; pts: GPPOINTF;
    count: Integer): GPSTATUS; stdcall;

GdipVectorTransformMatrixPointsI: function(matrix: GPMATRIX; pts: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipGetMatrixElements: function(matrix: GPMATRIX;  
    matrixOut: PSingle): GPSTATUS; stdcall;

GdipIsMatrixInvertible: function(matrix: GPMATRIX;  
    out result: Bool): GPSTATUS; stdcall;

GdipIsMatrixIdentity: function(matrix: GPMATRIX;  
    out result: Bool): GPSTATUS; stdcall;

GdipIsMatrixEqual: function(matrix: GPMATRIX; matrix2: GPMATRIX;  
    out result: Bool): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Region APIs
//----------------------------------------------------------------------------

GdipCreateRegion: function(out region: GPREGION): GPSTATUS; stdcall;  

GdipCreateRegionRect: function(rect: GPRECTF;  
    out region: GPREGION): GPSTATUS; stdcall;

GdipCreateRegionRectI: function(rect: GPRECT;  
    out region: GPREGION): GPSTATUS; stdcall;

GdipCreateRegionPath: function(path: GPPATH;  
    out region: GPREGION): GPSTATUS; stdcall;

GdipCreateRegionRgnData: function(regionData: PBYTE; size: Integer;  
    out region: GPREGION): GPSTATUS; stdcall;

GdipCreateRegionHrgn: function(hRgn: HRGN;  
    out region: GPREGION): GPSTATUS; stdcall;

GdipCloneRegion: function(region: GPREGION;  
    out cloneRegion: GPREGION): GPSTATUS; stdcall;

GdipDeleteRegion: function(region: GPREGION): GPSTATUS; stdcall;  

GdipSetInfinite: function(region: GPREGION): GPSTATUS; stdcall;  

GdipSetEmpty: function(region: GPREGION): GPSTATUS; stdcall;  

GdipCombineRegionRect: function(region: GPREGION; rect: GPRECTF;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipCombineRegionRectI: function(region: GPREGION; rect: GPRECT;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipCombineRegionPath: function(region: GPREGION; path: GPPATH;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipCombineRegionRegion: function(region: GPREGION; region2: GPREGION;
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipTranslateRegion: function(region: GPREGION; dx: Single;  
    dy: Single): GPSTATUS; stdcall;

GdipTranslateRegionI: function(region: GPREGION; dx: Integer;
    dy: Integer): GPSTATUS; stdcall;

GdipTransformRegion: function(region: GPREGION;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipGetRegionBounds: function(region: GPREGION; graphics: GPGRAPHICS;  
    rect: GPRECTF): GPSTATUS; stdcall;

GdipGetRegionBoundsI: function(region: GPREGION; graphics: GPGRAPHICS;  
    rect: GPRECT): GPSTATUS; stdcall;

GdipGetRegionHRgn: function(region: GPREGION; graphics: GPGRAPHICS;  
    out hRgn: HRGN): GPSTATUS; stdcall;

GdipIsEmptyRegion: function(region: GPREGION; graphics: GPGRAPHICS;  
    out result: Bool): GPSTATUS; stdcall;

GdipIsInfiniteRegion: function(region: GPREGION; graphics: GPGRAPHICS;  
    out result: Bool): GPSTATUS; stdcall;

GdipIsEqualRegion: function(region: GPREGION; region2: GPREGION;  
    graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipGetRegionDataSize: function(region: GPREGION;  
    out bufferSize: UINT): GPSTATUS; stdcall;

GdipGetRegionData: function(region: GPREGION; buffer: PBYTE;  
    bufferSize: UINT; sizeFilled: PUINT): GPSTATUS; stdcall;

GdipIsVisibleRegionPoint: function(region: GPREGION; x: Single; y: Single;  
    graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipIsVisibleRegionPointI: function(region: GPREGION; x: Integer; y: Integer;  
    graphics: GPGRAPHICS; out result: Bool): GPSTATUS; stdcall;

GdipIsVisibleRegionRect: function(region: GPREGION; x: Single; y: Single;  
    width: Single; height: Single; graphics: GPGRAPHICS;
    out result: Bool): GPSTATUS; stdcall;

GdipIsVisibleRegionRectI: function(region: GPREGION; x: Integer; y: Integer;  
    width: Integer; height: Integer; graphics: GPGRAPHICS;
    out result: Bool): GPSTATUS; stdcall;

GdipGetRegionScansCount: function(region: GPREGION; out count: UINT;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipGetRegionScans: function(region: GPREGION; rects: GPRECTF;  
    out count: Integer; matrix: GPMATRIX): GPSTATUS; stdcall;

GdipGetRegionScansI: function(region: GPREGION; rects: GPRECT;  
    out count: Integer; matrix: GPMATRIX): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Brush APIs
//----------------------------------------------------------------------------

GdipCloneBrush: function(brush: GPBRUSH;  
    out cloneBrush: GPBRUSH): GPSTATUS; stdcall;

GdipDeleteBrush: function(brush: GPBRUSH): GPSTATUS; stdcall;  

GdipGetBrushType: function(brush: GPBRUSH;  
    out type_: GPBRUSHTYPE): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// HatchBrush APIs
//----------------------------------------------------------------------------

GdipCreateHatchBrush: function(hatchstyle: Integer; forecol: ARGB;  
    backcol: ARGB; out brush: GPHATCH): GPSTATUS; stdcall;

GdipGetHatchStyle: function(brush: GPHATCH;  
    out hatchstyle: GPHATCHSTYLE): GPSTATUS; stdcall;

GdipGetHatchForegroundColor: function(brush: GPHATCH;  
    out forecol: ARGB): GPSTATUS; stdcall;

GdipGetHatchBackgroundColor: function(brush: GPHATCH;  
    out backcol: ARGB): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// TextureBrush APIs
//----------------------------------------------------------------------------


GdipCreateTexture: function(image: GPIMAGE; wrapmode: GPWRAPMODE;  
    var texture: GPTEXTURE): GPSTATUS; stdcall;

GdipCreateTexture2: function(image: GPIMAGE; wrapmode: GPWRAPMODE;  
    x: Single; y: Single; width: Single; height: Single;
    out texture: GPTEXTURE): GPSTATUS; stdcall;

GdipCreateTextureIA: function(image: GPIMAGE;  
    imageAttributes: GPIMAGEATTRIBUTES; x: Single; y: Single; width: Single;
    height: Single; out texture: GPTEXTURE): GPSTATUS; stdcall;

GdipCreateTexture2I: function(image: GPIMAGE; wrapmode: GPWRAPMODE; x: Integer;  
    y: Integer; width: Integer; height: Integer;
    out texture: GPTEXTURE): GPSTATUS; stdcall;

GdipCreateTextureIAI: function(image: GPIMAGE;  
    imageAttributes: GPIMAGEATTRIBUTES; x: Integer; y: Integer; width: Integer;
    height: Integer; out texture: GPTEXTURE): GPSTATUS; stdcall;

GdipGetTextureTransform: function(brush: GPTEXTURE;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipSetTextureTransform: function(brush: GPTEXTURE;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetTextureTransform: function(brush: GPTEXTURE): GPSTATUS; stdcall;  

GdipMultiplyTextureTransform: function(brush: GPTEXTURE; matrix: GPMATRIX;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslateTextureTransform: function(brush: GPTEXTURE; dx: Single;  
    dy: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScaleTextureTransform: function(brush: GPTEXTURE; sx: Single; sy: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotateTextureTransform: function(brush: GPTEXTURE; angle: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipSetTextureWrapMode: function(brush: GPTEXTURE;  
    wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipGetTextureWrapMode: function(brush: GPTEXTURE;  
    var wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipGetTextureImage: function(brush: GPTEXTURE;  
    out image: GPIMAGE): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// SolidBrush APIs
//----------------------------------------------------------------------------

GdipCreateSolidFill: function(color: ARGB;  
    out brush: GPSOLIDFILL): GPSTATUS; stdcall;

GdipSetSolidFillColor: function(brush: GPSOLIDFILL;
    color: ARGB): GPSTATUS; stdcall;

GdipGetSolidFillColor: function(brush: GPSOLIDFILL;  
    out color: ARGB): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// LineBrush APIs
//----------------------------------------------------------------------------

GdipCreateLineBrush: function(point1: GPPOINTF; point2: GPPOINTF; color1: ARGB;  
    color2: ARGB; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipCreateLineBrushI: function(point1: GPPOINT; point2: GPPOINT; color1: ARGB;  
    color2: ARGB; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipCreateLineBrushFromRect: function(rect: GPRECTF; color1: ARGB;  
    color2: ARGB; mode: LINEARGRADIENTMODE; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipCreateLineBrushFromRectI: function(rect: GPRECT; color1: ARGB;
    color2: ARGB; mode: LINEARGRADIENTMODE; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipCreateLineBrushFromRectWithAngle: function(rect: GPRECTF; color1: ARGB;  
    color2: ARGB; angle: Single; isAngleScalable: Bool; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipCreateLineBrushFromRectWithAngleI: function(rect: GPRECT; color1: ARGB;  
    color2: ARGB; angle: Single; isAngleScalable: Bool; wrapMode: GPWRAPMODE;
    out lineGradient: GPLINEGRADIENT): GPSTATUS; stdcall;

GdipSetLineColors: function(brush: GPLINEGRADIENT; color1: ARGB;  
    color2: ARGB): GPSTATUS; stdcall;

GdipGetLineColors: function(brush: GPLINEGRADIENT;  
    colors: PARGB): GPSTATUS; stdcall;

GdipGetLineRect: function(brush: GPLINEGRADIENT;  
    rect: GPRECTF): GPSTATUS; stdcall;

GdipGetLineRectI: function(brush: GPLINEGRADIENT;
    rect: GPRECT): GPSTATUS; stdcall;

GdipSetLineGammaCorrection: function(brush: GPLINEGRADIENT;  
    useGammaCorrection: Bool): GPSTATUS; stdcall;

GdipGetLineGammaCorrection: function(brush: GPLINEGRADIENT;  
    out useGammaCorrection: Bool): GPSTATUS; stdcall;

GdipGetLineBlendCount: function(brush: GPLINEGRADIENT;  
    out count: Integer): GPSTATUS; stdcall;

GdipGetLineBlend: function(brush: GPLINEGRADIENT; blend: PSingle;  
    positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetLineBlend: function(brush: GPLINEGRADIENT; blend: PSingle;  
    positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipGetLinePresetBlendCount: function(brush: GPLINEGRADIENT;  
    out count: Integer): GPSTATUS; stdcall;

GdipGetLinePresetBlend: function(brush: GPLINEGRADIENT; blend: PARGB;  
    positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetLinePresetBlend: function(brush: GPLINEGRADIENT; blend: PARGB;  
    positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetLineSigmaBlend: function(brush: GPLINEGRADIENT; focus: Single;  
    scale: Single): GPSTATUS; stdcall;

GdipSetLineLinearBlend: function(brush: GPLINEGRADIENT; focus: Single;  
    scale: Single): GPSTATUS; stdcall;

GdipSetLineWrapMode: function(brush: GPLINEGRADIENT;  
    wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipGetLineWrapMode: function(brush: GPLINEGRADIENT;  
    out wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipGetLineTransform: function(brush: GPLINEGRADIENT;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipSetLineTransform: function(brush: GPLINEGRADIENT;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetLineTransform: function(brush: GPLINEGRADIENT): GPSTATUS; stdcall;  

GdipMultiplyLineTransform: function(brush: GPLINEGRADIENT; matrix: GPMATRIX;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslateLineTransform: function(brush: GPLINEGRADIENT; dx: Single;  
    dy: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScaleLineTransform: function(brush: GPLINEGRADIENT; sx: Single; sy: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotateLineTransform: function(brush: GPLINEGRADIENT; angle: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// PathGradientBrush APIs
//----------------------------------------------------------------------------

GdipCreatePathGradient: function(points: GPPOINTF; count: Integer;  
    wrapMode: GPWRAPMODE; out polyGradient: GPPATHGRADIENT): GPSTATUS; stdcall;

GdipCreatePathGradientI: function(points: GPPOINT; count: Integer;  
    wrapMode: GPWRAPMODE; out polyGradient: GPPATHGRADIENT): GPSTATUS; stdcall;

GdipCreatePathGradientFromPath: function(path: GPPATH;  
    out polyGradient: GPPATHGRADIENT): GPSTATUS; stdcall;

GdipGetPathGradientCenterColor: function(brush: GPPATHGRADIENT;  
    out colors: ARGB): GPSTATUS; stdcall;

GdipSetPathGradientCenterColor: function(brush: GPPATHGRADIENT;  
    colors: ARGB): GPSTATUS; stdcall;

GdipGetPathGradientSurroundColorsWithCount: function(brush: GPPATHGRADIENT;  
    color: PARGB; var count: Integer): GPSTATUS; stdcall;

GdipSetPathGradientSurroundColorsWithCount: function(brush: GPPATHGRADIENT;  
    color: PARGB; var count: Integer): GPSTATUS; stdcall;

GdipGetPathGradientPath: function(brush: GPPATHGRADIENT;  
    path: GPPATH): GPSTATUS; stdcall;

GdipSetPathGradientPath: function(brush: GPPATHGRADIENT;  
    path: GPPATH): GPSTATUS; stdcall;

GdipGetPathGradientCenterPoint: function(brush: GPPATHGRADIENT;  
    points: GPPOINTF): GPSTATUS; stdcall;

GdipGetPathGradientCenterPointI: function(brush: GPPATHGRADIENT;  
    points: GPPOINT): GPSTATUS; stdcall;

GdipSetPathGradientCenterPoint: function(brush: GPPATHGRADIENT;  
    points: GPPOINTF): GPSTATUS; stdcall;

GdipSetPathGradientCenterPointI: function(brush: GPPATHGRADIENT;  
    points: GPPOINT): GPSTATUS; stdcall;

GdipGetPathGradientRect: function(brush: GPPATHGRADIENT;
    rect: GPRECTF): GPSTATUS; stdcall;

GdipGetPathGradientRectI: function(brush: GPPATHGRADIENT;  
    rect: GPRECT): GPSTATUS; stdcall;

GdipGetPathGradientPointCount: function(brush: GPPATHGRADIENT;
    var count: Integer): GPSTATUS; stdcall;

GdipGetPathGradientSurroundColorCount: function(brush: GPPATHGRADIENT;  
    var count: Integer): GPSTATUS; stdcall;

GdipSetPathGradientGammaCorrection: function(brush: GPPATHGRADIENT;  
    useGammaCorrection: Bool): GPSTATUS; stdcall;

GdipGetPathGradientGammaCorrection: function(brush: GPPATHGRADIENT;  
    var useGammaCorrection: Bool): GPSTATUS; stdcall;

GdipGetPathGradientBlendCount: function(brush: GPPATHGRADIENT;  
    var count: Integer): GPSTATUS; stdcall;

GdipGetPathGradientBlend: function(brush: GPPATHGRADIENT;  
    blend: PSingle; positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetPathGradientBlend: function(brush: GPPATHGRADIENT;  
    blend: PSingle; positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipGetPathGradientPresetBlendCount: function(brush: GPPATHGRADIENT;  
    var count: Integer): GPSTATUS; stdcall;

GdipGetPathGradientPresetBlend: function(brush: GPPATHGRADIENT;  
    blend: PARGB; positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetPathGradientPresetBlend: function(brush: GPPATHGRADIENT;  
    blend: PARGB; positions: PSingle; count: Integer): GPSTATUS; stdcall;

GdipSetPathGradientSigmaBlend: function(brush: GPPATHGRADIENT;  
    focus: Single; scale: Single): GPSTATUS; stdcall;

GdipSetPathGradientLinearBlend: function(brush: GPPATHGRADIENT;  
    focus: Single; scale: Single): GPSTATUS; stdcall;

GdipGetPathGradientWrapMode: function(brush: GPPATHGRADIENT;  
    var wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipSetPathGradientWrapMode: function(brush: GPPATHGRADIENT;  
    wrapmode: GPWRAPMODE): GPSTATUS; stdcall;

GdipGetPathGradientTransform: function(brush: GPPATHGRADIENT;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipSetPathGradientTransform: function(brush: GPPATHGRADIENT;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetPathGradientTransform: function(  
    brush: GPPATHGRADIENT): GPSTATUS; stdcall;

GdipMultiplyPathGradientTransform: function(brush: GPPATHGRADIENT;  
    matrix: GPMATRIX; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslatePathGradientTransform: function(brush: GPPATHGRADIENT;  
    dx: Single; dy: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScalePathGradientTransform: function(brush: GPPATHGRADIENT;  
    sx: Single; sy: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotatePathGradientTransform: function(brush: GPPATHGRADIENT;  
    angle: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipGetPathGradientFocusScales: function(brush: GPPATHGRADIENT;
    var xScale: Single; var yScale: Single): GPSTATUS; stdcall;

GdipSetPathGradientFocusScales: function(brush: GPPATHGRADIENT;  
    xScale: Single; yScale: Single): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Pen APIs
//----------------------------------------------------------------------------

GdipCreatePen1: function(color: ARGB; width: Single; unit_: GPUNIT;  
    out pen: GPPEN): GPSTATUS; stdcall;

GdipCreatePen2: function(brush: GPBRUSH; width: Single; unit_: GPUNIT;
    out pen: GPPEN): GPSTATUS; stdcall;

GdipClonePen: function(pen: GPPEN; out clonepen: GPPEN): GPSTATUS; stdcall;  

GdipDeletePen: function(pen: GPPEN): GPSTATUS; stdcall;  

GdipSetPenWidth: function(pen: GPPEN; width: Single): GPSTATUS; stdcall;  

GdipGetPenWidth: function(pen: GPPEN; out width: Single): GPSTATUS; stdcall;  

GdipSetPenUnit: function(pen: GPPEN; unit_: GPUNIT): GPSTATUS; stdcall;  

GdipGetPenUnit: function(pen: GPPEN; var unit_: GPUNIT): GPSTATUS; stdcall;  

GdipSetPenLineCap197819: function(pen: GPPEN; startCap: GPLINECAP;  
    endCap: GPLINECAP; dashCap: GPDASHCAP): GPSTATUS; stdcall;

GdipSetPenStartCap: function(pen: GPPEN;
    startCap: GPLINECAP): GPSTATUS; stdcall;

GdipSetPenEndCap: function(pen: GPPEN; endCap: GPLINECAP): GPSTATUS; stdcall;  

GdipSetPenDashCap197819: function(pen: GPPEN;  
    dashCap: GPDASHCAP): GPSTATUS; stdcall;

GdipGetPenStartCap: function(pen: GPPEN;  
    out startCap: GPLINECAP): GPSTATUS; stdcall;

GdipGetPenEndCap: function(pen: GPPEN;  
    out endCap: GPLINECAP): GPSTATUS; stdcall;

GdipGetPenDashCap197819: function(pen: GPPEN;  
    out dashCap: GPDASHCAP): GPSTATUS; stdcall;

GdipSetPenLineJoin: function(pen: GPPEN;  
    lineJoin: GPLINEJOIN): GPSTATUS; stdcall;

GdipGetPenLineJoin: function(pen: GPPEN;  
    var lineJoin: GPLINEJOIN): GPSTATUS; stdcall;

GdipSetPenCustomStartCap: function(pen: GPPEN;  
    customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipGetPenCustomStartCap: function(pen: GPPEN;  
    out customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipSetPenCustomEndCap: function(pen: GPPEN;  
    customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipGetPenCustomEndCap: function(pen: GPPEN;  
    out customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipSetPenMiterLimit: function(pen: GPPEN;  
    miterLimit: Single): GPSTATUS; stdcall;

GdipGetPenMiterLimit: function(pen: GPPEN;  
    out miterLimit: Single): GPSTATUS; stdcall;

GdipSetPenMode: function(pen: GPPEN;
    penMode: GPPENALIGNMENT): GPSTATUS; stdcall;

GdipGetPenMode: function(pen: GPPEN;  
    var penMode: GPPENALIGNMENT): GPSTATUS; stdcall;

GdipSetPenTransform: function(pen: GPPEN;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipGetPenTransform: function(pen: GPPEN;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetPenTransform: function(pen: GPPEN): GPSTATUS; stdcall;  

GdipMultiplyPenTransform: function(pen: GPPEN; matrix: GPMATRIX;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslatePenTransform: function(pen: GPPEN; dx: Single; dy: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScalePenTransform: function(pen: GPPEN; sx: Single; sy: Single;
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotatePenTransform: function(pen: GPPEN; angle: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipSetPenColor: function(pen: GPPEN; argb: ARGB): GPSTATUS; stdcall;  

GdipGetPenColor: function(pen: GPPEN; out argb: ARGB): GPSTATUS; stdcall;  

GdipSetPenBrushFill: function(pen: GPPEN; brush: GPBRUSH): GPSTATUS; stdcall;  

GdipGetPenBrushFill: function(pen: GPPEN;  
    out brush: GPBRUSH): GPSTATUS; stdcall;

GdipGetPenFillType: function(pen: GPPEN;  
    out type_: GPPENTYPE): GPSTATUS; stdcall;

GdipGetPenDashStyle: function(pen: GPPEN;  
    out dashstyle: GPDASHSTYLE): GPSTATUS; stdcall;

GdipSetPenDashStyle: function(pen: GPPEN;  
    dashstyle: GPDASHSTYLE): GPSTATUS; stdcall;

GdipGetPenDashOffset: function(pen: GPPEN;  
    out offset: Single): GPSTATUS; stdcall;

GdipSetPenDashOffset: function(pen: GPPEN; offset: Single): GPSTATUS; stdcall;  

GdipGetPenDashCount: function(pen: GPPEN;  
    var count: Integer): GPSTATUS; stdcall;

GdipSetPenDashArray: function(pen: GPPEN; dash: PSingle;
    count: Integer): GPSTATUS; stdcall;

GdipGetPenDashArray: function(pen: GPPEN; dash: PSingle;  
    count: Integer): GPSTATUS; stdcall;

GdipGetPenCompoundCount: function(pen: GPPEN;  
    out count: Integer): GPSTATUS; stdcall;

GdipSetPenCompoundArray: function(pen: GPPEN; dash: PSingle;  
    count: Integer): GPSTATUS; stdcall;

GdipGetPenCompoundArray: function(pen: GPPEN; dash: PSingle;  
    count: Integer): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// CustomLineCap APIs
//----------------------------------------------------------------------------

GdipCreateCustomLineCap: function(fillPath: GPPATH; strokePath: GPPATH;  
    baseCap: GPLINECAP; baseInset: Single;
    out customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipDeleteCustomLineCap: function(  
    customCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipCloneCustomLineCap: function(customCap: GPCUSTOMLINECAP;  
    out clonedCap: GPCUSTOMLINECAP): GPSTATUS; stdcall;

GdipGetCustomLineCapType: function(customCap: GPCUSTOMLINECAP;
    var capType: CUSTOMLINECAPTYPE): GPSTATUS; stdcall;

GdipSetCustomLineCapStrokeCaps: function(customCap: GPCUSTOMLINECAP;  
    startCap: GPLINECAP; endCap: GPLINECAP): GPSTATUS; stdcall;

GdipGetCustomLineCapStrokeCaps: function(customCap: GPCUSTOMLINECAP;  
    var startCap: GPLINECAP; var endCap: GPLINECAP): GPSTATUS; stdcall;

GdipSetCustomLineCapStrokeJoin: function(customCap: GPCUSTOMLINECAP;  
  lineJoin: GPLINEJOIN): GPSTATUS; stdcall;

GdipGetCustomLineCapStrokeJoin: function(customCap: GPCUSTOMLINECAP;  
  var lineJoin: GPLINEJOIN): GPSTATUS; stdcall;

GdipSetCustomLineCapBaseCap: function(customCap: GPCUSTOMLINECAP;  
  baseCap: GPLINECAP): GPSTATUS; stdcall;

GdipGetCustomLineCapBaseCap: function(customCap: GPCUSTOMLINECAP;  
  var baseCap: GPLINECAP): GPSTATUS; stdcall;

GdipSetCustomLineCapBaseInset: function(customCap: GPCUSTOMLINECAP;  
  inset: Single): GPSTATUS; stdcall;

GdipGetCustomLineCapBaseInset: function(customCap: GPCUSTOMLINECAP;  
  var inset: Single): GPSTATUS; stdcall;

GdipSetCustomLineCapWidthScale: function(customCap: GPCUSTOMLINECAP;  
  widthScale: Single): GPSTATUS; stdcall;

GdipGetCustomLineCapWidthScale: function(customCap: GPCUSTOMLINECAP;  
  var widthScale: Single): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// AdjustableArrowCap APIs
//----------------------------------------------------------------------------

GdipCreateAdjustableArrowCap: function(height: Single;  
  width: Single;
  isFilled: Bool;
  out cap: GPADJUSTABLEARROWCAP): GPSTATUS; stdcall;

GdipSetAdjustableArrowCapHeight: function(cap: GPADJUSTABLEARROWCAP;
  height: Single): GPSTATUS; stdcall;

GdipGetAdjustableArrowCapHeight: function(cap: GPADJUSTABLEARROWCAP;  
  var height: Single): GPSTATUS; stdcall;

GdipSetAdjustableArrowCapWidth: function(cap: GPADJUSTABLEARROWCAP;  
  width: Single): GPSTATUS; stdcall;

GdipGetAdjustableArrowCapWidth: function(cap: GPADJUSTABLEARROWCAP;  
  var width: Single): GPSTATUS; stdcall;

GdipSetAdjustableArrowCapMiddleInset: function(cap: GPADJUSTABLEARROWCAP;  
  middleInset: Single): GPSTATUS; stdcall;

GdipGetAdjustableArrowCapMiddleInset: function(cap: GPADJUSTABLEARROWCAP;  
  var middleInset: Single): GPSTATUS; stdcall;

GdipSetAdjustableArrowCapFillState: function(cap: GPADJUSTABLEARROWCAP;  
  fillState: Bool): GPSTATUS; stdcall;

GdipGetAdjustableArrowCapFillState: function(cap: GPADJUSTABLEARROWCAP;  
  var fillState: Bool): GPSTATUS; stdcall;

//---------------------------------------------------------------------------- 
// Image APIs
//----------------------------------------------------------------------------

GdipLoadImageFromStream: function(stream: ISTREAM;  
  out image: GPIMAGE): GPSTATUS; stdcall;

GdipLoadImageFromFile: function(filename: PWCHAR;  
  out image: GPIMAGE): GPSTATUS; stdcall;

GdipLoadImageFromStreamICM: function(stream: ISTREAM;  
  out image: GPIMAGE): GPSTATUS; stdcall;

GdipLoadImageFromFileICM: function(filename: PWCHAR;  
  out image: GPIMAGE): GPSTATUS; stdcall;

GdipCloneImage: function(image: GPIMAGE;  
  out cloneImage: GPIMAGE): GPSTATUS; stdcall;

GdipDisposeImage: function(image: GPIMAGE): GPSTATUS; stdcall;  

GdipSaveImageToFile: function(image: GPIMAGE;  
  filename: PWCHAR;
  clsidEncoder: PGUID;
  encoderParams: PENCODERPARAMETERS): GPSTATUS; stdcall;

GdipSaveImageToStream: function(image: GPIMAGE;  
  stream: ISTREAM;
  clsidEncoder: PGUID;
  encoderParams: PENCODERPARAMETERS): GPSTATUS; stdcall;

GdipSaveAdd: function(image: GPIMAGE;  
  encoderParams: PENCODERPARAMETERS): GPSTATUS; stdcall;

GdipSaveAddImage: function(image: GPIMAGE;  
  newImage: GPIMAGE;
  encoderParams: PENCODERPARAMETERS): GPSTATUS; stdcall;

GdipGetImageGraphicsContext: function(image: GPIMAGE;  
  out graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipGetImageBounds: function(image: GPIMAGE;  
  srcRect: GPRECTF;
  var srcUnit: GPUNIT): GPSTATUS; stdcall;

GdipGetImageDimension: function(image: GPIMAGE;  
  var width: Single;
  var height: Single): GPSTATUS; stdcall;

GdipGetImageType: function(image: GPIMAGE;  
  var type_: IMAGETYPE): GPSTATUS; stdcall;

GdipGetImageWidth: function(image: GPIMAGE;  
  var width: UINT): GPSTATUS; stdcall;

GdipGetImageHeight: function(image: GPIMAGE;  
  var height: UINT): GPSTATUS; stdcall;

GdipGetImageHorizontalResolution: function(image: GPIMAGE;  
  var resolution: Single): GPSTATUS; stdcall;

GdipGetImageVerticalResolution: function(image: GPIMAGE;  
  var resolution: Single): GPSTATUS; stdcall;

GdipGetImageFlags: function(image: GPIMAGE;  
  var flags: UINT): GPSTATUS; stdcall;

GdipGetImageRawFormat: function(image: GPIMAGE;  
  format: PGUID): GPSTATUS; stdcall;

GdipGetImagePixelFormat: function(image: GPIMAGE;  
  out format: TPIXELFORMAT): GPSTATUS; stdcall;

GdipGetImageThumbnail: function(image: GPIMAGE; thumbWidth: UINT;  
    thumbHeight: UINT; out thumbImage: GPIMAGE;
    callback: GETTHUMBNAILIMAGEABORT; callbackData: Pointer): GPSTATUS; stdcall;

GdipGetEncoderParameterListSize: function(image: GPIMAGE;  
    clsidEncoder: PGUID; out size: UINT): GPSTATUS; stdcall;

GdipGetEncoderParameterList: function(image: GPIMAGE; clsidEncoder: PGUID;
    size: UINT; buffer: PENCODERPARAMETERS): GPSTATUS; stdcall;

GdipImageGetFrameDimensionsCount: function(image: GPIMAGE;  
    var count: UINT): GPSTATUS; stdcall;

GdipImageGetFrameDimensionsList: function(image: GPIMAGE; dimensionIDs: PGUID;  
    count: UINT): GPSTATUS; stdcall;

GdipImageGetFrameCount: function(image: GPIMAGE; dimensionID: PGUID;  
    var count: UINT): GPSTATUS; stdcall;

GdipImageSelectActiveFrame: function(image: GPIMAGE; dimensionID: PGUID;  
    frameIndex: UINT): GPSTATUS; stdcall;

GdipImageRotateFlip: function(image: GPIMAGE;  
    rfType: ROTATEFLIPTYPE): GPSTATUS; stdcall;

GdipGetImagePalette: function(image: GPIMAGE; palette: PCOLORPALETTE;  
    size: Integer): GPSTATUS; stdcall;

GdipSetImagePalette: function(image: GPIMAGE;  
    palette: PCOLORPALETTE): GPSTATUS; stdcall;

GdipGetImagePaletteSize: function(image: GPIMAGE;  
    var size: Integer): GPSTATUS; stdcall;

GdipGetPropertyCount: function(image: GPIMAGE;  
    var numOfProperty: UINT): GPSTATUS; stdcall;

GdipGetPropertyIdList: function(image: GPIMAGE; numOfProperty: UINT;  
    list: PPROPID): GPSTATUS; stdcall;

GdipGetPropertyItemSize: function(image: GPIMAGE; propId: PROPID;  
    var size: UINT): GPSTATUS; stdcall;

GdipGetPropertyItem: function(image: GPIMAGE; propId: PROPID; propSize: UINT;  
    buffer: PPROPERTYITEM): GPSTATUS; stdcall;

GdipGetPropertySize: function(image: GPIMAGE; var totalBufferSize: UINT;  
    var numProperties: UINT): GPSTATUS; stdcall;

GdipGetAllPropertyItems: function(image: GPIMAGE; totalBufferSize: UINT;  
    numProperties: UINT; allItems: PPROPERTYITEM): GPSTATUS; stdcall;

GdipRemovePropertyItem: function(image: GPIMAGE;  
    propId: PROPID): GPSTATUS; stdcall;

GdipSetPropertyItem: function(image: GPIMAGE;  
    item: PPROPERTYITEM): GPSTATUS; stdcall;

GdipImageForceValidation: function(image: GPIMAGE): GPSTATUS; stdcall;

//---------------------------------------------------------------------------- 
// Bitmap APIs
//----------------------------------------------------------------------------

GdipCreateBitmapFromStream: function(stream: ISTREAM;  
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromFile: function(filename: PWCHAR;  
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromStreamICM: function(stream: ISTREAM;  
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromFileICM: function(filename: PWCHAR;  
    var bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromScan0: function(width: Integer; height: Integer;  
    stride: Integer; format: PIXELFORMAT; scan0: PBYTE;
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromGraphics: function(width: Integer; height: Integer;  
    target: GPGRAPHICS; out bitmap: GPBITMAP): GPSTATUS; stdcall;
    
GdipCreateBitmapFromDirectDrawSurface: function(surface: IDIRECTDRAWSURFACE7;
    out bitmap: GPBITMAP): GPSTATUS; stdcall;    

GdipCreateBitmapFromGdiDib: function(gdiBitmapInfo: PBitmapInfo;  
    gdiBitmapData: Pointer; out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateBitmapFromHBITMAP: function(hbm: HBITMAP; hpal: HPALETTE;  
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateHBITMAPFromBitmap: function(bitmap: GPBITMAP; out hbmReturn: HBITMAP;
    background: ARGB): GPSTATUS; stdcall;

GdipCreateBitmapFromHICON: function(hicon: HICON;  
    out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCreateHICONFromBitmap: function(bitmap: GPBITMAP;  
    out hbmReturn: HICON): GPSTATUS; stdcall;

GdipCreateBitmapFromResource: function(hInstance: HMODULE;  
    lpBitmapName: PWCHAR; out bitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCloneBitmapArea: function(x: Single; y: Single; width: Single;  
    height: Single; format: PIXELFORMAT; srcBitmap: GPBITMAP;
    out dstBitmap: GPBITMAP): GPSTATUS; stdcall;

GdipCloneBitmapAreaI: function(x: Integer; y: Integer; width: Integer;  
    height: Integer; format: PIXELFORMAT; srcBitmap: GPBITMAP;
    out dstBitmap: GPBITMAP): GPSTATUS; stdcall;

GdipBitmapLockBits: function(bitmap: GPBITMAP; rect: GPRECT; flags: UINT;  
    format: PIXELFORMAT; lockedBitmapData: PBITMAPDATA): GPSTATUS; stdcall;

GdipBitmapUnlockBits: function(bitmap: GPBITMAP;  
    lockedBitmapData: PBITMAPDATA): GPSTATUS; stdcall;

GdipBitmapGetPixel: function(bitmap: GPBITMAP; x: Integer; y: Integer;  
    var color: ARGB): GPSTATUS; stdcall;

GdipBitmapSetPixel: function(bitmap: GPBITMAP; x: Integer; y: Integer;  
    color: ARGB): GPSTATUS; stdcall;

GdipBitmapSetResolution: function(bitmap: GPBITMAP; xdpi: Single;  
    ydpi: Single): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// ImageAttributes APIs
//----------------------------------------------------------------------------

GdipCreateImageAttributes: function(  
    out imageattr: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipCloneImageAttributes: function(imageattr: GPIMAGEATTRIBUTES;
    out cloneImageattr: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipDisposeImageAttributes: function(  
    imageattr: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipSetImageAttributesToIdentity: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE): GPSTATUS; stdcall;

GdipResetImageAttributes: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE): GPSTATUS; stdcall;

GdipSetImageAttributesColorMatrix: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool; colorMatrix: PCOLORMATRIX;
    grayMatrix: PCOLORMATRIX; flags: COLORMATRIXFLAGS): GPSTATUS; stdcall;

GdipSetImageAttributesThreshold: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool;
    threshold: Single): GPSTATUS; stdcall;

GdipSetImageAttributesGamma: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool; gamma: Single): GPSTATUS; stdcall;

GdipSetImageAttributesNoOp: function(imageattr: GPIMAGEATTRIBUTES;  
  type_: COLORADJUSTTYPE; enableFlag: Bool): GPSTATUS; stdcall;

GdipSetImageAttributesColorKeys: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool; colorLow: ARGB;
    colorHigh: ARGB): GPSTATUS; stdcall;

GdipSetImageAttributesOutputChannel: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool;
    channelFlags: COLORCHANNELFLAGS): GPSTATUS; stdcall;

GdipSetImageAttributesOutputChannelColorProfile: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool;
    colorProfileFilename: PWCHAR): GPSTATUS; stdcall;

GdipSetImageAttributesRemapTable: function(imageattr: GPIMAGEATTRIBUTES;  
    type_: COLORADJUSTTYPE; enableFlag: Bool; mapSize: UINT;
    map: PCOLORMAP): GPSTATUS; stdcall;

GdipSetImageAttributesWrapMode: function(imageAttr: GPIMAGEATTRIBUTES;
    wrap: WRAPMODE; argb: ARGB; clamp: Bool): GPSTATUS; stdcall;

GdipSetImageAttributesICMMode: function(imageAttr: GPIMAGEATTRIBUTES;  
    on_: Bool): GPSTATUS; stdcall;

GdipGetImageAttributesAdjustedPalette: function(imageAttr: GPIMAGEATTRIBUTES;  
    colorPalette: PCOLORPALETTE;
    colorAdjustType: COLORADJUSTTYPE): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Graphics APIs
//----------------------------------------------------------------------------

GdipFlush: function(graphics: GPGRAPHICS;  
    intention: GPFLUSHINTENTION): GPSTATUS; stdcall;

GdipCreateFromHDC: function(hdc: HDC;  
    out graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipCreateFromHDC2: function(hdc: HDC; hDevice: THandle;  
    out graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipCreateFromHWND: function(hwnd: HWND;  
    out graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipCreateFromHWNDICM: function(hwnd: HWND;  
    out graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipDeleteGraphics: function(graphics: GPGRAPHICS): GPSTATUS; stdcall;  

GdipGetDC: function(graphics: GPGRAPHICS; var hdc: HDC): GPSTATUS; stdcall;  

GdipReleaseDC: function(graphics: GPGRAPHICS; hdc: HDC): GPSTATUS; stdcall;  

GdipSetCompositingMode: function(graphics: GPGRAPHICS;  
    compositingMode: COMPOSITINGMODE): GPSTATUS; stdcall;

GdipGetCompositingMode: function(graphics: GPGRAPHICS;  
    var compositingMode: COMPOSITINGMODE): GPSTATUS; stdcall;

GdipSetRenderingOrigin: function(graphics: GPGRAPHICS; x: Integer;
    y: Integer): GPSTATUS; stdcall;

GdipGetRenderingOrigin: function(graphics: GPGRAPHICS; var x: Integer;  
    var y: Integer): GPSTATUS; stdcall;

GdipSetCompositingQuality: function(graphics: GPGRAPHICS;
    compositingQuality: COMPOSITINGQUALITY): GPSTATUS; stdcall;

GdipGetCompositingQuality: function(graphics: GPGRAPHICS;  
    var compositingQuality: COMPOSITINGQUALITY): GPSTATUS; stdcall;

GdipSetSmoothingMode: function(graphics: GPGRAPHICS;  
    smoothingMode: SMOOTHINGMODE): GPSTATUS; stdcall;

GdipGetSmoothingMode: function(graphics: GPGRAPHICS;  
    var smoothingMode: SMOOTHINGMODE): GPSTATUS; stdcall;

GdipSetPixelOffsetMode: function(graphics: GPGRAPHICS;  
    pixelOffsetMode: PIXELOFFSETMODE): GPSTATUS; stdcall;

GdipGetPixelOffsetMode: function(graphics: GPGRAPHICS;  
    var pixelOffsetMode: PIXELOFFSETMODE): GPSTATUS; stdcall;

GdipSetTextRenderingHint: function(graphics: GPGRAPHICS;  
    mode: TEXTRENDERINGHINT): GPSTATUS; stdcall;

GdipGetTextRenderingHint: function(graphics: GPGRAPHICS;  
    var mode: TEXTRENDERINGHINT): GPSTATUS; stdcall;

GdipSetTextContrast: function(graphics: GPGRAPHICS;  
    contrast: Integer): GPSTATUS; stdcall;

GdipGetTextContrast: function(graphics: GPGRAPHICS;  
    var contrast: UINT): GPSTATUS; stdcall;

GdipSetInterpolationMode: function(graphics: GPGRAPHICS;  
    interpolationMode: INTERPOLATIONMODE): GPSTATUS; stdcall;

GdipGetInterpolationMode: function(graphics: GPGRAPHICS;  
    var interpolationMode: INTERPOLATIONMODE): GPSTATUS; stdcall;

GdipSetWorldTransform: function(graphics: GPGRAPHICS;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetWorldTransform: function(graphics: GPGRAPHICS): GPSTATUS; stdcall;  

GdipMultiplyWorldTransform: function(graphics: GPGRAPHICS; matrix: GPMATRIX;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipTranslateWorldTransform: function(graphics: GPGRAPHICS; dx: Single;  
    dy: Single; order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipScaleWorldTransform: function(graphics: GPGRAPHICS; sx: Single; sy: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipRotateWorldTransform: function(graphics: GPGRAPHICS; angle: Single;  
    order: GPMATRIXORDER): GPSTATUS; stdcall;

GdipGetWorldTransform: function(graphics: GPGRAPHICS;  
    matrix: GPMATRIX): GPSTATUS; stdcall;

GdipResetPageTransform: function(graphics: GPGRAPHICS): GPSTATUS; stdcall;  

GdipGetPageUnit: function(graphics: GPGRAPHICS;  
    var unit_: GPUNIT): GPSTATUS; stdcall;

GdipGetPageScale: function(graphics: GPGRAPHICS;  
    var scale: Single): GPSTATUS; stdcall;

GdipSetPageUnit: function(graphics: GPGRAPHICS;  
    unit_: GPUNIT): GPSTATUS; stdcall;

GdipSetPageScale: function(graphics: GPGRAPHICS;  
    scale: Single): GPSTATUS; stdcall;

GdipGetDpiX: function(graphics: GPGRAPHICS;  
    var dpi: Single): GPSTATUS; stdcall;

GdipGetDpiY: function(graphics: GPGRAPHICS;  
    var dpi: Single): GPSTATUS; stdcall;

GdipTransformPoints: function(graphics: GPGRAPHICS;  
    destSpace: GPCOORDINATESPACE; srcSpace: GPCOORDINATESPACE;
    points: GPPOINTF; count: Integer): GPSTATUS; stdcall;

GdipTransformPointsI: function(graphics: GPGRAPHICS;  
    destSpace: GPCOORDINATESPACE; srcSpace: GPCOORDINATESPACE;
    points: GPPOINT; count: Integer): GPSTATUS; stdcall;

GdipGetNearestColor: function(graphics: GPGRAPHICS;  
    argb: PARGB): GPSTATUS; stdcall;

// Creates the Win9x Halftone Palette (even on NT) with correct Desktop colors

GdipCreateHalftonePalette: function: HPALETTE; stdcall;

GdipDrawLine: function(graphics: GPGRAPHICS; pen: GPPEN; x1: Single;  
    y1: Single; x2: Single; y2: Single): GPSTATUS; stdcall;

GdipDrawLineI: function(graphics: GPGRAPHICS; pen: GPPEN; x1: Integer;  
    y1: Integer; x2: Integer; y2: Integer): GPSTATUS; stdcall;

GdipDrawLines: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawLinesI: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawArc: function(graphics: GPGRAPHICS; pen: GPPEN; x: Single; y: Single;  
    width: Single; height: Single; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipDrawArcI: function(graphics: GPGRAPHICS; pen: GPPEN; x: Integer;  
    y: Integer; width: Integer; height: Integer; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipDrawBezier: function(graphics: GPGRAPHICS; pen: GPPEN; x1: Single;  
    y1: Single; x2: Single; y2: Single; x3: Single; y3: Single; x4: Single;
    y4: Single): GPSTATUS; stdcall;

GdipDrawBezierI: function(graphics: GPGRAPHICS; pen: GPPEN; x1: Integer;  
    y1: Integer; x2: Integer; y2: Integer; x3: Integer; y3: Integer;
    x4: Integer; y4: Integer): GPSTATUS; stdcall;

GdipDrawBeziers: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawBeziersI: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawRectangle: function(graphics: GPGRAPHICS; pen: GPPEN; x: Single;  
    y: Single; width: Single; height: Single): GPSTATUS; stdcall;

GdipDrawRectangleI: function(graphics: GPGRAPHICS; pen: GPPEN; x: Integer;  
    y: Integer; width: Integer; height: Integer): GPSTATUS; stdcall;

GdipDrawRectangles: function(graphics: GPGRAPHICS; pen: GPPEN; rects: GPRECTF;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawRectanglesI: function(graphics: GPGRAPHICS; pen: GPPEN; rects: GPRECT;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawEllipse: function(graphics: GPGRAPHICS; pen: GPPEN; x: Single;
    y: Single; width: Single; height: Single): GPSTATUS; stdcall;

GdipDrawEllipseI: function(graphics: GPGRAPHICS; pen: GPPEN; x: Integer;  
    y: Integer; width: Integer; height: Integer): GPSTATUS; stdcall;

GdipDrawPie: function(graphics: GPGRAPHICS; pen: GPPEN; x: Single; y: Single;  
    width: Single;  height: Single; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipDrawPieI: function(graphics: GPGRAPHICS; pen: GPPEN; x: Integer;  
    y: Integer; width: Integer; height: Integer; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipDrawPolygon: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawPolygonI: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawPath: function(graphics: GPGRAPHICS; pen: GPPEN;  
    path: GPPATH): GPSTATUS; stdcall;

GdipDrawCurve: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawCurveI: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer): GPSTATUS; stdcall;

GdipDrawCurve2: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer; tension: Single): GPSTATUS; stdcall;

GdipDrawCurve2I: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer; tension: Single): GPSTATUS; stdcall;

GdipDrawCurve3: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINTF;  
    count: Integer; offset: Integer; numberOfSegments: Integer;
    tension: Single): GPSTATUS; stdcall;

GdipDrawCurve3I: function(graphics: GPGRAPHICS; pen: GPPEN; points: GPPOINT;  
    count: Integer; offset: Integer; numberOfSegments: Integer;
    tension: Single): GPSTATUS; stdcall;

GdipDrawClosedCurve: function(graphics: GPGRAPHICS; pen: GPPEN;  
    points: GPPOINTF; count: Integer): GPSTATUS; stdcall;

GdipDrawClosedCurveI: function(graphics: GPGRAPHICS; pen: GPPEN;  
    points: GPPOINT; count: Integer): GPSTATUS; stdcall;

GdipDrawClosedCurve2: function(graphics: GPGRAPHICS; pen: GPPEN;  
    points: GPPOINTF; count: Integer; tension: Single): GPSTATUS; stdcall;

GdipDrawClosedCurve2I: function(graphics: GPGRAPHICS; pen: GPPEN;  
    points: GPPOINT; count: Integer; tension: Single): GPSTATUS; stdcall;

GdipGraphicsClear: function(graphics: GPGRAPHICS;  
    color: ARGB): GPSTATUS; stdcall;

GdipFillRectangle: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Single;  
    y: Single; width: Single; height: Single): GPSTATUS; stdcall;

GdipFillRectangleI: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Integer;  
    y: Integer; width: Integer; height: Integer): GPSTATUS; stdcall;

GdipFillRectangles: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    rects: GPRECTF; count: Integer): GPSTATUS; stdcall;

GdipFillRectanglesI: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    rects: GPRECT; count: Integer): GPSTATUS; stdcall;

GdipFillPolygon: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINTF; count: Integer; fillMode: GPFILLMODE): GPSTATUS; stdcall;

GdipFillPolygonI: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINT; count: Integer; fillMode: GPFILLMODE): GPSTATUS; stdcall;

GdipFillPolygon2: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINTF; count: Integer): GPSTATUS; stdcall;

GdipFillPolygon2I: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINT; count: Integer): GPSTATUS; stdcall;

GdipFillEllipse: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Single;
    y: Single; width: Single; height: Single): GPSTATUS; stdcall;

GdipFillEllipseI: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Integer;  
    y: Integer; width: Integer; height: Integer): GPSTATUS; stdcall;

GdipFillPie: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Single;  
    y: Single; width: Single; height: Single; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipFillPieI: function(graphics: GPGRAPHICS; brush: GPBRUSH; x: Integer;  
    y: Integer; width: Integer; height: Integer; startAngle: Single;
    sweepAngle: Single): GPSTATUS; stdcall;

GdipFillPath: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    path: GPPATH): GPSTATUS; stdcall;

GdipFillClosedCurve: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINTF; count: Integer): GPSTATUS; stdcall;

GdipFillClosedCurveI: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINT; count: Integer): GPSTATUS; stdcall;

GdipFillClosedCurve2: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINTF; count: Integer; tension: Single;
    fillMode: GPFILLMODE): GPSTATUS; stdcall;

GdipFillClosedCurve2I: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    points: GPPOINT; count: Integer; tension: Single;
    fillMode: GPFILLMODE): GPSTATUS; stdcall;

GdipFillRegion: function(graphics: GPGRAPHICS; brush: GPBRUSH;  
    region: GPREGION): GPSTATUS; stdcall;

GdipDrawImage: function(graphics: GPGRAPHICS; image: GPIMAGE; x: Single;  
    y: Single): GPSTATUS; stdcall;

GdipDrawImageI: function(graphics: GPGRAPHICS; image: GPIMAGE; x: Integer;  
    y: Integer): GPSTATUS; stdcall;

GdipDrawImageRect: function(graphics: GPGRAPHICS; image: GPIMAGE; x: Single;  
    y: Single; width: Single; height: Single): GPSTATUS; stdcall;

GdipDrawImageRectI: function(graphics: GPGRAPHICS; image: GPIMAGE; x: Integer;  
    y: Integer; width: Integer; height: Integer): GPSTATUS; stdcall;

GdipDrawImagePoints: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    dstpoints: GPPOINTF; count: Integer): GPSTATUS; stdcall;

GdipDrawImagePointsI: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    dstpoints: GPPOINT; count: Integer): GPSTATUS; stdcall;

GdipDrawImagePointRect: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    x: Single; y: Single; srcx: Single; srcy: Single; srcwidth: Single;
    srcheight: Single; srcUnit: GPUNIT): GPSTATUS; stdcall;

GdipDrawImagePointRectI: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    x: Integer; y: Integer; srcx: Integer; srcy: Integer; srcwidth: Integer;
    srcheight: Integer; srcUnit: GPUNIT): GPSTATUS; stdcall;

GdipDrawImageRectRect: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    dstx: Single; dsty: Single; dstwidth: Single; dstheight: Single;
    srcx: Single; srcy: Single; srcwidth: Single; srcheight: Single;
    srcUnit: GPUNIT; imageAttributes: GPIMAGEATTRIBUTES;
    callback: DRAWIMAGEABORT; callbackData: Pointer): GPSTATUS; stdcall;

GdipDrawImageRectRectI: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    dstx: Integer; dsty: Integer; dstwidth: Integer; dstheight: Integer;
    srcx: Integer; srcy: Integer; srcwidth: Integer; srcheight: Integer;
    srcUnit: GPUNIT; imageAttributes: GPIMAGEATTRIBUTES;
    callback: DRAWIMAGEABORT; callbackData: Pointer): GPSTATUS; stdcall;

GdipDrawImagePointsRect: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    points: GPPOINTF; count: Integer; srcx: Single; srcy: Single;
    srcwidth: Single; srcheight: Single; srcUnit: GPUNIT;
    imageAttributes: GPIMAGEATTRIBUTES; callback: DRAWIMAGEABORT;
    callbackData: Pointer): GPSTATUS; stdcall;

GdipDrawImagePointsRectI: function(graphics: GPGRAPHICS; image: GPIMAGE;  
    points: GPPOINT; count: Integer; srcx: Integer; srcy: Integer;
    srcwidth: Integer; srcheight: Integer; srcUnit: GPUNIT;
    imageAttributes: GPIMAGEATTRIBUTES; callback: DRAWIMAGEABORT;
    callbackData: Pointer): GPSTATUS; stdcall;

GdipEnumerateMetafileDestPoint: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoint: PGPPointF; callback: ENUMERATEMETAFILEPROC;
    callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileDestPointI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoint: PGPPoint; callback: ENUMERATEMETAFILEPROC;
    callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileDestRect: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destRect: PGPRectF; callback: ENUMERATEMETAFILEPROC;
    callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileDestRectI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destRect: PGPRect; callback: ENUMERATEMETAFILEPROC;
    callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileDestPoints: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoints: PGPPointF; count: Integer;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileDestPointsI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoints: PGPPoint; count: Integer;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestPoint: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoint: PGPPointF; srcRect: PGPRectF; srcUnit: TUNIT;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestPointI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoint: PGPPoint; srcRect: PGPRect; srcUnit: TUNIT;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestRect: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destRect: PGPRectF; srcRect: PGPRectF; srcUnit: TUNIT;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestRectI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destRect: PGPRect; srcRect: PGPRect; srcUnit: TUNIT;
    callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestPoints: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoints: PGPPointF; count: Integer; srcRect: PGPRectF;
    srcUnit: TUNIT; callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipEnumerateMetafileSrcRectDestPointsI: function(graphics: GPGRAPHICS;  
    metafile: GPMETAFILE; destPoints: PGPPoint; count: Integer; srcRect: PGPRect;
    srcUnit: TUNIT; callback: ENUMERATEMETAFILEPROC; callbackData: Pointer;
    imageAttributes: GPIMAGEATTRIBUTES): GPSTATUS; stdcall;

GdipPlayMetafileRecord: function(metafile: GPMETAFILE;  
    recordType: EMFPLUSRECORDTYPE; flags: UINT; dataSize: UINT;
    data: PBYTE): GPSTATUS; stdcall;

GdipSetClipGraphics: function(graphics: GPGRAPHICS; srcgraphics: GPGRAPHICS;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipSetClipRect: function(graphics: GPGRAPHICS; x: Single; y: Single;  
    width: Single; height: Single; combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipSetClipRectI: function(graphics: GPGRAPHICS; x: Integer; y: Integer;  
    width: Integer; height: Integer;
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipSetClipPath: function(graphics: GPGRAPHICS; path: GPPATH;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipSetClipRegion: function(graphics: GPGRAPHICS; region: GPREGION;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipSetClipHrgn: function(graphics: GPGRAPHICS; hRgn: HRGN;  
    combineMode: COMBINEMODE): GPSTATUS; stdcall;

GdipResetClip: function(graphics: GPGRAPHICS): GPSTATUS; stdcall;  

GdipTranslateClip: function(graphics: GPGRAPHICS; dx: Single;  
    dy: Single): GPSTATUS; stdcall;

GdipTranslateClipI: function(graphics: GPGRAPHICS; dx: Integer;
    dy: Integer): GPSTATUS; stdcall;

GdipGetClip: function(graphics: GPGRAPHICS;  
    region: GPREGION): GPSTATUS; stdcall;

GdipGetClipBounds: function(graphics: GPGRAPHICS;  
    rect: GPRECTF): GPSTATUS; stdcall;

GdipGetClipBoundsI: function(graphics: GPGRAPHICS;  
    rect: GPRECT): GPSTATUS; stdcall;

GdipIsClipEmpty: function(graphics: GPGRAPHICS;  
    result: PBool): GPSTATUS; stdcall;

GdipGetVisibleClipBounds: function(graphics: GPGRAPHICS;  
    rect: GPRECTF): GPSTATUS; stdcall;

GdipGetVisibleClipBoundsI: function(graphics: GPGRAPHICS;  
    rect: GPRECT): GPSTATUS; stdcall;

GdipIsVisibleClipEmpty: function(graphics: GPGRAPHICS;
    var result: Bool): GPSTATUS; stdcall;

GdipIsVisiblePoint: function(graphics: GPGRAPHICS; x: Single; y: Single;  
    var result: Bool): GPSTATUS; stdcall;

GdipIsVisiblePointI: function(graphics: GPGRAPHICS; x: Integer; y: Integer;  
    var result: Bool): GPSTATUS; stdcall;

GdipIsVisibleRect: function(graphics: GPGRAPHICS; x: Single; y: Single;  
    width: Single; height: Single; var result: Bool): GPSTATUS; stdcall;

GdipIsVisibleRectI: function(graphics: GPGRAPHICS; x: Integer; y: Integer;  
    width: Integer; height: Integer; var result: Bool): GPSTATUS; stdcall;

GdipSaveGraphics: function(graphics: GPGRAPHICS;  
    var state: GRAPHICSSTATE): GPSTATUS; stdcall;

GdipRestoreGraphics: function(graphics: GPGRAPHICS;  
    state: GRAPHICSSTATE): GPSTATUS; stdcall;

GdipBeginContainer: function(graphics: GPGRAPHICS; dstrect: GPRECTF;  
    srcrect: GPRECTF; unit_: GPUNIT;
    var state: GRAPHICSCONTAINER): GPSTATUS; stdcall;

GdipBeginContainerI: function(graphics: GPGRAPHICS; dstrect: GPRECT;  
    srcrect: GPRECT; unit_: GPUNIT;
    var state: GRAPHICSCONTAINER): GPSTATUS; stdcall;

GdipBeginContainer2: function(graphics: GPGRAPHICS;  
    var state: GRAPHICSCONTAINER): GPSTATUS; stdcall;

GdipEndContainer: function(graphics: GPGRAPHICS;  
    state: GRAPHICSCONTAINER): GPSTATUS; stdcall;

GdipGetMetafileHeaderFromWmf: function(hWmf: HMETAFILE;  
    wmfPlaceableFileHeader: PWMFPLACEABLEFILEHEADER;
    header: Pointer): GPSTATUS; stdcall;

GdipGetMetafileHeaderFromEmf: function(hEmf: HENHMETAFILE;  
    header: Pointer): GPSTATUS; stdcall;

GdipGetMetafileHeaderFromFile: function(filename: PWCHAR;  
    header: Pointer): GPSTATUS; stdcall;

GdipGetMetafileHeaderFromStream: function(stream: ISTREAM;  
    header: Pointer): GPSTATUS; stdcall;

GdipGetMetafileHeaderFromMetafile: function(metafile: GPMETAFILE;  
    header: Pointer): GPSTATUS; stdcall;

GdipGetHemfFromMetafile: function(metafile: GPMETAFILE;  
    var hEmf: HENHMETAFILE): GPSTATUS; stdcall;

GdipCreateStreamOnFile: function(filename: PWCHAR; access: UINT;  
    out stream: ISTREAM): GPSTATUS; stdcall;

GdipCreateMetafileFromWmf: function(hWmf: HMETAFILE; deleteWmf: Bool;  
    wmfPlaceableFileHeader: PWMFPLACEABLEFILEHEADER;
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipCreateMetafileFromEmf: function(hEmf: HENHMETAFILE; deleteEmf: Bool;  
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipCreateMetafileFromFile: function(file_: PWCHAR;  
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipCreateMetafileFromWmfFile: function(file_: PWCHAR;  
    wmfPlaceableFileHeader: PWMFPLACEABLEFILEHEADER;
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipCreateMetafileFromStream: function(stream: ISTREAM;  
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafile: function(referenceHdc: HDC; type_: EMFTYPE;  
    frameRect: GPRECTF; frameUnit: METAFILEFRAMEUNIT;
    description: PWCHAR; out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafileI: function(referenceHdc: HDC; type_: EMFTYPE;  
    frameRect: GPRECT; frameUnit: METAFILEFRAMEUNIT; description: PWCHAR;
    out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafileFileName: function(fileName: PWCHAR; referenceHdc: HDC;  
    type_: EMFTYPE; frameRect: GPRECTF; frameUnit: METAFILEFRAMEUNIT;
    description: PWCHAR; out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafileFileNameI: function(fileName: PWCHAR; referenceHdc: HDC;  
    type_: EMFTYPE; frameRect: GPRECT; frameUnit: METAFILEFRAMEUNIT;
    description: PWCHAR; out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafileStream: function(stream: ISTREAM; referenceHdc: HDC;  
    type_: EMFTYPE; frameRect: GPRECTF; frameUnit: METAFILEFRAMEUNIT;
    description: PWCHAR; out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipRecordMetafileStreamI: function(stream: ISTREAM; referenceHdc: HDC;  
    type_: EMFTYPE; frameRect: GPRECT; frameUnit: METAFILEFRAMEUNIT;
    description: PWCHAR; out metafile: GPMETAFILE): GPSTATUS; stdcall;

GdipSetMetafileDownLevelRasterizationLimit: function(metafile: GPMETAFILE;  
    metafileRasterizationLimitDpi: UINT): GPSTATUS; stdcall;

GdipGetMetafileDownLevelRasterizationLimit: function(metafile: GPMETAFILE;  
    var metafileRasterizationLimitDpi: UINT): GPSTATUS; stdcall;

GdipGetImageDecodersSize: function(out numDecoders: UINT;
    out size: UINT): GPSTATUS; stdcall;

GdipGetImageDecoders: function(numDecoders: UINT; size: UINT;  
    decoders: PIMAGECODECINFO): GPSTATUS; stdcall;

GdipGetImageEncodersSize: function(out numEncoders: UINT;  
    out size: UINT): GPSTATUS; stdcall;

GdipGetImageEncoders: function(numEncoders: UINT; size: UINT;  
    encoders: PIMAGECODECINFO): GPSTATUS; stdcall;

GdipComment: function(graphics: GPGRAPHICS; sizeData: UINT;  
    data: PBYTE): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// FontFamily APIs
//----------------------------------------------------------------------------

GdipCreateFontFamilyFromName: function(name: PWCHAR;  
    fontCollection: GPFONTCOLLECTION;
    out FontFamily: GPFONTFAMILY): GPSTATUS; stdcall;

GdipDeleteFontFamily: function(FontFamily: GPFONTFAMILY): GPSTATUS; stdcall;  

GdipCloneFontFamily: function(FontFamily: GPFONTFAMILY;  
    out clonedFontFamily: GPFONTFAMILY): GPSTATUS; stdcall;

GdipGetGenericFontFamilySansSerif: function(  
    out nativeFamily: GPFONTFAMILY): GPSTATUS; stdcall;

GdipGetGenericFontFamilySerif: function(  
    out nativeFamily: GPFONTFAMILY): GPSTATUS; stdcall;

GdipGetGenericFontFamilyMonospace: function(  
    out nativeFamily: GPFONTFAMILY): GPSTATUS; stdcall;

GdipGetFamilyName: function(family: GPFONTFAMILY; name: PWideChar;  
    language: LANGID): GPSTATUS; stdcall;

GdipIsStyleAvailable: function(family: GPFONTFAMILY; style: Integer;  
    var IsStyleAvailable: Bool): GPSTATUS; stdcall;

GdipFontCollectionEnumerable: function(fontCollection: GPFONTCOLLECTION;  
    graphics: GPGRAPHICS; var numFound: Integer): GPSTATUS; stdcall;

GdipFontCollectionEnumerate: function(fontCollection: GPFONTCOLLECTION;  
    numSought: Integer; gpfamilies: array of GPFONTFAMILY;
    var numFound: Integer; graphics: GPGRAPHICS): GPSTATUS; stdcall;

GdipGetEmHeight: function(family: GPFONTFAMILY; style: Integer;  
    out EmHeight: UINT16): GPSTATUS; stdcall;

GdipGetCellAscent: function(family: GPFONTFAMILY; style: Integer;  
    var CellAscent: UINT16): GPSTATUS; stdcall;

GdipGetCellDescent: function(family: GPFONTFAMILY; style: Integer;  
    var CellDescent: UINT16): GPSTATUS; stdcall;

GdipGetLineSpacing: function(family: GPFONTFAMILY; style: Integer;  
    var LineSpacing: UINT16): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Font APIs
//----------------------------------------------------------------------------

GdipCreateFontFromDC: function(hdc: HDC; out font: GPFONT): GPSTATUS; stdcall;  

GdipCreateFontFromLogfontA: function(hdc: HDC; logfont: PLOGFONTA;  
    out font: GPFONT): GPSTATUS; stdcall;

GdipCreateFontFromLogfontW: function(hdc: HDC; logfont: PLOGFONTW;  
    out font: GPFONT): GPSTATUS; stdcall;

GdipCreateFont: function(fontFamily: GPFONTFAMILY; emSize: Single;  
    style: Integer; unit_: Integer; out font: GPFONT): GPSTATUS; stdcall;

GdipCloneFont: function(font: GPFONT;  
    out cloneFont: GPFONT): GPSTATUS; stdcall;

GdipDeleteFont: function(font: GPFONT): GPSTATUS; stdcall;  

GdipGetFamily: function(font: GPFONT;  
    out family: GPFONTFAMILY): GPSTATUS; stdcall;

GdipGetFontStyle: function(font: GPFONT;  
    var style: Integer): GPSTATUS; stdcall;

GdipGetFontSize: function(font: GPFONT; var size: Single): GPSTATUS; stdcall;  

GdipGetFontUnit: function(font: GPFONT; var unit_: TUNIT): GPSTATUS; stdcall;  

GdipGetFontHeight: function(font: GPFONT; graphics: GPGRAPHICS;  
    var height: Single): GPSTATUS; stdcall;

GdipGetFontHeightGivenDPI: function(font: GPFONT; dpi: Single;  
    var height: Single): GPSTATUS; stdcall;

GdipGetLogFontA: function(font: GPFONT; graphics: GPGRAPHICS;  
    var logfontA: LOGFONTA): GPSTATUS; stdcall;

GdipGetLogFontW: function(font: GPFONT; graphics: GPGRAPHICS;  
    var logfontW: LOGFONTW): GPSTATUS; stdcall;

GdipNewInstalledFontCollection: function(  
    out fontCollection: GPFONTCOLLECTION): GPSTATUS; stdcall;

GdipNewPrivateFontCollection: function(  
    out fontCollection: GPFONTCOLLECTION): GPSTATUS; stdcall;

GdipDeletePrivateFontCollection: function(  
    out fontCollection: GPFONTCOLLECTION): GPSTATUS; stdcall;

GdipGetFontCollectionFamilyCount: function(fontCollection: GPFONTCOLLECTION;  
    var numFound: Integer): GPSTATUS; stdcall;

GdipGetFontCollectionFamilyList: function(fontCollection: GPFONTCOLLECTION;  
    numSought: Integer; gpfamilies: GPFONTFAMILY;
    var numFound: Integer): GPSTATUS; stdcall;

GdipPrivateAddFontFile: function(fontCollection: GPFONTCOLLECTION;  
    filename: PWCHAR): GPSTATUS; stdcall;

GdipPrivateAddMemoryFont: function(fontCollection: GPFONTCOLLECTION;
    memory: Pointer; length: Integer): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Text APIs
//----------------------------------------------------------------------------

GdipDrawString: function(graphics: GPGRAPHICS; string_: PWCHAR;  
    length: Integer; font: GPFONT; layoutRect: PGPRectF;
    stringFormat: GPSTRINGFORMAT; brush: GPBRUSH): GPSTATUS; stdcall;

GdipMeasureString: function(graphics: GPGRAPHICS; string_: PWCHAR;  
    length: Integer; font: GPFONT; layoutRect: PGPRectF;
    stringFormat: GPSTRINGFORMAT; boundingBox: PGPRectF;
    codepointsFitted: PInteger; linesFilled: PInteger): GPSTATUS; stdcall;

GdipMeasureCharacterRanges: function(graphics: GPGRAPHICS; string_: PWCHAR;  
    length: Integer; font: GPFONT; layoutRect: PGPRectF;
    stringFormat: GPSTRINGFORMAT; regionCount: Integer;
    const regions: GPREGION): GPSTATUS; stdcall;

GdipDrawDriverString: function(graphics: GPGRAPHICS; const text: PUINT16;  
    length: Integer; const font: GPFONT; const brush: GPBRUSH;
    const positions: PGPPointF; flags: Integer;
    const matrix: GPMATRIX): GPSTATUS; stdcall;

GdipMeasureDriverString: function(graphics: GPGRAPHICS; text: PUINT16;  
    length: Integer; font: GPFONT; positions: PGPPointF; flags: Integer;
    matrix: GPMATRIX; boundingBox: PGPRectF): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// String format APIs
//----------------------------------------------------------------------------

GdipCreateStringFormat: function(formatAttributes: Integer; language: LANGID;  
    out format: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipStringFormatGetGenericDefault: function(  
    out format: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipStringFormatGetGenericTypographic: function(  
    out format: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipDeleteStringFormat: function(format: GPSTRINGFORMAT): GPSTATUS; stdcall;  

GdipCloneStringFormat: function(format: GPSTRINGFORMAT;  
    out newFormat: GPSTRINGFORMAT): GPSTATUS; stdcall;

GdipSetStringFormatFlags: function(format: GPSTRINGFORMAT;  
    flags: Integer): GPSTATUS; stdcall;

GdipGetStringFormatFlags: function(format: GPSTRINGFORMAT;  
    out flags: Integer): GPSTATUS; stdcall;

GdipSetStringFormatAlign: function(format: GPSTRINGFORMAT;  
    align: STRINGALIGNMENT): GPSTATUS; stdcall;

GdipGetStringFormatAlign: function(format: GPSTRINGFORMAT;  
    out align: STRINGALIGNMENT): GPSTATUS; stdcall;

GdipSetStringFormatLineAlign: function(format: GPSTRINGFORMAT;  
    align: STRINGALIGNMENT): GPSTATUS; stdcall;

GdipGetStringFormatLineAlign: function(format: GPSTRINGFORMAT;  
    out align: STRINGALIGNMENT): GPSTATUS; stdcall;

GdipSetStringFormatTrimming: function(format: GPSTRINGFORMAT;  
    trimming: STRINGTRIMMING): GPSTATUS; stdcall;

GdipGetStringFormatTrimming: function(format: GPSTRINGFORMAT;  
    out trimming: STRINGTRIMMING): GPSTATUS; stdcall;

GdipSetStringFormatHotkeyPrefix: function(format: GPSTRINGFORMAT;  
    hotkeyPrefix: Integer): GPSTATUS; stdcall;

GdipGetStringFormatHotkeyPrefix: function(format: GPSTRINGFORMAT;  
    out hotkeyPrefix: Integer): GPSTATUS; stdcall;

GdipSetStringFormatTabStops: function(format: GPSTRINGFORMAT;  
    firstTabOffset: Single; count: Integer;
    tabStops: PSingle): GPSTATUS; stdcall;

GdipGetStringFormatTabStops: function(format: GPSTRINGFORMAT;  
    count: Integer; firstTabOffset: PSingle;
    tabStops: PSingle): GPSTATUS; stdcall;

GdipGetStringFormatTabStopCount: function(format: GPSTRINGFORMAT;  
    out count: Integer): GPSTATUS; stdcall;

GdipSetStringFormatDigitSubstitution: function(format: GPSTRINGFORMAT;  
    language: LANGID;
    substitute: STRINGDIGITSUBSTITUTE): GPSTATUS; stdcall;

GdipGetStringFormatDigitSubstitution: function(format: GPSTRINGFORMAT;  
    language: PUINT; substitute: PSTRINGDIGITSUBSTITUTE): GPSTATUS; stdcall;

GdipGetStringFormatMeasurableCharacterRangeCount: function(format: GPSTRINGFORMAT;  
    out count: Integer): GPSTATUS; stdcall;

GdipSetStringFormatMeasurableCharacterRanges: function(format: GPSTRINGFORMAT;
    rangeCount: Integer; ranges: PCHARACTERRANGE): GPSTATUS; stdcall;

//----------------------------------------------------------------------------
// Cached Bitmap APIs
//----------------------------------------------------------------------------

GdipCreateCachedBitmap: function(bitmap: GPBITMAP; graphics: GPGRAPHICS;  
    out cachedBitmap: GPCACHEDBITMAP): GPSTATUS; stdcall;

GdipDeleteCachedBitmap: function(  
    cachedBitmap: GPCACHEDBITMAP): GPSTATUS; stdcall;

GdipDrawCachedBitmap: function(graphics: GPGRAPHICS;  
    cachedBitmap: GPCACHEDBITMAP; x: Integer;
    y: Integer): GPSTATUS; stdcall;

GdipEmfToWmfBits: function(hemf: HENHMETAFILE; cbData16: UINT; pData16: PBYTE;  
    iMapMode: Integer; eFlags: Integer): UINT; stdcall;


//=============================================  //9999
procedure LoadGdiplus;
procedure FreeGdiplus;

function MakeRect(x, y, width, height: Single): TGPRectF; overload;
function MakeRect(location: TGPPointF; size: TGPSizeF): TGPRectF; overload;
function MakeRect(x, y, width, height: Integer): TGPRect; overload;
function MakeRect(location: TGPPoint; size: TGPSize): TGPRect; overload;
function MakeRect(const Rect: TRect): TGPRect; overload;

function MakePoint(X, Y: Integer): TGPPoint; overload;
function MakePoint(X, Y: Single): TGPPointF; overload;

function MakeCharacterRange(First, Length: Integer): TCharacterRange;

var
  GDIPlusFileName: string;
  GdipLibrary: THandle = 0;
//========================================================================
implementation

procedure LoadGdiplus;
begin
  GDIPlusFileName := 'gdiplus.dll';

  GdipLibrary := LoadLibrary(PChar(GDIPlusFileName));

  if GdipLibrary > 0 then
  begin
    GdipAlloc := GetProcAddress(GdipLibrary, 'GdipAlloc');
    GdipFree := GetProcAddress(GdipLibrary, 'GdipFree');
    GdiplusStartup := GetProcAddress(GdipLibrary, 'GdiplusStartup');
    GdiplusShutdown := GetProcAddress(GdipLibrary, 'GdiplusShutdown');

    GdipCreatePath := GetProcAddress(GdipLibrary, 'GdipCreatePath');
    GdipCreatePath2 := GetProcAddress(GdipLibrary, 'GdipCreatePath2');
    GdipCreatePath2I := GetProcAddress(GdipLibrary, 'GdipCreatePath2I');
    GdipClonePath := GetProcAddress(GdipLibrary, 'GdipClonePath');
    GdipDeletePath := GetProcAddress(GdipLibrary, 'GdipDeletePath');
    GdipResetPath := GetProcAddress(GdipLibrary, 'GdipResetPath');
    GdipGetPointCount := GetProcAddress(GdipLibrary, 'GdipGetPointCount');
    GdipGetPathTypes := GetProcAddress(GdipLibrary, 'GdipGetPathTypes');
    GdipGetPathPoints := GetProcAddress(GdipLibrary, 'GdipGetPathPoints');
    GdipGetPathPointsI := GetProcAddress(GdipLibrary, 'GdipGetPathPointsI');
    GdipGetPathFillMode := GetProcAddress(GdipLibrary, 'GdipGetPathFillMode');
    GdipSetPathFillMode := GetProcAddress(GdipLibrary, 'GdipSetPathFillMode');
    GdipGetPathData := GetProcAddress(GdipLibrary, 'GdipGetPathData');
    GdipStartPathFigure := GetProcAddress(GdipLibrary, 'GdipStartPathFigure');
    GdipClosePathFigure := GetProcAddress(GdipLibrary, 'GdipClosePathFigure');
    GdipClosePathFigures := GetProcAddress(GdipLibrary, 'GdipClosePathFigures');
    GdipSetPathMarker := GetProcAddress(GdipLibrary, 'GdipSetPathMarker');
    GdipClearPathMarkers := GetProcAddress(GdipLibrary, 'GdipClearPathMarkers');
    GdipReversePath := GetProcAddress(GdipLibrary, 'GdipReversePath');
    GdipGetPathLastPoint := GetProcAddress(GdipLibrary, 'GdipGetPathLastPoint');
    GdipAddPathLine := GetProcAddress(GdipLibrary, 'GdipAddPathLine');
    GdipAddPathLine2 := GetProcAddress(GdipLibrary, 'GdipAddPathLine2');
    GdipAddPathArc := GetProcAddress(GdipLibrary, 'GdipAddPathArc');
    GdipAddPathBezier := GetProcAddress(GdipLibrary, 'GdipAddPathBezier');
    GdipAddPathBeziers := GetProcAddress(GdipLibrary, 'GdipAddPathBeziers');
    GdipAddPathCurve := GetProcAddress(GdipLibrary, 'GdipAddPathCurve');
    GdipAddPathCurve2 := GetProcAddress(GdipLibrary, 'GdipAddPathCurve2');
    GdipAddPathCurve3 := GetProcAddress(GdipLibrary, 'GdipAddPathCurve3');
    GdipAddPathClosedCurve := GetProcAddress(GdipLibrary, 'GdipAddPathClosedCurve');
    GdipAddPathClosedCurve2 := GetProcAddress(GdipLibrary, 'GdipAddPathClosedCurve2');
    GdipAddPathRectangle := GetProcAddress(GdipLibrary, 'GdipAddPathRectangle');
    GdipAddPathRectangles := GetProcAddress(GdipLibrary, 'GdipAddPathRectangles');
    GdipAddPathEllipse := GetProcAddress(GdipLibrary, 'GdipAddPathEllipse');
    GdipAddPathPie := GetProcAddress(GdipLibrary, 'GdipAddPathPie');
    GdipAddPathPolygon := GetProcAddress(GdipLibrary, 'GdipAddPathPolygon');
    GdipAddPathPath := GetProcAddress(GdipLibrary, 'GdipAddPathPath');
    GdipAddPathString := GetProcAddress(GdipLibrary, 'GdipAddPathString');
    GdipAddPathStringI := GetProcAddress(GdipLibrary, 'GdipAddPathStringI');
    GdipAddPathLineI := GetProcAddress(GdipLibrary, 'GdipAddPathLineI');
    GdipAddPathLine2I := GetProcAddress(GdipLibrary, 'GdipAddPathLine2I');
    GdipAddPathArcI := GetProcAddress(GdipLibrary, 'GdipAddPathArcI');
    GdipAddPathBezierI := GetProcAddress(GdipLibrary, 'GdipAddPathBezierI');
    GdipAddPathBeziersI := GetProcAddress(GdipLibrary, 'GdipAddPathBeziersI');
    GdipAddPathCurveI := GetProcAddress(GdipLibrary, 'GdipAddPathCurveI');
    GdipAddPathCurve2I := GetProcAddress(GdipLibrary, 'GdipAddPathCurve2I');
    GdipAddPathCurve3I := GetProcAddress(GdipLibrary, 'GdipAddPathCurve3I');
    GdipAddPathClosedCurveI := GetProcAddress(GdipLibrary, 'GdipAddPathClosedCurveI');
    GdipAddPathClosedCurve2I := GetProcAddress(GdipLibrary, 'GdipAddPathClosedCurve2I');
    GdipAddPathRectangleI := GetProcAddress(GdipLibrary, 'GdipAddPathRectangleI');
    GdipAddPathRectanglesI := GetProcAddress(GdipLibrary, 'GdipAddPathRectanglesI');
    GdipAddPathEllipseI := GetProcAddress(GdipLibrary, 'GdipAddPathEllipseI');
    GdipAddPathPieI := GetProcAddress(GdipLibrary, 'GdipAddPathPieI');
    GdipAddPathPolygonI := GetProcAddress(GdipLibrary, 'GdipAddPathPolygonI');
    GdipFlattenPath := GetProcAddress(GdipLibrary, 'GdipFlattenPath');
    GdipWindingModeOutline := GetProcAddress(GdipLibrary, 'GdipWindingModeOutline');
    GdipWidenPath := GetProcAddress(GdipLibrary, 'GdipWidenPath');
    GdipWarpPath := GetProcAddress(GdipLibrary, 'GdipWarpPath');
    GdipTransformPath := GetProcAddress(GdipLibrary, 'GdipTransformPath');
    GdipGetPathWorldBounds := GetProcAddress(GdipLibrary, 'GdipGetPathWorldBounds');
    GdipGetPathWorldBoundsI := GetProcAddress(GdipLibrary, 'GdipGetPathWorldBoundsI');
    GdipIsVisiblePathPoint := GetProcAddress(GdipLibrary, 'GdipIsVisiblePathPoint');
    GdipIsVisiblePathPointI := GetProcAddress(GdipLibrary, 'GdipIsVisiblePathPointI');
    GdipIsOutlineVisiblePathPoint := GetProcAddress(GdipLibrary, 'GdipIsOutlineVisiblePathPoint');
    GdipIsOutlineVisiblePathPointI := GetProcAddress(GdipLibrary, 'GdipIsOutlineVisiblePathPointI');
    GdipCreatePathIter := GetProcAddress(GdipLibrary, 'GdipCreatePathIter');
    GdipDeletePathIter := GetProcAddress(GdipLibrary, 'GdipDeletePathIter');
    GdipPathIterNextSubpath := GetProcAddress(GdipLibrary, 'GdipPathIterNextSubpath');
    GdipPathIterNextSubpathPath := GetProcAddress(GdipLibrary, 'GdipPathIterNextSubpathPath');
    GdipPathIterNextPathType := GetProcAddress(GdipLibrary, 'GdipPathIterNextPathType');
    GdipPathIterNextMarker := GetProcAddress(GdipLibrary, 'GdipPathIterNextMarker');
    GdipPathIterNextMarkerPath := GetProcAddress(GdipLibrary, 'GdipPathIterNextMarkerPath');
    GdipPathIterGetCount := GetProcAddress(GdipLibrary, 'GdipPathIterGetCount');
    GdipPathIterGetSubpathCount := GetProcAddress(GdipLibrary, 'GdipPathIterGetSubpathCount');
    GdipPathIterIsValid := GetProcAddress(GdipLibrary, 'GdipPathIterIsValid');
    GdipPathIterHasCurve := GetProcAddress(GdipLibrary, 'GdipPathIterHasCurve');
    GdipPathIterRewind := GetProcAddress(GdipLibrary, 'GdipPathIterRewind');
    GdipPathIterEnumerate := GetProcAddress(GdipLibrary, 'GdipPathIterEnumerate');
    GdipPathIterCopyData := GetProcAddress(GdipLibrary, 'GdipPathIterCopyData');
    GdipCreateMatrix := GetProcAddress(GdipLibrary, 'GdipCreateMatrix');
    GdipCreateMatrix2 := GetProcAddress(GdipLibrary, 'GdipCreateMatrix2');
    GdipCreateMatrix3 := GetProcAddress(GdipLibrary, 'GdipCreateMatrix3');
    GdipCreateMatrix3I := GetProcAddress(GdipLibrary, 'GdipCreateMatrix3I');
    GdipCloneMatrix := GetProcAddress(GdipLibrary, 'GdipCloneMatrix');
    GdipDeleteMatrix := GetProcAddress(GdipLibrary, 'GdipDeleteMatrix');
    GdipSetMatrixElements := GetProcAddress(GdipLibrary, 'GdipSetMatrixElements');
    GdipMultiplyMatrix := GetProcAddress(GdipLibrary, 'GdipMultiplyMatrix');
    GdipTranslateMatrix := GetProcAddress(GdipLibrary, 'GdipTranslateMatrix');
    GdipScaleMatrix := GetProcAddress(GdipLibrary, 'GdipScaleMatrix');
    GdipRotateMatrix := GetProcAddress(GdipLibrary, 'GdipRotateMatrix');
    GdipShearMatrix := GetProcAddress(GdipLibrary, 'GdipShearMatrix');
    GdipInvertMatrix := GetProcAddress(GdipLibrary, 'GdipInvertMatrix');
    GdipTransformMatrixPoints := GetProcAddress(GdipLibrary, 'GdipTransformMatrixPoints');
    GdipTransformMatrixPointsI := GetProcAddress(GdipLibrary, 'GdipTransformMatrixPointsI');
    GdipVectorTransformMatrixPoints := GetProcAddress(GdipLibrary, 'GdipVectorTransformMatrixPoints');
    GdipVectorTransformMatrixPointsI := GetProcAddress(GdipLibrary, 'GdipVectorTransformMatrixPointsI');
    GdipGetMatrixElements := GetProcAddress(GdipLibrary, 'GdipGetMatrixElements');
    GdipIsMatrixInvertible := GetProcAddress(GdipLibrary, 'GdipIsMatrixInvertible');
    GdipIsMatrixIdentity := GetProcAddress(GdipLibrary, 'GdipIsMatrixIdentity');
    GdipIsMatrixEqual := GetProcAddress(GdipLibrary, 'GdipIsMatrixEqual');
    GdipCreateRegion := GetProcAddress(GdipLibrary, 'GdipCreateRegion');
    GdipCreateRegionRect := GetProcAddress(GdipLibrary, 'GdipCreateRegionRect');
    GdipCreateRegionRectI := GetProcAddress(GdipLibrary, 'GdipCreateRegionRectI');
    GdipCreateRegionPath := GetProcAddress(GdipLibrary, 'GdipCreateRegionPath');
    GdipCreateRegionRgnData := GetProcAddress(GdipLibrary, 'GdipCreateRegionRgnData');
    GdipCreateRegionHrgn := GetProcAddress(GdipLibrary, 'GdipCreateRegionHrgn');
    GdipCloneRegion := GetProcAddress(GdipLibrary, 'GdipCloneRegion');
    GdipDeleteRegion := GetProcAddress(GdipLibrary, 'GdipDeleteRegion');
    GdipSetInfinite := GetProcAddress(GdipLibrary, 'GdipSetInfinite');
    GdipSetEmpty := GetProcAddress(GdipLibrary, 'GdipSetEmpty');
    GdipCombineRegionRect := GetProcAddress(GdipLibrary, 'GdipCombineRegionRect');
    GdipCombineRegionRectI := GetProcAddress(GdipLibrary, 'GdipCombineRegionRectI');
    GdipCombineRegionPath := GetProcAddress(GdipLibrary, 'GdipCombineRegionPath');
    GdipCombineRegionRegion := GetProcAddress(GdipLibrary, 'GdipCombineRegionRegion');
    GdipTranslateRegion := GetProcAddress(GdipLibrary, 'GdipTranslateRegion');
    GdipTranslateRegionI := GetProcAddress(GdipLibrary, 'GdipTranslateRegionI');
    GdipTransformRegion := GetProcAddress(GdipLibrary, 'GdipTransformRegion');
    GdipGetRegionBounds := GetProcAddress(GdipLibrary, 'GdipGetRegionBounds');
    GdipGetRegionBoundsI := GetProcAddress(GdipLibrary, 'GdipGetRegionBoundsI');
    GdipGetRegionHRgn := GetProcAddress(GdipLibrary, 'GdipGetRegionHRgn');
    GdipIsEmptyRegion := GetProcAddress(GdipLibrary, 'GdipIsEmptyRegion');
    GdipIsInfiniteRegion := GetProcAddress(GdipLibrary, 'GdipIsInfiniteRegion');
    GdipIsEqualRegion := GetProcAddress(GdipLibrary, 'GdipIsEqualRegion');
    GdipGetRegionDataSize := GetProcAddress(GdipLibrary, 'GdipGetRegionDataSize');
    GdipGetRegionData := GetProcAddress(GdipLibrary, 'GdipGetRegionData');
    GdipIsVisibleRegionPoint := GetProcAddress(GdipLibrary, 'GdipIsVisibleRegionPoint');
    GdipIsVisibleRegionPointI := GetProcAddress(GdipLibrary, 'GdipIsVisibleRegionPointI');
    GdipIsVisibleRegionRect := GetProcAddress(GdipLibrary, 'GdipIsVisibleRegionRect');
    GdipIsVisibleRegionRectI := GetProcAddress(GdipLibrary, 'GdipIsVisibleRegionRectI');
    GdipGetRegionScansCount := GetProcAddress(GdipLibrary, 'GdipGetRegionScansCount');
    GdipGetRegionScans := GetProcAddress(GdipLibrary, 'GdipGetRegionScans');
    GdipGetRegionScansI := GetProcAddress(GdipLibrary, 'GdipGetRegionScansI');
    GdipCloneBrush := GetProcAddress(GdipLibrary, 'GdipCloneBrush');
    GdipDeleteBrush := GetProcAddress(GdipLibrary, 'GdipDeleteBrush');
    GdipGetBrushType := GetProcAddress(GdipLibrary, 'GdipGetBrushType');
    GdipCreateHatchBrush := GetProcAddress(GdipLibrary, 'GdipCreateHatchBrush');
    GdipGetHatchStyle := GetProcAddress(GdipLibrary, 'GdipGetHatchStyle');
    GdipGetHatchForegroundColor := GetProcAddress(GdipLibrary, 'GdipGetHatchForegroundColor');
    GdipGetHatchBackgroundColor := GetProcAddress(GdipLibrary, 'GdipGetHatchBackgroundColor');
    GdipCreateTexture := GetProcAddress(GdipLibrary, 'GdipCreateTexture');
    GdipCreateTexture2 := GetProcAddress(GdipLibrary, 'GdipCreateTexture2');
    GdipCreateTextureIA := GetProcAddress(GdipLibrary, 'GdipCreateTextureIA');
    GdipCreateTexture2I := GetProcAddress(GdipLibrary, 'GdipCreateTexture2I');
    GdipCreateTextureIAI := GetProcAddress(GdipLibrary, 'GdipCreateTextureIAI');
    GdipGetTextureTransform := GetProcAddress(GdipLibrary, 'GdipGetTextureTransform');
    GdipSetTextureTransform := GetProcAddress(GdipLibrary, 'GdipSetTextureTransform');
    GdipResetTextureTransform := GetProcAddress(GdipLibrary, 'GdipResetTextureTransform');
    GdipMultiplyTextureTransform := GetProcAddress(GdipLibrary, 'GdipMultiplyTextureTransform');
    GdipTranslateTextureTransform := GetProcAddress(GdipLibrary, 'GdipTranslateTextureTransform');
    GdipScaleTextureTransform := GetProcAddress(GdipLibrary, 'GdipScaleTextureTransform');
    GdipRotateTextureTransform := GetProcAddress(GdipLibrary, 'GdipRotateTextureTransform');
    GdipSetTextureWrapMode := GetProcAddress(GdipLibrary, 'GdipSetTextureWrapMode');
    GdipGetTextureWrapMode := GetProcAddress(GdipLibrary, 'GdipGetTextureWrapMode');
    GdipGetTextureImage := GetProcAddress(GdipLibrary, 'GdipGetTextureImage');
    GdipCreateSolidFill := GetProcAddress(GdipLibrary, 'GdipCreateSolidFill');
    GdipSetSolidFillColor := GetProcAddress(GdipLibrary, 'GdipSetSolidFillColor');
    GdipGetSolidFillColor := GetProcAddress(GdipLibrary, 'GdipGetSolidFillColor');
    GdipCreateLineBrush := GetProcAddress(GdipLibrary, 'GdipCreateLineBrush');
    GdipCreateLineBrushI := GetProcAddress(GdipLibrary, 'GdipCreateLineBrushI');
    GdipCreateLineBrushFromRect := GetProcAddress(GdipLibrary, 'GdipCreateLineBrushFromRect');
    GdipCreateLineBrushFromRectI := GetProcAddress(GdipLibrary, 'GdipCreateLineBrushFromRectI');
    GdipCreateLineBrushFromRectWithAngle := GetProcAddress(GdipLibrary, 'GdipCreateLineBrushFromRectWithAngle');
    GdipCreateLineBrushFromRectWithAngleI := GetProcAddress(GdipLibrary, 'GdipCreateLineBrushFromRectWithAngleI');
    GdipSetLineColors := GetProcAddress(GdipLibrary, 'GdipSetLineColors');
    GdipGetLineColors := GetProcAddress(GdipLibrary, 'GdipGetLineColors');
    GdipGetLineRect := GetProcAddress(GdipLibrary, 'GdipGetLineRect');
    GdipGetLineRectI := GetProcAddress(GdipLibrary, 'GdipGetLineRectI');
    GdipSetLineGammaCorrection := GetProcAddress(GdipLibrary, 'GdipSetLineGammaCorrection');
    GdipGetLineGammaCorrection := GetProcAddress(GdipLibrary, 'GdipGetLineGammaCorrection');
    GdipGetLineBlendCount := GetProcAddress(GdipLibrary, 'GdipGetLineBlendCount');
    GdipGetLineBlend := GetProcAddress(GdipLibrary, 'GdipGetLineBlend');
    GdipSetLineBlend := GetProcAddress(GdipLibrary, 'GdipSetLineBlend');
    GdipGetLinePresetBlendCount := GetProcAddress(GdipLibrary, 'GdipGetLinePresetBlendCount');
    GdipGetLinePresetBlend := GetProcAddress(GdipLibrary, 'GdipGetLinePresetBlend');
    GdipSetLinePresetBlend := GetProcAddress(GdipLibrary, 'GdipSetLinePresetBlend');
    GdipSetLineSigmaBlend := GetProcAddress(GdipLibrary, 'GdipSetLineSigmaBlend');
    GdipSetLineLinearBlend := GetProcAddress(GdipLibrary, 'GdipSetLineLinearBlend');
    GdipSetLineWrapMode := GetProcAddress(GdipLibrary, 'GdipSetLineWrapMode');
    GdipGetLineWrapMode := GetProcAddress(GdipLibrary, 'GdipGetLineWrapMode');
    GdipGetLineTransform := GetProcAddress(GdipLibrary, 'GdipGetLineTransform');
    GdipSetLineTransform := GetProcAddress(GdipLibrary, 'GdipSetLineTransform');
    GdipResetLineTransform := GetProcAddress(GdipLibrary, 'GdipResetLineTransform');
    GdipMultiplyLineTransform := GetProcAddress(GdipLibrary, 'GdipMultiplyLineTransform');
    GdipTranslateLineTransform := GetProcAddress(GdipLibrary, 'GdipTranslateLineTransform');
    GdipScaleLineTransform := GetProcAddress(GdipLibrary, 'GdipScaleLineTransform');
    GdipRotateLineTransform := GetProcAddress(GdipLibrary, 'GdipRotateLineTransform');
    GdipCreatePathGradient := GetProcAddress(GdipLibrary, 'GdipCreatePathGradient');
    GdipCreatePathGradientI := GetProcAddress(GdipLibrary, 'GdipCreatePathGradientI');
    GdipCreatePathGradientFromPath := GetProcAddress(GdipLibrary, 'GdipCreatePathGradientFromPath');
    GdipGetPathGradientCenterColor := GetProcAddress(GdipLibrary, 'GdipGetPathGradientCenterColor');
    GdipSetPathGradientCenterColor := GetProcAddress(GdipLibrary, 'GdipSetPathGradientCenterColor');
    GdipGetPathGradientSurroundColorsWithCount := GetProcAddress(GdipLibrary, 'GdipGetPathGradientSurroundColorsWithCount');
    GdipSetPathGradientSurroundColorsWithCount := GetProcAddress(GdipLibrary, 'GdipSetPathGradientSurroundColorsWithCount');
    GdipGetPathGradientPath := GetProcAddress(GdipLibrary, 'GdipGetPathGradientPath');
    GdipSetPathGradientPath := GetProcAddress(GdipLibrary, 'GdipSetPathGradientPath');
    GdipGetPathGradientCenterPoint := GetProcAddress(GdipLibrary, 'GdipGetPathGradientCenterPoint');
    GdipGetPathGradientCenterPointI := GetProcAddress(GdipLibrary, 'GdipGetPathGradientCenterPointI');
    GdipSetPathGradientCenterPoint := GetProcAddress(GdipLibrary, 'GdipSetPathGradientCenterPoint');
    GdipSetPathGradientCenterPointI := GetProcAddress(GdipLibrary, 'GdipSetPathGradientCenterPointI');
    GdipGetPathGradientRect := GetProcAddress(GdipLibrary, 'GdipGetPathGradientRect');
    GdipGetPathGradientRectI := GetProcAddress(GdipLibrary, 'GdipGetPathGradientRectI');
    GdipGetPathGradientPointCount := GetProcAddress(GdipLibrary, 'GdipGetPathGradientPointCount');
    GdipGetPathGradientSurroundColorCount := GetProcAddress(GdipLibrary, 'GdipGetPathGradientSurroundColorCount');
    GdipSetPathGradientGammaCorrection := GetProcAddress(GdipLibrary, 'GdipSetPathGradientGammaCorrection');
    GdipGetPathGradientGammaCorrection := GetProcAddress(GdipLibrary, 'GdipGetPathGradientGammaCorrection');
    GdipGetPathGradientBlendCount := GetProcAddress(GdipLibrary, 'GdipGetPathGradientBlendCount');
    GdipGetPathGradientBlend := GetProcAddress(GdipLibrary, 'GdipGetPathGradientBlend');
    GdipSetPathGradientBlend := GetProcAddress(GdipLibrary, 'GdipSetPathGradientBlend');
    GdipGetPathGradientPresetBlendCount := GetProcAddress(GdipLibrary, 'GdipGetPathGradientPresetBlendCount');
    GdipGetPathGradientPresetBlend := GetProcAddress(GdipLibrary, 'GdipGetPathGradientPresetBlend');
    GdipSetPathGradientPresetBlend := GetProcAddress(GdipLibrary, 'GdipSetPathGradientPresetBlend');
    GdipSetPathGradientSigmaBlend := GetProcAddress(GdipLibrary, 'GdipSetPathGradientSigmaBlend');
    GdipSetPathGradientLinearBlend := GetProcAddress(GdipLibrary, 'GdipSetPathGradientLinearBlend');
    GdipGetPathGradientWrapMode := GetProcAddress(GdipLibrary, 'GdipGetPathGradientWrapMode');
    GdipSetPathGradientWrapMode := GetProcAddress(GdipLibrary, 'GdipSetPathGradientWrapMode');
    GdipGetPathGradientTransform := GetProcAddress(GdipLibrary, 'GdipGetPathGradientTransform');
    GdipSetPathGradientTransform := GetProcAddress(GdipLibrary, 'GdipSetPathGradientTransform');
    GdipResetPathGradientTransform := GetProcAddress(GdipLibrary, 'GdipResetPathGradientTransform');
    GdipMultiplyPathGradientTransform := GetProcAddress(GdipLibrary, 'GdipMultiplyPathGradientTransform');
    GdipTranslatePathGradientTransform := GetProcAddress(GdipLibrary, 'GdipTranslatePathGradientTransform');
    GdipScalePathGradientTransform := GetProcAddress(GdipLibrary, 'GdipScalePathGradientTransform');
    GdipRotatePathGradientTransform := GetProcAddress(GdipLibrary, 'GdipRotatePathGradientTransform');
    GdipGetPathGradientFocusScales := GetProcAddress(GdipLibrary, 'GdipGetPathGradientFocusScales');
    GdipSetPathGradientFocusScales := GetProcAddress(GdipLibrary, 'GdipSetPathGradientFocusScales');
    GdipCreatePen1 := GetProcAddress(GdipLibrary, 'GdipCreatePen1');
    GdipCreatePen2 := GetProcAddress(GdipLibrary, 'GdipCreatePen2');
    GdipClonePen := GetProcAddress(GdipLibrary, 'GdipClonePen');
    GdipDeletePen := GetProcAddress(GdipLibrary, 'GdipDeletePen');
    GdipSetPenWidth := GetProcAddress(GdipLibrary, 'GdipSetPenWidth');
    GdipGetPenWidth := GetProcAddress(GdipLibrary, 'GdipGetPenWidth');
    GdipSetPenUnit := GetProcAddress(GdipLibrary, 'GdipSetPenUnit');
    GdipGetPenUnit := GetProcAddress(GdipLibrary, 'GdipGetPenUnit');
    GdipSetPenLineCap197819 := GetProcAddress(GdipLibrary, 'GdipSetPenLineCap197819');
    GdipSetPenStartCap := GetProcAddress(GdipLibrary, 'GdipSetPenStartCap');
    GdipSetPenEndCap := GetProcAddress(GdipLibrary, 'GdipSetPenEndCap');
    GdipSetPenDashCap197819 := GetProcAddress(GdipLibrary, 'GdipSetPenDashCap197819');
    GdipGetPenStartCap := GetProcAddress(GdipLibrary, 'GdipGetPenStartCap');
    GdipGetPenEndCap := GetProcAddress(GdipLibrary, 'GdipGetPenEndCap');
    GdipGetPenDashCap197819 := GetProcAddress(GdipLibrary, 'GdipGetPenDashCap197819');
    GdipSetPenLineJoin := GetProcAddress(GdipLibrary, 'GdipSetPenLineJoin');
    GdipGetPenLineJoin := GetProcAddress(GdipLibrary, 'GdipGetPenLineJoin');
    GdipSetPenCustomStartCap := GetProcAddress(GdipLibrary, 'GdipSetPenCustomStartCap');
    GdipGetPenCustomStartCap := GetProcAddress(GdipLibrary, 'GdipGetPenCustomStartCap');
    GdipSetPenCustomEndCap := GetProcAddress(GdipLibrary, 'GdipSetPenCustomEndCap');
    GdipGetPenCustomEndCap := GetProcAddress(GdipLibrary, 'GdipGetPenCustomEndCap');
    GdipSetPenMiterLimit := GetProcAddress(GdipLibrary, 'GdipSetPenMiterLimit');
    GdipGetPenMiterLimit := GetProcAddress(GdipLibrary, 'GdipGetPenMiterLimit');
    GdipSetPenMode := GetProcAddress(GdipLibrary, 'GdipSetPenMode');
    GdipGetPenMode := GetProcAddress(GdipLibrary, 'GdipGetPenMode');
    GdipSetPenTransform := GetProcAddress(GdipLibrary, 'GdipSetPenTransform');
    GdipGetPenTransform := GetProcAddress(GdipLibrary, 'GdipGetPenTransform');
    GdipResetPenTransform := GetProcAddress(GdipLibrary, 'GdipResetPenTransform');
    GdipMultiplyPenTransform := GetProcAddress(GdipLibrary, 'GdipMultiplyPenTransform');
    GdipTranslatePenTransform := GetProcAddress(GdipLibrary, 'GdipTranslatePenTransform');
    GdipScalePenTransform := GetProcAddress(GdipLibrary, 'GdipScalePenTransform');
    GdipRotatePenTransform := GetProcAddress(GdipLibrary, 'GdipRotatePenTransform');
    GdipSetPenColor := GetProcAddress(GdipLibrary, 'GdipSetPenColor');
    GdipGetPenColor := GetProcAddress(GdipLibrary, 'GdipGetPenColor');
    GdipSetPenBrushFill := GetProcAddress(GdipLibrary, 'GdipSetPenBrushFill');
    GdipGetPenBrushFill := GetProcAddress(GdipLibrary, 'GdipGetPenBrushFill');
    GdipGetPenFillType := GetProcAddress(GdipLibrary, 'GdipGetPenFillType');
    GdipGetPenDashStyle := GetProcAddress(GdipLibrary, 'GdipGetPenDashStyle');
    GdipSetPenDashStyle := GetProcAddress(GdipLibrary, 'GdipSetPenDashStyle');
    GdipGetPenDashOffset := GetProcAddress(GdipLibrary, 'GdipGetPenDashOffset');
    GdipSetPenDashOffset := GetProcAddress(GdipLibrary, 'GdipSetPenDashOffset');
    GdipGetPenDashCount := GetProcAddress(GdipLibrary, 'GdipGetPenDashCount');
    GdipSetPenDashArray := GetProcAddress(GdipLibrary, 'GdipSetPenDashArray');
    GdipGetPenDashArray := GetProcAddress(GdipLibrary, 'GdipGetPenDashArray');
    GdipGetPenCompoundCount := GetProcAddress(GdipLibrary, 'GdipGetPenCompoundCount');
    GdipSetPenCompoundArray := GetProcAddress(GdipLibrary, 'GdipSetPenCompoundArray');
    GdipGetPenCompoundArray := GetProcAddress(GdipLibrary, 'GdipGetPenCompoundArray');
    GdipCreateCustomLineCap := GetProcAddress(GdipLibrary, 'GdipCreateCustomLineCap');
    GdipDeleteCustomLineCap := GetProcAddress(GdipLibrary, 'GdipDeleteCustomLineCap');
    GdipCloneCustomLineCap := GetProcAddress(GdipLibrary, 'GdipCloneCustomLineCap');
    GdipGetCustomLineCapType := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapType');
    GdipSetCustomLineCapStrokeCaps := GetProcAddress(GdipLibrary, 'GdipSetCustomLineCapStrokeCaps');
    GdipGetCustomLineCapStrokeCaps := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapStrokeCaps');
    GdipSetCustomLineCapStrokeJoin := GetProcAddress(GdipLibrary, 'GdipSetCustomLineCapStrokeJoin');
    GdipGetCustomLineCapStrokeJoin := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapStrokeJoin');
    GdipSetCustomLineCapBaseCap := GetProcAddress(GdipLibrary, 'GdipSetCustomLineCapBaseCap');
    GdipGetCustomLineCapBaseCap := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapBaseCap');
    GdipSetCustomLineCapBaseInset := GetProcAddress(GdipLibrary, 'GdipSetCustomLineCapBaseInset');
    GdipGetCustomLineCapBaseInset := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapBaseInset');
    GdipSetCustomLineCapWidthScale := GetProcAddress(GdipLibrary, 'GdipSetCustomLineCapWidthScale');
    GdipGetCustomLineCapWidthScale := GetProcAddress(GdipLibrary, 'GdipGetCustomLineCapWidthScale');
    GdipCreateAdjustableArrowCap := GetProcAddress(GdipLibrary, 'GdipCreateAdjustableArrowCap');
    GdipSetAdjustableArrowCapHeight := GetProcAddress(GdipLibrary, 'GdipSetAdjustableArrowCapHeight');
    GdipGetAdjustableArrowCapHeight := GetProcAddress(GdipLibrary, 'GdipGetAdjustableArrowCapHeight');
    GdipSetAdjustableArrowCapWidth := GetProcAddress(GdipLibrary, 'GdipSetAdjustableArrowCapWidth');
    GdipGetAdjustableArrowCapWidth := GetProcAddress(GdipLibrary, 'GdipGetAdjustableArrowCapWidth');
    GdipSetAdjustableArrowCapMiddleInset := GetProcAddress(GdipLibrary, 'GdipSetAdjustableArrowCapMiddleInset');
    GdipGetAdjustableArrowCapMiddleInset := GetProcAddress(GdipLibrary, 'GdipGetAdjustableArrowCapMiddleInset');
    GdipSetAdjustableArrowCapFillState := GetProcAddress(GdipLibrary, 'GdipSetAdjustableArrowCapFillState');
    GdipGetAdjustableArrowCapFillState := GetProcAddress(GdipLibrary, 'GdipGetAdjustableArrowCapFillState');
    GdipLoadImageFromStream := GetProcAddress(GdipLibrary, 'GdipLoadImageFromStream');
    GdipLoadImageFromFile := GetProcAddress(GdipLibrary, 'GdipLoadImageFromFile');
    GdipLoadImageFromStreamICM := GetProcAddress(GdipLibrary, 'GdipLoadImageFromStreamICM');
    GdipLoadImageFromFileICM := GetProcAddress(GdipLibrary, 'GdipLoadImageFromFileICM');
    GdipCloneImage := GetProcAddress(GdipLibrary, 'GdipCloneImage');
    GdipDisposeImage := GetProcAddress(GdipLibrary, 'GdipDisposeImage');
    GdipSaveImageToFile := GetProcAddress(GdipLibrary, 'GdipSaveImageToFile');
    GdipSaveImageToStream := GetProcAddress(GdipLibrary, 'GdipSaveImageToStream');
    GdipSaveAdd := GetProcAddress(GdipLibrary, 'GdipSaveAdd');
    GdipSaveAddImage := GetProcAddress(GdipLibrary, 'GdipSaveAddImage');
    GdipGetImageGraphicsContext := GetProcAddress(GdipLibrary, 'GdipGetImageGraphicsContext');
    GdipGetImageBounds := GetProcAddress(GdipLibrary, 'GdipGetImageBounds');
    GdipGetImageDimension := GetProcAddress(GdipLibrary, 'GdipGetImageDimension');
    GdipGetImageType := GetProcAddress(GdipLibrary, 'GdipGetImageType');
    GdipGetImageWidth := GetProcAddress(GdipLibrary, 'GdipGetImageWidth');
    GdipGetImageHeight := GetProcAddress(GdipLibrary, 'GdipGetImageHeight');
    GdipGetImageHorizontalResolution := GetProcAddress(GdipLibrary, 'GdipGetImageHorizontalResolution');
    GdipGetImageVerticalResolution := GetProcAddress(GdipLibrary, 'GdipGetImageVerticalResolution');
    GdipGetImageFlags := GetProcAddress(GdipLibrary, 'GdipGetImageFlags');
    GdipGetImageRawFormat := GetProcAddress(GdipLibrary, 'GdipGetImageRawFormat');
    GdipGetImagePixelFormat := GetProcAddress(GdipLibrary, 'GdipGetImagePixelFormat');
    GdipGetImageThumbnail := GetProcAddress(GdipLibrary, 'GdipGetImageThumbnail');
    GdipGetEncoderParameterListSize := GetProcAddress(GdipLibrary, 'GdipGetEncoderParameterListSize');
    GdipGetEncoderParameterList := GetProcAddress(GdipLibrary, 'GdipGetEncoderParameterList');
    GdipImageGetFrameDimensionsCount := GetProcAddress(GdipLibrary, 'GdipImageGetFrameDimensionsCount');
    GdipImageGetFrameDimensionsList := GetProcAddress(GdipLibrary, 'GdipImageGetFrameDimensionsList');
    GdipImageGetFrameCount := GetProcAddress(GdipLibrary, 'GdipImageGetFrameCount');
    GdipImageSelectActiveFrame := GetProcAddress(GdipLibrary, 'GdipImageSelectActiveFrame');
    GdipImageRotateFlip := GetProcAddress(GdipLibrary, 'GdipImageRotateFlip');
    GdipGetImagePalette := GetProcAddress(GdipLibrary, 'GdipGetImagePalette');
    GdipSetImagePalette := GetProcAddress(GdipLibrary, 'GdipSetImagePalette');
    GdipGetImagePaletteSize := GetProcAddress(GdipLibrary, 'GdipGetImagePaletteSize');
    GdipGetPropertyCount := GetProcAddress(GdipLibrary, 'GdipGetPropertyCount');
    GdipGetPropertyIdList := GetProcAddress(GdipLibrary, 'GdipGetPropertyIdList');
    GdipGetPropertyItemSize := GetProcAddress(GdipLibrary, 'GdipGetPropertyItemSize');
    GdipGetPropertyItem := GetProcAddress(GdipLibrary, 'GdipGetPropertyItem');
    GdipGetPropertySize := GetProcAddress(GdipLibrary, 'GdipGetPropertySize');
    GdipGetAllPropertyItems := GetProcAddress(GdipLibrary, 'GdipGetAllPropertyItems');
    GdipRemovePropertyItem := GetProcAddress(GdipLibrary, 'GdipRemovePropertyItem');
    GdipSetPropertyItem := GetProcAddress(GdipLibrary, 'GdipSetPropertyItem');
    GdipImageForceValidation := GetProcAddress(GdipLibrary, 'GdipImageForceValidation');
    GdipCreateBitmapFromStream := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromStream');
    GdipCreateBitmapFromFile := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromFile');
    GdipCreateBitmapFromStreamICM := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromStreamICM');
    GdipCreateBitmapFromFileICM := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromFileICM');
    GdipCreateBitmapFromScan0 := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromScan0');
    GdipCreateBitmapFromGraphics := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromGraphics');
    GdipCreateBitmapFromGdiDib := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromGdiDib');
    GdipCreateBitmapFromDirectDrawSurface := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromDirectDrawSurface');
    GdipCreateBitmapFromHBITMAP := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromHBITMAP');
    GdipCreateHBITMAPFromBitmap := GetProcAddress(GdipLibrary, 'GdipCreateHBITMAPFromBitmap');
    GdipCreateBitmapFromHICON := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromHICON');
    GdipCreateHICONFromBitmap := GetProcAddress(GdipLibrary, 'GdipCreateHICONFromBitmap');
    GdipCreateBitmapFromResource := GetProcAddress(GdipLibrary, 'GdipCreateBitmapFromResource');
    GdipCloneBitmapArea := GetProcAddress(GdipLibrary, 'GdipCloneBitmapArea');
    GdipCloneBitmapAreaI := GetProcAddress(GdipLibrary, 'GdipCloneBitmapAreaI');
    GdipBitmapLockBits := GetProcAddress(GdipLibrary, 'GdipBitmapLockBits');
    GdipBitmapUnlockBits := GetProcAddress(GdipLibrary, 'GdipBitmapUnlockBits');
    GdipBitmapGetPixel := GetProcAddress(GdipLibrary, 'GdipBitmapGetPixel');
    GdipBitmapSetPixel := GetProcAddress(GdipLibrary, 'GdipBitmapSetPixel');
    GdipBitmapSetResolution := GetProcAddress(GdipLibrary, 'GdipBitmapSetResolution');
    GdipCreateImageAttributes := GetProcAddress(GdipLibrary, 'GdipCreateImageAttributes');
    GdipCloneImageAttributes := GetProcAddress(GdipLibrary, 'GdipCloneImageAttributes');
    GdipDisposeImageAttributes := GetProcAddress(GdipLibrary, 'GdipDisposeImageAttributes');
    GdipSetImageAttributesToIdentity := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesToIdentity');
    GdipResetImageAttributes := GetProcAddress(GdipLibrary, 'GdipResetImageAttributes');
    GdipSetImageAttributesColorMatrix := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesColorMatrix');
    GdipSetImageAttributesThreshold := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesThreshold');
    GdipSetImageAttributesGamma := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesGamma');
    GdipSetImageAttributesNoOp := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesNoOp');
    GdipSetImageAttributesColorKeys := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesColorKeys');
    GdipSetImageAttributesOutputChannel := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesOutputChannel');
    GdipSetImageAttributesOutputChannelColorProfile := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesOutputChannelColorProfile');
    GdipSetImageAttributesRemapTable := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesRemapTable');
    GdipSetImageAttributesWrapMode := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesWrapMode');
    GdipSetImageAttributesICMMode := GetProcAddress(GdipLibrary, 'GdipSetImageAttributesICMMode');
    GdipGetImageAttributesAdjustedPalette := GetProcAddress(GdipLibrary, 'GdipGetImageAttributesAdjustedPalette');
    GdipFlush := GetProcAddress(GdipLibrary, 'GdipFlush');
    GdipCreateFromHDC := GetProcAddress(GdipLibrary, 'GdipCreateFromHDC');
    GdipCreateFromHDC2 := GetProcAddress(GdipLibrary, 'GdipCreateFromHDC2');
    GdipCreateFromHWND := GetProcAddress(GdipLibrary, 'GdipCreateFromHWND');
    GdipCreateFromHWNDICM := GetProcAddress(GdipLibrary, 'GdipCreateFromHWNDICM');
    GdipDeleteGraphics := GetProcAddress(GdipLibrary, 'GdipDeleteGraphics');
    GdipGetDC := GetProcAddress(GdipLibrary, 'GdipGetDC');
    GdipReleaseDC := GetProcAddress(GdipLibrary, 'GdipReleaseDC');
    GdipSetCompositingMode := GetProcAddress(GdipLibrary, 'GdipSetCompositingMode');
    GdipGetCompositingMode := GetProcAddress(GdipLibrary, 'GdipGetCompositingMode');
    GdipSetRenderingOrigin := GetProcAddress(GdipLibrary, 'GdipSetRenderingOrigin');
    GdipGetRenderingOrigin := GetProcAddress(GdipLibrary, 'GdipGetRenderingOrigin');
    GdipSetCompositingQuality := GetProcAddress(GdipLibrary, 'GdipSetCompositingQuality');
    GdipGetCompositingQuality := GetProcAddress(GdipLibrary, 'GdipGetCompositingQuality');
    GdipSetSmoothingMode := GetProcAddress(GdipLibrary, 'GdipSetSmoothingMode');
    GdipGetSmoothingMode := GetProcAddress(GdipLibrary, 'GdipGetSmoothingMode');
    GdipSetPixelOffsetMode := GetProcAddress(GdipLibrary, 'GdipSetPixelOffsetMode');
    GdipGetPixelOffsetMode := GetProcAddress(GdipLibrary, 'GdipGetPixelOffsetMode');
    GdipSetTextRenderingHint := GetProcAddress(GdipLibrary, 'GdipSetTextRenderingHint');
    GdipGetTextRenderingHint := GetProcAddress(GdipLibrary, 'GdipGetTextRenderingHint');
    GdipSetTextContrast := GetProcAddress(GdipLibrary, 'GdipSetTextContrast');
    GdipGetTextContrast := GetProcAddress(GdipLibrary, 'GdipGetTextContrast');
    GdipSetInterpolationMode := GetProcAddress(GdipLibrary, 'GdipSetInterpolationMode');
    GdipGetInterpolationMode := GetProcAddress(GdipLibrary, 'GdipGetInterpolationMode');
    GdipSetWorldTransform := GetProcAddress(GdipLibrary, 'GdipSetWorldTransform');
    GdipResetWorldTransform := GetProcAddress(GdipLibrary, 'GdipResetWorldTransform');
    GdipMultiplyWorldTransform := GetProcAddress(GdipLibrary, 'GdipMultiplyWorldTransform');
    GdipTranslateWorldTransform := GetProcAddress(GdipLibrary, 'GdipTranslateWorldTransform');
    GdipScaleWorldTransform := GetProcAddress(GdipLibrary, 'GdipScaleWorldTransform');
    GdipRotateWorldTransform := GetProcAddress(GdipLibrary, 'GdipRotateWorldTransform');
    GdipGetWorldTransform := GetProcAddress(GdipLibrary, 'GdipGetWorldTransform');
    GdipResetPageTransform := GetProcAddress(GdipLibrary, 'GdipResetPageTransform');
    GdipGetPageUnit := GetProcAddress(GdipLibrary, 'GdipGetPageUnit');
    GdipGetPageScale := GetProcAddress(GdipLibrary, 'GdipGetPageScale');
    GdipSetPageUnit := GetProcAddress(GdipLibrary, 'GdipSetPageUnit');
    GdipSetPageScale := GetProcAddress(GdipLibrary, 'GdipSetPageScale');
    GdipGetDpiX := GetProcAddress(GdipLibrary, 'GdipGetDpiX');
    GdipGetDpiY := GetProcAddress(GdipLibrary, 'GdipGetDpiY');
    GdipTransformPoints := GetProcAddress(GdipLibrary, 'GdipTransformPoints');
    GdipTransformPointsI := GetProcAddress(GdipLibrary, 'GdipTransformPointsI');
    GdipGetNearestColor := GetProcAddress(GdipLibrary, 'GdipGetNearestColor');
    GdipCreateHalftonePalette := GetProcAddress(GdipLibrary, 'GdipCreateHalftonePalette');
    GdipDrawLine := GetProcAddress(GdipLibrary, 'GdipDrawLine');
    GdipDrawLineI := GetProcAddress(GdipLibrary, 'GdipDrawLineI');
    GdipDrawLines := GetProcAddress(GdipLibrary, 'GdipDrawLines');
    GdipDrawLinesI := GetProcAddress(GdipLibrary, 'GdipDrawLinesI');
    GdipDrawArc := GetProcAddress(GdipLibrary, 'GdipDrawArc');
    GdipDrawArcI := GetProcAddress(GdipLibrary, 'GdipDrawArcI');
    GdipDrawBezier := GetProcAddress(GdipLibrary, 'GdipDrawBezier');
    GdipDrawBezierI := GetProcAddress(GdipLibrary, 'GdipDrawBezierI');
    GdipDrawBeziers := GetProcAddress(GdipLibrary, 'GdipDrawBeziers');
    GdipDrawBeziersI := GetProcAddress(GdipLibrary, 'GdipDrawBeziersI');
    GdipDrawRectangle := GetProcAddress(GdipLibrary, 'GdipDrawRectangle');
    GdipDrawRectangleI := GetProcAddress(GdipLibrary, 'GdipDrawRectangleI');
    GdipDrawRectangles := GetProcAddress(GdipLibrary, 'GdipDrawRectangles');
    GdipDrawRectanglesI := GetProcAddress(GdipLibrary, 'GdipDrawRectanglesI');
    GdipDrawEllipse := GetProcAddress(GdipLibrary, 'GdipDrawEllipse');
    GdipDrawEllipseI := GetProcAddress(GdipLibrary, 'GdipDrawEllipseI');
    GdipDrawPie := GetProcAddress(GdipLibrary, 'GdipDrawPie');
    GdipDrawPieI := GetProcAddress(GdipLibrary, 'GdipDrawPieI');
    GdipDrawPolygon := GetProcAddress(GdipLibrary, 'GdipDrawPolygon');
    GdipDrawPolygonI := GetProcAddress(GdipLibrary, 'GdipDrawPolygonI');
    GdipDrawPath := GetProcAddress(GdipLibrary, 'GdipDrawPath');
    GdipDrawCurve := GetProcAddress(GdipLibrary, 'GdipDrawCurve');
    GdipDrawCurveI := GetProcAddress(GdipLibrary, 'GdipDrawCurveI');
    GdipDrawCurve2 := GetProcAddress(GdipLibrary, 'GdipDrawCurve2');
    GdipDrawCurve2I := GetProcAddress(GdipLibrary, 'GdipDrawCurve2I');
    GdipDrawCurve3 := GetProcAddress(GdipLibrary, 'GdipDrawCurve3');
    GdipDrawCurve3I := GetProcAddress(GdipLibrary, 'GdipDrawCurve3I');
    GdipDrawClosedCurve := GetProcAddress(GdipLibrary, 'GdipDrawClosedCurve');
    GdipDrawClosedCurveI := GetProcAddress(GdipLibrary, 'GdipDrawClosedCurveI');
    GdipDrawClosedCurve2 := GetProcAddress(GdipLibrary, 'GdipDrawClosedCurve2');
    GdipDrawClosedCurve2I := GetProcAddress(GdipLibrary, 'GdipDrawClosedCurve2I');
    GdipGraphicsClear := GetProcAddress(GdipLibrary, 'GdipGraphicsClear');
    GdipFillRectangle := GetProcAddress(GdipLibrary, 'GdipFillRectangle');
    GdipFillRectangleI := GetProcAddress(GdipLibrary, 'GdipFillRectangleI');
    GdipFillRectangles := GetProcAddress(GdipLibrary, 'GdipFillRectangles');
    GdipFillRectanglesI := GetProcAddress(GdipLibrary, 'GdipFillRectanglesI');
    GdipFillPolygon := GetProcAddress(GdipLibrary, 'GdipFillPolygon');
    GdipFillPolygonI := GetProcAddress(GdipLibrary, 'GdipFillPolygonI');
    GdipFillPolygon2 := GetProcAddress(GdipLibrary, 'GdipFillPolygon2');
    GdipFillPolygon2I := GetProcAddress(GdipLibrary, 'GdipFillPolygon2I');
    GdipFillEllipse := GetProcAddress(GdipLibrary, 'GdipFillEllipse');
    GdipFillEllipseI := GetProcAddress(GdipLibrary, 'GdipFillEllipseI');
    GdipFillPie := GetProcAddress(GdipLibrary, 'GdipFillPie');
    GdipFillPieI := GetProcAddress(GdipLibrary, 'GdipFillPieI');
    GdipFillPath := GetProcAddress(GdipLibrary, 'GdipFillPath');
    GdipFillClosedCurve := GetProcAddress(GdipLibrary, 'GdipFillClosedCurve');
    GdipFillClosedCurveI := GetProcAddress(GdipLibrary, 'GdipFillClosedCurveI');
    GdipFillClosedCurve2 := GetProcAddress(GdipLibrary, 'GdipFillClosedCurve2');
    GdipFillClosedCurve2I := GetProcAddress(GdipLibrary, 'GdipFillClosedCurve2I');
    GdipFillRegion := GetProcAddress(GdipLibrary, 'GdipFillRegion');
    GdipDrawImage := GetProcAddress(GdipLibrary, 'GdipDrawImage');
    GdipDrawImageI := GetProcAddress(GdipLibrary, 'GdipDrawImageI');
    GdipDrawImageRect := GetProcAddress(GdipLibrary, 'GdipDrawImageRect');
    GdipDrawImageRectI := GetProcAddress(GdipLibrary, 'GdipDrawImageRectI');
    GdipDrawImagePoints := GetProcAddress(GdipLibrary, 'GdipDrawImagePoints');
    GdipDrawImagePointsI := GetProcAddress(GdipLibrary, 'GdipDrawImagePointsI');
    GdipDrawImagePointRect := GetProcAddress(GdipLibrary, 'GdipDrawImagePointRect');
    GdipDrawImagePointRectI := GetProcAddress(GdipLibrary, 'GdipDrawImagePointRectI');
    GdipDrawImageRectRect := GetProcAddress(GdipLibrary, 'GdipDrawImageRectRect');
    GdipDrawImageRectRectI := GetProcAddress(GdipLibrary, 'GdipDrawImageRectRectI');
    GdipDrawImagePointsRect := GetProcAddress(GdipLibrary, 'GdipDrawImagePointsRect');
    GdipDrawImagePointsRectI := GetProcAddress(GdipLibrary, 'GdipDrawImagePointsRectI');
    GdipEnumerateMetafileDestPoint := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestPoint');
    GdipEnumerateMetafileDestPointI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestPointI');
    GdipEnumerateMetafileDestRect := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestRect');
    GdipEnumerateMetafileDestRectI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestRectI');
    GdipEnumerateMetafileDestPoints := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestPoints');
    GdipEnumerateMetafileDestPointsI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileDestPointsI');
    GdipEnumerateMetafileSrcRectDestPoint := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestPoint');
    GdipEnumerateMetafileSrcRectDestPointI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestPointI');
    GdipEnumerateMetafileSrcRectDestRect := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestRect');
    GdipEnumerateMetafileSrcRectDestRectI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestRectI');
    GdipEnumerateMetafileSrcRectDestPoints := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestPoints');
    GdipEnumerateMetafileSrcRectDestPointsI := GetProcAddress(GdipLibrary, 'GdipEnumerateMetafileSrcRectDestPointsI');
    GdipPlayMetafileRecord := GetProcAddress(GdipLibrary, 'GdipPlayMetafileRecord');
    GdipSetClipGraphics := GetProcAddress(GdipLibrary, 'GdipSetClipGraphics');
    GdipSetClipRect := GetProcAddress(GdipLibrary, 'GdipSetClipRect');
    GdipSetClipRectI := GetProcAddress(GdipLibrary, 'GdipSetClipRectI');
    GdipSetClipPath := GetProcAddress(GdipLibrary, 'GdipSetClipPath');
    GdipSetClipRegion := GetProcAddress(GdipLibrary, 'GdipSetClipRegion');
    GdipSetClipHrgn := GetProcAddress(GdipLibrary, 'GdipSetClipHrgn');
    GdipResetClip := GetProcAddress(GdipLibrary, 'GdipResetClip');
    GdipTranslateClip := GetProcAddress(GdipLibrary, 'GdipTranslateClip');
    GdipTranslateClipI := GetProcAddress(GdipLibrary, 'GdipTranslateClipI');
    GdipGetClip := GetProcAddress(GdipLibrary, 'GdipGetClip');
    GdipGetClipBounds := GetProcAddress(GdipLibrary, 'GdipGetClipBounds');
    GdipGetClipBoundsI := GetProcAddress(GdipLibrary, 'GdipGetClipBoundsI');
    GdipIsClipEmpty := GetProcAddress(GdipLibrary, 'GdipIsClipEmpty');
    GdipGetVisibleClipBounds := GetProcAddress(GdipLibrary, 'GdipGetVisibleClipBounds');
    GdipGetVisibleClipBoundsI := GetProcAddress(GdipLibrary, 'GdipGetVisibleClipBoundsI');
    GdipIsVisibleClipEmpty := GetProcAddress(GdipLibrary, 'GdipIsVisibleClipEmpty');
    GdipIsVisiblePoint := GetProcAddress(GdipLibrary, 'GdipIsVisiblePoint');
    GdipIsVisiblePointI := GetProcAddress(GdipLibrary, 'GdipIsVisiblePointI');
    GdipIsVisibleRect := GetProcAddress(GdipLibrary, 'GdipIsVisibleRect');
    GdipIsVisibleRectI := GetProcAddress(GdipLibrary, 'GdipIsVisibleRectI');
    GdipSaveGraphics := GetProcAddress(GdipLibrary, 'GdipSaveGraphics');
    GdipRestoreGraphics := GetProcAddress(GdipLibrary, 'GdipRestoreGraphics');
    GdipBeginContainer := GetProcAddress(GdipLibrary, 'GdipBeginContainer');
    GdipBeginContainerI := GetProcAddress(GdipLibrary, 'GdipBeginContainerI');
    GdipBeginContainer2 := GetProcAddress(GdipLibrary, 'GdipBeginContainer2');
    GdipEndContainer := GetProcAddress(GdipLibrary, 'GdipEndContainer');
    GdipGetMetafileHeaderFromWmf := GetProcAddress(GdipLibrary, 'GdipGetMetafileHeaderFromWmf');
    GdipGetMetafileHeaderFromEmf := GetProcAddress(GdipLibrary, 'GdipGetMetafileHeaderFromEmf');
    GdipGetMetafileHeaderFromFile := GetProcAddress(GdipLibrary, 'GdipGetMetafileHeaderFromFile');
    GdipGetMetafileHeaderFromStream := GetProcAddress(GdipLibrary, 'GdipGetMetafileHeaderFromStream');
    GdipGetMetafileHeaderFromMetafile := GetProcAddress(GdipLibrary, 'GdipGetMetafileHeaderFromMetafile');
    GdipGetHemfFromMetafile := GetProcAddress(GdipLibrary, 'GdipGetHemfFromMetafile');
    GdipCreateStreamOnFile := GetProcAddress(GdipLibrary, 'GdipCreateStreamOnFile');
    GdipCreateMetafileFromWmf := GetProcAddress(GdipLibrary, 'GdipCreateMetafileFromWmf');
    GdipCreateMetafileFromEmf := GetProcAddress(GdipLibrary, 'GdipCreateMetafileFromEmf');
    GdipCreateMetafileFromFile := GetProcAddress(GdipLibrary, 'GdipCreateMetafileFromFile');
    GdipCreateMetafileFromWmfFile := GetProcAddress(GdipLibrary, 'GdipCreateMetafileFromWmfFile');
    GdipCreateMetafileFromStream := GetProcAddress(GdipLibrary, 'GdipCreateMetafileFromStream');
    GdipRecordMetafile := GetProcAddress(GdipLibrary, 'GdipRecordMetafile');
    GdipRecordMetafileI := GetProcAddress(GdipLibrary, 'GdipRecordMetafileI');
    GdipRecordMetafileFileName := GetProcAddress(GdipLibrary, 'GdipRecordMetafileFileName');
    GdipRecordMetafileFileNameI := GetProcAddress(GdipLibrary, 'GdipRecordMetafileFileNameI');
    GdipRecordMetafileStream := GetProcAddress(GdipLibrary, 'GdipRecordMetafileStream');
    GdipRecordMetafileStreamI := GetProcAddress(GdipLibrary, 'GdipRecordMetafileStreamI');
    GdipSetMetafileDownLevelRasterizationLimit := GetProcAddress(GdipLibrary, 'GdipSetMetafileDownLevelRasterizationLimit');
    GdipGetMetafileDownLevelRasterizationLimit := GetProcAddress(GdipLibrary, 'GdipGetMetafileDownLevelRasterizationLimit');
    GdipGetImageDecodersSize := GetProcAddress(GdipLibrary, 'GdipGetImageDecodersSize');
    GdipGetImageDecoders := GetProcAddress(GdipLibrary, 'GdipGetImageDecoders');
    GdipGetImageEncodersSize := GetProcAddress(GdipLibrary, 'GdipGetImageEncodersSize');
    GdipGetImageEncoders := GetProcAddress(GdipLibrary, 'GdipGetImageEncoders');
    GdipComment := GetProcAddress(GdipLibrary, 'GdipComment');
    GdipCreateFontFamilyFromName := GetProcAddress(GdipLibrary, 'GdipCreateFontFamilyFromName');
    GdipDeleteFontFamily := GetProcAddress(GdipLibrary, 'GdipDeleteFontFamily');
    GdipCloneFontFamily := GetProcAddress(GdipLibrary, 'GdipCloneFontFamily');
    GdipGetGenericFontFamilySansSerif := GetProcAddress(GdipLibrary, 'GdipGetGenericFontFamilySansSerif');
    GdipGetGenericFontFamilySerif := GetProcAddress(GdipLibrary, 'GdipGetGenericFontFamilySerif');
    GdipGetGenericFontFamilyMonospace := GetProcAddress(GdipLibrary, 'GdipGetGenericFontFamilyMonospace');
    GdipGetFamilyName := GetProcAddress(GdipLibrary, 'GdipGetFamilyName');
    GdipIsStyleAvailable := GetProcAddress(GdipLibrary, 'GdipIsStyleAvailable');
    GdipFontCollectionEnumerable := GetProcAddress(GdipLibrary, 'GdipFontCollectionEnumerable');
    GdipFontCollectionEnumerate := GetProcAddress(GdipLibrary, 'GdipFontCollectionEnumerate');
    GdipGetEmHeight := GetProcAddress(GdipLibrary, 'GdipGetEmHeight');
    GdipGetCellAscent := GetProcAddress(GdipLibrary, 'GdipGetCellAscent');
    GdipGetCellDescent := GetProcAddress(GdipLibrary, 'GdipGetCellDescent');
    GdipGetLineSpacing := GetProcAddress(GdipLibrary, 'GdipGetLineSpacing');
    GdipCreateFontFromDC := GetProcAddress(GdipLibrary, 'GdipCreateFontFromDC');
    GdipCreateFontFromLogfontA := GetProcAddress(GdipLibrary, 'GdipCreateFontFromLogfontA');
    GdipCreateFontFromLogfontW := GetProcAddress(GdipLibrary, 'GdipCreateFontFromLogfontW');
    GdipCreateFont := GetProcAddress(GdipLibrary, 'GdipCreateFont');
    GdipCloneFont := GetProcAddress(GdipLibrary, 'GdipCloneFont');
    GdipDeleteFont := GetProcAddress(GdipLibrary, 'GdipDeleteFont');
    GdipGetFamily := GetProcAddress(GdipLibrary, 'GdipGetFamily');
    GdipGetFontStyle := GetProcAddress(GdipLibrary, 'GdipGetFontStyle');
    GdipGetFontSize := GetProcAddress(GdipLibrary, 'GdipGetFontSize');
    GdipGetFontUnit := GetProcAddress(GdipLibrary, 'GdipGetFontUnit');
    GdipGetFontHeight := GetProcAddress(GdipLibrary, 'GdipGetFontHeight');
    GdipGetFontHeightGivenDPI := GetProcAddress(GdipLibrary, 'GdipGetFontHeightGivenDPI');
   
    GdipGetLogFontA := GetProcAddress(GdipLibrary, 'GdipGetLogFontA');    
    GdipGetLogFontW := GetProcAddress(GdipLibrary, 'GdipGetLogFontW');
    
    GdipNewInstalledFontCollection := GetProcAddress(GdipLibrary, 'GdipNewInstalledFontCollection');
    GdipNewPrivateFontCollection := GetProcAddress(GdipLibrary, 'GdipNewPrivateFontCollection');
    GdipDeletePrivateFontCollection := GetProcAddress(GdipLibrary, 'GdipDeletePrivateFontCollection');
    GdipGetFontCollectionFamilyCount := GetProcAddress(GdipLibrary, 'GdipGetFontCollectionFamilyCount');
    GdipGetFontCollectionFamilyList := GetProcAddress(GdipLibrary, 'GdipGetFontCollectionFamilyList');
    GdipPrivateAddFontFile := GetProcAddress(GdipLibrary, 'GdipPrivateAddFontFile');
    GdipPrivateAddMemoryFont := GetProcAddress(GdipLibrary, 'GdipPrivateAddMemoryFont');
    GdipDrawString := GetProcAddress(GdipLibrary, 'GdipDrawString');
    GdipMeasureString := GetProcAddress(GdipLibrary, 'GdipMeasureString');
    GdipMeasureCharacterRanges := GetProcAddress(GdipLibrary, 'GdipMeasureCharacterRanges');
    GdipDrawDriverString := GetProcAddress(GdipLibrary, 'GdipDrawDriverString');
    GdipMeasureDriverString := GetProcAddress(GdipLibrary, 'GdipMeasureDriverString');
    GdipCreateStringFormat := GetProcAddress(GdipLibrary, 'GdipCreateStringFormat');
    GdipStringFormatGetGenericDefault := GetProcAddress(GdipLibrary, 'GdipStringFormatGetGenericDefault');
    GdipStringFormatGetGenericTypographic := GetProcAddress(GdipLibrary, 'GdipStringFormatGetGenericTypographic');
    GdipDeleteStringFormat := GetProcAddress(GdipLibrary, 'GdipDeleteStringFormat');
    GdipCloneStringFormat := GetProcAddress(GdipLibrary, 'GdipCloneStringFormat');
    GdipSetStringFormatFlags := GetProcAddress(GdipLibrary, 'GdipSetStringFormatFlags');
    GdipGetStringFormatFlags := GetProcAddress(GdipLibrary, 'GdipGetStringFormatFlags');
    GdipSetStringFormatAlign := GetProcAddress(GdipLibrary, 'GdipSetStringFormatAlign');
    GdipGetStringFormatAlign := GetProcAddress(GdipLibrary, 'GdipGetStringFormatAlign');
    GdipSetStringFormatLineAlign := GetProcAddress(GdipLibrary, 'GdipSetStringFormatLineAlign');
    GdipGetStringFormatLineAlign := GetProcAddress(GdipLibrary, 'GdipGetStringFormatLineAlign');
    GdipSetStringFormatTrimming := GetProcAddress(GdipLibrary, 'GdipSetStringFormatTrimming');
    GdipGetStringFormatTrimming := GetProcAddress(GdipLibrary, 'GdipGetStringFormatTrimming');
    GdipSetStringFormatHotkeyPrefix := GetProcAddress(GdipLibrary, 'GdipSetStringFormatHotkeyPrefix');
    GdipGetStringFormatHotkeyPrefix := GetProcAddress(GdipLibrary, 'GdipGetStringFormatHotkeyPrefix');
    GdipSetStringFormatTabStops := GetProcAddress(GdipLibrary, 'GdipSetStringFormatTabStops');
    GdipGetStringFormatTabStops := GetProcAddress(GdipLibrary, 'GdipGetStringFormatTabStops');
    GdipGetStringFormatTabStopCount := GetProcAddress(GdipLibrary, 'GdipGetStringFormatTabStopCount');
    GdipSetStringFormatDigitSubstitution := GetProcAddress(GdipLibrary, 'GdipSetStringFormatDigitSubstitution');
    GdipGetStringFormatDigitSubstitution := GetProcAddress(GdipLibrary, 'GdipGetStringFormatDigitSubstitution');
    GdipGetStringFormatMeasurableCharacterRangeCount := GetProcAddress(GdipLibrary, 'GdipGetStringFormatMeasurableCharacterRangeCount');
    GdipSetStringFormatMeasurableCharacterRanges := GetProcAddress(GdipLibrary, 'GdipSetStringFormatMeasurableCharacterRanges');
    GdipCreateCachedBitmap := GetProcAddress(GdipLibrary, 'GdipCreateCachedBitmap');
    GdipDeleteCachedBitmap := GetProcAddress(GdipLibrary, 'GdipDeleteCachedBitmap');
    GdipDrawCachedBitmap := GetProcAddress(GdipLibrary, 'GdipDrawCachedBitmap');
    GdipEmfToWmfBits := GetProcAddress(GdipLibrary, 'GdipEmfToWmfBits');
  end;
end;

procedure FreeGdiplus;
begin
  if (GdipLibrary <> 0) then
  begin
    FreeLibrary(GdipLibrary);
    GdipLibrary := 0;

    GdipAlloc := nil;
    GdipFree := nil;
    GdiplusStartup := nil;
    GdiplusShutdown := nil;

    GdipCreatePath := nil;
    GdipCreatePath2 := nil;
    GdipCreatePath2I := nil;
    GdipClonePath := nil;
    GdipDeletePath := nil;
    GdipResetPath := nil;
    GdipGetPointCount := nil;
    GdipGetPathTypes := nil;
    GdipGetPathPoints := nil;
    GdipGetPathPointsI := nil;
    GdipGetPathFillMode := nil;
    GdipSetPathFillMode := nil;
    GdipGetPathData := nil;
    GdipStartPathFigure := nil;
    GdipClosePathFigure := nil;
    GdipClosePathFigures := nil;
    GdipSetPathMarker := nil;
    GdipClearPathMarkers := nil;
    GdipReversePath := nil;
    GdipGetPathLastPoint := nil;
    GdipAddPathLine := nil;
    GdipAddPathLine2 := nil;
    GdipAddPathArc := nil;
    GdipAddPathBezier := nil;
    GdipAddPathBeziers := nil;
    GdipAddPathCurve := nil;
    GdipAddPathCurve2 := nil;
    GdipAddPathCurve3 := nil;
    GdipAddPathClosedCurve := nil;
    GdipAddPathClosedCurve2 := nil;
    GdipAddPathRectangle := nil;
    GdipAddPathRectangles := nil;
    GdipAddPathEllipse := nil;
    GdipAddPathPie := nil;
    GdipAddPathPolygon := nil;
    GdipAddPathPath := nil;
    GdipAddPathString := nil;
    GdipAddPathStringI := nil;
    GdipAddPathLineI := nil;
    GdipAddPathLine2I := nil;
    GdipAddPathArcI := nil;
    GdipAddPathBezierI := nil;
    GdipAddPathBeziersI := nil;
    GdipAddPathCurveI := nil;
    GdipAddPathCurve2I := nil;
    GdipAddPathCurve3I := nil;
    GdipAddPathClosedCurveI := nil;
    GdipAddPathClosedCurve2I := nil;
    GdipAddPathRectangleI := nil;
    GdipAddPathRectanglesI := nil;
    GdipAddPathEllipseI := nil;
    GdipAddPathPieI := nil;
    GdipAddPathPolygonI := nil;
    GdipFlattenPath := nil;
    GdipWindingModeOutline := nil;
    GdipWidenPath := nil;
    GdipWarpPath := nil;
    GdipTransformPath := nil;
    GdipGetPathWorldBounds := nil;
    GdipGetPathWorldBoundsI := nil;
    GdipIsVisiblePathPoint := nil;
    GdipIsVisiblePathPointI := nil;
    GdipIsOutlineVisiblePathPoint := nil;
    GdipIsOutlineVisiblePathPointI := nil;
    GdipCreatePathIter := nil;
    GdipDeletePathIter := nil;
    GdipPathIterNextSubpath := nil;
    GdipPathIterNextSubpathPath := nil;
    GdipPathIterNextPathType := nil;
    GdipPathIterNextMarker := nil;
    GdipPathIterNextMarkerPath := nil;
    GdipPathIterGetCount := nil;
    GdipPathIterGetSubpathCount := nil;
    GdipPathIterIsValid := nil;
    GdipPathIterHasCurve := nil;
    GdipPathIterRewind := nil;
    GdipPathIterEnumerate := nil;
    GdipPathIterCopyData := nil;
    GdipCreateMatrix := nil;
    GdipCreateMatrix2 := nil;
    GdipCreateMatrix3 := nil;
    GdipCreateMatrix3I := nil;
    GdipCloneMatrix := nil;
    GdipDeleteMatrix := nil;
    GdipSetMatrixElements := nil;
    GdipMultiplyMatrix := nil;
    GdipTranslateMatrix := nil;
    GdipScaleMatrix := nil;
    GdipRotateMatrix := nil;
    GdipShearMatrix := nil;
    GdipInvertMatrix := nil;
    GdipTransformMatrixPoints := nil;
    GdipTransformMatrixPointsI := nil;
    GdipVectorTransformMatrixPoints := nil;
    GdipVectorTransformMatrixPointsI := nil;
    GdipGetMatrixElements := nil;
    GdipIsMatrixInvertible := nil;
    GdipIsMatrixIdentity := nil;
    GdipIsMatrixEqual := nil;
    GdipCreateRegion := nil;
    GdipCreateRegionRect := nil;
    GdipCreateRegionRectI := nil;
    GdipCreateRegionPath := nil;
    GdipCreateRegionRgnData := nil;
    GdipCreateRegionHrgn := nil;
    GdipCloneRegion := nil;
    GdipDeleteRegion := nil;
    GdipSetInfinite := nil;
    GdipSetEmpty := nil;
    GdipCombineRegionRect := nil;
    GdipCombineRegionRectI := nil;
    GdipCombineRegionPath := nil;
    GdipCombineRegionRegion := nil;
    GdipTranslateRegion := nil;
    GdipTranslateRegionI := nil;
    GdipTransformRegion := nil;
    GdipGetRegionBounds := nil;
    GdipGetRegionBoundsI := nil;
    GdipGetRegionHRgn := nil;
    GdipIsEmptyRegion := nil;
    GdipIsInfiniteRegion := nil;
    GdipIsEqualRegion := nil;
    GdipGetRegionDataSize := nil;
    GdipGetRegionData := nil;
    GdipIsVisibleRegionPoint := nil;
    GdipIsVisibleRegionPointI := nil;
    GdipIsVisibleRegionRect := nil;
    GdipIsVisibleRegionRectI := nil;
    GdipGetRegionScansCount := nil;
    GdipGetRegionScans := nil;
    GdipGetRegionScansI := nil;
    GdipCloneBrush := nil;
    GdipDeleteBrush := nil;
    GdipGetBrushType := nil;
    GdipCreateHatchBrush := nil;
    GdipGetHatchStyle := nil;
    GdipGetHatchForegroundColor := nil;
    GdipGetHatchBackgroundColor := nil;
    GdipCreateTexture := nil;
    GdipCreateTexture2 := nil;
    GdipCreateTextureIA := nil;
    GdipCreateTexture2I := nil;
    GdipCreateTextureIAI := nil;
    GdipGetTextureTransform := nil;
    GdipSetTextureTransform := nil;
    GdipResetTextureTransform := nil;
    GdipMultiplyTextureTransform := nil;
    GdipTranslateTextureTransform := nil;
    GdipScaleTextureTransform := nil;
    GdipRotateTextureTransform := nil;
    GdipSetTextureWrapMode := nil;
    GdipGetTextureWrapMode := nil;
    GdipGetTextureImage := nil;
    GdipCreateSolidFill := nil;
    GdipSetSolidFillColor := nil;
    GdipGetSolidFillColor := nil;
    GdipCreateLineBrush := nil;
    GdipCreateLineBrushI := nil;
    GdipCreateLineBrushFromRect := nil;
    GdipCreateLineBrushFromRectI := nil;
    GdipCreateLineBrushFromRectWithAngle := nil;
    GdipCreateLineBrushFromRectWithAngleI := nil;
    GdipSetLineColors := nil;
    GdipGetLineColors := nil;
    GdipGetLineRect := nil;
    GdipGetLineRectI := nil;
    GdipSetLineGammaCorrection := nil;
    GdipGetLineGammaCorrection := nil;
    GdipGetLineBlendCount := nil;
    GdipGetLineBlend := nil;
    GdipSetLineBlend := nil;
    GdipGetLinePresetBlendCount := nil;
    GdipGetLinePresetBlend := nil;
    GdipSetLinePresetBlend := nil;
    GdipSetLineSigmaBlend := nil;
    GdipSetLineLinearBlend := nil;
    GdipSetLineWrapMode := nil;
    GdipGetLineWrapMode := nil;
    GdipGetLineTransform := nil;
    GdipSetLineTransform := nil;
    GdipResetLineTransform := nil;
    GdipMultiplyLineTransform := nil;
    GdipTranslateLineTransform := nil;
    GdipScaleLineTransform := nil;
    GdipRotateLineTransform := nil;
    GdipCreatePathGradient := nil;
    GdipCreatePathGradientI := nil;
    GdipCreatePathGradientFromPath := nil;
    GdipGetPathGradientCenterColor := nil;
    GdipSetPathGradientCenterColor := nil;
    GdipGetPathGradientSurroundColorsWithCount := nil;
    GdipSetPathGradientSurroundColorsWithCount := nil;
    GdipGetPathGradientPath := nil;
    GdipSetPathGradientPath := nil;
    GdipGetPathGradientCenterPoint := nil;
    GdipGetPathGradientCenterPointI := nil;
    GdipSetPathGradientCenterPoint := nil;
    GdipSetPathGradientCenterPointI := nil;
    GdipGetPathGradientRect := nil;
    GdipGetPathGradientRectI := nil;
    GdipGetPathGradientPointCount := nil;
    GdipGetPathGradientSurroundColorCount := nil;
    GdipSetPathGradientGammaCorrection := nil;
    GdipGetPathGradientGammaCorrection := nil;
    GdipGetPathGradientBlendCount := nil;
    GdipGetPathGradientBlend := nil;
    GdipSetPathGradientBlend := nil;
    GdipGetPathGradientPresetBlendCount := nil;
    GdipGetPathGradientPresetBlend := nil;
    GdipSetPathGradientPresetBlend := nil;
    GdipSetPathGradientSigmaBlend := nil;
    GdipSetPathGradientLinearBlend := nil;
    GdipGetPathGradientWrapMode := nil;
    GdipSetPathGradientWrapMode := nil;
    GdipGetPathGradientTransform := nil;
    GdipSetPathGradientTransform := nil;
    GdipResetPathGradientTransform := nil;
    GdipMultiplyPathGradientTransform := nil;
    GdipTranslatePathGradientTransform := nil;
    GdipScalePathGradientTransform := nil;
    GdipRotatePathGradientTransform := nil;
    GdipGetPathGradientFocusScales := nil;
    GdipSetPathGradientFocusScales := nil;
    GdipCreatePen1 := nil;
    GdipCreatePen2 := nil;
    GdipClonePen := nil;
    GdipDeletePen := nil;
    GdipSetPenWidth := nil;
    GdipGetPenWidth := nil;
    GdipSetPenUnit := nil;
    GdipGetPenUnit := nil;
    GdipSetPenLineCap197819 := nil;
    GdipSetPenStartCap := nil;
    GdipSetPenEndCap := nil;
    GdipSetPenDashCap197819 := nil;
    GdipGetPenStartCap := nil;
    GdipGetPenEndCap := nil;
    GdipGetPenDashCap197819 := nil;
    GdipSetPenLineJoin := nil;
    GdipGetPenLineJoin := nil;
    GdipSetPenCustomStartCap := nil;
    GdipGetPenCustomStartCap := nil;
    GdipSetPenCustomEndCap := nil;
    GdipGetPenCustomEndCap := nil;
    GdipSetPenMiterLimit := nil;
    GdipGetPenMiterLimit := nil;
    GdipSetPenMode := nil;
    GdipGetPenMode := nil;
    GdipSetPenTransform := nil;
    GdipGetPenTransform := nil;
    GdipResetPenTransform := nil;
    GdipMultiplyPenTransform := nil;
    GdipTranslatePenTransform := nil;
    GdipScalePenTransform := nil;
    GdipRotatePenTransform := nil;
    GdipSetPenColor := nil;
    GdipGetPenColor := nil;
    GdipSetPenBrushFill := nil;
    GdipGetPenBrushFill := nil;
    GdipGetPenFillType := nil;
    GdipGetPenDashStyle := nil;
    GdipSetPenDashStyle := nil;
    GdipGetPenDashOffset := nil;
    GdipSetPenDashOffset := nil;
    GdipGetPenDashCount := nil;
    GdipSetPenDashArray := nil;
    GdipGetPenDashArray := nil;
    GdipGetPenCompoundCount := nil;
    GdipSetPenCompoundArray := nil;
    GdipGetPenCompoundArray := nil;
    GdipCreateCustomLineCap := nil;
    GdipDeleteCustomLineCap := nil;
    GdipCloneCustomLineCap := nil;
    GdipGetCustomLineCapType := nil;
    GdipSetCustomLineCapStrokeCaps := nil;
    GdipGetCustomLineCapStrokeCaps := nil;
    GdipSetCustomLineCapStrokeJoin := nil;
    GdipGetCustomLineCapStrokeJoin := nil;
    GdipSetCustomLineCapBaseCap := nil;
    GdipGetCustomLineCapBaseCap := nil;
    GdipSetCustomLineCapBaseInset := nil;
    GdipGetCustomLineCapBaseInset := nil;
    GdipSetCustomLineCapWidthScale := nil;
    GdipGetCustomLineCapWidthScale := nil;
    GdipCreateAdjustableArrowCap := nil;
    GdipSetAdjustableArrowCapHeight := nil;
    GdipGetAdjustableArrowCapHeight := nil;
    GdipSetAdjustableArrowCapWidth := nil;
    GdipGetAdjustableArrowCapWidth := nil;
    GdipSetAdjustableArrowCapMiddleInset := nil;
    GdipGetAdjustableArrowCapMiddleInset := nil;
    GdipSetAdjustableArrowCapFillState := nil;
    GdipGetAdjustableArrowCapFillState := nil;
    GdipLoadImageFromStream := nil;
    GdipLoadImageFromFile := nil;
    GdipLoadImageFromStreamICM := nil;
    GdipLoadImageFromFileICM := nil;
    GdipCloneImage := nil;
    GdipDisposeImage := nil;
    GdipSaveImageToFile := nil;
    GdipSaveImageToStream := nil;
    GdipSaveAdd := nil;
    GdipSaveAddImage := nil;
    GdipGetImageGraphicsContext := nil;
    GdipGetImageBounds := nil;
    GdipGetImageDimension := nil;
    GdipGetImageType := nil;
    GdipGetImageWidth := nil;
    GdipGetImageHeight := nil;
    GdipGetImageHorizontalResolution := nil;
    GdipGetImageVerticalResolution := nil;
    GdipGetImageFlags := nil;
    GdipGetImageRawFormat := nil;
    GdipGetImagePixelFormat := nil;
    GdipGetImageThumbnail := nil;
    GdipGetEncoderParameterListSize := nil;
    GdipGetEncoderParameterList := nil;
    GdipImageGetFrameDimensionsCount := nil;
    GdipImageGetFrameDimensionsList := nil;
    GdipImageGetFrameCount := nil;
    GdipImageSelectActiveFrame := nil;
    GdipImageRotateFlip := nil;
    GdipGetImagePalette := nil;
    GdipSetImagePalette := nil;
    GdipGetImagePaletteSize := nil;
    GdipGetPropertyCount := nil;
    GdipGetPropertyIdList := nil;
    GdipGetPropertyItemSize := nil;
    GdipGetPropertyItem := nil;
    GdipGetPropertySize := nil;
    GdipGetAllPropertyItems := nil;
    GdipRemovePropertyItem := nil;
    GdipSetPropertyItem := nil;
    GdipImageForceValidation := nil;
    GdipCreateBitmapFromStream := nil;
    GdipCreateBitmapFromFile := nil;
    GdipCreateBitmapFromStreamICM := nil;
    GdipCreateBitmapFromFileICM := nil;
    GdipCreateBitmapFromScan0 := nil;
    GdipCreateBitmapFromGraphics := nil;
    GdipCreateBitmapFromGdiDib := nil;
    GdipCreateBitmapFromHBITMAP := nil;
    GdipCreateHBITMAPFromBitmap := nil;
    GdipCreateBitmapFromHICON := nil;
    GdipCreateHICONFromBitmap := nil;
    GdipCreateBitmapFromResource := nil;
    GdipCloneBitmapArea := nil;
    GdipCloneBitmapAreaI := nil;
    GdipBitmapLockBits := nil;
    GdipBitmapUnlockBits := nil;
    GdipBitmapGetPixel := nil;
    GdipBitmapSetPixel := nil;
    GdipBitmapSetResolution := nil;
    GdipCreateImageAttributes := nil;
    GdipCloneImageAttributes := nil;
    GdipDisposeImageAttributes := nil;
    GdipSetImageAttributesToIdentity := nil;
    GdipResetImageAttributes := nil;
    GdipSetImageAttributesColorMatrix := nil;
    GdipSetImageAttributesThreshold := nil;
    GdipSetImageAttributesGamma := nil;
    GdipSetImageAttributesNoOp := nil;
    GdipSetImageAttributesColorKeys := nil;
    GdipSetImageAttributesOutputChannel := nil;
    GdipSetImageAttributesOutputChannelColorProfile := nil;
    GdipSetImageAttributesRemapTable := nil;
    GdipSetImageAttributesWrapMode := nil;
    GdipSetImageAttributesICMMode := nil;
    GdipGetImageAttributesAdjustedPalette := nil;
    GdipFlush := nil;
    GdipCreateFromHDC := nil;
    GdipCreateFromHDC2 := nil;
    GdipCreateFromHWND := nil;
    GdipCreateFromHWNDICM := nil;
    GdipDeleteGraphics := nil;
    GdipGetDC := nil;
    GdipReleaseDC := nil;
    GdipSetCompositingMode := nil;
    GdipGetCompositingMode := nil;
    GdipSetRenderingOrigin := nil;
    GdipGetRenderingOrigin := nil;
    GdipSetCompositingQuality := nil;
    GdipGetCompositingQuality := nil;
    GdipSetSmoothingMode := nil;
    GdipGetSmoothingMode := nil;
    GdipSetPixelOffsetMode := nil;
    GdipGetPixelOffsetMode := nil;
    GdipSetTextRenderingHint := nil;
    GdipGetTextRenderingHint := nil;
    GdipSetTextContrast := nil;
    GdipGetTextContrast := nil;
    GdipSetInterpolationMode := nil;
    GdipGetInterpolationMode := nil;
    GdipSetWorldTransform := nil;
    GdipResetWorldTransform := nil;
    GdipMultiplyWorldTransform := nil;
    GdipTranslateWorldTransform := nil;
    GdipScaleWorldTransform := nil;
    GdipRotateWorldTransform := nil;
    GdipGetWorldTransform := nil;
    GdipResetPageTransform := nil;
    GdipGetPageUnit := nil;
    GdipGetPageScale := nil;
    GdipSetPageUnit := nil;
    GdipSetPageScale := nil;
    GdipGetDpiX := nil;
    GdipGetDpiY := nil;
    GdipTransformPoints := nil;
    GdipTransformPointsI := nil;
    GdipGetNearestColor := nil;
    GdipCreateHalftonePalette := nil;
    GdipDrawLine := nil;
    GdipDrawLineI := nil;
    GdipDrawLines := nil;
    GdipDrawLinesI := nil;
    GdipDrawArc := nil;
    GdipDrawArcI := nil;
    GdipDrawBezier := nil;
    GdipDrawBezierI := nil;
    GdipDrawBeziers := nil;
    GdipDrawBeziersI := nil;
    GdipDrawRectangle := nil;
    GdipDrawRectangleI := nil;
    GdipDrawRectangles := nil;
    GdipDrawRectanglesI := nil;
    GdipDrawEllipse := nil;
    GdipDrawEllipseI := nil;
    GdipDrawPie := nil;
    GdipDrawPieI := nil;
    GdipDrawPolygon := nil;
    GdipDrawPolygonI := nil;
    GdipDrawPath := nil;
    GdipDrawCurve := nil;
    GdipDrawCurveI := nil;
    GdipDrawCurve2 := nil;
    GdipDrawCurve2I := nil;
    GdipDrawCurve3 := nil;
    GdipDrawCurve3I := nil;
    GdipDrawClosedCurve := nil;
    GdipDrawClosedCurveI := nil;
    GdipDrawClosedCurve2 := nil;
    GdipDrawClosedCurve2I := nil;
    GdipGraphicsClear := nil;
    GdipFillRectangle := nil;
    GdipFillRectangleI := nil;
    GdipFillRectangles := nil;
    GdipFillRectanglesI := nil;
    GdipFillPolygon := nil;
    GdipFillPolygonI := nil;
    GdipFillPolygon2 := nil;
    GdipFillPolygon2I := nil;
    GdipFillEllipse := nil;
    GdipFillEllipseI := nil;
    GdipFillPie := nil;
    GdipFillPieI := nil;
    GdipFillPath := nil;
    GdipFillClosedCurve := nil;
    GdipFillClosedCurveI := nil;
    GdipFillClosedCurve2 := nil;
    GdipFillClosedCurve2I := nil;
    GdipFillRegion := nil;
    GdipDrawImage := nil;
    GdipDrawImageI := nil;
    GdipDrawImageRect := nil;
    GdipDrawImageRectI := nil;
    GdipDrawImagePoints := nil;
    GdipDrawImagePointsI := nil;
    GdipDrawImagePointRect := nil;
    GdipDrawImagePointRectI := nil;
    GdipDrawImageRectRect := nil;
    GdipDrawImageRectRectI := nil;
    GdipDrawImagePointsRect := nil;
    GdipDrawImagePointsRectI := nil;
    GdipEnumerateMetafileDestPoint := nil;
    GdipEnumerateMetafileDestPointI := nil;
    GdipEnumerateMetafileDestRect := nil;
    GdipEnumerateMetafileDestRectI := nil;
    GdipEnumerateMetafileDestPoints := nil;
    GdipEnumerateMetafileDestPointsI := nil;
    GdipEnumerateMetafileSrcRectDestPoint := nil;
    GdipEnumerateMetafileSrcRectDestPointI := nil;
    GdipEnumerateMetafileSrcRectDestRect := nil;
    GdipEnumerateMetafileSrcRectDestRectI := nil;
    GdipEnumerateMetafileSrcRectDestPoints := nil;
    GdipEnumerateMetafileSrcRectDestPointsI := nil;
    GdipPlayMetafileRecord := nil;
    GdipSetClipGraphics := nil;
    GdipSetClipRect := nil;
    GdipSetClipRectI := nil;
    GdipSetClipPath := nil;
    GdipSetClipRegion := nil;
    GdipSetClipHrgn := nil;
    GdipResetClip := nil;
    GdipTranslateClip := nil;
    GdipTranslateClipI := nil;
    GdipGetClip := nil;
    GdipGetClipBounds := nil;
    GdipGetClipBoundsI := nil;
    GdipIsClipEmpty := nil;
    GdipGetVisibleClipBounds := nil;
    GdipGetVisibleClipBoundsI := nil;
    GdipIsVisibleClipEmpty := nil;
    GdipIsVisiblePoint := nil;
    GdipIsVisiblePointI := nil;
    GdipIsVisibleRect := nil;
    GdipIsVisibleRectI := nil;
    GdipSaveGraphics := nil;
    GdipRestoreGraphics := nil;
    GdipBeginContainer := nil;
    GdipBeginContainerI := nil;
    GdipBeginContainer2 := nil;
    GdipEndContainer := nil;
    GdipGetMetafileHeaderFromWmf := nil;
    GdipGetMetafileHeaderFromEmf := nil;
    GdipGetMetafileHeaderFromFile := nil;
    GdipGetMetafileHeaderFromStream := nil;
    GdipGetMetafileHeaderFromMetafile := nil;
    GdipGetHemfFromMetafile := nil;
    GdipCreateStreamOnFile := nil;
    GdipCreateMetafileFromWmf := nil;
    GdipCreateMetafileFromEmf := nil;
    GdipCreateMetafileFromFile := nil;
    GdipCreateMetafileFromWmfFile := nil;
    GdipCreateMetafileFromStream := nil;
    GdipRecordMetafile := nil;
    GdipRecordMetafileI := nil;
    GdipRecordMetafileFileName := nil;
    GdipRecordMetafileFileNameI := nil;
    GdipRecordMetafileStream := nil;
    GdipRecordMetafileStreamI := nil;
    GdipSetMetafileDownLevelRasterizationLimit := nil;
    GdipGetMetafileDownLevelRasterizationLimit := nil;
    GdipGetImageDecodersSize := nil;
    GdipGetImageDecoders := nil;
    GdipGetImageEncodersSize := nil;
    GdipGetImageEncoders := nil;
    GdipComment := nil;
    GdipCreateFontFamilyFromName := nil;
    GdipDeleteFontFamily := nil;
    GdipCloneFontFamily := nil;
    GdipGetGenericFontFamilySansSerif := nil;
    GdipGetGenericFontFamilySerif := nil;
    GdipGetGenericFontFamilyMonospace := nil;
    GdipGetFamilyName := nil;
    GdipIsStyleAvailable := nil;
    GdipFontCollectionEnumerable := nil;
    GdipFontCollectionEnumerate := nil;
    GdipGetEmHeight := nil;
    GdipGetCellAscent := nil;
    GdipGetCellDescent := nil;
    GdipGetLineSpacing := nil;
    GdipCreateFontFromDC := nil;
    GdipCreateFontFromLogfontA := nil;
    GdipCreateFontFromLogfontW := nil;
    GdipCreateFont := nil;
    GdipCloneFont := nil;
    GdipDeleteFont := nil;
    GdipGetFamily := nil;
    GdipGetFontStyle := nil;
    GdipGetFontSize := nil;
    GdipGetFontUnit := nil;
    GdipGetFontHeight := nil;
    GdipGetFontHeightGivenDPI := nil;
   
    GdipGetLogFontA := nil;    
    GdipGetLogFontW := nil;
    
    GdipNewInstalledFontCollection := nil;
    GdipNewPrivateFontCollection := nil;
    GdipDeletePrivateFontCollection := nil;
    GdipGetFontCollectionFamilyCount := nil;
    GdipGetFontCollectionFamilyList := nil;
    GdipPrivateAddFontFile := nil;
    GdipPrivateAddMemoryFont := nil;
    GdipDrawString := nil;
    GdipMeasureString := nil;
    GdipMeasureCharacterRanges := nil;
    GdipDrawDriverString := nil;
    GdipMeasureDriverString := nil;
    GdipCreateStringFormat := nil;
    GdipStringFormatGetGenericDefault := nil;
    GdipStringFormatGetGenericTypographic := nil;
    GdipDeleteStringFormat := nil;
    GdipCloneStringFormat := nil;
    GdipSetStringFormatFlags := nil;
    GdipGetStringFormatFlags := nil;
    GdipSetStringFormatAlign := nil;
    GdipGetStringFormatAlign := nil;
    GdipSetStringFormatLineAlign := nil;
    GdipGetStringFormatLineAlign := nil;
    GdipSetStringFormatTrimming := nil;
    GdipGetStringFormatTrimming := nil;
    GdipSetStringFormatHotkeyPrefix := nil;
    GdipGetStringFormatHotkeyPrefix := nil;
    GdipSetStringFormatTabStops := nil;
    GdipGetStringFormatTabStops := nil;
    GdipGetStringFormatTabStopCount := nil;
    GdipSetStringFormatDigitSubstitution := nil;
    GdipGetStringFormatDigitSubstitution := nil;
    GdipGetStringFormatMeasurableCharacterRangeCount := nil;
    GdipSetStringFormatMeasurableCharacterRanges := nil;
    GdipCreateCachedBitmap := nil;
    GdipDeleteCachedBitmap := nil;
    GdipDrawCachedBitmap := nil;
    GdipEmfToWmfBits := nil;
  end;
end;

// -----------------------------------------------------------------------------
// TGdiplusBase class
// -----------------------------------------------------------------------------

  class function TGdiplusBase.NewInstance: TObject;
  begin
    Result := InitInstance(GdipAlloc(ULONG(instanceSize)));
  end;

  procedure TGdiplusBase.FreeInstance;
  begin
    CleanupInstance;
    GdipFree(Self);
  end;

// -----------------------------------------------------------------------------
// macros
// -----------------------------------------------------------------------------

function ObjectTypeIsValid(type_: ObjectType): BOOL;
begin
  result :=  ((type_ >= ObjectTypeMin) and (type_ <= ObjectTypeMax));
end;

function GDIP_WMF_RECORD_TO_EMFPLUS(n: integer): Integer;
begin
  result := (n or GDIP_WMF_RECORD_BASE);
end;

function GDIP_EMFPLUS_RECORD_TO_WMF(n: integer): Integer;
begin
  result := n and (not GDIP_WMF_RECORD_BASE);
end;

function GDIP_IS_WMF_RECORDTYPE(n: integer): BOOL;
begin
  result := ((n and GDIP_WMF_RECORD_BASE) <> 0);
end;


//--------------------------------------------------------------------------
// TGPPoint Util
//--------------------------------------------------------------------------

  function MakePoint(X, Y: Integer): TGPPoint; overload;
  begin
    result.X := X;
    result.Y := Y;
  end;

  function MakePoint(X, Y: Single): TGPPointF; overload;
  begin
    Result.X := X;
    result.Y := Y;
  end;

//--------------------------------------------------------------------------
// TGPSize Util
//--------------------------------------------------------------------------

  function MakeSize(Width, Height: Single): TGPSizeF; overload;
  begin
    result.Width := Width;
    result.Height := Height;
  end;

  function MakeSize(Width, Height: Integer): TGPSize; overload;
  begin
    result.Width := Width;
    result.Height := Height;
  end;

//--------------------------------------------------------------------------
// TCharacterRange Util
//--------------------------------------------------------------------------

  function MakeCharacterRange(First, Length: Integer): TCharacterRange;
  begin
    result.First  := First;
    result.Length := Length;
  end;

// -----------------------------------------------------------------------------
// RectF class
// -----------------------------------------------------------------------------

  function MakeRect(x, y, width, height: Single): TGPRectF; overload;
  begin
    Result.X      := x;
    Result.Y      := y;
    Result.Width  := width;
    Result.Height := height;
  end;

  function MakeRect(location: TGPPointF; size: TGPSizeF): TGPRectF; overload;
  begin
    Result.X      := location.X;
    Result.Y      := location.Y;
    Result.Width  := size.Width;
    Result.Height := size.Height;
  end;

  function MakeRect(x, y, width, height: Integer): TGPRect; overload;
  begin
    Result.X      := x;
    Result.Y      := y;
    Result.Width  := width;
    Result.Height := height;
  end;

  function MakeRect(location: TGPPoint; size: TGPSize): TGPRect; overload;
  begin
    Result.X      := location.X;
    Result.Y      := location.Y;
    Result.Width  := size.Width;
    Result.Height := size.Height;
  end;

  function MakeRect(const Rect: TRect): TGPRect; overload;
  begin
    Result.X := rect.Left;
    Result.Y := Rect.Top;
    Result.Width := Rect.Right-Rect.Left;
    Result.Height:= Rect.Bottom-Rect.Top;
  end;

// -----------------------------------------------------------------------------
// PathData class
// -----------------------------------------------------------------------------

  constructor TPathData.Create;
  begin
    Count := 0;
    Points := nil;
    Types := nil;
  end;

  destructor TPathData.destroy;
  begin
    if assigned(Points) then freemem(Points);
    if assigned(Types) then freemem(Types);
  end;


function GetPixelFormatSize(pixfmt: PixelFormat): UINT;
begin
  result := (pixfmt shr 8) and $ff;
end;

function IsIndexedPixelFormat(pixfmt: PixelFormat): BOOL;
begin
  result := (pixfmt and PixelFormatIndexed) <> 0;
end;

function IsAlphaPixelFormat(pixfmt: PixelFormat): BOOL;
begin
  result := (pixfmt and PixelFormatAlpha) <> 0;
end;

function IsExtendedPixelFormat(pixfmt: PixelFormat): BOOL;
begin
  result := (pixfmt and PixelFormatExtended) <> 0;
end;

function IsCanonicalPixelFormat(pixfmt: PixelFormat): BOOL;
begin
  result := (pixfmt and PixelFormatCanonical) <> 0;
end;

// -----------------------------------------------------------------------------
// Color class
// -----------------------------------------------------------------------------

{  constructor TGPColor.Create;
  begin
    Argb := DWORD(Black);
  end;

  // Construct an opaque Color object with
  // the specified Red, Green, Blue values.
  //
  // Color values are not premultiplied.

  constructor TGPColor.Create(r, g, b: Byte);
  begin
    Argb := MakeARGB(255, r, g, b);
  end;

  constructor TGPColor.Create(a, r, g, b: Byte);
  begin
    Argb := MakeARGB(a, r, g, b);
  end;

  constructor TGPColor.Create(Value: ARGB);
  begin
    Argb := Value;
  end;

  function TGPColor.GetAlpha: BYTE;
  begin
    result := BYTE(Argb shr AlphaShift);
  end;

  function TGPColor.GetA: BYTE;
  begin
    result := GetAlpha;
  end;

  function TGPColor.GetRed: BYTE;
  begin
    result := BYTE(Argb shr RedShift);
  end;

  function TGPColor.GetR: BYTE;
  begin
    result := GetRed;
  end;

  function TGPColor.GetGreen: Byte;
  begin
    result := BYTE(Argb shr GreenShift);
  end;

  function TGPColor.GetG: Byte;
  begin
    result := GetGreen;
  end;

  function TGPColor.GetBlue: Byte;
  begin
    result := BYTE(Argb shr BlueShift);
  end;

  function TGPColor.GetB: Byte;
  begin
    result := GetBlue;
  end;

  function TGPColor.GetValue: ARGB;
  begin
    result := Argb;
  end;

  procedure TGPColor.SetValue(Value: ARGB);
  begin
    Argb := Value;
  end;

  procedure TGPColor.SetFromCOLORREF(rgb: COLORREF);
  begin
    Argb := MakeARGB(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
  end;

  function TGPColor.ToCOLORREF: COLORREF;
  begin
    result := RGB(GetRed, GetGreen, GetBlue);
  end;

  function TGPColor.MakeARGB(a, r, g, b: Byte): ARGB;
  begin
    result := ((DWORD(b) shl  BlueShift) or
               (DWORD(g) shl GreenShift) or
               (DWORD(r) shl   RedShift) or
               (DWORD(a) shl AlphaShift));
  end;  }

  function MakeColor(a, r, g, b: Byte): ARGB; overload;
  begin
    result := ((DWORD(b) shl  BlueShift) or
               (DWORD(g) shl GreenShift) or
               (DWORD(r) shl   RedShift) or
               (DWORD(a) shl AlphaShift));
  end;

  function MakeColor(r, g, b: Byte): ARGB; overload;
  begin
    result := MakeColor(255, r, g, b);
  end;

  function GetAlpha(color: ARGB): BYTE;
  begin
    result := BYTE(color shr AlphaShift);
  end;

  function GetRed(color: ARGB): BYTE;
  begin
    result := BYTE(color shr RedShift);
  end;

  function GetGreen(color: ARGB): BYTE;
  begin
    result := BYTE(color shr GreenShift);
  end;

  function GetBlue(color: ARGB): BYTE;
  begin
    result := BYTE(color shr BlueShift);
  end;

  function ColorRefToARGB(rgb: COLORREF): ARGB;
  begin
    result := MakeColor(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
  end;

  function ARGBToColorRef(Color: ARGB): COLORREF;
  begin
    result := RGB(GetRed(Color), GetGreen(Color), GetBlue(Color));
  end;


// -----------------------------------------------------------------------------
// MetafileHeader class
// -----------------------------------------------------------------------------

  procedure TMetafileHeader.GetBounds(out Rect: TGPRect);
  begin
    rect.X      := X;
    rect.Y      := Y;
    rect.Width  := Width;
    rect.Height := Height;
  end;

  function TMetafileHeader.IsWmf: BOOL;
  begin
    result :=  ((Type_ = MetafileTypeWmf) or (Type_ = MetafileTypeWmfPlaceable));
  end;

  function TMetafileHeader.IsWmfPlaceable: BOOL;
  begin
    result := (Type_ = MetafileTypeWmfPlaceable);
  end;

  function TMetafileHeader.IsEmf: BOOL;
  begin
    result := (Type_ = MetafileTypeEmf);
  end;

  function TMetafileHeader.IsEmfOrEmfPlus: BOOL;
  begin
    result := (Type_ >= MetafileTypeEmf);
  end;

  function TMetafileHeader.IsEmfPlus: BOOL;
  begin
    result := (Type_ >= MetafileTypeEmfPlusOnly)
  end;

  function TMetafileHeader.IsEmfPlusDual: BOOL;
  begin
    result := (Type_ = MetafileTypeEmfPlusDual)
  end;

  function TMetafileHeader.IsEmfPlusOnly: BOOL;
  begin
    result := (Type_ = MetafileTypeEmfPlusOnly)
  end;

  function TMetafileHeader.IsDisplay: BOOL;
  begin
    result := (IsEmfPlus and ((EmfPlusFlags and GDIP_EMFPLUSFLAGS_DISPLAY) <> 0));
  end;

  function TMetafileHeader.GetWmfHeader: PMetaHeader;
  begin
    if IsWmf then result :=  @WmfHeader
             else result := nil;
  end;

  function TMetafileHeader.GetEmfHeader: PENHMETAHEADER3;
  begin
    if IsEmfOrEmfPlus then result := @EmfHeader
                      else result := nil;
  end;



END.
