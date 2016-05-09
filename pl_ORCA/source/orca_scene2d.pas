{***************************************************
 ORCA 2D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit orca_scene2d;

{$MODE DELPHI}
{$I orca_define.inc}
{$H+}
{$ALIGN ON}
{$MINENUMSIZE 4}
interface

uses
  {$IFDEF WINDOWS}
   Windows, Messages, Registry, ShellAPI, ActiveX, CommCtrl, MultiMon,
   wincodec,
   // DirectX 10
    D2D1, DirectWrite, DXGI,
   // GDI
   GDIPAPI, GDIPOBJ, GDIPUTIL, GDICONST,
  {$ELSE}

    {$IFDEF DARWIN}
     MacOSAll,
     CarbonDef, CarbonPrivate, carboncanvas, CarbonProc, Maps,
    {$ELSE}
      cairo, cairoXlib, xlib, x,xutil, gtk2def, gtk2proc, gtk2, gdk2, gdk2x, gdk2pixbuf, glib2,
      pango, orca_api_pangocairo,
    {$ENDIF}

  {$ENDIF}

  LCLProc, LCLIntf, LCLType, LMessages, LResources,
  Classes, Variants, SysUtils, Contnrs, Forms, Controls, Dialogs, Graphics,
  StdCtrls, DB, {DBCtrls,} DBGrids, ExtCtrls, Menus, Clipbrd, ActnList, ImgList;

//=============================================================================
//=============== GLobal const/Emu Types ======================================
//=============================================================================

const
   cnGripSize = 3;
   cnRotSize = 10;
   cnInitRepeatPause = 400;  { pause before repeat timer (ms) }
   cnRepeatPause     = 100;  { pause before hint window displays (ms)}
   cnSpaceSize       =  5;   { size of space between special buttons }
   cnColorPickSize = 10;
   cnmerge3 = 0;
   cnmerged2d = 0;
   cnmergegdip = 0;
   cnmerge2 = 0;

const
  WM_ADDUPDATERECT = WM_USER + 123;

  InvalideCanvasState = $FFFFFFFF;

  vcAliceblue = '#FFF0F8FF';
  vcAntiquewhite = '#FFFAEBD7';
  vcAqua = '#FF00FFFF';
  vcAquamarine = '#FF7FFFD4';
  vcAzure = '#FFF0FFFF';
  vcBeige = '#FFF5F5DC';
  vcBisque = '#FFFFE4C4';
  vcBlack = '#FF000000';
  vcBlanchedalmond = '#FFFFEBCD';
  vcBlue = '#FF0000FF';
  vcBlueviolet = '#FF8A2BE2';
  vcBrown = '#FFA52A2A';
  vcBurlywood = '#FFDEB887';
  vcCadetblue = '#FF5F9EA0';
  vcChartreuse = '#FF7FFF00';
  vcChocolate = '#FFD2691E';
  vcCoral = '#FFFF7F50';
  vcCornflowerblue = '#FF6495ED';
  vcCornsilk = '#FFFFF8DC';
  vcCrimson = '#FFDC143C';
  vcCyan = '#FF00FFFF';
  vcDarkblue = '#FF00008B';
  vcDarkcyan = '#FF008B8B';
  vcDarkgoldenrod = '#FFB8860B';
  vcDarkgray = '#FFA9A9A9';
  vcDarkgreen = '#FF006400';
  vcDarkgrey = '#FFA9A9A9';
  vcDarkkhaki = '#FFBDB76B';
  vcDarkmagenta = '#FF8B008B';
  vcDarkolivegreen = '#FF556B2F';
  vcDarkorange = '#FFFF8C00';
  vcDarkorchid = '#FF9932CC';
  vcDarkred = '#FF8B0000';
  vcDarksalmon = '#FFE9967A';
  vcDarkseagreen = '#FF8FBC8F';
  vcDarkslateblue = '#FF483D8B';
  vcDarkslategray = '#FF2F4F4F';
  vcDarkslategrey = '#FF2F4F4F';
  vcDarkturquoise = '#FF00CED1';
  vcDarkviolet = '#FF9400D3';
  vcDeeppink = '#FFFF1493';
  vcDeepskyblue = '#FF00BFFF';
  vcDimgray = '#FF696969';
  vcDimgrey = '#FF696969';
  vcDodgerblue = '#FF1E90FF';
  vcFirebrick = '#FFB22222';
  vcFloralwhite = '#FFFFFAF0';
  vcForestgreen = '#FF228B22';
  vcFuchsia = '#FFFF00FF';
  vcGainsboro = '#FFDCDCDC';
  vcGhostwhite = '#FFF8F8FF';
  vcGold = '#FFFFD700';
  vcGoldenrod = '#FFDAA520';
  vcGray = '#FF808080';
  vcGreen = '#FF008000';
  vcGreenyellow = '#FFADFF2F';
  vcGrey = '#FF808080';
  vcHoneydew = '#FFF0FFF0';
  vcHotpink = '#FFFF69B4';
  vcIndianred = '#FFCD5C5C';
  vcIndigo = '#FF4B0082';
  vcIvory = '#FFFFFFF0';
  vcKhaki = '#FFF0E68C';
  vcLavender = '#FFE6E6FA';
  vcLavenderblush = '#FFFFF0F5';
  vcLawngreen = '#FF7CFC00';
  vcLemonchiffon = '#FFFFFACD';
  vcLightblue = '#FFADD8E6';
  vcLightcoral = '#FFF08080';
  vcLightcyan = '#FFE0FFFF';
  vcLightgoldenrodyellow = '#FFFAFAD2';
  vcLightgray = '#FFD3D3D3';
  vcLightgreen = '#FF90EE90';
  vcLightgrey = '#FFD3D3D3';
  vcLightpink = '#FFFFB6C1';
  vcLightsalmon = '#FFFFA07A';
  vcLightseagreen = '#FF20B2AA';
  vcLightskyblue = '#FF87CEFA';
  vcLightslategray = '#FF778899';
  vcLightslategrey = '#FF778899';
  vcLightsteelblue = '#FFB0C4DE';
  vcLightyellow = '#FFFFFFE0';
  vcLime = '#FF00FF00';
  vcLimegreen = '#FF32CD32';
  vcLinen = '#FFFAF0E6';
  vcMagenta = '#FFFF00FF';
  vcMaroon = '#FF800000';
  vcMediumaquamarine = '#FF66CDAA';
  vcMediumblue = '#FF0000CD';
  vcMediumorchid = '#FFBA55D3';
  vcMediumpurple = '#FF9370DB';
  vcMediumseagreen = '#FF3CB371';
  vcMediumslateblue = '#FF7B68EE';
  vcMediumspringgreen = '#FF00FA9A';
  vcMediumturquoise = '#FF48D1CC';
  vcMediumvioletred = '#FFC71585';
  vcMidnightblue = '#FF191970';
  vcMintcream = '#FFF5FFFA';
  vcMistyrose = '#FFFFE4E1';
  vcMoccasin = '#FFFFE4B5';
  vcNavajowhite = '#FFFFDEAD';
  vcNavy = '#FF000080';
  vcOldlace = '#FFFDF5E6';
  vcOlive = '#FF808000';
  vcOlivedrab = '#FF6B8E23';
  vcOrange = '#FFFFA500';
  vcOrangered = '#FFFF4500';
  vcOrchid = '#FFDA70D6';
  vcPalegoldenrod = '#FFEEE8AA';
  vcPalegreen = '#FF98FB98';
  vcPaleturquoise = '#FFAFEEEE';
  vcPalevioletred = '#FFDB7093';
  vcPapayawhip = '#FFFFEFD5';
  vcPeachpuff = '#FFFFDAB9';
  vcPeru = '#FFCD853F';
  vcPink = '#FFFFC0CB';
  vcPlum = '#FFDDA0DD';
  vcPowderblue = '#FFB0E0E6';
  vcPurple = '#FF800080';
  vcRed = '#FFFF0000';
  vcRosybrown = '#FFBC8F8F';
  vcRoyalblue = '#FF4169E1';
  vcSaddlebrown = '#FF8B4513';
  vcSalmon = '#FFFA8072';
  vcSandybrown = '#FFF4A460';
  vcSeagreen = '#FF2E8B57';
  vcSeashell = '#FFFFF5EE';
  vcSienna = '#FFA0522D';
  vcSilver = '#FFC0C0C0';
  vcSkyblue = '#FF87CEEB';
  vcSlateblue = '#FF6A5ACD';
  vcSlategray = '#FF708090';
  vcSlategrey = '#FF708090';
  vcSnow = '#FFFFFAFA';
  vcSpringgreen = '#FF00FF7F';
  vcSteelblue = '#FF4682B4';
  vcTan = '#FFD2B48C';
  vcTeal = '#FF008080';
  vcThistle = '#FFD8BFD8';
  vcTomato = '#FFFF6347';
  vcTurquoise = '#FF40E0D0';
  vcViolet = '#FFEE82EE';
  vcWheat = '#FFF5DEB3';
  vcWhite = '#FFFFFFFF';
  vcWhitesmoke = '#FFF5F5F5';
  vcYellow = '#FFFFFF00';
  vcYellowgreen = '#FF9ACD32';


type
  TD2ColorIdent = record
    Name: string;
    Value:string;
  end;

const

  d2ColorIdents: array [0..146] of TD2ColorIdent = (
    (Name: 'Aliceblue'; Value:'#FFF0F8FF'),
    (Name: 'Antiquewhite'; Value:'#FFFAEBD7'),
    (Name: 'Aqua'; Value:'#FF00FFFF'),
    (Name: 'Aquamarine'; Value:'#FF7FFFD4'),
    (Name: 'Azure'; Value:'#FFF0FFFF'),
    (Name: 'Beige'; Value:'#FFF5F5DC'),
    (Name: 'Bisque'; Value:'#FFFFE4C4'),
    (Name: 'Black'; Value:'#FF000000'),
    (Name: 'Blanchedalmond'; Value:'#FFFFEBCD'),
    (Name: 'Blue'; Value:'#FF0000FF'),
    (Name: 'Blueviolet'; Value:'#FF8A2BE2'),
    (Name: 'Brown'; Value:'#FFA52A2A'),
    (Name: 'Burlywood'; Value:'#FFDEB887'),
    (Name: 'Cadetblue'; Value:'#FF5F9EA0'),
    (Name: 'Chartreuse'; Value:'#FF7FFF00'),
    (Name: 'Chocolate'; Value:'#FFD2691E'),
    (Name: 'Coral'; Value:'#FFFF7F50'),
    (Name: 'Cornflowerblue'; Value:'#FF6495ED'),
    (Name: 'Cornsilk'; Value:'#FFFFF8DC'),
    (Name: 'Crimson'; Value:'#FFDC143C'),
    (Name: 'Cyan'; Value:'#FF00FFFF'),
    (Name: 'Darkblue'; Value:'#FF00008B'),
    (Name: 'Darkcyan'; Value:'#FF008B8B'),
    (Name: 'Darkgoldenrod'; Value:'#FFB8860B'),
    (Name: 'Darkgray'; Value:'#FFA9A9A9'),
    (Name: 'Darkgreen'; Value:'#FF006400'),
    (Name: 'Darkgrey'; Value:'#FFA9A9A9'),
    (Name: 'Darkkhaki'; Value:'#FFBDB76B'),
    (Name: 'Darkmagenta'; Value:'#FF8B008B'),
    (Name: 'Darkolivegreen'; Value:'#FF556B2F'),
    (Name: 'Darkorange'; Value:'#FFFF8C00'),
    (Name: 'Darkorchid'; Value:'#FF9932CC'),
    (Name: 'Darkred'; Value:'#FF8B0000'),
    (Name: 'Darksalmon'; Value:'#FFE9967A'),
    (Name: 'Darkseagreen'; Value:'#FF8FBC8F'),
    (Name: 'Darkslateblue'; Value:'#FF483D8B'),
    (Name: 'Darkslategray'; Value:'#FF2F4F4F'),
    (Name: 'Darkslategrey'; Value:'#FF2F4F4F'),
    (Name: 'Darkturquoise'; Value:'#FF00CED1'),
    (Name: 'Darkviolet'; Value:'#FF9400D3'),
    (Name: 'Deeppink'; Value:'#FFFF1493'),
    (Name: 'Deepskyblue'; Value:'#FF00BFFF'),
    (Name: 'Dimgray'; Value:'#FF696969'),
    (Name: 'Dimgrey'; Value:'#FF696969'),
    (Name: 'Dodgerblue'; Value:'#FF1E90FF'),
    (Name: 'Firebrick'; Value:'#FFB22222'),
    (Name: 'Floralwhite'; Value:'#FFFFFAF0'),
    (Name: 'Forestgreen'; Value:'#FF228B22'),
    (Name: 'Fuchsia'; Value:'#FFFF00FF'),
    (Name: 'Gainsboro'; Value:'#FFDCDCDC'),
    (Name: 'Ghostwhite'; Value:'#FFF8F8FF'),
    (Name: 'Gold'; Value:'#FFFFD700'),
    (Name: 'Goldenrod'; Value:'#FFDAA520'),
    (Name: 'Gray'; Value:'#FF808080'),
    (Name: 'Green'; Value:'#FF008000'),
    (Name: 'Greenyellow'; Value:'#FFADFF2F'),
    (Name: 'Grey'; Value:'#FF808080'),
    (Name: 'Honeydew'; Value:'#FFF0FFF0'),
    (Name: 'Hotpink'; Value:'#FFFF69B4'),
    (Name: 'Indianred'; Value:'#FFCD5C5C'),
    (Name: 'Indigo'; Value:'#FF4B0082'),
    (Name: 'Ivory'; Value:'#FFFFFFF0'),
    (Name: 'Khaki'; Value:'#FFF0E68C'),
    (Name: 'Lavender'; Value:'#FFE6E6FA'),
    (Name: 'Lavenderblush'; Value:'#FFFFF0F5'),
    (Name: 'Lawngreen'; Value:'#FF7CFC00'),
    (Name: 'Lemonchiffon'; Value:'#FFFFFACD'),
    (Name: 'Lightblue'; Value:'#FFADD8E6'),
    (Name: 'Lightcoral'; Value:'#FFF08080'),
    (Name: 'Lightcyan'; Value:'#FFE0FFFF'),
    (Name: 'Lightgoldenrodyellow'; Value:'#FFFAFAD2'),
    (Name: 'Lightgray'; Value:'#FFD3D3D3'),
    (Name: 'Lightgreen'; Value:'#FF90EE90'),
    (Name: 'Lightgrey'; Value:'#FFD3D3D3'),
    (Name: 'Lightpink'; Value:'#FFFFB6C1'),
    (Name: 'Lightsalmon'; Value:'#FFFFA07A'),
    (Name: 'Lightseagreen'; Value:'#FF20B2AA'),
    (Name: 'Lightskyblue'; Value:'#FF87CEFA'),
    (Name: 'Lightslategray'; Value:'#FF778899'),
    (Name: 'Lightslategrey'; Value:'#FF778899'),
    (Name: 'Lightsteelblue'; Value:'#FFB0C4DE'),
    (Name: 'Lightyellow'; Value:'#FFFFFFE0'),
    (Name: 'Lime'; Value:'#FF00FF00'),
    (Name: 'Limegreen'; Value:'#FF32CD32'),
    (Name: 'Linen'; Value:'#FFFAF0E6'),
    (Name: 'Magenta'; Value:'#FFFF00FF'),
    (Name: 'Maroon'; Value:'#FF800000'),
    (Name: 'Mediumaquamarine'; Value:'#FF66CDAA'),
    (Name: 'Mediumblue'; Value:'#FF0000CD'),
    (Name: 'Mediumorchid'; Value:'#FFBA55D3'),
    (Name: 'Mediumpurple'; Value:'#FF9370DB'),
    (Name: 'Mediumseagreen'; Value:'#FF3CB371'),
    (Name: 'Mediumslateblue'; Value:'#FF7B68EE'),
    (Name: 'Mediumspringgreen'; Value:'#FF00FA9A'),
    (Name: 'Mediumturquoise'; Value:'#FF48D1CC'),
    (Name: 'Mediumvioletred'; Value:'#FFC71585'),
    (Name: 'Midnightblue'; Value:'#FF191970'),
    (Name: 'Mintcream'; Value:'#FFF5FFFA'),
    (Name: 'Mistyrose'; Value:'#FFFFE4E1'),
    (Name: 'Moccasin'; Value:'#FFFFE4B5'),
    (Name: 'Navajowhite'; Value:'#FFFFDEAD'),
    (Name: 'Navy'; Value:'#FF000080'),
    (Name: 'Oldlace'; Value:'#FFFDF5E6'),
    (Name: 'Olive'; Value:'#FF808000'),
    (Name: 'Olivedrab'; Value:'#FF6B8E23'),
    (Name: 'Orange'; Value:'#FFFFA500'),
    (Name: 'Orangered'; Value:'#FFFF4500'),
    (Name: 'Orchid'; Value:'#FFDA70D6'),
    (Name: 'Palegoldenrod'; Value:'#FFEEE8AA'),
    (Name: 'Palegreen'; Value:'#FF98FB98'),
    (Name: 'Paleturquoise'; Value:'#FFAFEEEE'),
    (Name: 'Palevioletred'; Value:'#FFDB7093'),
    (Name: 'Papayawhip'; Value:'#FFFFEFD5'),
    (Name: 'Peachpuff'; Value:'#FFFFDAB9'),
    (Name: 'Peru'; Value:'#FFCD853F'),
    (Name: 'Pink'; Value:'#FFFFC0CB'),
    (Name: 'Plum'; Value:'#FFDDA0DD'),
    (Name: 'Powderblue'; Value:'#FFB0E0E6'),
    (Name: 'Purple'; Value:'#FF800080'),
    (Name: 'Red'; Value:'#FFFF0000'),
    (Name: 'Rosybrown'; Value:'#FFBC8F8F'),
    (Name: 'Royalblue'; Value:'#FF4169E1'),
    (Name: 'Saddlebrown'; Value:'#FF8B4513'),
    (Name: 'Salmon'; Value:'#FFFA8072'),
    (Name: 'Sandybrown'; Value:'#FFF4A460'),
    (Name: 'Seagreen'; Value:'#FF2E8B57'),
    (Name: 'Seashell'; Value:'#FFFFF5EE'),
    (Name: 'Sienna'; Value:'#FFA0522D'),
    (Name: 'Silver'; Value:'#FFC0C0C0'),
    (Name: 'Skyblue'; Value:'#FF87CEEB'),
    (Name: 'Slateblue'; Value:'#FF6A5ACD'),
    (Name: 'Slategray'; Value:'#FF708090'),
    (Name: 'Slategrey'; Value:'#FF708090'),
    (Name: 'Snow'; Value:'#FFFFFAFA'),
    (Name: 'Springgreen'; Value:'#FF00FF7F'),
    (Name: 'Steelblue'; Value:'#FF4682B4'),
    (Name: 'Tan'; Value:'#FFD2B48C'),
    (Name: 'Teal'; Value:'#FF008080'),
    (Name: 'Thistle'; Value:'#FFD8BFD8'),
    (Name: 'Tomato'; Value:'#FFFF6347'),
    (Name: 'Turquoise'; Value:'#FF40E0D0'),
    (Name: 'Violet'; Value:'#FFEE82EE'),
    (Name: 'Wheat'; Value:'#FFF5DEB3'),
    (Name: 'White'; Value:'#FFFFFFFF'),
    (Name: 'Whitesmoke'; Value:'#FFF5F5F5'),
    (Name: 'Yellow'; Value:'#FFFFFF00'),
    (Name: 'Yellowgreen'; Value:'#FF9ACD32')
  );

type

  PIntArray = ^TIntArray;
  TIntArray = array [0..0] of integer;

  TD2Point = packed record
    X:single;
    Y:single;
  end;

  TD2CubicBezier = array [0..3] of TD2Point;

  PD2PointArray = ^TD2PointArray;
  TD2PointArray = array [0..0] of TD2Point;

  TD2Rect = packed record
    case Integer of
      0: (Left, Top, Right, Bottom:single);
      1: (TopLeft, BottomRight: TD2Point);
  end;
  TSelArea = array of TD2Rect;

  TD2Corner = (d2CornerTopLeft, d2CornerTopRight, d2CornerBottomLeft, d2CornerBottomRight);
  TD2Corners = set of TD2Corner;

  TD2CornerType = (d2CornerRound, d2CornerBevel, d2CornerInnerRound, d2CornerInnerLine);

  TD2Side = (d2SideTop, d2SideLeft, d2SideBottom, d2SideRight);
  TD2Sides = set of TD2Side;

  TD2VectorArray = array [0..2] of single;

  TD2Vector = packed record
    case integer of
      0: (V: TD2VectorArray;);
      1: (X:single;Y:single;W:single;);
  end;

  TD2MatrixArray = array [0..2] of TD2Vector;

  TD2Matrix = packed record
    case integer of
      0: (M: TD2MatrixArray;);
      1: (m11,m12,m13:single;
          m21,m22,m23:single;
          m31,m32,m33:single);
   end;

  TD2Polygon = array of TD2Point;
  PD2Polygon = ^TD2Polygon;
  TMessage = TLMessage;

  PD2Color = ^TD2Color;
  TD2Color = cardinal;

  PD2Color24 = ^TD2Color24;
  TD2Color24 = packed record
    case longword of
      0: (R,G,B:Byte);
    end;
	
  PD2ColorRec = ^TD2ColorRec;
  TD2ColorRec = packed record
    case longword of
      0: (Color : TD2Color);
      2: (HiWord, LoWord : Word);
      {$ifdef  FPC_BIG_ENDIAN}
      3: (A,R,G,B:System.Byte);
      {$else}
      3: (B,G,R,A:System.Byte);
      {$endif}
    end;

  PD2ColorArray = ^TD2ColorArray;
  TD2ColorArray = array [0..4] of TD2Color;

  PD2ColorRecArray = ^TD2ColorRecArray;
  TD2ColorRecArray = array [0..0] of TD2ColorRec;


  PD2Color24Array = ^TD2Color24Array;
  TD2Color24Array = array [0..0] of TD2Color24;

  //.................................

  TD2LineType = (d2LineNormal, d2LineHorizontal, d2LineVertical);
  TD2GradientStyle = (d2LinearGradient, d2RadialGradient);
  TD2AniIndicatorStyle = (d2AniIndicatorLine, d2AniIndicatorCircle);
  TD2CloseAlign = (d2ButtonAlignLeft,d2ButtonAlignRight);
  TD2ListStyle = (d2ListVertical, d2ListHorizontal);
  TD2ValueType = (d2ValueInteger, d2ValueFloat);
  TD2CalDayOfWeek = (d2Monday, d2Tuesday, d2Wednesday, d2Thursday, d2Friday, d2Saturday, d2Sunday, d2LocaleDefault);
  TD2Nad2lyph = (ngEnabled, ngDisabled);
  TD2NavigateBtn = (nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbRefresh);
  TD2NavButtonSet = set of TD2NavigateBtn;
  TD2NavButtonStyle = set of (nsAllowTimer, nsFocusRect);

  TD2WrapMode = (d2WrapTile, d2WrapTileOriginal, d2WrapTileStretch);
  TD2BrushStyle = (d2BrushNone,d2BrushSolid,d2BrushGradient,d2BrushBitmap, d2BrushResource,d2BrushVisual);
  TD2FontStyle = (d2FontRegular, d2FontBold, d2FontItalic, d2FontBoldItalic, d2FontUnderline, d2FontStrikeout);
  TD2TextAlign = (d2TextAlignCenter, d2TextAlignNear, d2TextAlignFar);

  TD2PathPointKind = (d2PathPointMoveTo, d2PathPointLineTo, d2PathPointCurveTo, d2PathPointClose);
  TD2StrokeCap = (d2CapFlat, d2CapRound);
  TD2StrokeJoin = (d2JoinMiter, d2JoinRound, d2JoinBevel);
  TD2StrokeDash = (d2DashSolid, d2DashDash, d2DashDot, d2DashDashDot, d2DashDashDotDot, d2DashCustom);
  TD2AnimationType = (d2AnimationIn, d2AnimationOut, d2AnimationInOut);

  TD2InterpolationType = (d2InterpolationLinear, d2InterpolationQuadratic, d2InterpolationCubic, d2InterpolationQuartic,
                          d2InterpolationQuintic, d2InterpolationSinusoidal, d2InterpolationExponential,
                          d2InterpolationCircular, d2InterpolationElastic, d2InterpolationBack, d2InterpolationBounce);

  TD2Align = ( vaNone,vaTopLeft,  vaTopRight,vaBottomLeft, vaBottomRight, vaTop, vaLeft, vaRight, vaBottom,
               vaMostTop, vaMostBottom, vaMostLeft, vaMostRight, vaClient, vaContents, vaCenter, vaVertCenter,
               vaHorzCenter, vaHorizontal, vaVertical, vaScale, vaFit, vaFitLeft, vaFitRight);

  TD2CalloutPosition = (d2CalloutTop, d2CalloutLeft, d2CalloutBottom, d2CalloutRight);
  TD2PathWrap = (d2PathOriginal, d2PathFit, d2PathStretch, d2PathTile);
  TD2ButtonLayout = (d2GlyphLeft, d2GlyphRight, d2GlyphTop, d2GlyphBottom, d2GlyphCenter);

  TEditCharCase = (d2ecNormal, d2ecUpperCase, d2ecLowerCase);
  TD2DragMode = (d2DragManual, d2DragAutomatic);
  TD2Orientation = (d2Horizontal, d2Vertical);

  TD2Placement = (d2PlacementBottom, d2PlacementTop, d2PlacementLeft, d2PlacementRight, d2PlacementCenter,
                  d2PlacementBottomCenter, d2PlacementTopCenter, d2PlacementLeftCenter, d2PlacementRightCenter,
                  d2PlacementAbsolute, d2PlacementMouse, d2PlacementMouseCenter);

  TD2PlacementScene = (d2PlacementSceneNew, d2PlacementSceneTarget);

  TD2MessageType = (d2MessageWarning, d2MessageError, d2MessageInformation, d2MessageConfirmation, d2MessageCustom);

  TD2MessageButton = (d2ButtonYes, d2ButtonNo, d2ButtonOK, d2ButtonCancel, d2ButtonAbort, d2ButtonRetry,
                     d2ButtonIgnore, d2ButtonAll, d2ButtonNoToAll, d2ButtonYesToAll, d2ButtonHelp);
  TD2MessageButtons = set of TD2MessageButton;

  TInsertOption = (ioSelected, ioMoveCaret, ioCanUndo, ioUnDoPairedWithPriv);
  TInsertOptions = set of TInsertOption;

  TDeleteOption = (doMoveCaret, doCanUndo);
  TDeleteOptions = set of TDeleteOption;

  TActionType = (atDelete, atInsert);

  TD2ImageWrap = (d2ImageOriginal, d2ImageFit, d2ImageStretch, d2ImageTile);

  TLinesBegs = array of integer;
  PLinesBegs = ^TLinesBegs;

  TCaretPosition = record
    Line, Pos:integer;
  end;

  PEdtAction = ^TEdtAction;

  TEdtAction = record
    ActionType : TActionType;
    PairedWithPriv :boolean;
    StartPosition :integer;
    DeletedFragment : WideString;
    Length :integer;
  end;

   TD2PathPoint = packed record
    Kind: TD2PathPointKind;
    Point: TD2Point;
  end;

  //................................

const
  Epsilon: Single   = 1e-40;
  c180   : Single   = 180;
  c360   : Single   = 360;
  cPI    : Single   = 3.141592654;
  c2PI   : Single   = 6.283185307;
  cPIdiv2: Single   = 1.570796326;
  cPIdiv4: Single   = 0.785398163;
  c3PIdiv4 : Single = 2.35619449;
  cInv2PI  : Single = 1/6.283185307;
  cInv360  : Single = 1/360;
  cOneHalf : Single = 0.5;
  c180divPI: Single = 57.2957795155;
  cPIdiv180: Single = 0.017453292;
  CurveKappa        = 0.5522847498;
  CurveKappaInv     = 1 - CurveKappa;


  IdentityMatrix: TD2Matrix = (m11:1.0; m12:0.0; m13:0.0;
                               m21:0.0; m22:1.0; m23:0.0;
                               m31:0.0; m32:0.0; m33:1.0);

  ZeroMatrix: TD2Matrix = (m11:0.0; m12:0.0; m13:0.0;
                           m21:0.0; m22:0.0; m23:0.0;
                           m31:0.0; m32:0.0; m33:0.0);
  NullRect: TD2Rect = (Left: 0; Top: 0; Right: 0; Bottom: 0);

  AllCorners: TD2Corners = [d2CornerTopLeft, d2CornerTopRight, d2CornerBottomLeft, d2CornerBottomRight];
  AllSides: TD2Sides = [d2SideTop, d2SideLeft, d2SideBottom, d2SideRight];

  ClosePolygon: TD2Point = (X: $FFFF; Y: $FFFF);


  d2WideNull = System.WideChar(#0);
  d2WideTabulator = System.WideChar(#9);
  d2WideSpace = System.WideChar(#32);
  d2WideCarriageReturn = System.WideChar($D);
  d2WideLineFeed = System.WideChar($A);
  d2WideVerticalTab = System.WideChar($B);
  d2WideFormFeed = System.WideChar($C);
  d2WideLineSeparator = System.WideChar($2028);
  d2WideParagraphSeparator = System.WideChar($2029);

  BOM_LSB_FIRST = System.WideChar( $FEFF );
  BOM_MSB_FIRST = System.WideChar( $FFFE );

//=============================================================================
//=============== Classes =====================================================
//=============================================================================
type

  TD2Canvas  = class;
  TD2Object  = class;
  TD2CustomScene  = class;
  TD2VisualObject = class;
  TD2ObjectClass  = class of TD2Object;
  TD2Resources    = class;
  TD2Control      = class;
  TD2Bitmap       = class;
  TD2Brush        = class;
  TD2BrushObject  = class;
  TD2BitmapObject = class;
  TD2PathObject   = class;
  TD2Font         = class;
  TD2ControlActionLink = class;

  TD2KeyEvent = procedure(Sender: TObject; var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState) of object;
  TOnPaintEvent = procedure (Sender: TObject; const ACanvas: TD2Canvas; const ARect: TD2Rect) of object;
  TD2MouseEvent = procedure(Sender: TObject; Button: TMouseButton;  Shift: TShiftState; X, Y:single) of object;
  TD2MouseMoveEvent = procedure(Sender: TObject; Shift: TShiftState;  X, Y, Dx, Dy:single) of object;
  TD2MouseWheelEvent = procedure(Sender: TObject; Shift: TShiftState; WheelDelta: Integer; MousePos: TD2Point; var Handled: Boolean) of object;
  TD2ProcessTickEvent = procedure(Sender: TObject; time, deltaTime:single) of object;
  TD2ObjectSortCompare = function(item1, item2: TD2Object):integer;

TD2DragObject = record
    Source: TObject;
    Files: array of WideString;
    Data: Variant;
  end;

TD2SaveData = record
    Index : cardinal;
    Matrix: TD2Matrix;
    AbsoluteMatrix: TD2Matrix;
    InvertMatrix  : TD2Matrix;
    Fill  : TD2Brush;
    Stroke: TD2Brush;
    StrokeThickness:single;
    StrokeCap : TD2StrokeCap;
    StrokeJoin: TD2StrokeJoin;
    StrokeDash: TD2StrokeDash;
    Dash : array of single;
    DashOffset:single;
    Font : TD2Font;
    Data : Pointer;
  end;
TD2SaveDataArray = array of TD2SaveData;

TD2Timer = class(TTimer)
  end;

TD2AniThread = class(TD2Timer)
  private
    FAniList  : TList;
    FStartTime:single;
    FTime     :single;
    FDeltaTime:single;
    procedure OneStep;
    procedure DoSyncTimer(Sender: TObject);
  protected
  public
    constructor Create;
    destructor Destroy;  override;
  end;

TD2ControlActionLink = class(TActionLink)
  protected
    FClient: TD2Control;
    procedure AssignClient(AClient: TObject);  override;
    function  DoShowHint(var HintStr: string): Boolean;  virtual;
    procedure SetCaption(const Value:string);  override;
    procedure SetEnabled(Value:Boolean);  override;
    procedure SetHelpContext(Value:THelpContext);  override;
    procedure SetHelpKeyword(const Value:string);  override;
    procedure SetHelpType(Value:THelpType);  override;
    procedure SetHint(const Value:string);  override;
    procedure SetVisible(Value:Boolean);  override;
    procedure SetOnExecute(Value:TNotifyEvent);  override;
  public
    function  IsCaptionLinked: Boolean;  override;
    function  IsEnabledLinked: Boolean;  override;
    function  IsHelpLinked: Boolean;  override;
    function  IsHintLinked: Boolean;  override;
    function  IsVisibleLinked: Boolean;  override;
    function  IsOnExecuteLinked: Boolean;  override;
  end;
TD2ControlActionLinkClass = class of TD2ControlActionLink;

TD2Font = class(TPersistent)
  private
    FSize :single;
    FStyle: TD2FontStyle;
    FClearType :boolean;
    FFamily : string;
    FOnChanged :TNotifyEvent;
    function  isSizeStored:Boolean;
    function  isFamilyStored:Boolean;
    procedure SetSize(const Value:single);
    procedure SetFamily(const Value:string);
    procedure SetStyle(const Value:TD2FontStyle);
    procedure SetClearType(const Value:boolean);
  protected
    procedure AssignTo(Dest:TPersistent);  override;
  public
    constructor Create;
    destructor Destroy;  override;
    procedure Assign(Source:TPersistent);  override;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
  published
    property ClearType: boolean read FClearType write SetClearType  default true;
    property Family: string read FFamily write SetFamily stored isFamilyStored;
    property Size:single read FSize write SetSize stored isSizeStored;
    property Style: TD2FontStyle read FStyle write SetStyle  default d2FontRegular;
  end;


TD2BrushResource = class(TPersistent)
  private
    FResource: TD2BrushObject;
    FResourceName: string;
    FOnChanged:TNotifyEvent;
    function  GetBrush: TD2Brush;
    procedure SetResource(const Value:TD2BrushObject);
    function  GetResourceName: string;
    procedure SetResourceName(const Value:string);
  public
    destructor Destroy;  override;
    property  OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    procedure Assign(Source: TPersistent);  override;
    property  Brush: TD2Brush read GetBrush;
  published
    property Resource: TD2BrushObject read FResource write SetResource stored false;
    property ResourceName: string read GetResourceName write SetResourceName;
  end;

TD2BrushBitmap = class(TPersistent)
  private
    FOnChanged:TNotifyEvent;
    FBitmap: TD2Bitmap;
    FWrapMode: TD2WrapMode;
    procedure SetWrapMode(const Value:TD2WrapMode);
    procedure SetBitmap(Value:TD2Bitmap);
  public
    constructor Create;
    destructor Destroy;  override;
    property  OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    procedure Assign(Source: TPersistent);  override;
  published
    property Bitmap: TD2Bitmap read FBitmap write SetBitmap;
    property WrapMode: TD2WrapMode read FWrapMode write SetWrapMode;
  end;


TD2GradientPoint = class(TCollectionItem)
  private
    FColor: TD2Color;
    FOffset:single;
    function GetColor: string;
    procedure SetColor(const Value:string);
  protected
  public
    constructor Create(ACollection: TCollection);  override;
    procedure Assign(Source: TPersistent);  override;
    property IntColor: TD2Color read FColor write FColor;
  published
    property Color: string read GetColor write SetColor;
    property Offset:single read FOffset write FOffset;
  end;

TD2GradientPoints = class(TCollection)
  private
    function GetPoint(Index: integer): TD2GradientPoint;
  public
    property Points[Index: integer]: TD2GradientPoint read GetPoint; default;
  end;


TD2Position = class(TPersistent)
  private
    FY :single;
    FX :single;
    FDefaultValue : TD2Point;
    FOnChange:TNotifyEvent;
    procedure SetX(const Value:single);
    procedure SetY(const Value:single);
    procedure SetPoint(const Value:TD2Point);
    function  GetPoint:TD2Point;
    procedure SetVector(const Value:TD2Vector);
    function  GetVector:TD2Vector;
  protected
    procedure DefineProperties(Filer:TFiler);  override;
    procedure ReadPoint(Reader:TReader);
    procedure WritePoint(Writer:TWriter);
  public
    constructor Create(const ADefaultValue:TD2Point);  virtual;
    procedure Assign(Source:TPersistent);  override;
    function  Empty:boolean;
    procedure Reflect(const Normal: TD2Vector);
    property  Point: TD2Point read GetPoint write SetPoint;
    property  Vector: TD2Vector read GetVector write SetVector;
    property  DefaultValue:TD2Point read FDefaultValue write FDefaultValue;
    property  OnChange:TNotifyEvent read FOnChange write FOnChange;
  published
    property X:single read FX write SetX stored false;
    property Y:single read FY write SetY stored false;
  end;

TD2Transform = class(TPersistent)
  private
    FMatrix : TD2Matrix;
    FRotateAngle:single;
    FPosition: TD2Position;
    FScale: TD2Position;
    FSkew: TD2Position;
    FRotateCenter: TD2Position;
    FOnChanged:TNotifyEvent;
    procedure SetRotateAngle(const Value:single);
  protected
    procedure MatrixChanged(Sender: TObject);
    property Skew: TD2Position read FSkew write FSkew;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property Matrix: TD2Matrix read FMatrix;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
  published
    property Position: TD2Position read FPosition write FPosition;
    property Scale: TD2Position read FScale write FScale;
    property RotateAngle:single read FRotateAngle write SetRotateAngle;
    property RotateCenter: TD2Position read FRotateCenter write FRotateCenter;
  end;

TD2Gradient = class(TPersistent)
  private
    FPoints: TD2GradientPoints;
    FOnChanged:TNotifyEvent;
    FStartPosition: TD2Position;
    FStopPosition: TD2Position;
    FStyle: TD2GradientStyle;
    FRadialTransform: TD2Transform;
    procedure SetStartPosition(const Value:TD2Position);
    procedure SetStopPosition(const Value:TD2Position);
    procedure PositionChanged(Sender: TObject);
    procedure SetColor(const Value:string);
    procedure SetColor1(const Value:string);
    function isLinearStored: Boolean;
    procedure SetStyle(const Value:TD2GradientStyle);
    function isRadialStored: Boolean;
    procedure SetRadialTransform(const Value:TD2Transform);
  public
    constructor Create;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    procedure Change;
    function InterpolateColor(Offset:single): TD2Color;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    property Color: string write SetColor;
    property Color1: string write SetColor1;
  published
    property Points: TD2GradientPoints read FPoints write FPoints;
    property Style: TD2GradientStyle read FStyle write SetStyle;
    property StartPosition: TD2Position read FStartPosition write SetStartPosition stored isLinearStored;
    property StopPosition: TD2Position read FStopPosition write SetStopPosition stored isLinearStored;
    property RadialTransform: TD2Transform read FRadialTransform write SetRadialTransform stored isRadialStored;
  end;

TD2Visual = class(TPersistent)
  private
    FOnChanged:TNotifyEvent;
    FVisualObject: TD2VisualObject;
    procedure SetVisualObject(const Value:TD2VisualObject);
  public
    constructor Create;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property  OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
  published
    property VisualObject: TD2VisualObject read FVisualObject write SetVisualObject;
  end;

TD2Brush = class(TPersistent)
  private
    FColor: TD2Color;
    FStyle: TD2BrushStyle;
    FOnChanged:TNotifyEvent;
    FGradient: TD2Gradient;
    FVisual: TD2Visual;
    FDefaultStyle: TD2BrushStyle;
    FDefaultColor: TD2Color;
    FResource: TD2BrushResource;
    FBitmap: TD2BrushBitmap;
    procedure SetColor(const Value:string);
    procedure SetStyle(const Value:TD2BrushStyle);
    procedure SetGradient(const Value:TD2Gradient);
    procedure SetVisual(const Value:TD2Visual);
    function  isColorStored: Boolean;
    function  isGradientStored: Boolean;
    function  isVisualStored: Boolean;
    function  GetColor: string;
    procedure SetSolidColor(const Value:TD2Color);
    function  isStyleStored: Boolean;
    procedure SetResource(const Value:TD2BrushResource);
    function  isResourceStored: Boolean;
    function  isBitmapStored: Boolean;
    function  GetSolidColor: TD2Color;
  protected
    procedure GradientChanged(Sender: TObject);
    procedure VisualChanged(Sender: TObject);
    procedure ResourceChanged(Sender: TObject);
    procedure BitmapChanged(Sender: TObject);
  public
    constructor Create(const ADefaultStyle: TD2BrushStyle; const ADefaultColor: TD2Color);
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    property SolidColor: TD2Color read GetSolidColor write SetSolidColor;
    property DefaultColor: TD2Color read FDefaultColor write FDefaultColor;
    property DefaultStyle: TD2BrushStyle read FDefaultStyle write FDefaultStyle;
  published
    property Color: string read GetColor write SetColor stored isColorStored;
    property Bitmap: TD2BrushBitmap read FBitmap write FBitmap stored isBitmapStored;
    property Style: TD2BrushStyle read FStyle write SetStyle stored isStyleStored;
    property Gradient: TD2Gradient read FGradient write SetGradient stored isGradientStored;
    property Resource: TD2BrushResource read FResource write SetResource stored isResourceStored;
    property Visual: TD2Visual read FVisual write SetVisual stored isVisualStored;
  end;

Id2SizeGrip = interface
    ['{86960F17-A3D9-4A26-A1A1-E7F4F0016F71}']
  end;

Id2Scene = interface
    ['{5F9591A0-DA32-4AF3-B3A1-91283599DE97}']
    procedure AddObject(AObject: TD2Object);
    procedure RemoveObject(AObject: TD2Object);
    procedure BeginDrag;
    procedure BeginResize;
    procedure AddUpdateRect(R: TD2Rect);
    procedure InsertObject(const ClassName: string);
    function  GetActiveControl: TD2Control;
    function  GetDisableUpdate:boolean;
    procedure SetDisableUpdate(Value:boolean);
    function  GetDesignTime:boolean;
    function  GetCanvas: TD2Canvas;
    function  GetRoot: TD2Object;
    function  GetOwner: TComponent;
    function  GetComponent: TComponent;
    function  GetStyle: TD2Resources;
    procedure SetStyle(const Value:TD2Resources);
    function  GetTransparency:boolean;
    procedure UpdateResource;
    procedure Notification(AComponent: TComponent; Operation: TOperation);
    function  GetSelected: TD2VisualObject;
    function  GetDesignPlaceObject: TD2VisualObject;
    function  GetUpdateRectsCount:integer;
    function  GetUpdateRect(const Index: integer): TD2Rect;
    procedure SetCaptured(const Value:TD2VisualObject);
    function  GetCaptured: TD2VisualObject;
    procedure SetFocused(const Value:TD2VisualObject);
    function  GetFocused: TD2VisualObject;
    procedure SetDesignRoot(const Value:TD2VisualObject);
    function  GetMousePos: TD2Point;
    function  LocalToScreen(const Point: TD2Point): TD2Point;
    procedure BeginVCLDrag(Source: TObject; ABitmap: TD2Bitmap);
    procedure DoDesignSelect(AObject: TObject);
    function  GetAnimatedCaret:boolean;
    function  ShowKeyboardForControl(AObject: TD2Object):boolean;
    function  HideKeyboardForControl(AObject: TD2Object):boolean;
  end;

TD2WideStrings = class(TPersistent)
  private
    FUpdateCount: Integer;
    FCaseSensitive: Boolean;
    FSorted: Boolean;
    function  GetCommaText: WideString;
    function  GetName(Index: Integer): WideString;
    function  GetValue(const Name: WideString): WideString;
    procedure ReadData(Reader:TReader);
    procedure SetCommaText(const Value:WideString);
    procedure SetValue(const Name, Value:WideString);
    procedure WriteData(Writer:TWriter);
    function  GetValueFromIndex(Index: Integer): WideString;
    procedure SetValueFromIndex(Index: Integer; const Value:WideString);
    procedure SetCaseSensitive(const Value:Boolean);
    procedure SetSorted(const Value:Boolean);
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure Error(const Msg: String; Data: Integer);
    function  CompareStrings(const S1, S2: WideString): Integer;  virtual;
    function  Get(Index: Integer): WideString;  virtual; abstract;
    function  GetCapacity: Integer;  virtual;
    function  GetCount: Integer;  virtual; abstract;
    function  GetObject(Index: Integer): TObject;  virtual;
    function  GetTextStr: WideString;  virtual;
    procedure Put(Index: Integer; const S: WideString);  virtual;
    procedure PutObject(Index: Integer; AObject: TObject);  virtual;
    procedure SetCapacity(NewCapacity: Integer);  virtual;
    procedure SetTextStr(const Value:WideString);  virtual;
    procedure SetUpdateState(Updating: Boolean);  virtual;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure BeginUpdate;
    procedure EndUpdate;
    function  AddObject(const S: WideString; AObject: TObject): Integer;  virtual;
    procedure Append(const S: WideString);
    procedure AddStrings(Strings: TStrings); overload;  virtual;
    procedure AddStrings(Strings: TD2WideStrings); overload;  virtual;
    procedure Assign(Source: TPersistent);  override;
    procedure AssignTo(Dest: TPersistent);  override;
    function  Equals(Strings: TD2WideStrings): Boolean;
    procedure Exchange(Index1, Index2: Integer);  virtual;
    function  IndexOf(const S: WideString): Integer;  virtual;
    function  IndexOfName(const Name: WideString): Integer;  virtual;
    function  IndexOfValue(const Name: WideString): Integer;  virtual;
    function  IndexOfObject(AObject: TObject): Integer;
    procedure Insert(Index: Integer; const S: WideString);  virtual; abstract;
    procedure InsertObject(Index: Integer; const S: WideString; AObject: TObject);
    procedure LoadFromFile(const FileName: String);  virtual;
    procedure LoadFromStream(Stream: TStream);  virtual;
    procedure Move(CurIndex, NewIndex: Integer);  virtual;
    procedure SaveToFile(const FileName: String);  virtual;
    procedure SaveToAnsiFile(const FileName: String);  virtual;
    procedure SaveToStream(Stream: TStream);  virtual;
    procedure SetText(Text: PWideChar);  virtual;
    procedure Sort;  virtual;
    property  Capacity: Integer read GetCapacity write SetCapacity;
    property  Names[Index: Integer]: WideString read GetName;
    property  Objects[Index: Integer]: TObject read GetObject write PutObject;
    property  Values[const Name: WideString]: WideString read GetValue write SetValue;
    property  ValueFromIndex[Index: Integer]: WideString read GetValueFromIndex write SetValueFromIndex;
    property  Strings[Index: Integer]: WideString read Get write Put; default;
    property  Text: WideString read GetTextStr write SetTextStr stored true;
    function  Items(AIndex: integer): WideString;
    procedure SetItem(index:integer; AText: wideString);
    function  Add(const S: WideString): Integer;  virtual;
    procedure Delete(Index: Integer);  virtual;
    procedure Clear;  virtual;
    property Count: Integer read GetCount;
    property CommaText: WideString read GetCommaText write SetCommaText stored false;
    property CaseSensitive: Boolean read FCaseSensitive write SetCaseSensitive stored false;
    property Sorted: Boolean read FSorted write SetSorted stored false;
  published
  end;

  PWideStringItem = ^TWideStringItem;
  TWideStringItem = record
    FString: WideString;
    FObject: TObject;
  end;

  PWideStringItemList = ^TWideStringItemList;
  TWideStringItemList = array[0..MaxListSize] of TWideStringItem;

TD2WideStringList = class(TD2WideStrings)
  private
    FList: PWideStringItemList;
    FCount: Integer;
    FCapacity:integer;
    FDuplicates: TDuplicates;
    FOnChange:TNotifyEvent;
    FOnChanging:TNotifyEvent;
    FSortByObject:boolean;
    procedure ExchangeItems(Index1, Index2: Integer);
    procedure Grow;
    procedure QuickSort(L, R: Integer);
    procedure QuickSortByObject(L, R: Integer);
    procedure InsertItem(Index: Integer; const S: WideString);
  protected
    procedure Changed;  virtual;
    procedure Changing;  virtual;
    function  CompareStrings(const S1, S2: WideString): Integer;  override;
    function  Get(Index: Integer): WideString;  override;
    function  GetCapacity: Integer;  override;
    function  GetCount: Integer;  override;
    function  GetObject(Index: Integer): TObject;  override;
    procedure Put(Index: Integer; const S: WideString);  override;
    procedure PutObject(Index: Integer; AObject: TObject);  override;
    procedure SetCapacity(NewCapacity: Integer);  override;
    procedure SetUpdateState(Updating: Boolean);  override;
  public
    destructor Destroy;  override;
    procedure Delete(Index: Integer);  override;
    procedure Exchange(Index1, Index2: Integer);  override;
    function  Find(const S: WideString; var Index: Integer): Boolean;  virtual;
    function  FindByObject(const S: TObject; var Index: Integer): Boolean;  virtual;
    function  IndexOf(const S: WideString): Integer;  override;
    procedure Insert(Index: Integer; const S: WideString);  override;
    procedure Sort;  override;
    property  Duplicates: TDuplicates read FDuplicates write FDuplicates;
    property  OnChange:TNotifyEvent read FOnChange write FOnChange;
    property  OnChanging:TNotifyEvent read FOnChanging write FOnChanging;
    function  Add(const S: WideString): Integer;  override;
    procedure Clear;  override;
    property  SortByObject: boolean read FSortByObject write FSortByObject  default false;
  published
  end;

TD2StorageItem = class(TPersistent)
  private
    FStream   : TMemoryStream;
    FType     : TValueType;
    FAsString : WideString;
    FAsFloat  :single;
    FAsInteger :integer;
    function  GetAsStream: TMemoryStream;
    function  GetAsBool  :boolean;
    procedure SetAsString(const Value:WideString);
    procedure SetAsFloat(const Value:single);
    procedure SetAsInteger(const Value:integer);
    procedure SetAsBool(const Value:boolean);
  protected
    procedure ReadItem(R:TReader);
    procedure WriteItem(W:TWriter);
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    property AsStream : TMemoryStream read GetAsStream;
    property AsString : WideString read FAsString write SetAsString;
    property AsFloat  :single read FAsFloat write SetAsFloat;
    property AsInteger: integer read FAsInteger write SetAsInteger;
    property AsBool   : boolean read GetAsBool write SetAsBool;
  end;

  TD2Storage = class(TPersistent)
  private
    FItems: TD2WideStrings;
    function GetValues(Name: string): TD2StorageItem;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure LoadFromStream(S:TStream);
    procedure SaveToStream(S:TStream);
    procedure LoadFromFile(FileName:string);
    procedure SaveToFile(FileName:string);
    property  Values[Name:string]:TD2StorageItem read GetValues;
  end;

  TD2SplineVector = array [0..3] of single;
  TD2SplineMatrix = array of TD2SplineVector;

TD2Spline = class(TObject)
  private
    len :integer;
    matX, matY : TD2SplineMatrix;
  public
    constructor Create(const Polygon: TD2Polygon);
    destructor  Destroy;  override;
    procedure SplineXY(const t:single; var X,Y:Single);
  end;

TD2Bounds = class(TPersistent)
  private
    FLeft   :single;
    FTop    :single;
    FRight  :single;
    FBottom :single;
    FOnChange    :TNotifyEvent;
    FDefaultValue:TD2Rect;
    procedure SetLeft(const Value:single);
    procedure SetTop(const Value:single);
    procedure SetRight(const Value:single);
    procedure SetBottom(const Value:single);
    function  GetRect: TD2Rect;
    procedure SetRect(const Value:TD2Rect);
  protected
    procedure DefineProperties(Filer:TFiler);  override;
    procedure ReadRect(Reader:TReader);
    procedure WriteRect(Writer:TWriter);
  public
    constructor Create(const ADefaultValue:TD2Rect);  virtual;
    procedure Assign(Source: TPersistent);  override;
    function Width :single;
    function Height:single;
    property Rect: TD2Rect read GetRect write SetRect;
    function MarginRect(const R:TD2Rect): TD2Rect;
    function PaddinRect(const R:TD2Rect): TD2Rect;
    function Empty:boolean;
    function MarginEmpty:boolean;
    property DefaultValue:TD2Rect read FDefaultValue write FDefaultValue;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  published
    property Left :single read FLeft write SetLeft stored false;
    property Top  :single read FTop  write SetTop  stored false;
    property Right:single read FRight write SetRight stored false;
    property Bottom:single read FBottom write SetBottom stored false;
  end;


TD2PathData = class(TPersistent)
  private
    FOnChanged:TNotifyEvent;
    FResource: TD2PathObject;
    FResourceName: string;
    FStartPoint: TD2Point;
    function  GetPathString: Ansistring;
    procedure SetPathString(const Value:Ansistring);
    procedure AddArcSd2Part(const Center, Radius: TD2Point; StartAngle, SweepAngle:single);
    procedure AddArcSd2(const P1, Radius: TD2Point; Angle:single; const LargeFlag, SweepFlag:boolean; const P2: TD2Point);
    procedure SetResource(const Value:TD2PathObject);
    function  GetResourceName: string;
    procedure SetResourceName(const Value:string);
    function  GetPath: TD2PathData;
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadPath(Stream: TStream);
    procedure WritePath(Stream: TStream);
  public
    PathData: array of TD2PathPoint;
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property  OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    function  LastPoint: TD2Point;
    procedure MoveTo(const P: TD2Point);
    procedure MoveToRel(const P: TD2Point);
    procedure LineTo(const P: TD2Point);
    procedure LineToRel(const P: TD2Point);
    procedure HLineTo(const x:single);
    procedure HLineToRel(const x:single);
    procedure VLineTo(const y:single);
    procedure VLineToRel(const y:single);
    procedure CurveTo(const ControlPoint1, ControlPoint2, EndPoint: TD2Point);
    procedure CurveToRel(const ControlPoint1, ControlPoint2, EndPoint: TD2Point);
    procedure SmoothCurveTo(const ControlPoint2, EndPoint: TD2Point);
    procedure SmoothCurveToRel(const ControlPoint2, EndPoint: TD2Point);
    procedure ClosePath;
    procedure AddEllipse(const ARect: TD2Rect);
    procedure AddRectangle(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners;
                           const ACornerType: TD2CornerType = d2CornerRound);
    procedure AddArc(const Center, Radius: TD2Point; StartAngle, SweepAngle:single);
    procedure Clear;
    procedure Flatten(const Flatness:single = 0.25);
    procedure Scale(const scaleX, scaleY:single);
    procedure Offset(const dX, dY:single);
    procedure ApplyMatrix(const M: TD2Matrix);
    function  GetBounds: TD2Rect;
    function  FlattenToPolygon(var Polygon: TD2Polygon; const Flatness:single = 0.25): TD2Point;
    function  IsEmpty:boolean;
    property  ResourcePath: TD2PathData read GetPath;
  published
    property Data: AnsiString read GetPathString write SetPathString stored false;
    property Resource: TD2PathObject read FResource write SetResource stored false;
    property ResourceName: string read GetResourceName write SetResourceName;
  end;


TD2Filter = class(TPersistent)
  published
    class function GetFileTypes: string;  virtual;
    class function GetImageSize(const AFileName: string): TD2Point;  virtual;
    function LoadFromFile(const AFileName: string; const Rotate:single; var Bitmap: TD2Bitmap):boolean;  virtual; abstract;
    function LoadThumbnailFromFile(const AFileName: string; const AFitWidth, AFitHeight:single; const UseEmbedded:boolean;
                                   var Bitmap: TD2Bitmap):boolean;  virtual; abstract;
    function SaveToFile(const AFileName: string; var Bitmap: TD2Bitmap; const Params: string = ''):boolean;  virtual; abstract;
    function LoadFromStream(const AStream: TStream; var Bitmap: TD2Bitmap):boolean;  virtual; abstract;
    function SaveToStream(const AStream: TStream; var Bitmap: TD2Bitmap; const Format: string;
                          const Params: string = ''):boolean;  virtual; abstract;
  end;
  TD2FilterClass = class of TD2Filter;


TD2Bitmap = class(TPersistent)
  private
    FBits: PD2ColorArray;
    FHandle: THandle;
    FHeight:integer;
    FOnChange:TNotifyEvent;
    FWidth:integer;
    FNeedUpdate:boolean;
    FOnThreadLoaded:TNotifyEvent;
    FOnBitmapCreate:TNotifyEvent;
    FOnBitmapDestroy:TNotifyEvent;
    FResource: TD2BitmapObject;
    FResourceName: string;
    function GetCanvas: TD2Canvas;
    function GetScanline(y: integer): PD2ColorArray;
    procedure SetHeight(const Value:integer);
    procedure SetWidth(const Value:integer);
    function GetPixels(x, y: integer): TD2Color;
    procedure SetResource(const Value:TD2BitmapObject);
    function GetResourceName: string;
    procedure SetResourceName(const Value:string);
    function GetBitmap: TD2Bitmap;
  protected
    FCanvas: TD2Canvas;
    FOnDestroyHandle:TNotifyEvent;
    procedure Recreate;
    procedure DoLoaded(Sender: TObject);
    procedure AssignTo(Dest: TPersistent);  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadBitmap(Stream: TStream);
    procedure WriteBitmap(Stream: TStream);
  public
    constructor Create(const AWidth, AHeight:integer; const APremulAlpha: boolean = true);  virtual;
    constructor CreateFromStream(const AStream: TStream);  virtual;
    constructor CreateFromBitmapAndMask(const Bitmap, Mask: TD2Bitmap);
    procedure Assign(Source: TPersistent);  override;
    destructor Destroy;  override;
    procedure SetSize(const AWidth, AHeight: integer);
    procedure Clear(const AColor: TD2Color = 0);  virtual;
    procedure ClearRect(const ARect: TD2Rect; const AColor: TD2Color = 0);  virtual;
    procedure BitmapChanged;
    function  IsEmpty:boolean;
    procedure DrawGraphic(const Graphic: TGraphic; const DstRect: TD2Rect);
    procedure Rotate(const Angle:single);
    procedure FlipHorizontal;
    procedure FlipVertical;
    procedure InvertAlpha;
    procedure FillColor(const Color: TD2Color);
    function  CreateMask: PByteArray;
    procedure ApplyMask(const Mask: PByteArray; const DstX: integer = 0; const DstY: integer = 0);
    function  CreateThumbnail(const Width, Height: integer): TD2Bitmap;
    procedure LoadFromFile(const AFileName: string; const Rotate:single = 0);
    procedure LoadThumbnailFromFile(const AFileName: string; const AFitWidth, AFitHeight:single; const UseEmbedded: boolean = true);
    procedure SaveToFile(const AFileName: string; const Params: string = '');
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToStream(Stream: TStream);
    property Canvas: TD2Canvas read GetCanvas;
    property Pixels[x, y: integer]: TD2Color read GetPixels;
    property Scanline[y: integer]: PD2ColorArray read GetScanline;
    property StartLine: PD2ColorArray read FBits;
    property Width: integer read FWidth write SetWidth;
    property Height: integer read FHeight write SetHeight;
    property Handle: THandle read FHandle write FHandle;
    property OnDestroyHandle:TNotifyEvent read FOnDestroyHandle write FOnDestroyHandle;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnThreadLoaded:TNotifyEvent read FOnThreadLoaded write FOnThreadLoaded;
    property OnBitmapCreate:TNotifyEvent read FOnBitmapCreate write FOnBitmapCreate;
    property OnBitmapDestroy:TNotifyEvent read FOnBitmapDestroy write FOnBitmapDestroy;
    property NeedUpdate: boolean read FNeedUpdate write FNeedUpdate;
    property ResourceBitmap: TD2Bitmap read GetBitmap;
  published
    property Resource: TD2BitmapObject read FResource write SetResource stored false;
    property ResourceName: string read GetResourceName write SetResourceName;
  end;

TD2Canvas = class(TPersistent)
  private
  protected
    FWidth, FHeight:integer;
    FMatrix: TD2Matrix;
    FFill: TD2Brush;
    FStroke: TD2Brush;
    FStrokeThickness:single;
    FStrokeCap: TD2StrokeCap;
    FStrokeJoin: TD2StrokeJoin;
    FStrokeDash: TD2StrokeDash;
    FDash: array of single;
    FDashOffset:single;
    FFont: TD2Font;
    FBitmap: TD2Bitmap;
    FResized:boolean;
    FSaveData: TD2SaveDataArray;
    FBuffered:boolean;
    FBufferBits: Pointer;
    FHandle: THandle;
    FParent: THandle;
    FScene: Id2Scene;
    procedure FontChanged(Sender: TObject);  virtual;
    procedure SetStrokeDash(const Value:TD2StrokeDash);
    procedure AssignTo(Dest: TPersistent);  override;
  public
    constructor Create(const AWidth, AHeight: integer);  virtual;
    constructor CreateFromBitmap(const ABitmap: TD2Bitmap);  virtual;
    destructor Destroy;  override;
    function  BeginScene:boolean;  virtual;
    procedure EndScene;  virtual;
    procedure FlushBuffer(const X, Y:integer; const DC: Cardinal);  virtual; abstract;
    procedure FlushBufferRect(const X, Y:integer; const DC: Cardinal; const ARect: TD2Rect);  virtual; abstract;
    procedure FreeBuffer;  virtual; abstract;
    procedure ResizeBuffer(const AWidth, AHeight: integer);  virtual; abstract;
    procedure Clear(const Color: cardinal);  virtual; abstract;
    procedure ClearRect(const ARect: TD2Rect; const AColor: TD2Color = 0);  virtual; abstract;
    class function GetBitmapScanline(Bitmap: TD2Bitmap; y: integer): PD2ColorArray;  virtual;
    procedure SaveToStream(S: TStream);
    procedure SaveToBits(Bits: Pointer);
    procedure SetMatrix(const M: TD2Matrix);  virtual;
    procedure MultyMatrix(const M: TD2Matrix);  virtual;
    function  SaveCanvas: cardinal;  virtual; abstract;
    procedure RestoreCanvas(const AState: cardinal);  virtual; abstract;
    procedure SetClipRects(const ARects: array of TD2Rect);  virtual; abstract;
    procedure IntersectClipRect(const ARect: TD2Rect);  virtual; abstract;
    procedure ExcludeClipRect(const ARect: TD2Rect);  virtual; abstract;
    procedure ResetClipRect;  virtual; abstract;
    procedure DrawLine(const APt1, APt2: TD2Point; const AOpacity:single);  virtual; abstract;
    procedure FillRect(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners; const AOpacity:single;
                      const ACornerType: TD2CornerType = d2CornerRound);  virtual; abstract;
    procedure DrawRect(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners; const AOpacity:single;
                       const ACornerType: TD2CornerType = d2CornerRound);  virtual; abstract;
    procedure FillEllipse(const ARect: TD2Rect; const AOpacity:single);  virtual; abstract;
    procedure DrawEllipse(const ARect: TD2Rect; const AOpacity:single);  virtual; abstract;
    procedure FillArc(const Center, Radius: TD2Point; StartAngle, SweepAngle:single; const AOpacity:single);
    procedure DrawArc(const Center, Radius: TD2Point; StartAngle, SweepAngle:single; const AOpacity:single);
    function  PtInPath(const APoint: TD2Point; const ARect: TD2Rect; const APath: TD2PathData):boolean;  virtual; abstract;
    procedure FillPath(const APath: TD2PathData; const ARect: TD2Rect; const AOpacity:single);  virtual; abstract;
    procedure DrawPath(const APath: TD2PathData; const ARect: TD2Rect; const AOpacity:single);  virtual; abstract;
    procedure DrawBitmap(const ABitmap: TD2Bitmap; const SrcRect, DstRect: TD2Rect; const AOpacity:single;
                         const HighSpeed: boolean = false);  virtual; abstract;
    procedure DrawThumbnail(const ABitmap: TD2Bitmap; const Width, Height:single);  virtual; abstract;
    procedure DrawRectSides(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners; const AOpacity:single;
                            const ASides: TD2Sides;
                            const ACornerType: TD2CornerType = d2CornerRound);
    procedure FillPolygon(const Points: TD2Polygon; const AOpacity:single);  virtual;
    procedure DrawPolygon(const Points: TD2Polygon; const AOpacity:single);  virtual;
    function  LoadFontFromStream(AStream: TStream):boolean;  virtual;
    procedure FillText(const ARect, AClipRect: TD2Rect; const AText: WideString; const WordWrap:boolean;
                       const AOpacity:single; const ATextAlign: TD2TextAlign; const AVTextAlign: TD2TextAlign = d2TextAlignCenter);  virtual; abstract;
    procedure MeasureText(var ARect: TD2Rect; AClipRect: TD2Rect; const AText: WideString; const WordWrap:boolean; const ATextAlign: TD2TextAlign;
                          const AVTextAlign: TD2TextAlign = d2TextAlignCenter);  virtual; abstract;
    function TextToPath(Path: TD2PathData; const ARect: TD2Rect; const AText: WideString; const WordWrap:boolean; const ATextAlign: TD2TextAlign;
                        const AVTextAlign: TD2TextAlign = d2TextAlignCenter):boolean;  virtual; abstract;
    function TextWidth(const AText: WideString):single;
    function TextHeight(const AText: WideString):single;
    procedure SetCustomDash(Dash: array of single; Offset:single);
    property Stroke: TD2Brush read FStroke;
    property StrokeThickness:single read FStrokeThickness write FStrokeThickness;
    property StrokeCap: TD2StrokeCap read FStrokeCap write FStrokeCap;
    property StrokeDash: TD2StrokeDash read FStrokeDash write SetStrokeDash;
    property StrokeJoin: TD2StrokeJoin read FStrokeJoin write FStrokeJoin;
    property Fill: TD2Brush read FFill;
    property Font: TD2Font read FFont;
    property Matrix: TD2Matrix read FMatrix;
    property Width: integer read FWidth;
    property Height: integer read FHeight;
    property Scene: Id2Scene read FScene write FScene;
    property Handle: THandle read FHandle write FHandle;
    property Parent: THandle read FParent write FParent;
    property Buffered: boolean read FBuffered;
    property BufferBits: Pointer read FBufferBits;
  published
  end;
  TD2CanvasClass = class of TD2Canvas;

TD2Object = class(TComponent)
  private
    FStored:boolean;
    FResourceName: string;
    FNotifyList: TList;
    FTagObject: TObject;
    FTagFloat:single;
    FTagString: string;
    FBindingName: string;
    FIndex:integer;
    function  GetScene: Id2Scene;
    procedure ReaderSetName(Reader:TReader; Component: TComponent; var Name: string);
    procedure ReaderError(Reader:TReader; const Message: string; var Handled: Boolean);
    procedure SetResourceName(const Value:string);
    procedure SetStored(const Value:boolean);
    function  GetChild(Index: integer): TD2Object;
    function  GetChildrenCount:integer;
    procedure SetBindingName(const Value:string);
    function  GetIndex:integer;
    procedure SetIndex(Idx: integer);
  protected
    FIsVisual:boolean;
    FVisual: TD2VisualObject;
    FChildren: TList;
    FParent: TD2Object;
    FScene: Id2Scene;
    procedure SetNewScene(AScene: Id2Scene);  virtual;
    procedure DoReleaseTimer(Sender: TObject);
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure ChangeParent;  virtual;
    procedure SetParent(const Value:TD2Object);  virtual;
    function  HasClipParent: TD2VisualObject;
    function  GetBinding(Index: string): Variant;  virtual;
    procedure SetBinding(Index: string; const Value:Variant);  virtual;
    function  GetData: Variant;  virtual;
    procedure SetData(const Value:Variant);  virtual;
    procedure IntLoadFromBinStream(const AStream: TStream);
    procedure IntSaveToBinStream(const AStream: TStream);
    procedure DoAniFinished(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure SetParentComponent(Value:TComponent);  override;
    function  GetParentComponent: TComponent;  override;
    function  HasParent: Boolean;  override;
    procedure Release(Delay:single = 0.1);
    function  ItemClass: string;  virtual;
    function  Clone(const AOwner: TComponent): TD2Object;
    procedure CloneChildFromStream(AStream: TStream);
    procedure AddObject(AObject: TD2Object);  virtual;
    procedure RemoveObject(AObject: TD2Object);  virtual;
    procedure Exchange(AObject1, AObject2: TD2Object);  virtual;
    procedure DeleteChildren;  virtual;
    procedure BringToFront;
    procedure SendToBack;
    procedure AddObjectsToList(const AList: TList);
    procedure AddControlsToList(const AList: TList);
    procedure Sort(Compare: TD2ObjectSortCompare);
    procedure AddFreeNotify(const AObject: TObject);
    procedure RemoveFreeNotify(const AObject: TObject);
    procedure LoadFromStream(const AStream: TStream);
    procedure SaveToStream(const Stream: TStream);
    procedure LoadFromBinStream(const AStream: TStream);
    procedure SaveToBinStream(const AStream: TStream);
    function  FindResource(const AResource: string): TD2Object;  virtual;
    procedure UpdateResource;  virtual;
    procedure StartAnimation(const AName: WideString);  virtual;
    procedure StopAnimation(const AName: WideString);  virtual;
    procedure StartTriggerAnimation(AInstance: TD2Object; ATrigger: string);  virtual;
    procedure StartTriggerAnimationWait(AInstance: TD2Object; ATrigger: string);  virtual;
    procedure StopTriggerAnimation(AInstance: TD2Object);  virtual;
    procedure ApplyTriggerEffect(AInstance: TD2Object; ATrigger: string);  virtual;
    procedure AnimateFloat(const APropertyName: string; const NewValue:single; Duration:single = 0.2;
                           AType: TD2AnimationType = d2AnimationIn; AInterpolation: TD2InterpolationType = d2InterpolationLinear);
    procedure AnimateColor(const APropertyName: string; const NewValue:string; Duration:single = 0.2;
                           AType: TD2AnimationType = d2AnimationIn; AInterpolation: TD2InterpolationType = d2InterpolationLinear);
    procedure AnimateFloatDelay(const APropertyName: string; const NewValue:single; Duration:single = 0.2;
                           Delay:single = 0.0; AType: TD2AnimationType = d2AnimationIn; AInterpolation: TD2InterpolationType = d2InterpolationLinear);
    procedure AnimateFloatWait(const APropertyName: string; const NewValue:single; Duration:single = 0.2;
                           AType: TD2AnimationType = d2AnimationIn; AInterpolation: TD2InterpolationType = d2InterpolationLinear);
    property IsVisual: boolean read FIsVisual;
    property Visual: TD2VisualObject read FVisual;
    property Scene: Id2Scene read FScene;
    property Stored: boolean read FStored write SetStored;
    property TagObject: TObject read FTagObject write FTagObject;
    property TagFloat:single read FTagFloat write FTagFloat;
    property TagString: string read FTagString write FTagString;
    property ChildrenCount: integer read GetChildrenCount;
    property Children[Index: integer]: TD2Object read GetChild;
    function FindBinding(const ABinding: string): TD2Object;
    property Data: Variant read GetData write SetData;
    property Binding[Index: string]: Variant read GetBinding write SetBinding;
  published
    property Index: integer read GetIndex write SetIndex stored false;
    property Parent: TD2Object read FParent write SetParent stored false;
    property BindingName: string read FBindingName write SetBindingName;
    property ResourceName: string read FResourceName write SetResourceName;
  end;

  TD2Animation = class(TD2Object)
  private
    FDuration:single;
    FDelay, FDelayTime:single;
    FTime:single;
    FInverse:boolean;
    FTrigger, FTriggerInverse: string;
    FLoop:boolean;
    FPause:boolean;
    FRunning:boolean;
    FOnFinish:TNotifyEvent;
    FOnProcess:TNotifyEvent;
    FHideOnFinish:boolean;
    FInterpolation: TD2InterpolationType;
    FAnimationType: TD2AnimationType;
    FEnabled:boolean;
    FAutoReverse:boolean;
    procedure SetEnabled(const Value:boolean);
  protected
    function NormalizedTime:single;
    procedure ProcessAnimation;  virtual;
    procedure Loaded;  override;
    procedure Finish;
    procedure DoFinish;  virtual;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  virtual;
    procedure Stop;  virtual;
    procedure StopAtCurrent;  virtual;
    procedure StartTrigger(AInstance: TD2Object; ATrigger: string);  virtual;
    procedure ProcessTick(time, deltaTime:single);
    property  Running: boolean read FRunning;
    property  Pause: boolean read FPause write FPause;
  published
    property AnimationType: TD2AnimationType read FAnimationType write FAnimationType  default d2AnimationIn;
    property AutoReverse: boolean read FAutoReverse write FAutoReverse  default false;
    property Enabled: boolean read FEnabled write SetEnabled  default false;
    property Delay:single read FDelay write FDelay;
    property Duration:single read FDuration write FDuration;
    property Interpolation: TD2InterpolationType read FInterpolation write FInterpolation  default d2InterpolationLinear;
    property Inverse: boolean read FInverse write FInverse  default false;
    property HideOnFinish: boolean read FHideOnFinish write FHideOnFinish  default false;
    property Loop: boolean read FLoop write FLoop  default false;
    property Trigger: string read FTrigger write FTrigger;
    property TriggerInverse: string read FTriggerInverse write FTriggerInverse;
    property OnProcess:TNotifyEvent read FOnProcess write FOnProcess;
    property OnFinish:TNotifyEvent read FOnFinish write FOnFinish;
  end;

TD2Effect = class(TD2Object)
  private
    FEnabled:boolean;
    FTrigger: string;
    procedure SetEnabled(const Value:boolean);
  protected
    DisablePaint:boolean;
    AfterPaint:boolean;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetRect(const ARect: TD2Rect): TD2Rect;  virtual;
    function  GetOffset: TD2Point;  virtual;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  virtual;
    procedure ApplyTrigger(AInstance: TD2Object; ATrigger: string);  virtual;
    procedure UpdateParentEffects;
    property  GetDisablePaint: boolean read DisablePaint;
  published
    property Trigger: string read FTrigger write FTrigger;
    property Enabled: boolean read FEnabled write SetEnabled  default true;
  end;


  TD2DragEnterEvent = procedure(Sender: TObject; const Data: TD2DragObject; const Point: TD2Point) of object;
  TD2DragOverEvent = procedure(Sender: TObject; const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean) of object;
  TD2DragDropEvent = procedure(Sender: TObject; const Data: TD2DragObject; const Point: TD2Point) of object;

  TD2CanFocusedEvent = procedure(Sender: TObject; var ACanFocused: boolean) of object;


  TD2Popup = class;

TD2VisualObject = class(TD2Object)
  private
    FOnMouseUp: TD2MouseEvent;
    FOnMouseDown: TD2MouseEvent;
    FOnMouseMove: TD2MouseMoveEvent;
    FOnMouseWheel: TD2MouseWheelEvent;
    FOnClick:TNotifyEvent;
    FOnDblClick:TNotifyEvent;
    FMouseInObject:boolean;
    FHitTest:boolean;
    FClipChildren:boolean;
	FDragClipChildren:boolean; //Added by GoldenFox
    FAutoCapture:boolean;
    FMargins: TD2Bounds;
    FAlign: TD2Align;
    FDisableDefaultAlign:boolean;
    FPadding: TD2Bounds;
    FTempCanvas: TD2Canvas;
    FRotateAngle:single;
    FPosition: TD2Position;
    FScale: TD2Position;
    FSkew: TD2Position;
    FRotateCenter: TD2Position;
    FCanFocused:boolean;
    FIsMouseOver:boolean;
	FIsMouseOverChildren:boolean; //Added by GoldenFox
    FIsFocused:boolean;
    FOnCanFocused: TD2CanFocusedEvent;
    FOnEnterFocus:TNotifyEvent;
    FOnKillFocus:TNotifyEvent;
    FDisableFocusEffect:boolean;
    FClipParent:boolean;
    //FVelocity: TD2Position;
    FOnMouseLeave:TNotifyEvent;
    FOnMouseLeaveChildren:TNotifyEvent; //Added by GoldenFox
    FOnMouseEnter:TNotifyEvent;
    FOnMouseEnterChildren:TNotifyEvent; //Added by GoldenFox
    FMouseChildren: TObject;			//Added by GoldenFox
    FDesignHide:boolean;
    FOnPaint: TOnPaintEvent;
    FOnBeforePaint: TOnPaintEvent;
    FCanClipped:boolean;
    FCursor: TCursor;
    FDragMode: TD2DragMode;
    FDragDisableHighlight:boolean;
    FOnDragEnter: TD2DragEnterEvent;
    FOnDragDrop: TD2DragDropEvent;
    FOnDragLeave:TNotifyEvent;
    FOnDragOver: TD2DragOverEvent;
    FOnDragEnd:TNotifyEvent;
    FIsDragOver:boolean;
    FOnKeyDown: TD2KeyEvent;
    FOnKeyUp: TD2KeyEvent;
    FHint: WideString;
    FShowHint:boolean;
    FPopupMenu: TPopupMenu;
    FPopup: TD2Popup;
    FRecalcEnabled, FEnabled, FAbsoluteEnabled:boolean;
    FTabOrder: TTabOrder;
    FTabList: TList;
    FNeedAlign:boolean;
    FOnApplyResource:TNotifyEvent;
    procedure CreateCaret;
    procedure SetEnabled(const Value:boolean);
    function  GetInvertAbsoluteMatrix: TD2Matrix;
    procedure SetRotateAngle(const Value:single);
    procedure SetPosition(const Value:TD2Position);
    procedure SetHitTest(const Value:boolean);
    procedure SetClipChildren(const Value:boolean);
    function  CheckHitTest(const AHitTest: boolean):boolean;
    function  GetCanvas: TD2Canvas;
    procedure SetLocked(const Value:boolean);
    procedure SetTempCanvas(const Value:TD2Canvas);
    procedure SetOpacity(const Value:single);
    procedure SetDesignHide(const Value:boolean);
    procedure SetTabOrder(const Value:TTabOrder);
    procedure UpdateDesignHide(const Value:boolean);
    function  isOpacityStored: Boolean;
    function  GetChildrenRect: TD2Rect;
    procedure SetCursor(const Value:TCursor);
    function  GetAbsoluteWidth:single;
    function  GetAbsoluteHeight:single;
    function  GetTabOrder: TTabOrder;
    procedure UpdateTabOrder(Value:TTabOrder);
  protected
    FHeight, FLastHeight:single;
    FWidth, FLastWidth:single;
    FVisible:boolean;
    FLocalMatrix: TD2Matrix;
    FAbsoluteMatrix: TD2Matrix;
    FRecalcAbsolute:boolean;
    FUpdateEffects:boolean;
    FEffectBitmap: TD2Bitmap;
    FLocked:boolean;
    FOpacity, FAbsoluteOpacity:single;
    FRecalcOpacity:boolean;
    FInPaintTo:boolean;
    FUpdateRect: TD2Rect;
    FRecalcUpdateRect:boolean;
    FCaret: TD2VisualObject;
    FPressed, FDoubleClick:boolean;
    FUpdating:integer;
    FDisableAlign:boolean;
    FDisableEffect:boolean;
    FHasEffect:boolean;
    function  GetHint:String;  virtual;
    procedure SetHint(const Value:String);  virtual;
    function  HasDisablePaintEffect:boolean;
    function  HasAfterPaintEffect:boolean;
    procedure SetInPaintTo(value: boolean);
    procedure Loaded;  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure SetVisible(const Value:boolean);  virtual;
    procedure SetHeight(const Value:single);  virtual;
    procedure SetWidth(const Value:single);  virtual;
    procedure SetAlign(const Value:TD2Align);  virtual;
    function  GetAbsoluteRect: TD2Rect;  virtual;
    function  GetAbsoluteMatrix: TD2Matrix;  virtual;
    function  GetChildrenMatrix: TD2Matrix;  virtual;
    function  GetAbsoluteScale: TD2Point;  virtual;
    function  GetLocalRect: TD2Rect;  virtual;
    function  GetUpdateRect: TD2Rect;  virtual;
    function  GetBoundsRect: TD2Rect;  virtual;
    function  GetParentedRect: TD2Rect;  virtual;
    function  GetClipRect: TD2Rect;  virtual;
    function  GetEffectsRect: TD2Rect;  virtual;
    function  GetAbsoluteEnabled:boolean;  virtual;
    procedure SetBoundsRect(const Value:TD2Rect);  virtual;
    procedure RecalcOpacity;  virtual;
    procedure RecalcAbsolute;  virtual;
    procedure RecalcAbsoluteNow;
    procedure RecalcUpdateRect;
    procedure RecalcNeedAlign;
    procedure RecalcEnabled;
    procedure RecalcHasEffect;
    procedure FixupTabList;
    function  GetAbsoluteOpacity:single;  virtual;
    procedure DesignSelect;  virtual;
    procedure DesignClick;  virtual;
    procedure DesignInsert;  virtual;
    procedure BeginAutoDrag;  virtual;
    procedure Capture;
    procedure ReleaseCapture;
    procedure Click;  virtual;
    procedure DblClick;  virtual;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  virtual;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  virtual;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  virtual;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  virtual;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  virtual;
    procedure KeyUp(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  virtual;
    procedure DialogKey(var Key: Word; Shift: TShiftState);  virtual;
    procedure MouseEnter;  virtual;
    procedure MouseEnterChildren;  virtual;	//Added by GoldenFox
    procedure MouseLeave;  virtual;
    procedure MouseLeaveChildren;  virtual;	//Added by GoldenFox
    procedure SetMouseOverChildren(Sender: TObject; Value:Boolean);  virtual;	//Added by GoldenFox
    procedure ContextMenu(const ScreenPosition: TD2Point);  virtual;
    procedure DragEnter(const Data: TD2DragObject; const Point: TD2Point);  virtual;
    procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);  virtual;
    procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  virtual;
    procedure DragLeave;  virtual;
    procedure DragEnd;  virtual;
    function  EnterFocusChildren(AObject: TD2VisualObject):boolean;  virtual;
    procedure EnterFocus;  virtual;
    procedure KillFocus;  virtual;
    procedure SetNewScene(AScene: Id2Scene);  override;
    procedure ApplyResource;  virtual;
    procedure BeforePaint;  virtual;
    procedure Paint;  virtual;
    procedure AfterPaint;  virtual;
    procedure PaintChildren;  virtual;
    procedure MarginsChanged(Sender: TObject);  virtual;
    procedure PaddingChanged(Sender: TObject);  virtual;
    procedure MatrixChanged(Sender: TObject);  virtual;
    property  MouseInObject: boolean read FMouseInObject write FMouseInObject;
    property  TempCanvas: TD2Canvas read FTempCanvas write SetTempCanvas;
    property  Skew: TD2Position read FSkew write FSkew;
  public
    DisableDesignResize:boolean;
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure DeleteChildren;  override;
    function  AbsoluteToLocal(P: TD2Point): TD2Point;  virtual;
    function  LocalToAbsolute(P: TD2Point): TD2Point;  virtual;
    function  AbsoluteToLocalVector(P: TD2Vector): TD2Vector;  virtual;
    function  LocalToAbsoluteVector(P: TD2Vector): TD2Vector;  virtual;
    function  ObjectByPoint(X, Y:single): TD2VisualObject;  virtual;
    function  PointInObject(X, Y:single):boolean;  virtual;
    procedure SetBounds(X, Y, AWidth, AHeight:single);  virtual;
    procedure SetSizeWithoutChange(AWidth, AHeight:single);
    function  CheckParentVisible:boolean;  virtual;
    function  MakeScreenshot: TD2Bitmap;
    function  FindTarget(const APoint: TD2Point; const Data: TD2DragObject): TD2VisualObject;
    procedure ShowCaretProc;
    procedure SetCaretPos(const APoint: TD2Point);
    procedure SetCaretSize(const ASize: TD2Point);
    procedure SetCaretColor(const AColor: TD2Color);
    procedure HideCaret;
    procedure BeginUpdate;  virtual;
    procedure EndUpdate;  virtual;
    procedure Realign;  virtual;
    procedure UpdateEffects;
    procedure GetTabOrderList(List: TList; Children: boolean);
    procedure SetFocus;
    procedure PaintTo(const ACanvas: TD2Canvas; const ARect: TD2Rect; const AParent: TD2Object = nil);
    procedure ApplyEffect;
    procedure Repaint;
    procedure InvalidateRect(ARect: TD2Rect);
    procedure Lock;
    property HintW: WideString read fHint write fHint;
    property AbsoluteMatrix: TD2Matrix read GetAbsoluteMatrix;
    property AbsoluteOpacity:single read GetAbsoluteOpacity;
    property AbsoluteWidth:single read GetAbsoluteWidth;
    property AbsoluteHeight:single read GetAbsoluteHeight;
    property AbsoluteScale: TD2Point read GetAbsoluteScale;
    property AbsoluteEnabled: boolean read GetAbsoluteEnabled;
    property InvertAbsoluteMatrix: TD2Matrix read GetInvertAbsoluteMatrix;
    property LocalRect: TD2Rect read GetLocalRect;
    property AbsoluteRect: TD2Rect read GetAbsoluteRect;
    property UpdateRect: TD2Rect read GetUpdateRect;
    property BoundsRect: TD2Rect read GetBoundsRect write SetBoundsRect;
    property ParentedRect: TD2Rect read GetParentedRect;
    property ClipRect: TD2Rect read GetClipRect;
    property Canvas: TD2Canvas read GetCanvas;
    property AutoCapture: boolean read FAutoCapture write FAutoCapture  default false;
    property CanFocused: boolean read FCanFocused write FCanFocused  default false;
    property DisableFocusEffect: boolean read FDisableFocusEffect write FDisableFocusEffect  default false;
    property DisableDefaultAlign: boolean read FDisableDefaultAlign write FDisableDefaultAlign;
    property TabOrder: TTabOrder read GetTabOrder write SetTabOrder  default -1;
  published
    property IsMouseOver: boolean read FIsMouseOver;
    property IsMouseOverChildren:boolean read FIsMouseOverChildren; //Added by GoldenFox
    property IsDragOver: boolean read FIsDragOver;
    property IsFocused: boolean read FIsFocused;
    property IsVisible: boolean read FVisible;
    property Align: TD2Align read FAlign write SetAlign  default vaNone;
    property Cursor: TCursor read FCursor write SetCursor  default crDefault;
    property DragMode: TD2DragMode read FDragMode write FDragMode  default d2DragManual;
    property DragDisableHighlight: boolean read FDragDisableHighlight write FDragDisableHighlight  default false;
    property Enabled: boolean read FEnabled write SetEnabled  default true;
    property Position: TD2Position read FPosition write SetPosition;
    property RotateAngle:single read FRotateAngle write SetRotateAngle;
    property RotateCenter: TD2Position read FRotateCenter write FRotateCenter;
    property Locked: boolean read FLocked write SetLocked  default false;
    property Width:single read FWidth write SetWidth;
    property Height:single read FHeight write SetHeight;
    property Margins: TD2Bounds read FMargins write FMargins;
    property Padding: TD2Bounds read FPadding write FPadding;
    property Opacity:single read FOpacity write SetOpacity stored isOpacityStored;
    property DragClipChildren:boolean read FDragClipChildren write FDragClipChildren default false; //Added by GoldenFox
    property ClipChildren: boolean read FClipChildren write SetClipChildren  default false;
    property ClipParent: boolean read FClipParent write FClipParent  default false;
    property HitTest: boolean read FHitTest write SetHitTest  default true;
    property Hint: String read GetHint write SetHint;
    property ShowHint: boolean read FShowHint write FShowHint  default false;
    property CanClipped: boolean read FCanClipped write FCanClipped  default true;
    property PopupMenu: TPopupMenu read FPopupMenu write FPopupMenu;
    property Popup: TD2Popup read FPopup write FPopup;
    property Scale: TD2Position read FScale write FScale;
    property Visible: boolean read FVisible write SetVisible  default true;
    property DesignHide: boolean read FDesignHide write SetDesignHide  default false;
    property OnDragEnter: TD2DragEnterEvent read FOnDragEnter write FOnDragEnter;
    property OnDragLeave:TNotifyEvent read FOnDragLeave write FOnDragLeave;
    property OnDragOver: TD2DragOverEvent read FOnDragOver write FOnDragOver;
    property OnDragDrop: TD2DragDropEvent read FOnDragDrop write FOnDragDrop;
    property OnDragEnd:TNotifyEvent read FOnDragEnd write FOnDragEnd;
    property OnKeyDown: TD2KeyEvent read FOnKeyDown write FOnKeyDown;
    property OnKeyUp: TD2KeyEvent read FOnKeyUp write FOnKeyUp;
    property OnClick:TNotifyEvent read FOnClick write FOnClick;
    property OnDblClick:TNotifyEvent read FOnDblClick write FOnDblClick;
    property OnCanFocused: TD2CanFocusedEvent read FOnCanFocused write FOnCanFocused;
    property OnEnterFocus:TNotifyEvent read FOnEnterFocus write FOnEnterFocus;
    property OnKillFocus:TNotifyEvent read FOnKillFocus write FOnKillFocus;
    property OnMouseDown: TD2MouseEvent read FOnMouseDown write FOnMouseDown;
    property OnMouseMove: TD2MouseMoveEvent read FOnMouseMove write FOnMouseMove;
    property OnMouseUp: TD2MouseEvent read FOnMouseUp write FOnMouseUp;
    property OnMouseWheel: TD2MouseWheelEvent read FOnMouseWheel write FOnMouseWheel;
    property OnMouseEnter:TNotifyEvent read FOnMouseEnter write FOnMouseEnter;
    property OnMouseEnterChildren:TNotifyEvent read FOnMouseEnterChildren write FOnMouseEnterChildren; //Added by GoldenFox
    property OnMouseLeave:TNotifyEvent read FOnMouseLeave write FOnMouseLeave;
    property OnMouseLeaveChildren:TNotifyEvent read FOnMouseLeaveChildren write FOnMouseLeaveChildren; //Added by GoldenFox
    property OnBeforePaint: TOnPaintEvent read FOnBeforePaint write FOnBeforePaint;
    property OnPaint: TOnPaintEvent read FOnPaint write FOnPaint;
    property OnApplyResource:TNotifyEvent read FOnApplyResource write FOnApplyResource;
  end;

TD2BrushObject = class(TD2Object)
  private
    FBrush: TD2Brush;
  protected
    procedure SetName(const NewName: TComponentName);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Brush: TD2Brush read FBrush write FBrush;
  end;

TD2PathObject = class(TD2Object)
  private
    FPath: TD2PathData;
  protected
    procedure SetName(const NewName: TComponentName);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Path: TD2PathData read FPath write FPath;
  end;

TD2BitmapObject = class(TD2Object)
  private
    FBitmap: TD2Bitmap;
  protected
    procedure SetName(const NewName: TComponentName);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Bitmap: TD2Bitmap read FBitmap write FBitmap;
  end;

TD2Control = class(TD2VisualObject)
  private
    procedure SetResource(const Value:string);
    procedure SetBindingSource(const Value:TD2Control);
  protected
    FResourceLink: TD2Object;
    FNeedResource:boolean;
    FBindingObjects: TList;
    FBindingSource: TD2Control;
    FAutoTranslate:boolean;
    FActionLink: TD2ControlActionLink;
    FHelpType: THelpType;
    FHelpKeyword: string;
    FHelpContext: THelpContext;
    function  IsHelpContextStored: Boolean;
    procedure SetHelpContext(const Value:THelpContext);
    procedure SetHelpKeyword(const Value:string);
    function  GetAction: TBasicAction;  virtual;
    procedure SetAction(Value:TBasicAction);
    procedure DoActionChange(Sender: TObject);
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean);  virtual;
    procedure InitiateAction;  virtual;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure Loaded;  override;
    procedure ApplyStyle;  virtual;
    procedure FreeStyle;  virtual;
    procedure EnterFocus;  override;
    procedure BeforePaint;  override;
    function  GetResourceObject: TD2VisualObject;
    procedure SetData(const Value:Variant);  override;
    procedure ToBindingObjects;
    procedure AddBindingObject(AObject: TD2Control);
    procedure RemoveBindingObject(AObject: TD2Control);
  public
    FResource: string;
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  FindResource(const AResource: string): TD2Object;  override;
    procedure Paint;  override;
    procedure ApplyResource;  override;
    procedure UpdateResource;  override;
    property  BindingSource: TD2Control read FBindingSource write SetBindingSource;
    property  AutoTranslate: boolean read FAutoTranslate write FAutoTranslate;
    property  Action: TBasicAction read GetAction write SetAction;
  published
    property HelpType: THelpType read FHelpType write FHelpType  default htContext;
    property HelpKeyword: String read FHelpKeyword write SetHelpKeyword stored IsHelpContextStored;
    property HelpContext: THelpContext read FHelpContext write SetHelpContext stored IsHelpContextStored  default 0;
    property Resource: string read FResource write SetResource;
    property TabOrder  default -1;
  end;

TD2Background = class(TD2Control)
  private
    FFill: TD2Brush;
    procedure SetFill(const Value:TD2Brush);
  protected
    procedure BeforePaint;  override;
    procedure PaintChildren;  override;
    procedure FillChanged(Sender: TObject);  virtual;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
  published
    property Resource;
    property Fill: TD2Brush read FFill write SetFill;
  end;

TD2Content = class(TD2VisualObject)
  private
  protected
    FParentAligning:boolean;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function GetParentComponent: TComponent;  override;
    procedure Realign;  override;
    procedure Paint;  override;
  published
  end;

TD2Resources = class(TComponent)
  private
    FResource: TStrings;
    FRoot: TD2Object;
    FSceneList: TList;
    FFileName: string;
    procedure SetResource(const Value:TStrings);
    procedure SetFileName(const Value:string);
    procedure DoResourceChanged(Sender: TObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure Loaded;  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadResources(Stream: TStream);
    procedure WriteResources(Stream: TStream);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure AddSceneUpdater(const Scene: Id2Scene);
    procedure RemoveSceneUpdater(const Scene: Id2Scene);
    procedure FillStrings;
    procedure UpdateScenes;
    property  Root: TD2Object read FRoot write FRoot;
  published
    property Resource: TStrings read FResource write SetResource stored false;
    property FileName: string read FFileName write SetFileName;
  end;

TD2Lang = class(TComponent)
  private
    FLang: string;
    FResources: TD2WideStrings;
    FOriginal: TD2WideStrings;
    FAutoSelect:boolean;
    FFileName: string;
    FStoreInForm:boolean;
    procedure SetLang(const Value:string);
    function  GetLangStr(Index: WideString): TD2WideStrings;
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadResources(Stream: TStream);
    procedure WriteResources(Stream: TStream);
    procedure Loaded;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure AddLang(AName: WideString);
    procedure LoadFromFile(AFileName: string);
    procedure SaveToFile(AFileName: string);
    property Original: TD2WideStrings read FOriginal;
    property Resources: TD2WideStrings read FResources;
    property LangStr[Index: WideString]: TD2WideStrings read GetLangStr;
  published
    property AutoSelect: boolean read FAutoSelect write FAutoSelect  default true;
    property FileName: string read FFileName write FFileName;
    property StoreInForm: boolean read FStoreInForm write FStoreInForm  default true;
    property Lang: string read FLang write SetLang;
  end;

TD2Designer = class(TComponent)
  private
    FScenes: TList;
  protected
    procedure CallDesignSelect(AObject: TObject);
  public
    procedure SelectObject(ADesigner: TComponent; AObject: TD2Object; MultiSelection: array of TD2Object);  virtual; abstract;
    procedure Modified(ADesigner: TComponent);  virtual; abstract;
    function  UniqueName(ADesigner: TComponent; ClassName: string): string;  virtual; abstract;
    function  IsSelected(ADesigner: TComponent; const AObject: TObject):boolean;  virtual; abstract;
    procedure AddScene(const Scene: Id2Scene);  virtual;
    procedure RemoveScene(const Scene: Id2Scene);  virtual;
    procedure EditStyle(const Res: TD2Resources; const ASelected: string);  virtual;
    procedure AddObject(AObject: TD2Object);  virtual;
    procedure DeleteObject(AObject: TD2Object);  virtual;
    function  AddMethod(MethodName: string): TMethod;  virtual;
    function  GetMethodName(Method: TMethod): string;  virtual;
  end;

TD2Frame = class(TD2VisualObject)
  private
    FNeedClone:boolean;
    FSceneObject: TD2CustomScene;
    FBuffer: TD2Bitmap;
    procedure SetSceneObject(const Value:TD2CustomScene);
  protected
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation);  override;
    procedure Loaded;  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property SceneObject: TD2CustomScene read FSceneObject write SetSceneObject;
  end;


TD2CustomScene = class(TCustomControl, Id2Scene {$IFDEF WINDOWS},IDropTarget{$ENDIF})
  private
    {$IFDEF WINDOWS}
    FDC: THandle;
    PrevWndProc: WNDPROC;
    //FWStyle: string;
    {$ENDIF}
    FShift: TShiftState;
    FCanvas: TD2Canvas;
    FDisableUpdate:boolean;
    FChildren: TList;
    FDesignRoot, FSelected, FCaptured, FHovered, FFocused: TD2VisualObject;
    FSelection: array of TD2Object;
    FDesignPlaceObject: TD2VisualObject;
    FDesignGridLines: array of TD2VisualObject;
    FDesignPopup: TPopupMenu;
    FDesignChangeSelection:TNotifyEvent;
    FUnsnapMousePos, FMousePos, FDownPos: TD2Point;
    FMoving, FLeftTop, FRightTop, FLeftBottom, FRightBottom, FTop, FBottom, FLeft, FRight, FRotate:boolean;
    FLeftTopHot, FRightTopHot, FLeftBottomHot, FRightBottomHot, FTopHot, FBottomHot, FLeftHot, FRightHot, FRotateHot:boolean;
    FResizeSize: TPoint;
    FDragging, FResizing:boolean;
    FDesignTime:boolean;
    FFill: TD2Brush;
    FTransparency:boolean;
    FSnapToGrid:boolean;
    FSnapToLines:boolean;
    FSnapGridShow:boolean;
    FSnapGridSize:single;
    FInsertObject: string;
    FAlignRoot:boolean;
    FDesignPopupEnabled:boolean;
    FOpenInFrame: TD2Frame;
    FCloneFrame: TForm;
    FDrawing:boolean;
    FOnFlush:TNotifyEvent;
    FShowTimer: TD2Timer;
    //FDBCSLeadChar: Word;
    FStyle: TD2Resources;
    FShowUpdateRects:boolean;
    FLoadCursor: TCursor;
    VCLDragSource: TCustomControl;
    FActiveControl: TD2Control;
    FAnimatedCaret:boolean;
    procedure SetActiveControl(AControl: TD2Control);
    procedure DoShowTimer(Sender: TObject);
    {$IFDEF WINDOWS}
    function GetDataObject: TD2DragObject;
    function DragEnter(const dataObj: IDataObject; grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    function DragOver(grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    function DragLeave: HResult; stdcall;
    function Drop(const dataObj: IDataObject; grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    procedure WMAddUpdateRect(var Msg: TMessage); message WM_ADDUPDATERECT;
    {$ENDIF}

    procedure WMEraseBkgnd(var Msg: TLMEraseBkgnd); message LM_ERASEBKGND;
    procedure CMMouseLeave(var Message :TLMessage); message CM_MOUSELEAVE;
    procedure WMPaint(var Msg: TLMPaint); message LM_PAINT;
    procedure CMShowingChanged(var Message:  TLMessage ); message CM_SHOWINGCHANGED;
    procedure CMDesignHitTest(var Msg: TLMMouse ); message CM_DESIGNHITTEST;
    procedure CMHintShow(var Message:  TLMessage ); message CM_HINTSHOW;
    function  GetCount:integer;
    procedure SetChildren(Index:integer; const Value:TD2Object);
    function  GetChildrenObject(Index: integer): TD2Object;
    procedure SetFill(const Value:TD2Brush);
    procedure FillChanged(Sender: TObject);
    procedure SetSnapGridShow(const Value:boolean);
    procedure AddUpdateRectsFromGridLines;
    function  SnapToGridValue(Value:single):single;
    procedure SetSnapGridSize(const Value:single);
    procedure SnapToGridLines(AllowChangePosition: boolean);
    function  SnapPointToGridLines(const APoint: TD2Point): TD2Point;
    procedure ReadDesignSnapGridShow(Reader:TReader);
    procedure WriteDesignSnapGridShow(Writer:TWriter);
    procedure ReadDesignSnapToGrid(Reader:TReader);
    procedure WriteDesignSnapToGrid(Writer:TWriter);
    procedure ReadDesignSnapToLines(Reader:TReader);
    procedure WriteDesignSnapToLines(Writer:TWriter);
    procedure doDesignTabOrderBtnClick(Sender: TObject);
    procedure doDesignTabOrderRebuildList(ListBox: TD2VisualObject);
    procedure doDesignPopupTabOrder(Sender: TObject);
    procedure doDesignPopupEditStyle(Sender: TObject);
    procedure doDesignPopupCreateStyle(Sender: TObject);
    procedure doDesignPopupLoadFromFile(Sender: TObject);
    procedure doDesignPopupDesignHide(Sender: TObject);
    procedure doDesignPopupAddItem(Sender: TObject);
    procedure doDesignPopupAdd(Sender: TObject);
    procedure doDesignPopupDel(Sender: TObject);
    procedure doDesignPopupReorder(Sender: TObject);
    procedure doDesignPopupGrid(Sender: TObject);
    procedure doDesignPopupCopy(Sender: TObject);
    function  GetRoot: TD2Object;
    procedure SetFocused(const Value:TD2VisualObject);
    procedure DoDesignSelect(AObject: TObject);
    procedure UpdateLayer;
    procedure SetSelected(const Value:TD2VisualObject);
    procedure doDesignPopupPaste(Sender: TObject);
    procedure SetStyle(const Value:TD2Resources);

    function  GetActiveControl: TD2Control;
    function  GetDisableUpdate:boolean;
    function  GetDesignTime:boolean;
    function  GetCanvas: TD2Canvas;
    function  GetOwner: TComponent;
    function  GetComponent: TComponent;
    function  GetSelected: TD2VisualObject;
    function  GetDesignPlaceObject: TD2VisualObject;
    procedure SetDisableUpdate(Value:boolean);
    function  GetUpdateRectsCount:integer;
    function  GetUpdateRect(const Index: integer): TD2Rect;
    procedure SetCaptured(const Value:TD2VisualObject);
    function  GetCaptured: TD2VisualObject;
    function  GetFocused: TD2VisualObject;
    procedure SetDesignRoot(const Value:TD2VisualObject);
    function  GetMousePos: TD2Point;
    function  GetStyle: TD2Resources;
    function  GetTransparency:boolean;
    procedure SetTransparency(const Value:boolean);
    procedure BeginVCLDrag(Source: TObject; ABitmap: TD2Bitmap);
    procedure EndDragEvent(Sender, Target: TObject; X, Y: Integer);
    function  GetAnimatedCaret:boolean;
    function  LocalToScreen(const Point: TD2Point): TD2Point;
    function  ShowKeyboardForControl(AObject: TD2Object):boolean;
    function  HideKeyboardForControl(AObject: TD2Object):boolean;
  protected
    FUpdateRects: array of TD2Rect;
    procedure CreateHandle;  override;
    procedure CreateWnd;  override;
    procedure DestroyWnd;  override;
    procedure Loaded;  override;
    procedure Resize;  override;
    procedure Draw;  virtual;
    procedure Paint;  override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);  override;
    procedure MouseMove(Shift: TShiftState; X, Y: Integer);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState;  X, Y: Integer);  override;
    procedure DoDragOver(Sender, Source: TObject; X, Y: Integer; State: TDragState; var Accept: Boolean);
    procedure DoDragDrop(Sender, Source: TObject;   X, Y: Integer);

    procedure KeyUp(var Key: Word; Shift: TShiftState);  override;
    procedure KeyDown(var Key: Word; Shift: TShiftState);  override;
    procedure KeyPress(var Key: char);  override;
    procedure UTF8KeyPress(var UTF8Key: TUTF8Char);  override;

    procedure UnicodeKeyUp(var Key: Word; var Char: System.WideChar; Shift: TShiftState);
    procedure UnicodeKeyDown(var Key: Word; var Char: System.WideChar; Shift: TShiftState);
    function  DoMouseWheel(Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint): Boolean;  override;
    function  ObjectByPoint(X, Y:single): TD2VisualObject;
  public
    FPopupPos: TPoint;
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure InitiateAction;  override;
    procedure DeleteChildren;
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer);  override;
    procedure UpdateBuffer;
    procedure UpdateResource;
    property  Canvas: TD2Canvas read FCanvas;
    procedure AddObject(AObject: TD2Object);
    procedure RemoveObject(AObject: TD2Object);
    procedure RealignRoot;
    procedure BeginDrag;
    procedure BeginResize;
    procedure AddUpdateRect(R: TD2Rect);
    procedure OpenDesignPopup;
    procedure InsertObject(const ClassName: string);
    property  DesignTime: boolean read FDesignTime write FDesignTime stored false;
    property  ShowUpdateRects: boolean read FShowUpdateRects write FShowUpdateRects stored false;
    property  Count: integer read GetCount;
    property  Root: TD2Object read GetRoot;
    property  Children[Index: integer]: TD2Object read GetChildrenObject write SetChildren;
    property  Selected: TD2VisualObject read FSelected write SetSelected;
    property  Captured: TD2VisualObject read FCaptured;
    property  Hovered: TD2VisualObject read FHovered;
    property  Focused: TD2VisualObject read FFocused write SetFocused;
    property  DisableUpdate: boolean read FDisableUpdate;
    property  IsDrawing: boolean read FDrawing;
    procedure CreateEmbedded(const AWidth, AHeight:integer; AOnFlush:TNotifyEvent);
    procedure EmbeddedMouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure EmbeddedMouseMove(Shift: TShiftState; X, Y: Integer);
    procedure EmbeddedMouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    function  EmbeddedMouseWheel(Shift: TShiftState; WheelDelta: Integer): Boolean;
    procedure EmbeddedKeyUp(var Key: Word; var Char: System.WideChar; Shift: TShiftState);
    procedure EmbeddedKeyDown(var Key: Word; var Char: System.WideChar; Shift: TShiftState);

    property OnFlush:TNotifyEvent read FOnFlush write FOnFlush;
    property DesignPopupEnabled: boolean read FDesignPopupEnabled write FDesignPopupEnabled;
    property DesignSnapGridShow: boolean read FSnapGridShow write SetSnapGridShow;
    property DesignSnapToGrid: boolean read FSnapToGrid write FSnapToGrid;
    property DesignSnapToLines: boolean read FSnapToLines write FSnapToLines;
    property DesignChangeSelection:TNotifyEvent read FDesignChangeSelection write FDesignChangeSelection;
    property Align;
    property AnimatedCaret: boolean read FAnimatedCaret write FAnimatedCaret  default true;
    property ActiveControl: TD2Control read FActiveControl write SetActiveControl;
    property DesignSnapGridSize:single read FSnapGridSize write SetSnapGridSize;
    property Fill: TD2Brush read FFill write SetFill;
    property Transparency: boolean read FTransparency write SetTransparency  default false;
    property Style: TD2Resources read FStyle write SetStyle;
    property TabStop;
  published
  end;

TD2Scene = class(TD2CustomScene)
  published
    property Align;
    property AnimatedCaret;
    property Fill;
    property Transparency;
    property Style;
    property TabStop;
    property ActiveControl; // must be last
  end;

TD2PopupForm = class(TCustomForm)
  private
    FOwnerForm: TCustomForm;
    FPopup: TD2Popup;
    FNoFree:boolean;
    procedure WMDeactivate(var Msg : TLMActivate); message CM_DEACTIVATE;
  protected
    procedure DoClose(var Action: TCloseAction);  override;
    procedure CreateParams(var Params: TCreateParams);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
  end;

TD2Popup = class(TD2Control)
  private
    FSaveParent: TD2Object;
    FSaveFocused: TD2VisualObject;
    FSaveScale: TD2Point;
    FPopupForm: TCustomForm;
    FPopupScene: TD2Scene;
    FDragTimer: TD2Timer;
    FPopupLayout: TD2VisualObject;
    FIsOpen:boolean;
    FStaysOpen:boolean;
    FPlacement: TD2Placement;
    FPlacementTarget: TD2VisualObject;
    FPlacementRectangle: TD2Bounds;
    FHorizontalOffset:single;
    FVerticalOffset:single;
    FDragWithParent:boolean;
    FAnimating:boolean;
    FStyle: TD2Resources;
    FPlacementScene: TD2PlacementScene;
    FModalResult: TModalResult;
    FModal:boolean;
    FOnClosePopup:TNotifyEvent;
    procedure SetIsOpen(const Value:boolean);
    procedure SetPlacementRectangle(const Value:TD2Bounds);
    procedure SetModalResult(const Value:TModalResult);
    procedure Dotimer(Sender: TObject);
  protected
    procedure ApplyPlacement;  virtual;
    procedure KillFocus;  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DialogKey(var Key: Word; Shift: TShiftState);  override;
    procedure DoFormClose(Sender: TObject; var Action: TCloseAction);
    procedure DoPopupLayoutClick(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    function  PopupModal: TModalResult;  virtual;
    procedure Popup;  virtual;
    procedure ClosePopup;  virtual;
    property ModalResult: TModalResult read FModalResult write SetModalResult;
    property PlacementScene: TD2PlacementScene read FPlacementScene write FPlacementScene  default d2PlacementSceneNew;
  published
    property IsOpen: boolean read FIsOpen write SetIsOpen;
    property HorizontalOffset:single read FHorizontalOffset write FHorizontalOffset;
    property VerticalOffset:single read FVerticalOffset write FVerticalOffset;
    property Placement: TD2Placement read FPlacement write FPlacement  default d2PlacementBottom;
    property PlacementTarget: TD2VisualObject read FPlacementTarget write FPlacementTarget;
    property PlacementRectangle: TD2Bounds read FPlacementRectangle write SetPlacementRectangle;
    property StaysOpen: boolean read FStaysOpen write FStaysOpen  default false;
    property Style: TD2Resources read FStyle write FStyle;
    property DragWithParent: boolean read FDragWithParent write FDragWithParent  default false;
    property OnClosePopup:TNotifyEvent read FOnClosePopup write FOnClosePopup;
    property Resource;
    property Visible  default false;
  end;

TD2PopupItem = class(TD2Control)
  private
    FPopup: TD2Popup;
    FPlacement: TD2Placement;
    FHorizontalOffset:single;
    FVerticalOffset:single;
    procedure SetPlacement(Value:TD2Placement);
    procedure SetHorizontalOffset(Value:single);
    procedure SetVerticalOffset(Value:single);
  protected
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadRect(Reader:TReader);
    procedure WriteRect(Writer:TWriter);
    procedure MouseEnter;  override;
    procedure MouseLeave;  override;
    procedure DesignClick;  override;
    procedure ApplyPlacement;
    procedure DoClosePopup(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure Realign;  override;
  published
    property Resource;
    property Placement: TD2Placement read FPlacement write SetPlacement  default d2PlacementBottom;
    property HorizontalOffset:single read FHorizontalOffset write SetHorizontalOffset;
    property VerticalOffset:single read FVerticalOffset write SetVerticalOffset;
  end;

TD2MessagePopup = class(TD2Popup)
  private
    FDisableScene:boolean;
  protected
    procedure ApplyPlacement;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function PopupModal: TModalResult;  override;
  published
    property Resource;
    property StaysOpen  default true;
    property DragWithParent  default true;
    property DisableScene: boolean read FDisableScene write FDisableScene  default true;
  end;

type

TD2CustomTranslateProc = function (AText: WideString): WideString;

TD2ColorAnimation = class(TD2Animation)
  private
    FStartColor: TD2Color;
    FStopColor: TD2Color;
    FPath, FPropertyName: AnsiString;
    FInstance: TObject;
    FStartFromCurrent:boolean;
    function GetStartColor: string;
    function GetStopColor: string;
    procedure SetStartColor(const Value:string);
    procedure SetStopColor(const Value:string);
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property StartValue:string read GetStartColor write SetStartColor;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent  default false;
    property StopValue:string read GetStopColor write SetStopColor;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

  TD2GradientAnimation = class(TD2Animation)
  private
    FStartGradient: TD2Gradient;
    FStopGradient: TD2Gradient;
    FPath, FPropertyName: AnsiString;
    FInstance: TObject;
    FStartFromCurrent:boolean;
    procedure SetStartGradient(const Value:TD2Gradient);
    procedure SetStopGradient(const Value:TD2Gradient);
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property StartValue:TD2Gradient read FStartGradient write SetStartGradient;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent  default false;
    property StopValue:TD2Gradient read FStopGradient write SetStopGradient;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

  TD2FloatAnimation = class(TD2Animation)
  private
    FStartFloat:single;
    FStopFloat:single;
    FPath, FPropertyName: AnsiString;
    FInstance: TObject;
    FStartFromCurrent:boolean;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
    procedure Stop;  override;
  published
    property StartValue:single read FStartFloat write FStartFloat stored true;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent  default false;
    property StopValue:single read FStopFloat write FStopFloat stored true;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

  TD2RectAnimation = class(TD2Animation)
  private
    FStartRect: TD2Bounds;
    FCurrent: TD2Bounds;
    FStopRect: TD2Bounds;
    FPath, FPropertyName: AnsiString;
    FInstance: TObject;
    FStartFromCurrent:boolean;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property StartValue:TD2Bounds read FStartRect write FStartRect;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent  default false;
    property StopValue:TD2Bounds read FStopRect write FStopRect;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

  TD2BitmapAnimation = class(TD2Animation)
  private
    FPropertyName: AnsiString;
    FInstance: TObject;
    FStartBitmap: TD2Bitmap;
    FStopBitmap: TD2Bitmap;
    FCurrent: TD2Bitmap;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property StartValue:TD2Bitmap read FStartBitmap write FStartBitmap;
    property StopValue:TD2Bitmap read FStopBitmap write FStopBitmap;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

  TD2BitmapListAnimation = class(TD2Animation)
  private
    FPropertyName: AnsiString;
    FInstance: TObject;
    FCurrent: TD2Bitmap;
    FAnimationCount: Integer;
    FAnimationBitmap: TD2Bitmap;
    FLastAnimationStep: Integer;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
    property AnimationBitmap: TD2Bitmap read FAnimationBitmap write FAnimationBitmap;
    property AnimationCount: Integer read FAnimationCount write FAnimationCount;
  end;


TD2Key = class(TCollectionItem)
  private
    FKey:single;
    procedure SetKey(const Value:single);
  public
  published
    property Key:single read FKey write SetKey;
  end;

TD2Keys = class(TCollection)
  private
  public
    function FindKeys(const Time:single; var Key1, Key2: TD2Key):boolean;
  published
  end;

TD2ColorKey = class(TD2Key)
  private
    FValue:string;
  public
  published
    property Value:string read FValue write FValue;
  end;

TD2ColorKeyAnimation = class(TD2Animation)
  private
    FPropertyName: AnsiString;
    FInstance: TObject;
    FKeys: TD2Keys;
    FPath: AnsiString;
    FStartFromCurrent:boolean;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
    property Keys: TD2Keys read FKeys write FKeys;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent;
  end;

TD2FloatKey = class(TD2Key)
  private
    FValue:single;
  public
  published
    property Value:single read FValue write FValue;
  end;

TD2FloatKeyAnimation = class(TD2Animation)
  private
    FPropertyName: AnsiString;
    FInstance: TObject;
    FKeys: TD2Keys;
    FPath: AnsiString;
    FStartFromCurrent:boolean;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
    property Keys: TD2Keys read FKeys write FKeys;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent;
  end;


TD2PathAnimation = class(TD2Animation)
  private
    FPath: TD2PathData;
    FPolygon: TD2Polygon;
    FObj: TD2VisualObject;
    FStart: TD2Point;
    FRotate:boolean;
    FSpline: TD2Spline;
    procedure SetPath(const Value:TD2PathData);
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property Path: TD2PathData read FPath write SetPath;
    property Rotate: boolean read FRotate write FRotate  default false;
  end;

TD2PathSwitcher = class(TD2Animation)
  private
    FPath, FPropertyName: AnsiString;
    //FInstance: TObject;
    FPathTrue: string;
    FPathFalse: string;
    procedure SetPathFalse(const Value:string);
    procedure SetPathTrue(const Value:string);
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property PathTrue: string read FPathTrue write SetPathTrue;
    property PathFalse: string read FPathFalse write SetPathFalse;
    property PropertyName: AnsiString read FPropertyName write FPropertyName;
  end;

TD2ShadowEffect = class(TD2Effect)
  private
    FDistance:single;
    FSoftness:single;
    FShadowColor: TD2Color;
    FOpacity:single;
    FDirection:single;
    procedure SetDistance(const Value:single);
    procedure SetSoftness(const Value:single);
    procedure SetShadowColor(const Value:string);
    procedure SetOpacity(const Value:single);
    function GetShadowColor: string;
    procedure SetDirection(const Value:single);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
    property Distance:single read FDistance write SetDistance;
    property Direction:single read FDirection write SetDirection;
    property Softness:single read FSoftness write SetSoftness;
    property Opacity:single read FOpacity write SetOpacity;
    property ShadowColor: string read GetShadowColor write SetShadowColor;
  end;

TD2BlurEffect = class(TD2Effect)
  private
    FSoftness:single;
    procedure SetSoftness(const Value:single);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function  GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
    property Softness:single read FSoftness write SetSoftness;
  end;

TD2GlowEffect = class(TD2Effect)
  private
    FSoftness:single;
    FGlowColor: TD2Color;
    FOpacity:single;
    procedure SetSoftness(const Value:single);
    procedure SetOpacity(const Value:single);
    function  GetGlowColor: string;
    procedure SetGlowColor(const Value:string);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function  GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
    property Softness:single read FSoftness write SetSoftness;
    property Opacity:single read FOpacity write SetOpacity;
    property GlowColor: string read GetGlowColor write SetGlowColor;
  end;

TD2InnerGlowEffect = class(TD2GlowEffect)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function  GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
  end;

TD2BevelEffect = class(TD2Effect)
  private
    FDirection:single;
    FSize:integer;
    procedure SetDirection(const Value:single);
    procedure SetSize(const Value:integer);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
    property Direction:single read FDirection write SetDirection;
    property Size: integer read FSize write SetSize;
  end;

TD2ReflectionEffect = class(TD2Effect)
  private
    FOffset:integer;
    FOpacity:single;
    FLength:single;
    procedure SetOpacity(const Value:single);
    procedure SetOffset(const Value:integer);
    procedure SetLength(const Value:single);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetRect(const ARect: TD2Rect): TD2Rect;  override;
    function  GetOffset: TD2Point;  override;
    procedure ProcessEffect(Canvas: TD2Canvas; const Visual: TD2Bitmap; const Data:single);  override;
  published
    property Opacity:single read FOpacity write SetOpacity;
    property Offset: integer read FOffset write SetOffset;
    property Length:single read FLength write SetLength;
  end;

type

TD2Shape = class(TD2VisualObject)
  private
    FFill: TD2Brush;
    FStrokeThickness:single;
    FStroke: TD2Brush;
    FStrokeCap: TD2StrokeCap;
    FStrokeJoin: TD2StrokeJoin;
    FStrokeDash: TD2StrokeDash;
    procedure SetFill(const Value:TD2Brush);
    procedure SetStroke(const Value:TD2Brush);
    procedure SetStrokeThickness(const Value:single);
    function isStrokeThicknessStored: Boolean;
    procedure SetStrokeCap(const Value:TD2StrokeCap);
    procedure SetStrokeJoin(const Value:TD2StrokeJoin);
    procedure SetStrokeDash(const Value:TD2StrokeDash);
  protected
    procedure FillChanged(Sender: TObject);  virtual;
    procedure StrokeChanged(Sender: TObject);  virtual;
    function GetShapeRect: TD2Rect;
    procedure BeforePaint;  override;
    procedure AfterPaint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property Fill: TD2Brush read FFill write SetFill;
    property Stroke: TD2Brush read FStroke write SetStroke;
    property StrokeThickness:single read FStrokeThickness write SetStrokeThickness stored isStrokeThicknessStored;
    property StrokeCap: TD2StrokeCap read FStrokeCap write SetStrokeCap  default d2CapFlat;
    property StrokeDash: TD2StrokeDash read FStrokeDash write SetStrokeDash  default d2DashSolid;
    property StrokeJoin: TD2StrokeJoin read FStrokeJoin write SetStrokeJoin  default d2JoinMiter;
    property ShapeRect: TD2Rect read GetShapeRect;
  published
  end;

TD2Line = class(TD2Shape)
  private
    FLineType: TD2LineType;
    procedure SetLineType(const Value:TD2LineType);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
  published
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
    property LineType: TD2LineType read FLineType write SetLineType;
  end;

TD2Rectangle = class(TD2Shape)
  private
    FyRadius:single;
    FxRadius:single;
    FCorners: TD2Corners;
    FCornerType: TD2CornerType;
    FSides: TD2Sides;
    function IsCornersStored: Boolean;
    function IsSidesStored: Boolean;
  protected
    procedure SetxRadius(const Value:single);  virtual;
    procedure SetyRadius(const Value:single);  virtual;
    procedure SetCorners(const Value:TD2Corners);  virtual;
    procedure SetCornerType(const Value:TD2CornerType);  virtual;
    procedure SetSides(const Value:TD2Sides);  virtual;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Fill;
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
    property xRadius:single read FxRadius write SetxRadius;
    property yRadius:single read FyRadius write SetyRadius;
    property Corners: TD2Corners read FCorners write SetCorners stored IsCornersStored;
    property CornerType: TD2CornerType read FCornerType write SetCornerType  default d2CornerRound;
    property Sides: TD2Sides read FSides write SetSides stored IsSidesStored;
  end;

  TD2SidesRectangle = class(TD2Rectangle)
  end;

TD2BlurRectangle = class(TD2Rectangle)
  private
    FBuffer: TD2Bitmap;
    FSoftness:single;
    FRecreate:boolean;
    procedure SetSoftness(const Value:single);
  protected
    procedure FillChanged(Sender: TObject);  override;
    procedure SetxRadius(const Value:single);  override;
    procedure SetyRadius(const Value:single);  override;
    procedure SetCorners(const Value:TD2Corners);  override;
    procedure SetCornerType(const Value:TD2CornerType);  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Softness:single read FSoftness write SetSoftness;
  end;

TD2RoundRect = class(TD2Shape)
  private
    FCorners: TD2Corners;
    function IsCornersStored: Boolean;
  protected
    procedure SetCorners(const Value:TD2Corners);  virtual;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Fill;
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
    property Corners: TD2Corners read FCorners write SetCorners stored IsCornersStored;
  end;

TD2BlurRoundRect = class(TD2RoundRect)
  private
    FBuffer: TD2Bitmap;
    FSoftness:single;
    FRecreate:boolean;
    procedure SetSoftness(const Value:single);
  protected
    procedure SetCorners(const Value:TD2Corners);  override;
    procedure FillChanged(Sender: TObject);  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Softness:single read FSoftness write SetSoftness;
  end;

TD2CalloutRectangle = class(TD2Rectangle)
  private
    FPath: TD2PathData;
    FCalloutWidth:single;
    FCalloutLength:single;
    FCalloutPosition: TD2CalloutPosition;
    FCalloutOffset:single;
    procedure SetCalloutWidth(const Value:single);
    procedure SetCalloutLength(const Value:single);
    procedure SetCalloutPosition(const Value:TD2CalloutPosition);
    procedure SetCalloutOffset(const Value:single);
  protected
    procedure CreatePath;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Fill;
    property CalloutWidth:single read FCalloutWidth write SetCalloutWidth;
    property CalloutLength:single read FCalloutLength write SetCalloutLength;
    property CalloutPosition: TD2CalloutPosition read FCalloutPosition write SetCalloutPosition  default d2CalloutTop;
    property CalloutOffset:single read FCalloutOffset write SetCalloutOffset;
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
  end;

TD2Ellipse = class(TD2Shape)
  private
  protected
    procedure Paint;  override;
  public
    function PointInObject(X, Y:single):boolean;  override;
  published
    property Fill;
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
  end;

TD2Circle = class(TD2Ellipse)
  private
  protected
    procedure Paint;  override;
  public
  published
  end;

TD2Pie = class(TD2Ellipse)
  private
    FStartAngle:single;
    FEndAngle:single;
    procedure SetEndAngle(const Value:single);
    procedure SetStartAngle(const Value:single);
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    function PointInObject(X, Y:single):boolean;  override;
  published
    property StartAngle:single read FStartAngle write SetStartAngle;
    property EndAngle:single read FEndAngle write SetEndAngle;
  end;

TD2Arc = class(TD2Ellipse)
  private
    FStartAngle:single;
    FEndAngle:single;
    procedure SetEndAngle(const Value:single);
    procedure SetStartAngle(const Value:single);
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    function  PointInObject(X, Y:single):boolean;  override;
  published
    property StartAngle:single read FStartAngle write SetStartAngle;
    property EndAngle:single read FEndAngle write SetEndAngle;
  end;

TD2CustomPath = class(TD2Shape)
  private
    FData: TD2PathData;
    FWrapMode: TD2PathWrap;
    procedure SetData(const Value:TD2PathData);
    procedure SetWrapMode(const Value:TD2PathWrap);
  protected
    procedure DoChanged(Sender: TObject);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function PointInObject(X, Y:single):boolean;  override;
    property Data: TD2PathData read FData write SetData;
    property WrapMode: TD2PathWrap read FWrapMode write SetWrapMode  default d2PathStretch;
  published
    property Fill;
    property Stroke;
    property StrokeCap;
    property StrokeDash;
    property StrokeJoin;
    property StrokeThickness;
  end;

TD2Path = class(TD2CustomPath)
  private
  published
    property Data;
    property WrapMode;
  end;

TD2Text = class(TD2Shape)
  private
    FText: WideString;
    FFont: TD2Font;
    FVertTextAlign: TD2TextAlign;
    FHorzTextAlign: TD2TextAlign;
    FWordWrap:boolean;
    FAutoSize:boolean;
    FStretch:boolean;
    Function  GetText:WideString; overload;                // 5555
    Function  GetText:String; overload;                    // 5555
    procedure SetText(const Value:WideString); overload;  // 5555
    procedure SetText(const Value:String); overload;      // 5555
    procedure SetFont(const Value:TD2Font);
    procedure SetHorzTextAlign(const Value:TD2TextAlign);
    procedure SetVertTextAlign(const Value:TD2TextAlign);
    procedure SetWordWrap(const Value:boolean);
    procedure SetAutoSize(const Value:boolean);
    procedure SetStretch(const Value:boolean);
  protected
    procedure FontChanged(Sender: TObject);  virtual;
    procedure Paint;  override;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure AdjustSize;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure  Realign;  override;
    property TextW: WideString read FText write SetText;
  published
    property AutoSize: boolean read FAutoSize write SetAutoSize  default false;
    property Fill;
    property Font: TD2Font read FFont write SetFont;
    property HorzTextAlign: TD2TextAlign read FHorzTextAlign write SetHorzTextAlign  default d2TextAlignCenter;
    property VertTextAlign: TD2TextAlign read FVertTextAlign write SetVertTextAlign  default d2TextAlignCenter;
    property Text: String read GetText write SetText;
    property Stretch: boolean read FStretch write SetStretch  default false;
    property WordWrap: boolean read FWordWrap write SetWordWrap  default true;
  end;

TD2Image = class(TD2VisualObject)
  private
    FBitmap: TD2Bitmap;
    FBuffer: TD2Bitmap;
    //FStretchThread: TThread;
    FOnBitmapLoaded:TNotifyEvent;
    FBitmapMargins: TD2Bounds;
    FWrapMode: TD2ImageWrap;
    FDisableInterpolation:boolean;
    procedure SetBitmap(const Value:TD2Bitmap);
    procedure SetWrapMode(const Value:TD2ImageWrap);
  protected
    procedure DoBitmapLoaded(Sender: TObject);
    procedure DoBitmapDestroy(Sender: TObject);
    procedure DoBitmapChanged(Sender: TObject);  virtual;
    procedure Loaded;  override;
    procedure Paint;  override;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
  published
    property Bitmap: TD2Bitmap read FBitmap write SetBitmap;
    property BitmapMargins: TD2Bounds read FBitmapMargins write FBitmapMargins;
    property WrapMode: TD2ImageWrap read FWrapMode write SetWrapMode;
    property DisableInterpolation: boolean read FDisableInterpolation write FDisableInterpolation  default false;
    property OnBitmapLoaded:TNotifyEvent read FOnBitmapLoaded write FOnBitmapLoaded;
  end;

TD2PaintEvent = procedure (Sender: TObject; const Canvas: TD2Canvas) of object;

TD2PaintBox = class(TD2VisualObject)
  private
    FOnPaint: TD2PaintEvent;
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property OnPaint: TD2PaintEvent read FOnPaint write FOnPaint;
  end;

TD2Selection = class(TD2VisualObject)
  private
    FParentBounds:boolean;
    FOnChange:TNotifyEvent;
    FHideSelection:boolean;
    FMinSize:integer;
    FOnTrack:TNotifyEvent;
    FProportional:boolean;
    FGripSize:single;
    procedure SetHideSelection(const Value:boolean);
    procedure SetMinSize(const Value:integer);
    procedure SetGripSize(const Value:single);
  protected
    FRatio:single;
    FMove, FLeftTop, FLeftBottom, FRightTop, FRightBottom:boolean;
    FLeftTopHot, FLeftBottomHot, FRightTopHot, FRightBottomHot:boolean;
    FDownPos, FMovePos: TD2Point;
    function GetAbsoluteRect: TD2Rect;  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function PointInObject(X, Y:single):boolean;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseLeave;  override;
  published
    property GripSize:single read FGripSize write SetGripSize;
    property ParentBounds: boolean read FParentBounds write FParentBounds  default true;
    property HideSelection: boolean read FHideSelection write SetHideSelection;
    property MinSize: integer read FMinSize write SetMinSize  default 15;
    property Proportional: boolean read FProportional write FProportional;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnTrack:TNotifyEvent read FOnTrack write FOnTrack;
  end;

TD2SelectionPoint = class(TD2VisualObject)
  private
    FOnChange:TNotifyEvent;
    FOnTrack:TNotifyEvent;
    FParentBounds:boolean;
    FGripSize:single;
    procedure SetGripSize(const Value:single);
  protected
    FPressed:boolean;
    procedure Paint;  override;
    procedure SetHeight(const Value:single);  override;
    procedure SetWidth(const Value:single);  override;
    function  GetUpdateRect: TD2Rect;  override;
    procedure MouseEnter;  override;
    procedure MouseLeave;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  PointInObject(X, Y:single):boolean;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property GripSize:single read FGripSize write SetGripSize;
    property ParentBounds: boolean read FParentBounds write FParentBounds  default true;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnTrack:TNotifyEvent read FOnTrack write FOnTrack;
  end;

TD2DesignFrame = class(TD2VisualObject)
  private
    FOnChange:TNotifyEvent;
    FOnTrack:TNotifyEvent;
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseLeave;  override;
  published
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnTrack:TNotifyEvent read FOnTrack write FOnTrack;
  end;

TD2ScrollArrowLeft = class(TD2CustomPath)
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2ScrollArrowRight = class(TD2CustomPath)
  public
    constructor Create(AOwner: TComponent);  override;
  end;

type

TD2SelectionItem = class(TD2Control)
  private
  protected
    procedure PaintChildren;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
  published
    property Resource;
  end;

TD2Panel = class(TD2Control)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Resource;
  end;

TD2CalloutPanel = class(TD2Panel)
  private
    FCalloutLength:single;
    FCalloutWidth:single;
    FCalloutPosition: TD2CalloutPosition;
    FCalloutOffset:single;
    procedure SetCalloutLength(const Value:single);
    procedure SetCalloutPosition(const Value:TD2CalloutPosition);
    procedure SetCalloutWidth(const Value:single);
    procedure SetCalloutOffset(const Value:single);
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property CalloutWidth:single read FCalloutWidth write SetCalloutWidth;
    property CalloutLength:single read FCalloutLength write SetCalloutLength;
    property CalloutPosition: TD2CalloutPosition read FCalloutPosition write SetCalloutPosition  default d2CalloutTop;
    property CalloutOffset:single read FCalloutOffset write SetCalloutOffset;
  end;

TD2StatusBar = class(TD2Control)
  private
    FShowSizeGrip:boolean;
    procedure SetShowSizeGrip(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Align  default vaBottom;
    property Resource;
    property ShowSizeGrip: boolean read FShowSizeGrip write SetShowSizeGrip;
  end;

TD2ToolBar = class(TD2Control)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Align  default vaTop;
    property Resource;
  end;

TD2TextControl = class(TD2Control)
  private
    FFont: TD2Font;
    FTextAlign: TD2TextAlign;
    FVertTextAlign: TD2TextAlign;
    FFontFill: TD2Brush;
    FWordWrap:boolean;
    function  GetText:WideString; overload;  // 5555
    function  GetText:String; overload;      // 5555
    procedure SetFont(const Value:TD2Font);
    procedure SetTextAlign(const Value:TD2TextAlign);
    procedure SetVertTextAlign(const Value:TD2TextAlign);
    procedure SetFontFill(const Value:TD2Brush);
    procedure FontFillChanged(Sender: TObject);
    procedure SetWordWrap(const Value:boolean);
  protected
    FText: WideString;
    procedure ApplyStyle;  override;
    procedure SetText(const Value:WideString);  virtual; overload;  // 5555
    procedure SetText(const Value:String);  virtual; overload;      // 5555
    procedure FontChanged(Sender: TObject);  virtual;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property Font: TD2Font read FFont write SetFont;
    property FontFill: TD2Brush read FFontFill write SetFontFill;
    property TextW:WideString read FText write SetText;
    property Text :String read GetText write SetText;
    property VertTextAlign: TD2TextAlign read FVertTextAlign write SetVertTextAlign;
    property TextAlign: TD2TextAlign read FTextAlign write SetTextAlign;
    property WordWrap: boolean read FWordWrap write SetWordWrap  default false;
  published
  end;

TD2CustomLabel = class(TD2TextControl)
  private
    FWordWrap:boolean;
    FAutoSize:boolean;
    procedure SetWordWrap(const Value:boolean);
    procedure SetAutoSize(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
    procedure SetText(const Value:String);  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property AutoSize: boolean read FAutoSize write SetAutoSize  default false;
    property AutoTranslate  default true;
    property BindingSource;
    property Font;
    property TextAlign;
    property VertTextAlign;
    property Resource;
    property HitTest  default false;
    property WordWrap: boolean read FWordWrap write SetWordWrap  default true;
  end;

TD2Label = class(TD2CustomLabel)
  private
  protected
  public
  published
    property Text;
end;

TD2ValueLabel = class(TD2Label)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property AutoTranslate  default false;
    property WordWrap  default false;
  end;

TD2CustomButton = class(TD2TextControl)
  private
    FPressing:boolean;
    FIsPressed:boolean;
    FModalResult: TModalResult;
    FStaysPressed:boolean;
    FRepeatTimer: TD2Timer;
    FRepeat:boolean;
    procedure SetIsPressed(const Value:boolean);
  protected
    procedure Click;  override;
    procedure DblClick;  override;
    procedure SetData(const Value:Variant);  override;
    procedure ApplyStyle;  override;
    procedure DoRepeatTimer(Sender: TObject);
    procedure DoRepeatDelayTimer(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
  published
    property Action;
    property AutoTranslate  default true;
    property StaysPressed: boolean read FStaysPressed write FStaysPressed;
    property IsPressed: boolean read FIsPressed write SetIsPressed;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Font;
    property ModalResult: TModalResult read FModalResult write FModalResult  default mrNone;
    property TextAlign;
    property Text;
    property RepeatClick: boolean read FRepeat write FRepeat  default false;
    property WordWrap  default false;
    property Resource;
  end;

TD2Button = class(TD2CustomButton)
  private
    FDefault:boolean;
    FCancel:boolean;
  protected
    procedure DialogKey(var Key: Word; Shift: TShiftState);  override;
  public
  published
    property CanFocused  default true;
    property DisableFocusEffect;
    property Default: boolean read FDefault write FDefault  default false;
    property Cancel: boolean read FCancel write FCancel  default false;
    property TabOrder;
  end;

TD2RoundButton = class(TD2Button)
  private
  protected
  public
  published
  end;

TD2CircleButton = class(TD2Button)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2PopupButton = class(TD2Button)
  private
    FPopupMenu: TPopupMenu;
  protected
    procedure Click;  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property PopupMenu: TPopupMenu read FPopupMenu write FPopupMenu;
  end;

TD2BitmapButton = class(TD2CustomButton)
  private
    FBitmap: TD2Bitmap;
    FBitmapLayout: TD2ButtonLayout;
    FBitmapSpacing:single;
    FBitmapSize:single;
    FBitmapPadding:single;
    procedure SetBitmap(const Value:TD2Bitmap);
    procedure SetBitmapLayout(const Value:TD2ButtonLayout);
    procedure SetBitmapSpacing(const Value:single);
    procedure SetBitmapSize(const Value:single);
    procedure SetBitmapPadding(const Value:single);
  protected
    procedure ActionChange(Sender: TObject; CheckDefaults: Boolean);  override;
    procedure DoBitmapChanged(Sender: TObject);
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Bitmap: TD2Bitmap read FBitmap write SetBitmap;
    property BitmapLayout: TD2ButtonLayout read FBitmapLayout write SetBitmapLayout  default d2GlyphLeft;
    property BitmapSpacing:single read FBitmapSpacing write SetBitmapSpacing;
    property BitmapSize:single read FBitmapSize write SetBitmapSize;
    property BitmapPadding:single read FBitmapPadding write SetBitmapPadding;
  end;

TD2PathButton = class(TD2CustomButton)
  private
    FPath: TD2PathData;
    FPathLayout: TD2ButtonLayout;
    FPathSize:single;
    FPathSpacing:single;
    FPathPadding:single;
    FStrokeThickness:single;
    FFill: TD2Brush;
    FStroke: TD2Brush;
    FStrokeCap: TD2StrokeCap;
    FStrokeDash: TD2StrokeDash;
    FStrokeJoin: TD2StrokeJoin;
    procedure SetPath(const Value:TD2PathData);
    procedure SetPathLayout(const Value:TD2ButtonLayout);
    procedure SetPathPadding(const Value:single);
    procedure SetPathSize(const Value:single);
    procedure SetPathSpacing(const Value:single);
    function isStrokeThicknessStored: Boolean;
    procedure SetFill(const Value:TD2Brush);
    procedure SetStroke(const Value:TD2Brush);
    procedure SetStrokeCap(const Value:TD2StrokeCap);
    procedure SetStrokeDash(const Value:TD2StrokeDash);
    procedure SetStrokeJoin(const Value:TD2StrokeJoin);
    procedure SetStrokeThickness(const Value:single);
  protected
    procedure DoPathChanged(Sender: TObject);
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Path: TD2PathData read FPath write SetPath;
    property PathLayout: TD2ButtonLayout read FPathLayout write SetPathLayout  default d2GlyphLeft;
    property PathSpacing:single read FPathSpacing write SetPathSpacing;
    property PathSize:single read FPathSize write SetPathSize;
    property PathPadding:single read FPathPadding write SetPathPadding;
    property PathFill: TD2Brush read FFill write SetFill;
    property PathStroke: TD2Brush read FStroke write SetStroke;
    property PathStrokeThickness:single read FStrokeThickness write SetStrokeThickness stored isStrokeThicknessStored;
    property PathStrokeCap: TD2StrokeCap read FStrokeCap write SetStrokeCap  default d2CapFlat;
    property PathStrokeDash: TD2StrokeDash read FStrokeDash write SetStrokeDash  default d2DashSolid;
    property PathStrokeJoin: TD2StrokeJoin read FStrokeJoin write SetStrokeJoin  default d2JoinMiter;
  end;

TD2ToolButton = class(TD2BitmapButton)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property CanFocused  default false;
    property TabOrder;
    property BitmapLayout  default d2GlyphTop;
  end;

TD2ToolPathButton = class(TD2PathButton)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property CanFocused  default false;
    property TabOrder;
    property PathLayout  default d2GlyphTop;
  end;

TD2BitmapStateButton = class(TD2CustomButton)
  private
    FBitmap: TD2Bitmap;
    FBitmapDown: TD2Bitmap;
    FBitmapHot: TD2Bitmap;
    procedure SetBitmap(const Value:TD2Bitmap);
    procedure SetBitmapDown(const Value:TD2Bitmap);
    procedure SetBitmapHot(const Value:TD2Bitmap);
  protected
    procedure DoBitmapChanged(Sender: TObject);
    procedure ApplyStyle;  override;
    procedure MouseEnter;  override;
    procedure MouseLeave;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure StartTriggerAnimation(AInstance: TD2Object; ATrigger: string);  override;
  published
    property Bitmap: TD2Bitmap read FBitmap write SetBitmap;
    property BitmapHot: TD2Bitmap read FBitmapHot write SetBitmapHot;
    property BitmapDown: TD2Bitmap read FBitmapDown write SetBitmapDown;
  end;

TD2SpeedButton = class(TD2CustomButton)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property CanFocused  default false;
    property RepeatClick  default true;
    property TabOrder;
  end;

TD2ColorButton = class(TD2CustomButton)
  private
    FFill: TD2Shape;
    FColor: string;
    FOnChange:TNotifyEvent;
    FUseStandardDialog:boolean;
    procedure SetColor(const Value:string);
  protected
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure Click;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property AutoTranslate  default false;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Color: string read FColor write SetColor;
    property UseStandardDialog: boolean read FUseStandardDialog write FUseStandardDialog  default true;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CornerButton = class(TD2CustomButton)
  private
    FyRadius:single;
    FxRadius:single;
    FCorners: TD2Corners;
    FCornerType: TD2CornerType;
    FSides: TD2Sides;
    function IsCornersStored: Boolean;
    procedure SetxRadius(const Value:single);
    procedure SetyRadius(const Value:single);
    procedure SetCorners(const Value:TD2Corners);
    procedure SetCornerType(const Value:TD2CornerType);
    procedure SetSides(const Value:TD2Sides);
    function IsSidesStored: Boolean;
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property xRadius:single read FxRadius write SetxRadius;
    property yRadius:single read FyRadius write SetyRadius;
    property Corners: TD2Corners read FCorners write SetCorners stored IsCornersStored;
    property CornerType: TD2CornerType read FCornerType write SetCornerType  default d2CornerRound;
    property Sides: TD2Sides read FSides write SetSides stored IsSidesStored;
  end;

TD2CheckBox = class(TD2TextControl)
  private
    FPressing:boolean;
    FOnChange:TNotifyEvent;
    FIsPressed:boolean;
    FIsChecked:boolean;
    procedure SetIsChecked(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  published
    property IsPressed: boolean read FIsPressed;
    property IsChecked: boolean read FIsChecked write SetIsChecked;
    property AutoTranslate  default true;
    property BindingSource;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Font;
    property TextAlign;
    property Text;
    property Resource;
    property WordWrap;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2PathCheckBox = class(TD2CheckBox)
  private
    FPath: TD2PathData;
    procedure SetPath(const Value:TD2PathData);
  protected
    procedure ApplyStyle;  override;
    procedure DoPathChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Resource;
    property Path: TD2PathData read FPath write SetPath;
  end;

TD2RadioButton = class(TD2TextControl)
  private
    FPressing:boolean;
    FOnChange:TNotifyEvent;
    FIsPressed:boolean;
    FIsChecked:boolean;
    FGroupName: string;
    procedure SetIsChecked(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
    procedure EnterFocus;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  published
    property IsPressed: boolean read FIsPressed;
    property IsChecked: boolean read FIsChecked write SetIsChecked;
    property AutoTranslate  default true;
    property BindingSource;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Font;
    property TextAlign;
    property Text;
    property Resource;
    property WordWrap;
    property GroupName: string read FGroupName write FGroupName;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2GroupBox = class(TD2TextControl)
  private
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property AutoTranslate  default true;
    property Font;
    property TextAlign;
    property Text;
    property Resource;
  end;

TD2CloseButton = class(TD2Control)
  private
    FPressing:boolean;
    FOnClick:TNotifyEvent;
    FCloseForm:boolean;
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property Resource;
    property CloseForm: boolean read FCloseForm write FCloseForm  default true;
    property OnClick:TNotifyEvent read FOnClick write FOnClick;
  end;

TD2SizeGrip = class(TD2Control, Id2SizeGrip)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Resource;
  end;

TD2Splitter = class(TD2Control)
  private
    FPressed:boolean;
    FControl: TD2VisualObject;
    FDownPos: TD2Point;
    FMinSize:single;
    FMaxSize:single;
    FNewSize, FOldSize:single;
    FSplit:single;
    FShowGrip:boolean;
    procedure SetShowGrip(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
    procedure SetAlign(const Value:TD2Align);  override;
    function  FindObject: TD2VisualObject;
    procedure CalcSplitSize(X, Y:single; var NewSize, Split:single);
    procedure UpdateSize(X, Y:single);
    function  DoCanResize(var NewSize:single): Boolean;
    procedure UpdateControlSize;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property MinSize:single read FMinSize write FMinSize;
    property ShowGrip: boolean read FShowGrip write SetShowGrip  default true;
  end;

TD2ProgressBar = class(TD2Control)
  private
    FMin:single;
    FValue:single;
    FMax:single;
    FOrientation: TD2Orientation;
    procedure SetMax(const Value:single);
    procedure SetMin(const Value:single);
    procedure SetOrientation(const Value:TD2Orientation);
    procedure SetValue(const Value:single);
  protected
    procedure ApplyStyle;  override;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
  published
    property BindingSource;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
    property Value:single read FValue write SetValue;
    property Resource;
  end;

  TD2CustomTrack = class;
  TD2ScrollBar = class;

TD2Thumb = class(TD2Control)
  private
    FTrack: TD2CustomTrack;
    FDownOffset: TD2Point;
    FPressed:boolean;
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property Resource;
  end;

TD2CustomTrack = class(TD2Control)
  private
    function GetThumb: TD2Thumb;
    procedure SetFrequency(const Value:single);
    function GetIsTracking:boolean;
  protected
    FOnChange, FOnTracking:TNotifyEvent;
    FValue:single;
    FMin:single;
    FMax:single;
    FViewportSize:single;
    FOrientation: TD2Orientation;
    FTracking:boolean;
    FFrequency:single;
    procedure SetMax(const Value:single);  virtual;
    procedure SetMin(const Value:single);  virtual;
    procedure SetValue(Value:single);
    procedure SetViewportSize(const Value:single);
    procedure SetOrientation(const Value:TD2Orientation);
    function  GetThumbRect: TD2Rect;
    property  Thumb: TD2Thumb read GetThumb;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    property IsTracking: boolean read GetIsTracking;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Frequency:single read FFrequency write SetFrequency;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
    property Value:single read FValue write SetValue;
    property ViewportSize:single read FViewportSize write SetViewportSize;
    property Tracking: boolean read FTracking write FTracking  default true;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnTracking:TNotifyEvent read FOnTracking write FOnTracking;
  end;

TD2Track = class(TD2CustomTrack)
  private
  protected
  public
  published
    property BindingSource;
    property Resource;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Frequency:single read FFrequency write SetFrequency;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
    property Value:single read FValue write SetValue;
    property ViewportSize:single read FViewportSize write SetViewportSize;
    property Tracking: boolean read FTracking write FTracking;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2TrackBar = class(TD2CustomTrack)
  private
  protected
    procedure SetMax(const Value:single);  override;
    procedure SetMin(const Value:single);  override;
    procedure Loaded;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property BindingSource;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Resource;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Frequency:single read FFrequency write SetFrequency;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
    property Value:single read FValue write SetValue;
    property Tracking: boolean read FTracking write FTracking;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2ScrollBar = class(TD2Control)
  private
    FOnChange:TNotifyEvent;
    FValue:single;
    FMin:single;
    FMax:single;
    FViewportSize:single;
    FOrientation: TD2Orientation;
    FSmallChange:single;
    procedure SetMax(const Value:single);
    procedure SetMin(const Value:single);
    procedure SetValue(const Value:single);
    procedure SetViewportSize(const Value:single);
    procedure SetOrientation(const Value:TD2Orientation);
  protected
    procedure DoTrackChanged(Sender: TObject);
    procedure DoMinButtonClick(Sender: TObject);
    procedure DoMaxButtonClick(Sender: TObject);
    function Track: TD2CustomTrack;
    function MinButton: TD2CustomButton;
    function MaxButton: TD2CustomButton;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
  published
    property BindingSource;
    property Resource;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
    property Value:single read FValue write SetValue;
    property ViewportSize:single read FViewportSize write SetViewportSize;
    property SmallChange:single read FSmallChange write FSmallChange;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2SmallScrollBar = class(TD2ScrollBar)
  private
  public
    constructor Create(AOwner: TComponent);  override;
  end;

TD2AniIndicator = class(TD2Control)
  private
    //FDragTimer: TD2Timer;
    FLayout: TD2VisualObject;
    FAni: TD2FloatAnimation;
    FEnabled:boolean;
    FStyle: TD2AniIndicatorStyle;
    procedure SetEnabled(const Value:boolean);
    procedure SetStyle(const Value:TD2AniIndicatorStyle);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
  published
    property Enabled: boolean read FEnabled write SetEnabled;
    property Style: TD2AniIndicatorStyle read FStyle write SetStyle;
  end;

TD2AngleButton = class(TD2Control)
  private
    FPressing:boolean;
    FOnChange:TNotifyEvent;
    FOldPos: TD2Point;
    FSaveValue, FValue:single;
    FFrequency:single;
    FTracking:boolean;
    FShowValue:boolean;
    procedure SetValue(const Value:single);
    procedure SetShowValue(const Value:boolean);
  protected
    function  Tick: TD2VisualObject;
    function  Text: TD2Text;
    procedure ApplyStyle;  override;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property BindingSource;
    property Resource;
    property Frequency:single read FFrequency write FFrequency;
    property Tracking: boolean read FTracking write FTracking  default true;
    property ShowValue:boolean read FShowValue write SetShowValue  default false;
    property Value:single read FValue write SetValue;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2ExpanderButton = class(TD2CustomButton)
  private
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property CanFocused  default false;
  end;

TD2Expander = class(TD2TextControl)
  private
    FShowCheck:boolean;
    FIsChecked:boolean;
    FOnCheckChange:TNotifyEvent;
    procedure DoButtonClick(Sender: TObject);
    procedure SetIsChecked(const Value:boolean);
    procedure SetShowCheck(const Value:boolean);
  protected
    FIsExpanded:boolean;
    FContent: TD2Content;
    FButton: TD2CustomButton;
    FCheck: TD2CheckBox;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure SetIsExpanded(const Value:boolean);  virtual;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadContentSize(Reader:TReader);
    procedure WriteContentSize(Writer:TWriter);
    procedure DesignClick;  override;
    procedure DoCheckChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    procedure AddObject(AObject: TD2Object);  override;
  published
    property Font;
    property TextAlign;
    property Text;
    property Resource;
    property AutoTranslate  default true;
    property IsExpanded: boolean read FIsExpanded write SetIsExpanded  default true;
    property IsChecked: boolean read FIsChecked write SetIsChecked  default true;
    property ShowCheck: boolean read FShowCheck write SetShowCheck;
    property OnCheckChange:TNotifyEvent read FOnCheckChange write FOnCheckChange;
  end;

TD2PopupBox = class(TD2CustomButton)
  private
    FItems: TD2WideStrings;
    FItemIndex:integer;
    FPopup: TPopupMenu;
    FOnChange:TNotifyEvent;
    procedure SetItems(const Value:TD2WideStrings);
    procedure SetItemIndex(const Value:integer);
  protected
    procedure ApplyStyle;  override;
    procedure Click;  override;
    procedure DoItemsChanged(Sender: TObject);  virtual;
    procedure DoItemClick(Sender: TObject);
    procedure DoPopup;  virtual;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure SetText(const Value:String);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property BindingSource;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Text stored false;
    property Items: TD2WideStrings read FItems write SetItems;
    property ItemIndex: integer read FItemIndex write SetItemIndex;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2Window = class(TD2TextControl)
  private
    FShowCloseButton:boolean;
    FShowSizeGrip:boolean;
    FOnCloseClick:TNotifyEvent;
    procedure SetShowCloseButton(const Value:boolean);
    procedure SetShowSizeGrip(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property ShowCloseButton: boolean read FShowCloseButton write SetShowCloseButton  default true;
    property ShowSizeGrip: boolean read FShowSizeGrip write SetShowSizeGrip  default true;
    property OnCloseClick:TNotifyEvent read FOnCloseClick write FOnCloseClick;
    property AutoTranslate  default true;
    property Font;
    property TextAlign;
    property Text;
    property Resource;
  end;

TD2HudWindow = class(TD2Window)
  private
    FDisableShadowOnOSX:boolean;
    FFill: TD2Brush;
    FStrokeThickness:single;
    FStroke: TD2Brush;
    FStrokeCap: TD2StrokeCap;
    FStrokeDash: TD2StrokeDash;
    FStrokeJoin: TD2StrokeJoin;
    FCloseAlign: TD2CloseAlign;
    FShowCaption:boolean;
    procedure SetDisableShadowOnOSX(const Value:boolean);
    procedure SetFill(const Value:TD2Brush);
    function isStrokeThicknessStored: Boolean;
    procedure SetStroke(const Value:TD2Brush);
    procedure SetStrokeCap(const Value:TD2StrokeCap);
    procedure SetStrokeDash(const Value:TD2StrokeDash);
    procedure SetStrokeJoin(const Value:TD2StrokeJoin);
    procedure SetStrokeThickness(const Value:single);
    procedure SetCloseAlign(const Value:TD2CloseAlign);
    procedure SetShowCaption(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
    procedure DoFillChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property DisableShadowOnOSX: boolean read FDisableShadowOnOSX write SetDisableShadowOnOSX  default true;
    property ButtonAlign: TD2CloseAlign read FCloseAlign write SetCloseAlign  default d2ButtonAlignLeft;
    property Fill: TD2Brush read FFill write SetFill;
    property Stroke: TD2Brush read FStroke write SetStroke;
    property StrokeThickness:single read FStrokeThickness write SetStrokeThickness stored isStrokeThicknessStored;
    property StrokeCap: TD2StrokeCap read FStrokeCap write SetStrokeCap  default d2CapFlat;
    property StrokeDash: TD2StrokeDash read FStrokeDash write SetStrokeDash  default d2DashSolid;
    property StrokeJoin: TD2StrokeJoin read FStrokeJoin write SetStrokeJoin  default d2JoinMiter;
    property ShowCaption: boolean read FShowCaption write SetShowCaption  default true;
  end;

TD2ImageControl = class(TD2Control)
  private
    FImage: TD2Image;
    FOnChange:TNotifyEvent;
    FBitmap: TD2Bitmap;
    FEnableOpenDialog:boolean;
    procedure SetBitmap(const Value:TD2Bitmap);
  protected
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure Click;  override;
    procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);  override;
    procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  override;
    procedure DoBitmapChanged(Sender: TObject);  virtual;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property CanFocused  default true;
    property DisableFocusEffect;
    property EnableOpenDialog: boolean read FEnableOpenDialog write FEnableOpenDialog  default true;
    property TabOrder;
    property Bitmap: TD2Bitmap read FBitmap write SetBitmap;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2HudLabel = class(TD2Label)
  private
  protected
  public
  published
  end;

TD2HudButton = class(TD2Button)
  private
  protected
  public
  published
  end;

TD2HudRoundButton = class(TD2Button)
  private
  protected
  public
  published
  end;

TD2HudCircleButton = class(TD2CircleButton)
  private
  protected
  public
  published
  end;

TD2HudCornerButton = class(TD2CornerButton)
  private
  protected
  public
  published
  end;

TD2HudSpeedButton = class(TD2SpeedButton)
  private
  protected
  public
  published
  end;

TD2HudCheckBox = class(TD2CheckBox)
  private
  protected
  public
  published
  end;

TD2HudRadioButton = class(TD2RadioButton)
  private
  protected
  public
  published
  end;

TD2HudGroupBox = class(TD2GroupBox)
  private
  protected
  public
  published
  end;

TD2HudPopupBox = class(TD2PopupBox)
  private
  protected
  public
  published
  end;

TD2HudAngleButton = class(TD2AngleButton)
  private
  protected
  public
  published
  end;

TD2HudTrack = class(TD2Track)
  private
  protected
  public
  published
  end;

TD2HudTrackBar = class(TD2TrackBar)
  private
  protected
  public
  published
  end;

TD2HudScrollBar = class(TD2ScrollBar)
  private
  protected
  public
  published
  end;

TD2LayerWindow = class(TD2HudWindow)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
  end;

TD2HudPanel = class(TD2Panel)
  private
  protected
  public
  published
  end;

TD2HudCloseButton = class(TD2CloseButton)
  private
  protected
  public
  published
  end;

TD2HudSizeGrip = class(TD2SizeGrip)
  private
  protected
  public
  published
  end;

TD2HudStatusBar = class(TD2StatusBar)
  private
  protected
  public
  published
  end;

TD2Layout = class(TD2VisualObject)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
  published
    property HitTest  default false;
  end;

TD2ScaledLayout = class(TD2VisualObject)
  private
    FOriginalWidth:single;
    FOriginalHeight:single;
    procedure SetOriginalWidth(const Value:single);
    procedure SetOriginalHeight(const Value:single);
  protected
    function GetChildrenMatrix: TD2Matrix;  override;
    procedure SetHeight(const Value:single);  override;
    procedure SetWidth(const Value:single);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure Realign;  override;
  published
    property OriginalWidth:single read FOriginalWidth write SetOriginalWidth;
    property OriginalHeight:single read FOriginalHeight write SetOriginalHeight;
  end;

TD2ScrollContent = class(TD2Content)
  private
  protected
    function GetClipRect: TD2Rect;  override;
    procedure PaintChildren;  override;
    function GetUpdateRect: TD2Rect;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
    function  ObjectByPoint(X, Y:single): TD2VisualObject;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure RemoveObject(AObject: TD2Object);  override;
  end;

TD2CustomScrollBox = class(TD2Control)
  private
    FScrollDuration: single;
    FAutoHide:boolean;
    FDisableMouseWheel:boolean;
    FDown:boolean;
    FHScrollAni: TD2FloatAnimation;
    FVScrollAni: TD2FloatAnimation;
    FAnimated:boolean;
    FShowScrollBars:boolean;
    FShowSizeGrip:boolean;
    FMouseTracking:boolean;
    FUseSmallScrollBars:boolean;
    procedure SetShowScrollBars(const Value:boolean);
    procedure SetShowSizeGrip(const Value:boolean);
    procedure SetUseSmallScrollBars(const Value:boolean);
  protected
    FScrollDesign: TD2Point;
    FContent: TD2ScrollContent;
    FHScrollBar: TD2ScrollBar;
    FVScrollBar: TD2ScrollBar;
    FContentLayout: TD2VisualObject;
    FDownPos, FLastDelta, FCurrentPos: TD2Point;
    procedure Loaded;  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadScrollDesign(Reader:TReader);
    procedure WriteScrollDesign(Writer:TWriter);
    procedure ContentAddObject(AObject: TD2Object);  virtual;
    procedure ContentBeforeRemoveObject(AObject: TD2Object);  virtual;
    procedure ContentRemoveObject(AObject: TD2Object);  virtual;
    procedure HScrollChange(Sender: TObject);  virtual;
    procedure VScrollChange(Sender: TObject);  virtual;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure CreateVScrollAni;
    procedure CreateHScrollAni;
    function  ContentRect: TD2Rect;
    function  VScrollBarValue:single;
    function  HScrollBarValue:single;
    function  GetContentBounds: TD2Rect;  virtual;
    procedure ContentPaint(const Canvas: TD2Canvas; const ARect: TD2Rect);  virtual;
    procedure RealignContent(R: TD2Rect);  virtual;
    property  ContentLayout: TD2VisualObject read FContentLayout;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
    procedure Realign;  override;
    procedure Centre;
    procedure ScrollTo(const Dx, Dy:single);
    procedure InViewRect(const Rect: TD2Rect);
    function  ClientWidth:single;
    function  ClientHeight:single;
    property  HScrollBar: TD2ScrollBar read FHScrollBar;
    property  VScrollBar: TD2ScrollBar read FVScrollBar;
    property AutoHide: boolean read FAutoHide write FAutoHide  default true;
    property Animated: boolean read FAnimated write FAnimated  default true;
    property ScrollDuration: single read FScrollDuration write FScrollDuration default 0.7;
    property DisableMouseWheel: boolean read FDisableMouseWheel write FDisableMouseWheel  default false;
    property MouseTracking: boolean read FMouseTracking write FMouseTracking  default false;
    property ShowScrollBars: boolean read FShowScrollBars write SetShowScrollBars  default true;
    property ShowSizeGrip: boolean read FShowSizeGrip write SetShowSizeGrip  default false;
    property UseSmallScrollBars: boolean read FUseSmallScrollBars write SetUseSmallScrollBars  default false;
  end;

TD2ScrollBox = class(TD2CustomScrollBox)
  published
    property AutoHide;
    property Animated;
    property ScrollDuration;
    property DisableMouseWheel;
    property MouseTracking;
    property ShowScrollBars;
    property ShowSizeGrip;
    property UseSmallScrollBars;
end;

TD2VertScrollBox = class(TD2ScrollBox)
  private
  protected
    function GetContentBounds: TD2Rect;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2FramedScrollBox = class(TD2ScrollBox)
  private
  protected
  public
  published
  end;

TD2FramedVertScrollBox = class(TD2VertScrollBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2GridLayout = class(TD2VisualObject)
  private
    FItemWidth:single;
    FItemHeight:single;
    FOrientation: TD2Orientation;
    procedure SetItemHeight(const Value:single);
    procedure SetItemWidth(const Value:single);
    procedure SetOrientation(const Value:TD2Orientation);
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Realign;  override;
  published
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property ItemWidth:single read FItemWidth write SetItemWidth;
    property Orientation: TD2Orientation read FOrientation write SetOrientation;
  end;

TD2SplitLayout = class(TD2VisualObject)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property HitTest  default false;
  end;

TD2Nond2Layout = class(TD2Layout)
  private
    FControl: TControl;
    FVisibleTimer: TD2Timer;
    procedure SetControl(const Value:TControl);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DoVisibleTimer(Sender: TObject);
    procedure MatrixChanged(Sender: TObject);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  CheckParentVisible:boolean;  override;
    procedure Realign;  override;
  published
    property Nond2Control: TControl read FControl write SetControl;
  end;


type

  TD2ListBox = class;
  TD2ComboBox = class;

  TD2ListBoxItem = class(TD2TextControl)
  private
    FIsChecked:boolean;
    FCheck: TD2CheckBox;
    FIsSelected:boolean;
    procedure SetIsChecked(const Value:boolean);
    procedure DoCheckClick(Sender: TObject);
    procedure UpdateCheck;
    procedure SetIsSelected(const Value:boolean);
  protected
    function  ListBox: TD2ListBox;
    function  ComboBox: TD2ComboBox;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure DesignInsert;  override;
    procedure DesignSelect;  override;
    function  EnterFocusChildren(AObject: TD2VisualObject):boolean;  override;
    procedure DragEnd;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  GetParentComponent: TComponent;  override;
    procedure Paint;  override;
  published
    property IsChecked: boolean read FIsChecked write SetIsChecked;
    property IsSelected: boolean read FIsSelected write SetIsSelected;
    property AutoTranslate  default true;
    property Font;
    property Resource;
    property Text;
    property TextAlign  default d2TextAlignNear;
    property WordWrap;
  end;

  TOnCompareListBoxItemEvent = function(Item1, Item2: TD2ListBoxItem): integer of object;
  TOnListBoxDragChange = procedure (SourceItem, DestItem: TD2ListBoxItem; Allow: boolean) of object;

  TD2ListBox = class(TD2ScrollBox)
  private
    FMouseSelecting:boolean;
    FOnChange:TNotifyEvent;
    FHideSelectionUnfocused:boolean;
    FShowCheckboxes:boolean;
    FOnChangeCheck:TNotifyEvent;
    FSorted:boolean;
    FOnCompare: TOnCompareListBoxItemEvent;
    FMultiSelect:boolean;
    FAlternatingRowBackground:boolean;
    FAllowDrag:boolean;
    FDragItem: TD2ListBoxItem;
    FOnDragChange: TOnListBoxDragChange;
    function GetCount:integer;
    function GetSelected: TD2ListBoxItem;
    procedure SetColumns(const Value:integer);
    procedure SetItemHeight(const Value:single);
    procedure SetItemWidth(const Value:single);
    procedure SetListStyle(const Value:TD2ListStyle);
    procedure SetShowCheckboxes(const Value:boolean);
    function  GetItem(Index: integer): TD2ListBoxItem;
    procedure SetSorted(const Value:boolean);
    procedure SetAlternatingRowBackground(const Value:boolean);
    procedure SetMultiSelect(const Value:boolean);
    procedure SetAllowDrag(const Value:boolean);
  protected
    FColumns:integer;
    FItemWidth:single;
    FItemHeight:single;
    FListStyle: TD2ListStyle;
    FFirstSelect: TD2ListBoxItem;
    FSelection: TD2VisualObject;
    FSelections: TList;
    FOddFill: TD2Brush;
    FItemIndex:integer;
    procedure SortItems;  virtual;
    procedure SetItemIndex(const Value:integer);  virtual;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure KeyUp(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);  override;
    procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  override;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    function  GetContentBounds: TD2Rect;  override;
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    procedure HScrollChange(Sender: TObject);  override;
    procedure VScrollChange(Sender: TObject);  override;
    procedure ContentAddObject(AObject: TD2Object);  override;
    procedure ContentBeforeRemoveObject(AObject: TD2Object);  override;
    procedure ContentRemoveObject(AObject: TD2Object);  override;
    procedure UpdateSelection;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    function  ItemClass: string;  override;
    procedure Clear;  virtual;
    procedure SelectAll;
    procedure ClearSelection;
    procedure SelectRange(Item1, Item2: TD2ListBoxItem);
    function  ItemByPoint(const X, Y:single): TD2ListBoxItem;
    function  ItemByIndex(const Idx: integer): TD2ListBoxItem;
    procedure Exchange(Item1, Item2: TD2ListBoxItem);
    procedure AddObject(AObject: TD2Object);  override;
    property Count: integer read GetCount;
    property Selected: TD2ListBoxItem read GetSelected;
    property Items[Index: integer]: TD2ListBoxItem read GetItem;
  published
    property Resource;
    property AllowDrag: boolean read FAllowDrag write SetAllowDrag  default false;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property AlternatingRowBackground: boolean read FAlternatingRowBackground write SetAlternatingRowBackground  default false;
    property Columns: integer read FColumns write SetColumns  default 1;
    property HideSelectionUnfocused: boolean read FHideSelectionUnfocused write FHideSelectionUnfocused  default true;
    property ItemIndex: integer read FItemIndex write SetItemIndex  default -1;
    property ItemWidth:single read FItemWidth write SetItemWidth;
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property ListStyle: TD2ListStyle read FListStyle write SetListStyle  default d2ListVertical;
    property MultiSelect: boolean read FMultiSelect write SetMultiSelect  default false;
    property Sorted: boolean read FSorted write SetSorted  default false;
    property ShowCheckboxes: boolean read FShowCheckboxes write SetShowCheckboxes  default false;
    property BindingSource;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnChangeCheck:TNotifyEvent read FOnChangeCheck write FOnChangeCheck;
    property OnCompare: TOnCompareListBoxItemEvent read FOnCompare write FOnCompare;
    property OnDragChange: TOnListBoxDragChange read FOnDragChange write FOnDragChange;
  end;

  TD2ComboListBox = class(TD2Listbox)
  private
  protected
    FComboBox: TD2Control;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure KillFocus;  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure ApplyResource;  override;
    function  GetParentComponent: TComponent;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
  end;

  TD2ComboBox = class(TD2Control)
  private
    FDropDownCount:integer;
    FPopup: TD2Popup;
    FListBox: TD2ComboListBox;
    FOnChange:TNotifyEvent;
    FPlacement: TD2Placement;
    procedure SetItemIndex(const Value:integer);
    function GetItemIndex:integer;
    function GetCount:integer;
    procedure SetListBoxResource(const Value:string);
    function GetListBoxResource: string;
    function GetItemHeight:single;
    procedure SetItemHeight(const Value:single);
  protected
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure ApplyStyle;  override;
    procedure DoListBoxChange(Sender: TObject);
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);  virtual;
    procedure DesignClick;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  ItemClass: string;  override;
    procedure Paint;  override;
    procedure PaintChildren;  override;
    procedure Realign;  override;
    procedure Clear;  virtual;
    procedure DropDown;  virtual;
    procedure AddObject(AObject: TD2Object);  override;
    property  ListBox: TD2ComboListBox read FListBox write FListBox;
    property  Count: integer read GetCount;
  published
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Resource;
    property ItemIndex: integer read GetItemIndex write SetItemIndex;
    property ItemHeight:single read GetItemHeight write SetItemHeight;
    property DropDownCount: integer read FDropDownCount write FDropDownCount  default 8;
    property Placement: TD2Placement read FPlacement write FPlacement;
    property BindingSource;
    property ListBoxResource: string read GetListBoxResource write SetListBoxResource;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

  TD2StringListBox = class(TD2ListBox)
  private
    FItems: TD2WideStrings;
    FTextAlign: TD2TextAlign;
    FItemStyle: string;
    FFont: TD2Font;
    procedure SetItems(const Value:TD2WideStrings);
    procedure SetTextAlign(const Value:TD2TextAlign);
    procedure SetFont(const Value:TD2Font);
  protected
    procedure SortItems;  override;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure ApplyStyle;  override;
    procedure RebuildList;
    procedure DoItemsChanged(Sender: TObject);
    procedure FontChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure EndUpdate;  override;
  published
    property AutoTranslate  default true;
    property TextAlign: TD2TextAlign read FTextAlign write SetTextAlign  default d2TextAlignCenter;
    property Font: TD2Font read FFont write SetFont;
    property Items: TD2WideStrings read FItems write SetItems;
    property ItemIndex;
    property BindingSource;
  end;

  TD2StringComboBox = class(TD2ComboBox)
  private
    FItemHeight:single;
    FItems: TD2WideStrings;
    FTextAlign: TD2TextAlign;
    FItemStyle: string;
    procedure SetItemHeight(const Value:single);
    procedure SetItems(const Value:TD2WideStrings);
    procedure SetTextAlign(const Value:TD2TextAlign);
  protected
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure ApplyStyle;  override;
    procedure RebuildList;
    procedure DoItemsChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Clear;  override;
  published
    property AutoTranslate  default true;
    property TextAlign: TD2TextAlign read FTextAlign write SetTextAlign  default d2TextAlignCenter;
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property Items: TD2WideStrings read FItems write SetItems;
    property ItemIndex;
  end;

  TD2HorzListBox = class(TD2ListBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property ListStyle  default d2ListHorizontal;
  end;

  TD2ImageThread = class(TThread)
  private
    FImage: TD2Image;
    FTempBitmap: TD2Bitmap;
    FFileName: string;
    FUseThumbnails:boolean;
  protected
    procedure Execute;  override;
    procedure Finished;
  public
    constructor Create(const AImage: TD2Image; const AFileName: string; const AUseThumbnails: boolean);
    destructor Destroy;  override;
  end;

  TD2ImageListBoxItem = class(TD2ListBoxItem)
  private
    function TextBorder: TD2VisualObject;
  protected
    procedure ApplyStyle;  override;
  public
    function Text: TD2Text;
  published
  end;

  TD2ImageListBox = class(TD2ListBox)
  private
    //FFolder: string;
    FShowFileName:boolean;
    FItemHeight:single;
    FUseThumbnails:boolean;
    function GetSelectedFileName: string;
    procedure SetShowFileName(const Value:boolean);
    procedure SetItemHeight(const Value:single);
    function GetSelectedImage: TD2Image;
    function GetImage(Index: integer): TD2Image;
  protected
    procedure DoApplyResource(Sender: TObject);
    procedure BeginAutoDrag;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    procedure AddFolder(const Folder: string);
    procedure AddFile(const AFile: string);
    procedure AddBitmap(const AFile: string; const ABitmap: TD2Bitmap);
    procedure Clear;  override;
    property Images[Index: integer]: TD2Image read GetImage;
    property SelectedFileName: string read GetSelectedFileName;
    property SelectedImage: TD2Image read GetSelectedImage;
  published
    property ShowFileName: boolean read FShowFileName write SetShowFileName;
    property UseThumbnails: boolean read FUseThumbnails write FUseThumbnails  default true;
    property ItemHeight:single read FItemHeight write SetItemHeight;
  end;

  TD2HudImageListBox = class(TD2ImageListBox)
  private
  protected
  public
  published
  end;

  TD2HorzImageListBox = class(TD2ImageListBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property ListStyle  default d2ListHorizontal;
  end;

  TD2HudHorzImageListBox = class(TD2HorzImageListBox)
  private
  protected
  public
  published
  end;

  TD2HudListBox = class(TD2ListBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2HudHorzListBox = class(TD2HorzListBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2HudStringListBox = class(TD2StringListBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2HudComboBox = class(TD2ComboBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2HudStringComboBox = class(TD2StringComboBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2TreeView = class;

  TD2TreeViewItem = class(TD2TextControl)
  private
    FIsExpanded:boolean;
    FButton: TD2CustomButton;
    FCheck: TD2CheckBox;
    FGlobalIndex:integer;
    FIsChecked:boolean;
    FIsSelected:boolean;
    procedure SetIsExpanded(const Value:boolean);
    procedure DoButtonClick(Sender: TObject);
    procedure DoCheckClick(Sender: TObject);
    function GetCount:integer;
    procedure SetIsChecked(const Value:boolean);
    procedure UpdateCheck;
    function GetItem(Index: integer): TD2TreeViewItem;
    procedure SetIsSelected(const Value:boolean);
  protected
    procedure DesignClick;  override;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure DragEnd;  override;
    function EnterFocusChildren(AObject: TD2VisualObject):boolean;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
    procedure Realign;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure RemoveObject(AObject: TD2Object);  override;
    function ItemClass: string;  override;
    function ItemByPoint(const X, Y:single): TD2TreeViewItem;
    function ItemByIndex(const Idx: integer): TD2TreeViewItem;
    property Count: integer read GetCount;
    property GlobalIndex: integer read FGlobalIndex write FGlobalIndex;
    function TreeView: TD2TreeView;
    function Level:integer;
    property Items[Index: integer]: TD2TreeViewItem read GetItem;
  published
    property IsChecked: boolean read FIsChecked write SetIsChecked;
    property IsExpanded: boolean read FIsExpanded write SetIsExpanded;
    property IsSelected: boolean read FIsSelected write SetIsSelected;
    property AutoTranslate  default true;
    property Font;
    property Resource;
    property Text;
    property TextAlign  default d2TextAlignNear;
  end;

  TOnCompareTreeViewItemEvent = function(Item1, Item2: TD2TreeViewItem): integer of object;
  TOnTreeViewDragChange = procedure (SourceItem, DestItem: TD2TreeViewItem; Allow: boolean) of object;

  TD2TreeView = class(TD2ScrollBox)
  private
    FMouseSelecting:boolean;
    FOnChange:TNotifyEvent;
    FSelected: TD2TreeViewItem;
    FItemHeight:single;
    FCountExpanded:integer;
    FHideSelectionUnfocused:boolean;
    FGlobalCount:integer;
    FShowCheckboxes:boolean;
    FOnChangeCheck:TNotifyEvent;
    FSorted:boolean;
    FOnCompare: TOnCompareTreeViewItemEvent;
    FMultiSelect:boolean;
    FFirstSelect: TD2TreeViewItem;
    FSelection: TD2VisualObject;
    FSelections: TList;
    FAllowDrag:boolean;
    FDragItem: TD2TreeViewItem;
    FOnDragChange: TOnTreeViewDragChange;
    FGlobalList: TList;
    procedure SetItemHeight(const Value:single);
    procedure SetShowCheckboxes(const Value:boolean);
    function GetItem(Index: integer): TD2TreeViewItem;
    procedure SetSorted(const Value:boolean);
    procedure SortItems;
    function GetSelection: TD2SelectionItem;
    procedure ClearSelection;
    procedure SelectAll;
    procedure SelectRange(Item1, Item2: TD2TreeViewItem);
    procedure UpdateSelection;
    procedure SetAllowDrag(const Value:boolean);
    function GetCount:integer;
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure SetSelected(const Value:TD2TreeViewItem);  virtual;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    procedure HScrollChange(Sender: TObject);  override;
    procedure VScrollChange(Sender: TObject);  override;
    function  GetContentBounds: TD2Rect;  override;
    procedure UpdateGlobalIndexes;
    procedure ContentAddObject(AObject: TD2Object);  override;
    procedure ContentRemoveObject(AObject: TD2Object);  override;
    function  GetItemRect(Item: TD2TreeViewItem): TD2Rect;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function ItemClass: string;  override;
    procedure EndUpdate;  override;
    procedure Clear;
    procedure ExpandAll;
    procedure CollapseAll;
    function  ItemByPoint(const X, Y:single): TD2TreeViewItem;
    function  ItemByIndex(const Idx: integer): TD2TreeViewItem;
    function  ItemByGlobalIndex(const Idx: integer): TD2TreeViewItem;
    procedure AddObject(AObject: TD2Object);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure KeyUp(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);  override;
    procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  override;
    property Count: integer read GetCount;
    property GlobalCount: integer read FGlobalCount;
    property CountExpanded: integer read FCountExpanded;
    property Selected: TD2TreeViewItem read FSelected write SetSelected;
    property Items[Index: integer]: TD2TreeViewItem read GetItem;
  published
    property Resource;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property AllowDrag: boolean read FAllowDrag write SetAllowDrag  default false;
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property HideSelectionUnfocused: boolean read FHideSelectionUnfocused write FHideSelectionUnfocused  default true;
    property MultiSelect: boolean read FMultiSelect write FMultiSelect  default false;
    property ShowCheckboxes: boolean read FShowCheckboxes write SetShowCheckboxes  default false;
    property Sorted: boolean read FSorted write SetSorted  default false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnChangeCheck:TNotifyEvent read FOnChangeCheck write FOnChangeCheck;
    property OnCompare: TOnCompareTreeViewItemEvent read FOnCompare write FOnCompare;
    property OnDragChange: TOnTreeViewDragChange read FOnDragChange write FOnDragChange;
  end;


TD2CustomTextBox = class(TD2Control)
  private
    FText: WideString;
    FFontFill: TD2Brush;
    FFont: TD2Font;
    FTextAlign: TD2TextAlign;
    FOnChange:TNotifyEvent;
    FReadOnly:boolean;
    FSelStart:integer;
    FSelLength:integer;
    FCaretPosition:integer;
    FMaxLength:integer;
    FFirstVisibleChar:integer;
    FLMouseSelecting:boolean;
    FDisableCaret:boolean;
    FPassword:boolean;
    FPopupMenu: TPopupMenu;
    FOnTyping:TNotifyEvent;
    FSelectionFill: TD2Brush;
    FOnChangeTracking:TNotifyEvent;
    procedure InsertText(const AText: WideString);
    function  GetSelLength:integer;
    function  GetSelStart:integer;
    function  GetSelText: WideString;
    procedure SetSelLength(const Value:integer);
    procedure SetSelStart(const Value:integer);
    function  GetSelRect: TD2Rect;
    procedure SetCaretPosition(const Value:integer);
    function  GetCoordinatePosition(x:single):integer;
    procedure SetMaxLength(const Value:Integer);
    function  GetNextWordBeging(StartPosition: integer):integer;
    function  GetPrivWordBeging(StartPosition: integer):integer;
    procedure UpdateFirstVisibleChar;
    procedure UpdateCaretePosition;
    procedure SetPassword(const Value:boolean);
    procedure CreatePopupMenu;
    procedure DoCopy(Sender: TObject);
    procedure DoCut(Sender: TObject);
    procedure DoDelete(Sender: TObject);
    procedure DoPaste(Sender: TObject);
    procedure UpdatePopupMenuItems;
    procedure DoSelectAll(Sender: TObject);
    procedure SetFont(const Value:TD2Font);
    procedure SetTextAlign(const Value:TD2TextAlign);
  protected
    FNeedChange:boolean;
    FFilterChar: WideString;
    FShowCaret:boolean;
    FLastKey: Word;
    FLastChar: System.WideChar;
    procedure ApplyStyle;  override;
    procedure Change;  virtual;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    function  GetPasswordCharWidth:single;
    function  TextWidth(const Str: WideString):single;
    function  GetTextW: WideString;  virtual; overload;                // 5555
    function  GetText:String;  virtual; overload;                     // 5555
    procedure SetText(const Value:String);  virtual; overload;       // 5555
    procedure SetTextW(const Value:WideString);  virtual; overload;   // 5555
    procedure FontChanged(Sender: TObject);  virtual;
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure KeyUp(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    procedure DblClick;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure ContextMenu(const ScreenPosition: TD2Point);  override;
    procedure ClearSelection;
    procedure CopyToClipboard;
    procedure CutToClipboard;
    procedure PasteFromClipboard;
    procedure SelectAll;
    function GetCharX(a: integer):single;
    function ContentRect: TD2Rect;
    property CaretPosition: integer read FCaretPosition write SetCaretPosition;
    property SelStart: integer read GetSelStart write SetSelStart;
    property SelLength: integer read GetSelLength write SetSelLength;
    property SelText: WideString read GetSelText;
    property MaxLength: Integer read FMaxLength write SetMaxLength  default 0;
    property ShowCaret: boolean read FShowCaret write FShowCaret  default true;
    property FontFill: TD2Brush read FFontFill;
    property SelectionFill: TD2Brush read FSelectionFill;
    property Password: boolean read FPassword write SetPassword;
    property TextW: WideString read FText write SetTextW;
    property Text : String read GetText write SetText;
    property FilterChar: WideString read FFilterChar write FFilterChar;
  published
    property Cursor  default crIBeam;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Font: TD2Font read FFont write SetFont;
    property TextAlign: TD2TextAlign read FTextAlign write SetTextAlign  default d2TextAlignNear;
    property Resource;
    property BindingSource;
    property ReadOnly: boolean read FReadOnly write FReadOnly;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnChangeTracking:TNotifyEvent read FOnChangeTracking write FOnChangeTracking;
    property OnTyping:TNotifyEvent read FOnTyping write FOnTyping;
  end;

  TD2TextBox = class(TD2CustomTextBox)
  private
  protected
  public
  published
    property Password;
    property Text;
  end;

  TD2NumberBox = class(TD2CustomTextBox)
  private
    FValue:single;
    FMin:single;
    FMax:single;
    FPressed:boolean;
    FPressedPos: TD2Point;
    FPressedVert:boolean;
    FPressedInc:single;
    FValueType: TD2ValueType;
    FHorzIncrement:single;
    FVertIncrement:single;
    FDecimalDigits:integer;
    procedure SetMax(const Value:single);
    procedure SetMin(const Value:single);
    procedure SetValue(const AValue:single);
    procedure SetValueType(const Value:TD2ValueType);
    procedure SetDecimalDigits(const Value:integer);
  protected
    procedure Change;  override;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure SetText(const Value:String);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure PaintChildren;  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property DecimalDigits: integer read FDecimalDigits write SetDecimalDigits  default 2;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property ShowCaret  default true;
    property Value:single read FValue write SetValue;
    property ValueType: TD2ValueType read FValueType write SetValueType;
    property HorzIncrement:single read FHorzIncrement write FHorzIncrement;
    property VertIncrement:single read FVertIncrement write FVertIncrement;
  end;

  TD2SpinBox = class(TD2CustomTextBox)
  private
    FValue:single;
    FMin:single;
    FMax:single;
    FValueType: TD2ValueType;
    FMinus, FPlus: TD2CustomButton;
    FIncrement:single;
    FDecimalDigits:integer;
    procedure SetMax(const Value:single);
    procedure SetMin(const Value:single);
    procedure SetValue(const AValue:single);
    procedure SetValueType(const Value:TD2ValueType);
    procedure SetDecimalDigits(const Value:integer);
  protected
    procedure SetText(const Value:String);  override;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure Change;  override;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure DoMinusClick(Sender: TObject);
    procedure DoPlusClick(Sender: TObject);
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property DecimalDigits: integer read FDecimalDigits write SetDecimalDigits  default 2;
    property Min:single read FMin write SetMin;
    property Max:single read FMax write SetMax;
    property Increment:single read FIncrement write FIncrement;
    property ShowCaret  default true;
    property Value:single read FValue write SetValue;
    property ValueType: TD2ValueType read FValueType write SetValueType;
    property TextAlign  default d2TextAlignCenter;
  end;

  TD2ComboTextBox = class(TD2CustomTextBox)
  private
    FDropDownCount:integer;
    FPopup: TD2Popup;
    FListBox: TD2ComboListBox;
    FPlacement: TD2Placement;
    FItems: TD2WideStrings;
    FItemHeight:single;
    procedure DoItemsChanged(Sender: TObject);
    procedure RebuildList;
    procedure SetItemHeight(const Value:single);
    procedure SetItems(const Value:TD2WideStrings);
    function  GetItemIndex:integer;
    procedure SetItemIndex(const Value:integer);
    function  GetCount:integer;
    function  GetListBoxResource: string;
    procedure SetListBoxResource(const Value:string);
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure ChangeParent;  override;
    procedure DoTyping(Sender: TObject);
    procedure DoClosePopup(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    procedure DropDown;
    property  ListBox: TD2ComboListBox read FListBox write FListBox;
    property  Count: integer read GetCount;
  published
    property Cursor  default crDefault;
    property DropDownCount: integer read FDropDownCount write FDropDownCount  default 8;
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property ItemIndex: integer read GetItemIndex write SetItemIndex;
    property Items: TD2WideStrings read FItems write SetItems;
    property ListBoxResource: string read GetListBoxResource write SetListBoxResource;
    property Text;
  end;

  TD2ComboTrackBar = class(TD2CustomTextBox)
  private
    FPopup: TD2Popup;
    FTrackBar: TD2TrackBar;
    FPlacement: TD2Placement;
    function GetFrequency:single;
    function GetMax:single;
    function GetMin:single;
    procedure SetFrequency(const Value:single);
    procedure SetMax(const Value:single);
    procedure SetMin(const Value:single);
    procedure SetValue(const AValue:single);
    function GetValue:single;
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure ChangeParent;  override;
    procedure DoTrackChange(Sender: TObject);  virtual;
    procedure DropDown;
    procedure DoClosePopup(Sender: TObject);
    procedure Change;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property TrackBar: TD2TrackBar read FTrackBar write FTrackBar;
  published
    property Min:single read GetMin write SetMin;
    property Max:single read GetMax write SetMax;
    property Value:single read GetValue write SetValue;
    property Frequency:single read GetFrequency write SetFrequency;
    property Text stored false;
  end;

  TD2TextBoxClearBtn = class(TD2CustomTextBox)
  private
    FClearBtn: TD2CustomButton;
  protected
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure DoClearBtnClick(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Password;
    property Text;
  end;

  TD2RoundTextBox = class(TD2TextBox)
  private
  protected
  public
  published
  end;

  TD2HudTextBox = class(TD2TextBox)
  private
  protected
  public
  published
  end;

  TD2HudRoundTextBox = class(TD2TextBox)
  private
  protected
  public
  published
  end;

  TD2HudNumberBox = class(TD2NumberBox)
  private
  protected
  public
  published
  end;

  TD2HudSpinBox = class(TD2SpinBox)
  private
  protected
  public
  published
  end;

  TD2HudComboTextBox = class(TD2ComboTextBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

  TD2HudComboTrackBar = class(TD2ComboTrackBar)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;


type

 TD2CustomMemo = class;
 //TD2Memo = class;

TEdtActionStack = class(TStack)
  private
    FOwner : TD2CustomMemo;
  public
    constructor Create(AOwner : TD2CustomMemo);
    destructor Destroy;  override;
    procedure FragmentInserted(StartPos, FragmentLength :integer; IsPairedWithPriv : boolean);
    procedure FragmentDeleted(StartPos :integer; Fragment : WideString);
    procedure CaretMovedBy(Shift : integer);
    function RollBackAction :boolean;
  end;

TD2MemoLines = class(TD2WideStrings)
  private
    FMemo: TD2CustomMemo;
  protected
    function Get(Index: Integer): WideString;  override;
    function GetCount: Integer;  override;
    procedure SetUpdateState(Updating: Boolean);  override;
  public
    procedure Clear;  override;
    procedure Delete(Index: Integer);  override;
    procedure Insert(Index: Integer; const S: WideString);  override;
  end;

//TD2Memo = class(TD2ScrollBox)            //Deletedby GoldenFox
TD2CustomMemo = class(TD2ScrollBox)        //Added by GoldenFox
  private
    FNeedChange:boolean;
    FText: WideString;
    FFontFill: TD2Brush;
    FFont: TD2Font;
    FTextAlign: TD2TextAlign;
    FInternalMustUpdateLines :boolean;
    FLMouseSelecting:boolean;
    FOldMPt : TD2Point;
    FCaretPosition: TCaretPosition;
    FFirstVisibleChar:integer;
    FUnwrapLines: TD2WideStrings;
    FPopupMenu: TPopupMenu;
    FAutoSelect:boolean;
    FCharCase: TEditCharCase;
    FHideSelection: Boolean;
    FMaxLength: Integer;
    FReadOnly: Boolean;
    FOnChange:TNotifyEvent;
    FTextAlignment: TAlignment;
    FActionStack : TEdtActionStack;
    FLines: TD2WideStrings;
    FWordWrap:boolean;
    FLinesBegs : array of integer;
    FSelStart: TCaretPosition;
    FSelEnd: TCaretPosition;
    FSelected :boolean;
    FOldSelStartPos, FOldSelEndPos, FOldCaretPos :integer;
    FSelectionFill: TD2Brush;
    FOnChangeTracking:TNotifyEvent;
    function GetSelBeg : TCaretPosition;
    function GetSelEnd : TCaretPosition;
    procedure StorePositions;
    procedure RestorePositions;
    procedure SelectAtPos(APos : TCaretPosition);
    procedure SetCaretPosition(const Value:TCaretPosition);
    procedure SetSelLength(const Value:integer);
    procedure SetSelStart(const Value:integer);
    function GetSelStart:integer;
    function GetSelLength:integer;
    procedure UpdateHScrlBarByCaretPos;
    procedure UpdateVScrlBarByCaretPos;
    function  GetSelText: WideString;
    procedure SetAutoSelect(const Value:boolean);
    procedure SetCharCase(const Value:TEditCharCase);
    procedure SetHideSelection(const Value:Boolean);
    procedure SetMaxLength(const Value:Integer);
    procedure SelectAtMousePoint;
    function  GetNextWordBeging(StartPosition: TCaretPosition): TCaretPosition;
    function  GetPrivWordBeging(StartPosition: TCaretPosition): TCaretPosition;
    function  GetPositionShift(APos : TCaretPosition; Delta: integer {char number}):TCaretPosition;
    procedure MoveCareteBy(Delta : integer);
    procedure MoveCaretLeft;
    procedure MoveCaretRight;
    procedure MoveCaretVertical(LineDelta : integer);
    procedure MoveCaretDown;
    procedure MoveCaretUp;
    procedure MoveCaretPageUp;
    procedure MoveCaretPageDown;
    procedure UpdateCaretPosition(UpdateScrllBars : boolean);
    procedure SetTextAlignment(const Value:TAlignment);
    procedure SetLines(const Value:TD2WideStrings);
    procedure GetLineBounds(LineIndex :integer; var LineBeg, LineLength : integer);
    function  GetLineCount :integer;
    function  GetLine(Index : integer) : WideString; //Returns Line without special symbols at the end.
    function  GetLineInternal(Index : integer) : WideString; //Returns Line with special symbols at the end.
    procedure InsertLine(Index: Integer; const S: WideString);
    procedure DeleteLine(Index: Integer);
    procedure ClearLines;
    procedure SetWordWrap(const Value:boolean);
    function  GetPageSize :single;
    function  GetLineWidth(LineNum: Integer):single;
    function  GetWidestLine :integer;
    function  FillLocalLinesBegs(PText: PWideString; ABegChar, AEndChar:integer;TmpLinesBegs: PLinesBegs) :integer;
    procedure UpdateRngLinesBegs(PText : PWideString;AUpdBegLine, AUpdEndLine, AUpdBegChar, AUpdEndChar, ACharDelta, AOldWideslLineWidth : integer);
    function  GetShowSelection :boolean;
    function  GetLineRealEnd(AStartPos: TCaretPosition; PText: PWideString): TCaretPosition;
    procedure SetFont(const Value:TD2Font);
    procedure SetTextAlign(const Value:TD2TextAlign);
    function  TextWidth(const Str: WideString):single;
    procedure HScrlBarChange(Sender: TObject);
    procedure SetUpdateState(Updating: Boolean);
    function  GetUnwrapLines: TD2WideStrings;
  protected
    FUpdating:boolean;
    FWidesLineIndex :integer;
    FTextWidth : array of integer;
    function  GetLineHeight :single;
    function  GetPointPosition(Pt : TD2Point): TCaretPosition;
    function  GetText:WideString;  virtual; overload;              // 5555
    function  GetText:String;  virtual; overload;                  // 5555
    procedure SetText(const Value:String);  virtual; overload;    // 5555
    procedure SetText(const Value:WideString);  virtual; overload;// 5555
    function  GetSelArea: TSelArea;  virtual;
    procedure DrawPasswordChar(SymbolRect: TD2Rect; Selected: boolean);  virtual;
    procedure CreatePopupMenu;  virtual;
    procedure UpdatePopupMenuItems;  virtual;
    procedure ApplyStyle;  override;
    function  ContentPos: TD2Point;
    procedure Change;  virtual;
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    function  ValidText(NewText: WideString):boolean;  virtual;
    function  CanAutoSize(var NewWidth, NewHeight: Integer): Boolean; //override;
    procedure ContextMenu(const ScreenPosition: TD2Point);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure SelectWord;
    procedure FontChanged(Sender: TObject);
    procedure DoUndo(Sender: TObject);
    procedure DoCut(Sender: TObject);
    procedure DoCopy(Sender: TObject);
    procedure DoPaste(Sender: TObject);
    procedure DoDelete(Sender: TObject);
    procedure DoSelectAll(Sender: TObject);
    procedure UpdateLines;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    procedure VScrollChange(Sender: TObject);
    function  GetContentBounds: TD2Rect;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; x, y:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; x, y:single);  override;
    procedure MouseMove(Shift: TShiftState; x, y, dx, dy:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
    procedure CopyToClipboard;
    procedure PasteFromClipboard;
    procedure CutToClipboard;
    procedure ClearSelection;
    procedure SelectAll;
    procedure GoToTextEnd;
    procedure GoToTextBegin;
    procedure GotoLineEnd;
    procedure GoToLineBegin;
    function  GetPositionPoint(ACaretPos : TCaretPosition): TD2Point;
    procedure UnDo;
    procedure InsertAfter(Position: TCaretPosition; S: WideString; Options : TInsertOptions);
    procedure DeleteFrom(Position: TCaretPosition; ALength :integer; Options : TDeleteOptions);
    function TextPosToPos(APos: integer): TCaretPosition;
    function PosToTextPos(APostion: TCaretPosition):integer;
    property SelStart: integer read GetSelStart write SetSelStart;
    property SelLength: integer read GetSelLength write SetSelLength;
    property SelText: WideString read GetSelText;
    property CaretPosition: TCaretPosition read FCaretPosition write SetCaretPosition;
    property LineWidth[LineNum: Integer]:single read GetLineWidth;
    property UnwrapLines: TD2WideStrings read GetUnwrapLines;
    property FontFill: TD2Brush read FFontFill;
    property SelectionFill: TD2Brush read FSelectionFill;
    property TextW: WideString read FText write SetText;
    property Text: String read GetText write SetText stored true;  //Added by GoldenFox
  published
    property Cursor  default crIBeam;
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property AutoSelect: boolean read FAutoSelect write SetAutoSelect  default true;
    property CharCase: TEditCharCase read FCharCase write SetCharCase  default d2ecNormal;
    property Enabled;
    property HideSelection: Boolean read FHideSelection write SetHideSelection  default True;
    property Lines: TD2WideStrings read FLines write SetLines stored false;
    property MaxLength: Integer read FMaxLength write SetMaxLength  default 0;
    property ReadOnly: Boolean read FReadOnly write FReadOnly  default False;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnChangeTracking:TNotifyEvent read FOnChangeTracking write FOnChangeTracking;
    property WordWrap: boolean read FWordWrap write SetWordWrap;
    property Font: TD2Font read FFont write SetFont;
    //property Text: String read GetText write SetText stored true;  //Deleted by GoldenFox
    property TextAlign: TD2TextAlign read FTextAlign write SetTextAlign  default d2TextAlignNear;
    property Resource;
  end;

TD2Memo = class(TD2CustomMemo)
  private
  protected
  public
  published
    property Text;
end;

TD2HudMemo = class(TD2Memo)
  private
  protected
  public
  published
end;

type

TD2BitmapTrackBar = class(TD2TrackBar)
  private
    FBitmap: TD2Bitmap;
    FBackground: TD2Shape;
  protected
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure UpdateBitmap;
    procedure FillBitmap;  virtual;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
  published
  end;

TD2HueTrackBar = class(TD2BitmapTrackBar)
  private
  protected
    procedure FillBitmap;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2AlphaTrackBar = class(TD2BitmapTrackBar)
  private
  protected
    procedure FillBitmap;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2BWTrackBar = class(TD2BitmapTrackBar)
  private
  protected
    procedure FillBitmap;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2ColorBox = class(TD2Control)
  private
    FColor: TD2Color;
    procedure SetColor(const Value:TD2Color);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
    property Color: TD2Color read FColor write SetColor;
  published
  end;

TD2ColorQuad = class(TD2Control)
  private
    FColorBox: TD2ColorBox;
    FColorBitmap: TD2Bitmap;
    FHue:single;
    FSat:single;
    FLum:single;
    FOnChange:TNotifyEvent;
    FAlpha:single;
    procedure SetHue(const Value:single);
    procedure SetLum(const Value:single);
    procedure SetSat(const Value:single);
    procedure SetAlpha(const Value:single);
    procedure SetColorBox(const Value:TD2ColorBox);
  protected
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    function GetAbsoluteRect: TD2Rect;  override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    function  pointInObject(X, Y:single):boolean;  override;
  published
    property Hue:single read FHue write SetHue;
    property Lum:single read FLum write SetLum;
    property Sat:single read FSat write SetSat;
    property Alpha:single read FAlpha write SetAlpha;
    property ColorBox: TD2ColorBox read FColorBox write SetColorBox;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2ColorPicker = class(TD2Control)
  private
    FHueBitmap: TD2Bitmap;
    FHue:single;
    FColorQuad: TD2ColorQuad;
    procedure SetHue(const Value:single);
    function GetColor: TD2Color;
    procedure SetColor(const Value:TD2Color);
  protected
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    function GetAbsoluteRect: TD2Rect;  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    function pointInObject(X, Y:single):boolean;  override;
    property Color: TD2Color read GetColor write SetColor;
  published
    property Hue:single read FHue write SetHue;
    property ColorQuad: TD2ColorQuad read FColorQuad write FColorQuad;
  end;

TD2GradientEdit = class(TD2Control)
  private
    FBitmap: TD2Bitmap;
    FGradient: TD2Gradient;
    FCurrentPoint:integer;
    FCurrentPointInvisible:boolean;
    FMoving:boolean;
    FOnChange:TNotifyEvent;
    FOnSelectPoint:TNotifyEvent;
    FColorPicker: TD2ColorPicker;
    procedure SetGradient(const Value:TD2Gradient);
    function GetPointRect(const Point: integer): TD2Rect;
    procedure DoChanged(Sender: TObject);
    procedure SetCurrentPoint(const Value:integer);
    procedure SetColorPicker(const Value:TD2ColorPicker);
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Paint;  override;
    procedure UpdateGradient;
    property Gradient: TD2Gradient read FGradient write SetGradient;
    property CurrentPoint: integer read FCurrentPoint write SetCurrentPoint;
  published
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnSelectPoint:TNotifyEvent read FOnSelectPoint write FOnSelectPoint;
    property ColorPicker: TD2ColorPicker read FColorPicker write SetColorPicker;
  end;

TD2ColorPanel = class(TD2Control)
  private
    FOnChange:TNotifyEvent;
    FColorQuad: TD2ColorQuad;
    FAlphaTrack: TD2AlphaTrackBar;
    FHueTrack: TD2HueTrackBar;
    FColorBox: TD2ColorBox;
    FUseAlpha:boolean;
    function GetColor: string;
    procedure SetColor(const Value:string);
    procedure SetColorBox(const Value:TD2ColorBox);
    procedure SetUseAlpha(const Value:boolean);
  protected
    procedure DoAlphaChange(Sender: TObject);
    procedure DoHueChange(Sender: TObject);
    procedure DoQuadChange(Sender: TObject);
    procedure Notification(AComponent: TComponent;
      Operation: TOperation);  override;
    procedure Loaded;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property Color: string read GetColor write SetColor;
    property ColorBox: TD2ColorBox read FColorBox write SetColorBox;
    property UseAlpha: boolean read FUseAlpha write SetUseAlpha  default true;
  end;

TD2ComboColorBox = class(TD2Control)
  private
    FPopup: TD2Popup;
    FColorPanel: TD2ColorPanel;
    FColorBox: TD2ColorBox;
    FColorText: TD2TextBox;
    FPlacement: TD2Placement;
    FOnChange:TNotifyEvent;
    function GetValue:string;
    procedure SetValue(const Value:string);
    function GetUseAlpha:boolean;
    procedure SetUseAlpha(const Value:boolean);
  protected
    procedure ApplyStyle;  override;
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure ChangeParent;  override;
    procedure DoColorChange(Sender: TObject);  virtual;
    procedure DoTextChange(Sender: TObject);  virtual;
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure DropDown;
  published
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Color: string read GetValue write SetValue;
    property UseAlpha: boolean read GetUseAlpha write SetUseAlpha  default true;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2HudHueTrackBar = class(TD2HueTrackBar)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2HudAlphaTrackBar = class(TD2AlphaTrackBar)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2HudBWTrackBar = class(TD2BitmapTrackBar)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2HudComboColorBox = class(TD2ComboColorBox)
  private
  protected
  public
    constructor Create(AOwner: TComponent);  override;
  published
  end;

TD2TabItem = class(TD2TextControl)
  private
    FIndex:integer;
    FLayout: TD2VisualObject;
    FIsSelected:boolean;
    procedure SetIndex(const Value:integer);
  protected
    procedure ApplyStyle;  override;
    procedure DesignSelect;  override;
    procedure DesignInsert;  override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation);  override;
    procedure SetVisible(const Value:boolean);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure Realign;  override;
    procedure Select(ASelected: boolean);
  published
    property IsSelected: boolean read FIsSelected;
    property AutoTranslate  default true;
    property Font;
    property Index: integer read FIndex write SetIndex;
    property TextAlign;
    property VertTextAlign;
    property Text;
    property Layout: TD2VisualObject read FLayout write FLayout;
    property Resource;
  end;

TD2HackTabItem = class(TD2TabItem);

TD2TabControl = class(TD2Control)
  private
    FItemIndex:integer;
    FOnChange:TNotifyEvent;
    FItemHeight:single;
    FFullSize:boolean;
    FBackground: TD2VisualObject;
    procedure SetItemIndex(const Value:integer);
    procedure SetItemHeight(const Value:single);
    procedure SetFullSize(const Value:boolean);
  protected
    function TabItem(AIndex: integer): TD2TabItem;
    function TabCount:integer;
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure PaintChildren;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function ItemClass: string;  override;
    procedure Realign;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
    procedure KeyDown(var Key: Word; var KeyChar: WideChar; Shift: TShiftState); //override;
    procedure KeyUp(var Key: Word; var KeyChar: WideChar; Shift: TShiftState); //override;
    procedure SetItem(AItem: TD2TabItem);
  published
    property Resource;
    property FullSize: boolean read FFullSize write SetFullSize  default false;
    property ItemIndex: integer read FItemIndex write SetItemIndex  default -1;
    property ItemHeight:single read FItemHeight write SetItemHeight;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2HudTabItem = class(TD2TabItem)
  private
  public
  end;

TD2HudTabControl = class(TD2TabControl)
  private
  protected
  public
    function ItemClass: string;  override;
  published
  end;

TD2IPhoneButton = class(TD2BitmapButton)
  private
    FBackground: TD2Brush;
    procedure SetBackground(const Value:TD2Brush);
  protected
    procedure BackChanged(Sender: TObject);
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Background: TD2Brush read FBackground write SetBackground;
    property Resource;
  end;

TD2DockBar = class(TD2Control)
  private
    FMousePos: TD2Point;
    FMaxSize:single;
    FMinSize:single;
    //FAmplitude:single;
    procedure SetMaxSize(const Value:single);
    procedure SetMinSize(const Value:single);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseLeave;  override;
    procedure Paint;  override;
  published
    property MinSize:single read FMinSize write SetMinSize;
    property MaxSize:single read FMaxSize write SetMaxSize;
    property Resource;
  end;

TD2DropTarget = class(TD2TextControl)
  private
    FOnDrop: TD2DragDropEvent;
    FFilter: string;
  protected
    procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);  override;
    procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Filter: string read FFilter write FFilter;
    property Font;
    property Text;
    property OnDroped: TD2DragDropEvent read FOnDrop write FOnDrop;
  end;

TD2PlotGrid = class(TD2VisualObject)
  private
    FMarks:single;
    FFrequency:single;
    FLineFill: TD2Brush;
    procedure SetFrequency(const Value:single);
    procedure SetMarks(const Value:single);
    procedure SetLineFill(const Value:TD2Brush);
    procedure LineFillChanged(Sender: TObject);
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property LineFill: TD2Brush read FLineFill write SetLineFill;
    property Marks:single read FMarks write SetMarks;
    property Frequency:single read FFrequency write SetFrequency;
  end;

TD2ImageViewer = class(TD2ScrollBox)
  private
    FBack: TD2Rectangle;
    FImage: TD2Image;
    FScale:single;
    FMouseScaling:boolean;
    FShowBackground:boolean;
    function GetBitmap: TD2Bitmap;
    procedure SetBitmap(const Value:TD2Bitmap);
    procedure SetScale(const Value:single);
    function GetBackgroundFill: TD2Brush;
    procedure SetBackgroundFill(const Value:TD2Brush);
    procedure SetShowBackground(const Value:boolean);
  protected
    function GetContentBounds: TD2Rect;  override;
    procedure DoBitmapChange(Sender: TObject);
    function GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
    procedure BestFit;
  published
    property BackgroundFill: TD2Brush read GetBackgroundFill write SetBackgroundFill;
    property Bitmap: TD2Bitmap read GetBitmap write SetBitmap;
    property BitmapScale:single read FScale write SetScale;
    property ShowBackground: boolean read FShowBackground write SetShowBackground  default false;
    property MouseScaling: boolean read FMouseScaling write FMouseScaling  default true;
  end;


TD2Calendar = class(TD2Control)
  private
    FDateTime: TDateTime;
    FDays: TD2ListBox;
    FToday, FPrev, FNext: TD2Button;
    FMonths: TD2PopupBox;
    FYears: TD2PopupBox;
    FWeeks: TD2GridLayout;
    FFirstDayOfWeek: TD2CalDayOfWeek;
    FFirstDayOfWeekNum:integer;
    FWeek: TD2GridLayout;
    FTodayDefault: Boolean;
    FOnChange:TNotifyEvent;
    FWeekNumbers: Boolean;
    FOnDayChange:TNotifyEvent;
    function  GetDate: TDate;
    procedure SetDate(Value:TDate);
    procedure SetDateTime(const Value:TDateTime);
    procedure SetFirstDayOfWeek(const Value:TD2CalDayOfWeek);
    procedure SetTodayDefault(const Value:Boolean);
    procedure SetWeekNumbers(const Value:Boolean);
  protected
    FDisableDayChange:integer;
    procedure DoPrevClick(Sender: TObject);
    procedure DoNextClick(Sender: TObject);
    procedure DoTodayClick(Sender: TObject);
    procedure DoDayChange(Sender: TObject);
    procedure DoMonthChange(Sender: TObject);
    procedure DoYearChange(Sender: TObject);
    procedure FillList;
    function  GetData: Variant;  override;
    procedure SetData(const Value:Variant);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta:integer; var Handled: boolean);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    property  DateTime: TDateTime read FDateTime write SetDateTime;
  published
    property BindingSource;
    property Date: TDate read GetDate write SetDate;
    property FirstDayOfWeek: TD2CalDayOfWeek read FFirstDayOfWeek write SetFirstDayOfWeek  default d2LocaleDefault;
    property TodayDefault: Boolean read FTodayDefault write SetTodayDefault  default false;
    property WeekNumbers: Boolean read FWeekNumbers write SetWeekNumbers  default false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnDayChange:TNotifyEvent read FOnDayChange write FOnDayChange;
  end;

TD2CalendarBox = class(TD2TextControl)
  private
    FPopup: TD2Popup;
    FCalendar: TD2Calendar;
    FPlacement: TD2Placement;
    function  GetDate: TDate;
    procedure SetDate(const Value:TDate);
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure DoClosePopup(Sender: TObject);
    procedure DoCalendarChanged(Sender: TObject);
    procedure DoDayChanged(Sender: TObject);
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure DropDown;
    property Calendar: TD2Calendar read FCalendar;
  published
    property CanFocused  default true;
    property DisableFocusEffect;
    property TabOrder;
    property Cursor  default crDefault;
    property Date: TDate read GetDate write SetDate;
    property TextAlign  default d2TextAlignNear;
  end;

TD2CalendarTextBox = class(TD2CustomTextBox)
  private
    FPopup: TD2Popup;
    FCalendar: TD2Calendar;
    FPlacement: TD2Placement;
    function GetDate: TDate;
    procedure SetDate(const Value:TDate);
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure DoClosePopup(Sender: TObject);
    procedure DoCalendarChanged(Sender: TObject);
    procedure DoDayChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure DropDown;
    property Calendar: TD2Calendar read FCalendar;
  published
    property Cursor  default crDefault;
    property Date: TDate read GetDate write SetDate;
  end;


TD2CompoundTrackBar = class(TD2Control)
  private
    FValueLabel: TD2Label;
    FTextLabel: TD2Label;
    FTrackBar: TD2TrackBar;
    FDecimalDigits:integer;
    FOnChange:TNotifyEvent;
    FSuffix: WideString;
    function GetValue:single;
    procedure SetValue(const Value:single);
    procedure SetDecimalDigits(const Value:integer);
    procedure SetSuffix(const Value:WideString);
  protected
    procedure DoTrack(Sender: TObject);
    procedure DoTracking(Sender: TObject);
    procedure UpdateLabel;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property DecimalDigits: integer read FDecimalDigits write SetDecimalDigits  default 2;
    property TextLabel: TD2Label read FTextLabel;
    property TrackBar: TD2TrackBar read FTrackBar;
    property ValueLabel: TD2Label read FValueLabel;
    property Suffix: WideString read FSuffix write SetSuffix;
    property Value:single read GetValue write SetValue stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundAngleBar = class(TD2Control)
  private
    FValueLabel: TD2Label;
    FTextLabel: TD2Label;
    FAngleBar: TD2AngleButton;
    FDecimalDigits:integer;
    FOnChange:TNotifyEvent;
    function  GetValue:single;
    procedure SetValue(const Value:single);
  protected
    procedure DoChange(Sender: TObject);
    procedure UpdateLabel;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property TextLabel: TD2Label read FTextLabel;
    property AngleButton: TD2AngleButton read FAngleBar;
    property ValueLabel: TD2Label read FValueLabel;
    property Value:single read GetValue write SetValue stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundTextBox = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FTextBox: TD2TextBox;
    FOnChange:TNotifyEvent;
    function  GetText: WideString; overload;              // 5555
    function  GetText: String; overload;                  // 5555
    procedure SetText(const Value:WideString); overload; // 5555
    procedure SetText(const Value:String); overload;     // 5555
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    property ValueW: WideString read GetText write SetText;
  published
    property TextLabel: TD2Label read FTextLabel;
    property TextBox: TD2TextBox read FTextBox;
    property Value:String read GetText write SetText stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundMemo = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FMemo: TD2Memo;
    FOnChange:TNotifyEvent;
    function  GetText: WideString; overload;              // 5555
    function  GetText: String; overload;                  // 5555
    procedure SetText(const Value:WideString); overload; // 5555
    procedure SetText(const Value:String); overload;     // 5555
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    property ValueW: WideString read GetText write SetText stored false;
  published
    property TextLabel: TD2Label read FTextLabel;
    property Memo: TD2Memo read FMemo;
    property Value:String read GetText write SetText stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundNumberBox = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FNumberBox: TD2NumberBox;
    FOnChange:TNotifyEvent;
    function GetValue:single;
    procedure SetValue(const Value:single);
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property TextLabel: TD2Label read FTextLabel;
    property NumberBox: TD2NumberBox read FNumberBox;
    property Value:single read GetValue write SetValue stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundPopupBox = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FPopupBox: TD2PopupBox;
    FOnChange:TNotifyEvent;
    function GetItemIndex:integer;
    procedure SetItemIndex(const Value:integer);
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property TextLabel: TD2Label read FTextLabel;
    property PopupBox: TD2PopupBox read FPopupBox;
    property Value:integer read GetItemIndex write SetItemIndex;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundColorButton = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FColorButton: TD2ColorButton;
    FOnChange:TNotifyEvent;
    function GetValue:string;
    procedure SetValue(const Value:string);
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property TextLabel: TD2Label read FTextLabel;
    property ColorButton: TD2ColorButton read FColorButton;
    property Value:string read GetValue write SetValue stored false;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2CompoundImage = class(TD2Control)
  private
    FTextLabel: TD2Label;
    FImage: TD2ImageControl;
    FOnChange:TNotifyEvent;
  protected
    procedure DoChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property TextLabel: TD2Label read FTextLabel;
    property Image: TD2ImageControl read FImage;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  end;

TD2ImageList = class(TCustomImageList)
  private
    FImages: TList;
    function GetCount:integer;
    function GetBitmap(Index: integer): TD2Bitmap;
    procedure UpdateList;
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadImage(Stream: TStream);
    procedure WriteImage(Stream: TStream);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function Add(Bitmap: TD2Bitmap):integer;
    procedure Clear;
    procedure ReadData(Stream: TStream);  override;
    procedure WriteData(Stream: TStream);  override;
    property Images[Index: integer]: TD2Bitmap read GetBitmap;
    property Count: integer read GetCount;
  published
    property Height  default 32;
    property Width  default 32;
  end;


TD2BrushDesign = class(TForm)
    panelSolid: TD2Rectangle;
    panelGradient: TD2Rectangle;
    solidQuad: TD2ColorQuad;
    solidPicker: TD2ColorPicker;
    gradQuad: TD2ColorQuad;
    d2BrushDesigner: TD2Scene;
    solidCont: TD2Rectangle;
    gradEditor: TD2GradientEdit;
    dsgnRoot: TD2Background;
    Layout1: TD2Layout;
    ext1: TD2Label;
    Layout2: TD2Layout;
    Layout3: TD2Layout;
    gradPicker: TD2ColorPicker;
    brushTabControl: TD2TabControl;
    tabNone: TD2TabItem;
    tabSolid: TD2TabItem;
    tabGradient: TD2TabItem;
    Text1: TD2Label;
    Text2: TD2Label;
    Text3: TD2Label;
    brushList: TD2ListBox;
    textSolidR: TD2NumberBox;
    textSolidG: TD2NumberBox;
    textSolidB: TD2NumberBox;
    textSolidA: TD2NumberBox;
    textGradR: TD2NumberBox;
    textGradG: TD2NumberBox;
    textGradB: TD2NumberBox;
    textGradA: TD2NumberBox;
    textSolidHex: TD2TextBox;
    textGradHex: TD2TextBox;
    gradColorRect: TD2ColorBox;
    solidColorRect: TD2ColorBox;
    tabBitmap: TD2TabItem;
    panelBitmap: TD2Layout;
    tabRes: TD2TabItem;
    panerRes: TD2Layout;
    bitmapImage: TD2Image;
    Layout5: TD2Layout;
    btnSelectBitmap: TD2Button;
    resList: TD2ListBox;
    Layout6: TD2Layout;
    btnMakeRes: TD2Button;
    Label1: TD2Label;
    Rectangle1: TD2Rectangle;
    tileModeList: TD2PopupBox;
    btnCancel: TD2Button;
    btnOK: TD2Button;
    makeResLayout: TD2Layout;
    gradAngle: TD2AngleButton;
    gradAlabel: TD2Label;
    gradKind: TD2PopupBox;
    gradAngleLabel: TD2Label;
    HudWindow1: TD2HudWindow;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure solidQuadChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure gradEditorChange(Sender: TObject);
    procedure gradQuadChange(Sender: TObject);
    procedure brushListChange(Sender: TObject);
    procedure brushTabControlChange(Sender: TObject);
    procedure textGradRChange(Sender: TObject);
    procedure textGradHexChange(Sender: TObject);
    procedure textSolidHexChange(Sender: TObject);
    procedure btnSelectBitmapClick(Sender: TObject);
    procedure btnMakeResClick(Sender: TObject);
    procedure resListChange(Sender: TObject);
    procedure tileModeListChange(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure textSolidRChange(Sender: TObject);
    procedure gradAngleChange(Sender: TObject);
    procedure gradKindChange(Sender: TObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); //override;
    function UniqueName(S: string): string; //override;
  private
    FBrush: TD2Brush;
    FScene: Id2Scene;
    FComp: TPersistent;
    procedure SetBrush(const Value:TD2Brush);
    procedure SetComp(const Value:TPersistent);
    procedure rebuilResList;
  public
    property Comp: TPersistent read FComp write SetComp;
    property Brush: TD2Brush read FBrush write SetBrush;
  end;

TD2BrushStyles = set of TD2BrushStyle;

TD2BrushDialog = class(TComponent)
  private
    FShowStyles: TD2BrushStyles;
    FShowBrushList:boolean;
    FShowMakeResource:boolean;
    FBrush: TD2Brush;
    FComponent: TComponent;
    FTitle: WideString;
    procedure SetBrush(const Value:TD2Brush);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function Execute:boolean;
    property Brush: TD2Brush read FBrush write SetBrush;
    property Component: TComponent read FComponent write FComponent;
  published
    property ShowStyles: TD2BrushStyles read FShowStyles write FShowStyles;
    property ShowBrushList: boolean read FShowBrushList write FShowBrushList  default true;
    property ShowMakeResource: boolean read FShowMakeResource write FShowMakeResource;
    property Title: WideString read FTitle write FTitle;
  end;

TD2BitmapEditor = class(TForm)
    d2Scene1  : TD2Scene;
    Root1     : TD2Background;
    Button1   : TD2Button;
    Button2   : TD2Button;
    Button3   : TD2Button;
    Button4   : TD2Button;
    Button5   : TD2Button;
    Layout1   : TD2Layout;
    Layout2   : TD2Layout;
    Label1    : TD2Label;
    Label2    : TD2Label;
    btnOk     : TD2Button;
    ScrollBox1: TD2ScrollBox;
    Rectangle1: TD2Panel;
    Preview   : TD2PaintBox;
    labelScale: TD2Label;
    trackScale: TD2Track;
    cropButton: TD2Button;
    Image1    : TD2Image;
    btnPaste  : TD2Button;
    btnFit    : TD2Button;
    btnOriginal: TD2Button;
    editControl: TD2Control;
    GroupBox1 : TD2GroupBox;
    newWidth  : TD2NumberBox;
    newHeight : TD2NumberBox;
    resizeLayout: TD2Background;
    ShadowEffect1: TD2ShadowEffect;
    btnResize : TD2Button;
    btnSave   : TD2Button;
    SaveDialog1 : TSaveDialog;
    procedure Button1Click(Sender : TObject);
    procedure Button2Click(Sender : TObject);
    procedure Button3Click(Sender : TObject);
    procedure Button4Click(Sender : TObject);
    procedure Button5Click(Sender : TObject);
    procedure PreviewPaint(Sender : TObject; const Canvas: TD2Canvas);
    procedure trackScaleChange(Sender : TObject);
    procedure cropButtonClick(Sender : TObject);
    procedure btnOkClick(Sender : TObject);
    procedure trackScaleMouseDown(Sender : TObject; Button: TMouseButton;  Shift: TShiftState; X, Y: Single);
    procedure btnPasteClick(Sender : TObject);
    procedure btnFitClick(Sender : TObject);
    procedure btnResizeClick(Sender : TObject);
    procedure newWidthChange(Sender : TObject);
    procedure newHeightChange(Sender : TObject);
    procedure btnSaveClick(Sender : TObject);
    procedure FormCreate(Sender : TObject);
    procedure FormDestroy(Sender : TObject);
    procedure btnOriginalClick(Sender : TObject);
  private
    FBitmap: TD2Bitmap;
    FSourceRect: TD2Rect;
    FCropRect: TD2Selection;
    FOldScale:single;
    FFileName: string;
  public
    procedure AssignFromBitmap(B: TD2Bitmap);
    procedure AssignToBitmap(B: TD2Bitmap);
    property FileName: string read FFileName write FFileName;
  end;

TfrmDsgnImageList = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    HudWindow1: TD2HudWindow;
    ImageList: TD2ImageListBox;
    btnAddFiles: TD2HudButton;
    OpenDialog1: TOpenDialog;
    HudButton1: TD2HudButton;
    btnDelete: TD2HudButton;
    btnClear: TD2HudButton;
    btnCancel: TD2HudButton;
    procedure btnAddFilesClick(Sender: TObject);
    procedure btnClearClick(Sender: TObject);
    procedure btnDeleteClick(Sender: TObject);
  private
  public
  end;

TD2OnChangeProperty = procedure (Sender: TObject; PropertyName: string) of object;

TD2Inspector = class(TD2TreeView)
  private
    FShowEvents       :boolean;
    FShowProperties   :boolean;
    FEditButton       : TD2Button;
    FComboBox         : TD2PopupBox;
    FSelectedObject   : TComponent;
    FEditBox          : TD2TextBox;
    FDisabledProperties : TStrings;
    FOnChangeProperty : TD2OnChangeProperty;
    procedure SetDisabledProperties(const Value:TStrings);
    procedure SetSelectedObject(const Value:TComponent);
    procedure SetShowEvents(const Value:boolean);
    procedure SetShowProperties(const Value:boolean);
    procedure UpdateEditorPos;
    procedure RebuildList;
    procedure RebuildEditor;
  protected
    function Editor: TD2Control;
    procedure InsAddObject(ItemRoot: TD2Object; Root: TObject);
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DialogKey(var Key: Word; Shift: TShiftState);  override;
    procedure SetSelected(const Value:TD2TreeViewItem);  override;
    procedure VScrollChange(Sender: TObject);  override;
    procedure DoEditorChange(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property DisabledProperties : TStrings read FDisabledProperties write SetDisabledProperties;
    property SelectedObject : TComponent read FSelectedObject write SetSelectedObject;
    property ShowProperties : boolean read FShowProperties write SetShowProperties  default true;
    property ShowEvents : boolean read FShowEvents write SetShowEvents  default false;
    property OnChangeProperty : TD2OnChangeProperty read FOnChangeProperty write FOnChangeProperty;
  end;

TD2LangDesigner = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2HudWindow;
    d2Resources1: TD2Resources;
    OriginalList: TD2HudListBox;
    btnAddItem: TD2HudCornerButton;
    langList: TD2HudPopupBox;
    ToolBar1: TD2ToolBar;
    inputLang: TD2HudTextBox;
    HudLabel1: TD2HudLabel;
    layoutSelect: TD2Layout;
    HudLabel2: TD2HudLabel;
    btnAddNewLang: TD2HudButton;
    btnCancalAdd: TD2HudButton;
    layoutAdd: TD2Layout;
    layoutAddText: TD2Layout;
    btnAddText: TD2HudButton;
    btnCancelAddText: TD2HudButton;
    inputAddText: TD2HudTextBox;
    btnRemoveItem: TD2HudCornerButton;
    btnCollect: TD2HudCornerButton;
    SaveDialog1: TSaveDialog;
    OpenDialog1: TOpenDialog;
    btnAddLang: TD2HudButton;
    btnLoadTxt: TD2HudButton;
    btnCreateTemplate: TD2HudCornerButton;
    btnLoadLng: TD2HudCornerButton;
    btnSaveLng: TD2HudCornerButton;
    OpenDialog2: TOpenDialog;
    SaveDialog2: TSaveDialog;
    procedure btnAddClick(Sender: TObject);
    procedure btnAddLangClick(Sender: TObject);
    procedure langListChange(Sender: TObject);
    procedure btnAddItemClick(Sender: TObject);
    procedure btnRemoveItemClick(Sender: TObject);
    procedure btnAddNewLangClick(Sender: TObject);
    procedure btnCancalAddClick(Sender: TObject);
    procedure btnCancelAddTextClick(Sender: TObject);
    procedure btnAddTextClick(Sender: TObject);
    procedure btnCollectClick(Sender: TObject);
    procedure btnCreateTemplateClick(Sender: TObject);
    procedure btnLoadTxtClick(Sender: TObject);
    procedure btnLoadLngClick(Sender: TObject);
    procedure btnSaveLngClick(Sender: TObject);
  private
    FLang: TD2Lang;
    //FCurLang: WideString;
    procedure RebuildOriginalList;
    procedure DoTranslateChanged(Sender: TObject);
  public
  end;


TD2PathDataDesigner = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    previewLayout: TD2Layout;
    Layout2: TD2Layout;
    PathData: TD2Memo;
    Button2: TD2Button;
    Button3: TD2Button;
    labelMemo: TD2Label;
    Label3: TD2Label;
    previewPath: TD2Path;
    Button1: TD2Button;
    procedure PathDataChange(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
  public
  end;


TD2StyleDesigner = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    ObjectsTree: TD2TreeView;
    d2Scene2: TD2Scene;
    Root2: TD2Background;
    Inspector: TD2Inspector;
    DesignScene: TD2Scene;
    d2Scene3: TD2Scene;
    Root4: TD2Background;
    OpenDialog1: TOpenDialog;
    Button1: TD2Button;
    SaveDialog1: TSaveDialog;
    Button2: TD2Button;
    btnClear: TD2Button;
    btnLoadDefault: TD2Button;
    btnBack: TD2Button;
    rectBack: TD2Rectangle;
    d2BrushDialog1: TD2BrushDialog;
    Button3: TD2Button;
    Button4: TD2Button;
    btnCancel: TD2Button;
    ClearTimer: TTimer;
    Resources1: TD2Resources;
    textFilter: TD2TextBoxClearBtn;
    Splitter1: TSplitter;
    Splitter2: TSplitter;
    d2Scene4: TD2Scene;
    Root3: TD2Background;
    StatusBar1: TD2StatusBar;
    Label1: TD2Label;
    procedure ObjectsTreeChange(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure btnClearClick(Sender: TObject);
    procedure btnLoadDefaultClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnBackClick(Sender: TObject);
    procedure ObjectsTreeMouseUp(Sender: TObject; Button: TMouseButton;  Shift: TShiftState; X, Y: Single);
    procedure ClearTimerTimer(Sender: TObject);
    procedure ObjectsTreeDragChange(SourceItem, DestItem: TD2TreeViewItem; Allow: Boolean);
    procedure InspectorChangeProperty(Sender: TObject;   PropertyName: String);
    procedure ObjectsTreeChangeCheck(Sender: TObject);
    procedure DesignRootDragOver(Sender: TObject; const Data: TD2DragObject; const Point: TD2Point;var Accept: Boolean);
    procedure textFilterChangeTracking(Sender: TObject);
  private
    //FDragObj: TD2Object;
    FResource: TD2Resources;
    procedure UpdateTree;
    procedure LoadFromStrings(Str: TStrings);
    procedure SaveToStrings(Str: TStrings);
    procedure DoDeleteButton(Sender: TObject);
    procedure DoVisCheck(Sender: TObject);
    procedure DeleteObject(AObject: TD2Object; FreeObject: boolean);
    procedure TreeItemApplyResource(Sender: TObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
  public
  end;

type

TD2CustomGrid = class;
TD2Header = class;

TD2HeaderItem = class(TD2CornerButton)
  private
    FLastPosition: TD2Point;     //последняя позиция закладки Added by GoldenFox
    FMouseDownPos: TD2Point;     //положение мыши при нажатии  Added by GoldenFox
    FOldIndex: integer;          //индекс элемента до начала перемещения Added by GoldenFox
    FNewIndex: integer;          //индекс элемента после начала перемещения Added by GoldenFox
    FSplitter: TD2VisualObject;  //указатель на сплиттер
    //FLeftSplitter: TD2VisualObject; //Deleted by GoldenFox
  protected
    procedure ApplyStyle;  override; //применить стиль Added by GoldenFox
    procedure FreeStyle;  override;	 //освободить стиль Added by GoldenFox	
    //procedure DragOver(const Data: TD2DragObject; const Point: TD2Point; var Accept:boolean);  override; //Deleted by GoldenFox
    //procedure DragDrop(const Data: TD2DragObject; const Point: TD2Point);  override;                      //Deleted by GoldenFox
    //procedure DragEnd;  override;                                                                         //Deleted by GoldenFox
    procedure DoSplitterMouseMove(Sender: TObject; Shift: TShiftState; X, Y, Dx, Dy:single);
    //procedure DoLeftSplitterMouseMove(Sender: TObject; Shift: TShiftState; X, Y, Dx, Dy:single);        //Deleted by GoldenFox
    function Header: TD2Header;
  public
    constructor Create(AOwner: TComponent);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  published
    property CanFocused  default false;
    property TextAlign  default d2TextAlignNear;
    property DragMode  default d2DragAutomatic;
  end;

TD2OnRealignItemEvent = procedure (Sender: TObject; OldIndex, NewIndex: integer) of object;
TD2OnResizeItemEvent = procedure (Sender: TObject; var NewSize:single) of object;

TD2Header = class(TD2Control)
  private
    FContent: TD2Content;			//Added by GoldenFox
    FBackground: TD2VisualObject;	//Added by GoldenFox
    FGrid: TD2CustomGrid; //указатель на грид Added by GoldenFox
    FItemsWidth:single;   //общая ширина полей Added by GoldenFox
    FOnRealignItem: TD2OnRealignItemEvent;
    FOnResizeItem: TD2OnResizeItemEvent;
    FOffset:single; // сдвиг окна просмотра используется в гриде hscroll offset used in grid
    //FLastItem: TD2HeaderItem;  //Deleted by GoldenFox
    //FRadius:single; //Deleted by GoldenFox
    //FSides: TD2Sides; //Deleted by GoldenFox
    function GetItem(Index: integer): TD2HeaderItem;
    function GetItemsCount: integer;                  //получить кол-во полей Added by GoldenFox
    //procedure SetRadius(const Value:single);  //Deleted by GoldenFox
    //procedure SetSides(const Value:TD2Sides); //Deleted by GoldenFox
  protected
    procedure ApplyStyle;  override; //применить стиль Added by GoldenFox
    procedure FreeStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override; //Added by GoldenFox
    procedure Paint;  override;
    procedure AddObject(AObject: TD2Object);  override; //Added by GoldenFox
    procedure Realign;  override;
    function ItemClass: string;  override;
    property ItemsCount: integer read GetItemsCount; //кол-во полей Added by GoldenFox
    property Items[Index: integer]: TD2HeaderItem read GetItem;
  published
    property CanFocused  default false;
    property ClipChildren  default true;
    property OnRealignItem: TD2OnRealignItemEvent read FOnRealignItem write FOnRealignItem;
    property OnResizeItem: TD2OnResizeItemEvent read FOnResizeItem write FOnResizeItem;
    //property Radius:single read GetRadius write SetRadius; //Deleted by GoldenFox
    //property Sides: TD2Sides read GetSides write SetSides; //Deleted by GoldenFox
  end;

TD2TextCell = class(TD2TextBox)
  private
  end;

TD2CheckCell = class(TD2CheckBox)
  private
  public
  end;

TD2ProgressCell = class(TD2ProgressBar)
  private
  public
  end;

TD2PopupCell = class(TD2PopupBox)
  private
  public
  end;

TD2ImageCell = class(TD2ImageControl)
  private
  public
  end;

TD2Column = class(TD2Control)
  private
    FGrid: TD2CustomGrid;
    FReadOnly:boolean;
    procedure SetHeader(const Value:String);
    function GetGrid: TD2CustomGrid; //Added by GoldenFox
  protected
    FCellControls: array of TD2Control;
    FUpdateColumn:boolean;
    FHeader: String;
    FSaveData: Variant;
    FDisableChange:boolean;
    procedure UpdateColumn;  virtual;
    procedure UpdateSelected;
    procedure ClearColumn;
    function CreateCellControl: TD2Control;  virtual;
    procedure DoTextChanged(Sender: TObject);
    procedure DoCanFocused(Sender: TObject; var ACanFocused: boolean);
    procedure DoEnterFocus(Sender: TObject);
    procedure DoKeyDown(Sender: TObject; var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);
    procedure SetWidth(const Value:single);  override; //Added by GoldenFox
  public
    constructor Create(AOwner: TComponent);  override;
    function CellControlByPoint(X, Y:single): TD2Control;
    function CellControlByRow(Row: integer): TD2Control;
    property Grid: TD2CustomGrid read FGrid; //Added by GoldenFox
    procedure Realign;  override; //Added by GoldenFox
  published
    property Resource;
    property Header: String read FHeader write SetHeader;
    property ReadOnly: boolean read FReadOnly write FReadOnly  default false;
  end;

TD2TextColumn = class(TD2Column)
  end;

TD2CheckColumn = class(TD2Column)
  private
    procedure DoCheckChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    function CreateCellControl: TD2Control;  override;
  end;

TD2ProgressColumn = class(TD2Column)
  private
    FMin:single;
    FMax:single;
  protected
    function CreateCellControl: TD2Control;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Min:single read FMin write FMin;
    property Max:single read FMax write FMax;
  end;

TD2PopupColumn = class(TD2Column)
  private
    FItems: TD2WideStrings;
    procedure SetItems(const Value:TD2WideStrings);
  protected
    function CreateCellControl: TD2Control;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Items: TD2WideStrings read FItems write SetItems;
  end;

TD2ImageColumn = class(TD2Column)
  private
  public
    constructor Create(AOwner: TComponent);  override;
    function CreateCellControl: TD2Control;  override;
  end;

  TOnGetValue = procedure (Sender: TObject; const Col, Row:integer; var Value:Variant) of object;
  TOnSetValue = procedure (Sender: TObject; const Col, Row:integer; const Value:Variant) of object;
  TOnEdititingDone = procedure (Sender: TObject; const Col, Row: integer) of object;

{ TD2CustomGrid }

TD2CustomGrid = class(TD2CustomScrollBox)
  private
    //FMouseSelecting:boolean;
    FItemHeight:single;
    FSelection: TD2VisualObject;
    FIsPreSelected:boolean;            //Added by GoldenFox //флаг пред.выбора строки
    FPreSelection: TD2VisualObject;   //Added by GoldenFox //указатель на маркер пред.выбора строки
    FFocus: TD2VisualObject;
    FRowCount:integer;
    FOnGetValue:TOnGetValue;
    FOnSetValue:TOnSetValue;
    FSelections: TList;
    FSelectedRows: array of integer;      //Added by GoldenFox    //массив выбранных строк
    FAlternatingRowBackground:boolean;
    FOddFill: TD2Brush;
    FLineFill: TD2Brush;
    FShowHorzLines:boolean;
    FShowVertLines:boolean;
    FReadOnly:boolean;
    FColumnIndex:integer;
    FHeader: TD2Header;
    FShowHeader:boolean;
    FShowSelectedCell:boolean;
    FOnEdititingDone: TOnEdititingDone;
    FMultiSelect:boolean;                       //Uncomment by GoldenFox
    function GetColumnCount:integer;
    function GetColumn(Index: integer): TD2Column;
    procedure SetRowCount(const Value:integer);
    procedure SetRowHeight(const Value:single);
    function GetVisibleRows:integer;
    procedure SetAlternatingRowBackground(const Value:boolean);
    procedure SetShowHorzLines(const Value:boolean);
    procedure SetShowVertLines(const Value:boolean);
    procedure SetColumnIndex(const Value:integer);
    procedure SetMultiSelect (const Value:boolean);               //Adedd by GoldenFox
    procedure SetShowHeader(const Value:boolean);
    procedure SetShowSelectedCell(const Value:boolean);
  protected
    FSelected:integer;
    FRowHeight:single;
    procedure ContentRemoveObject(AObject: TD2Object);  override; //Adedd by GoldenFox
    //procedure Notification(AComponent: TComponent; Operation: TOperation);  override; //Deleted by GoldenFox
    procedure ApplyStyle;  override;
    procedure FreeStyle;  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure HScrollChange(Sender: TObject);  override;
    procedure VScrollChange(Sender: TObject);  override;
    function  GetContentBounds: TD2Rect;  override;
    procedure UpdateColumns;  virtual;
    procedure UpdateHeader;
    procedure UpdateSelection;
    procedure Reset;  virtual;
    procedure DoContentPaint(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    procedure DoContentPaint2(Sender: TObject; const Canvas: TD2Canvas; const ARect: TD2Rect);
    function  GetTopRow:integer;  virtual;
    function  GetValue(Col, Row: integer): Variant;  virtual;
    procedure SetValue(Col, Row:integer; const Value:Variant);  virtual;
    function  IsSelected(Row: integer):boolean;
    function  IsOneRowSelected:boolean;  //Added by GoldenFox  true - если выбрана 1 стока
    procedure SetSelected(const Value: integer);  virtual;
    procedure SetSelectedMoreRow(Idx: integer);  virtual;  //Added by GoldenFox
    function  ChangeSelectionRow(Idx: integer):boolean; //Инвертировать выделение строки Idx. Результат: true - строка выделена, false - развыделена //Added by GoldenFox
    function  CanEditAcceptKey(Key: System.WideChar): Boolean;  virtual;
    function  CanEditModify: Boolean;  virtual;
    procedure DoRealignItem(Sender: TObject; OldIndex, NewIndex: integer);
    procedure DoResizeItem(Sender: TObject; var NewSize:single);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
    //procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;  //Deleted by GoldenFox
    function ItemClass: string;  override;
    function ColumnByIndex(const Idx: integer): TD2Column;
    function ColumnByPoint(const X, Y:single): TD2Column;
    function RowByPoint(const X, Y:single):integer;
    procedure AddObject(AObject: TD2Object);  override;
    procedure ApplyResource;  override;	//Added by GoldenFox
    procedure ScrollToRow(ARow: integer); //Added by GoldenFox
    //procedure RemoveObject(AObject: TD2Object);  override;  //Deleted by GoldenFox
    property AlternatingRowBackground: boolean read FAlternatingRowBackground write SetAlternatingRowBackground  default false;
    property CanFocused  default true;
    property ColumnCount: integer read GetColumnCount;
    property ColumnIndex: integer read FColumnIndex write SetColumnIndex;
    property Columns[Index: integer]: TD2Column read GetColumn;
    property MultiSelect:boolean read FMultiSelect write SetMultiSelect default false; //Multiple row select enable //Adedd by GoldenFox
    property ReadOnly: boolean read FReadOnly write FReadOnly  default false;
    property RowCount: integer read FRowCount write SetRowCount;
    property RowHeight:single read FRowHeight write SetRowHeight;
    property Selected: integer read FSelected write SetSelected;
    property ShowHeader: boolean read FShowHeader write SetShowHeader  default true;
    property ShowHorzLines: boolean read FShowHorzLines write SetShowHorzLines  default true;
    property ShowSelectedCell: boolean read FShowSelectedCell write SetShowSelectedCell  default true;
    property ShowVertLines: boolean read FShowVertLines write SetShowVertLines  default true;
    property TopRow: integer read GetTopRow;
    property VisibleRows: integer read GetVisibleRows;
    property OnEdititingDone: TOnEdititingDone read FOnEdititingDone write FOnEdititingDone;
    property OnGetValue:TOnGetValue read FOnGetValue write FOnGetValue;
    property OnSetValue:TOnSetValue read FOnSetValue write FOnSetValue;
    property AutoHide;
    property Animated;
    property ScrollDuration;
    property DisableMouseWheel;
    property ShowScrollBars;
    property ShowSizeGrip;
    property UseSmallScrollBars;
end;

TD2Grid = class(TD2CustomGrid)
  published
    property AutoHide;
    property Animated;
    property AlternatingRowBackground;
    property CanFocused;
    property DisableFocusEffect;
    property MultiSelect;
    property ReadOnly;
    property RowCount;
    property RowHeight;
    property ScrollDuration;
    property ShowHeader;
    property ShowHorzLines;
    property ShowSelectedCell;
    property ShowScrollBars;
    property ShowSizeGrip;
    property ShowVertLines;
    property TabOrder;
    property UseSmallScrollBars;
    property OnGetValue;
    property OnSetValue;
    property OnEdititingDone;
end;

TD2StringColumn = class(TD2Column)
  private
    FCells: array of WideString;
  published
    procedure UpdateColumn;  override;
  //public
    //constructor Create(AOwner: TComponent);  override;
    //destructor Destroy;  override;
  end;

TD2StringGrid = class(TD2CustomGrid)
private
    function GetCells(ACol, ARow: Integer): WideString;
    procedure SetCells(ACol, ARow: Integer; const Value:WideString);
  protected
    function GetValue(Col, Row: integer): Variant;  override;
    procedure SetValue(Col, Row:integer; const Value:Variant);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function ItemClass: string;  override;
    property Cells[ACol, ARow: Integer]: WideString read GetCells write SetCells;
  published
    property RowCount;
  end;

type
  TD2NavButton = class;
  TD2NavDataLink = class;

  Ed2NavClick = procedure (Sender: TObject; Button: TD2NavigateBtn) of object;

TD2DBNavigator = class (TD2Layout)
  private
    FDataLink: TD2NavDataLink;
    FVisibleButtons: TD2NavButtonSet;
    FHints: TStrings;
    FDefHints: TStrings;
    ButtonWidth: Integer;
    MinBtnSize: TD2Point;
    FOnNavClick: Ed2NavClick;
    FBeforeAction: Ed2NavClick;
    FocusedButton: TD2NavigateBtn;
    FConfirmDelete: Boolean;
    //FFlat: Boolean;
    FyRadius:single;
    FxRadius:single;
    FCornerType: TD2CornerType;
    FCorners: TD2Corners;
    procedure BtnMouseDown (Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y:single);
    procedure ClickHandler(Sender: TObject);
    function  GetDataSource: TDataSource;
    function  GetHints: TStrings;
    procedure HintsChanged(Sender: TObject);
    procedure InitButtons;
    procedure InitHints;
    procedure SetDataSource(Value:TDataSource);
    procedure SetHints(Value:TStrings);
    procedure SetSize(var W:single;var H:single);
    procedure SetVisible(Value:TD2NavButtonSet);
    procedure SetCornerType(const Value:TD2CornerType);
    procedure SetxRadius(const Value:single);
    procedure SetyRadius(const Value:single);
    function  IsCornersStored: Boolean;
    procedure SetCorners(const Value:TD2Corners);
  protected
    Buttons: array[TD2NavigateBtn] of TD2NavButton;
    procedure DataChanged;
    procedure EditingChanged;
    procedure ActiveChanged;
    procedure Loaded;  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure CalcMinSize(var W, H:single);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Realign;  override;
    procedure BtnClick(Index: TD2NavigateBtn);  virtual;
  published
    property DataSource: TDataSource read GetDataSource write SetDataSource;
    property VisibleButtons: TD2NavButtonSet read FVisibleButtons write SetVisible
                               default [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbRefresh];
    property Align;
    property Enabled;
    property CornerType: TD2CornerType read FCornerType write SetCornerType  default d2CornerRound;
    property Corners: TD2Corners read FCorners write SetCorners stored IsCornersStored;
    property xRadius:single read FxRadius write SetxRadius;
    property yRadius:single read FyRadius write SetyRadius;
    property Hints: TStrings read GetHints write SetHints;
    property PopupMenu;
    property ConfirmDelete: Boolean read FConfirmDelete write FConfirmDelete  default True;
    property ShowHint;
    property Visible;
    property BeforeAction: Ed2NavClick read FBeforeAction write FBeforeAction;
    property OnClick: Ed2NavClick read FOnNavClick write FOnNavClick;
  end;


TD2NavButton = class(TD2CornerButton)
  private
    FIndex: TD2NavigateBtn;
    FNavStyle: TD2NavButtonStyle;
    FRepeatTimer: TTimer;
    FPath: TD2Path;
    procedure TimerExpired(Sender: TObject);
  protected
    procedure ApplyStyle;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
    property NavStyle: TD2NavButtonStyle read FNavStyle write FNavStyle;
    property Index : TD2NavigateBtn read FIndex write FIndex;
  end;

TD2NavDataLink = class(TDataLink)
  private
    FNavigator: TD2DBNavigator;
  protected
    procedure EditingChanged;  override;
    procedure DataSetChanged;  override;
    procedure ActiveChanged;  override;
  public
    constructor Create(ANav: TD2DBNavigator);
    destructor Destroy;  override;
  end;

{ TD2FieldDataController }

TD2FieldDataController=class(TDataLink)   //Added by GoldenFox  Based on TFieldDataLink
  private
    FField: TField;
    FFieldName: string;
    FOnDataChange: TNotifyEvent;
    FOnEditingChange: TNotifyEvent;
    FOnUpdateData: TNotifyEvent;
    FOnActiveChange: TNotifyEvent;
    FEditing: Boolean;
    IsModified: Boolean;
    function FieldCanModify: boolean;
    function IsKeyField(aField: TField): Boolean;
    function GetCanModify: Boolean;
    procedure SetFieldName(const Value: string);
    procedure UpdateField;
    procedure ValidateField;
  protected
    procedure ActiveChanged; override;
    procedure EditingChanged; override;
    procedure LayoutChanged; override;
    procedure RecordChanged(aField: TField); override;
    procedure UpdateData; override;
  public
    constructor Create;
    function Edit: Boolean;
    procedure Modified;
    procedure Reset;
    property Field: TField read FField;
    property CanModify: Boolean read GetCanModify;
    property Editing: Boolean read FEditing;
    property OnDataChange: TNotifyEvent read FOnDataChange write FOnDataChange;
    property OnEditingChange: TNotifyEvent read FOnEditingChange write FOnEditingChange;
    property OnUpdateData: TNotifyEvent read FOnUpdateData write FOnUpdateData;
    property OnActiveChange: TNotifyEvent read FOnActiveChange write FOnActiveChange;
  published
    property DataSource;
    property FieldName: string read FFieldName write SetFieldName;
end;

TD2DBLabel = class(TD2CustomLabel)
  private
    //FDataLink: TFieldDataLink;         //Deleted by GoldenFox
    FDataController: TD2FieldDataController;    //Added by GoldenFox
    procedure DataChange(Sender: TObject);
    function  GetDataField: string;
    function  GetDataSource: TDataSource;
    procedure SetDataController(const AValue: TD2FieldDataController);   //Added by GoldenFox
    //procedure SetDataField(const Value:string);            //Deleted by GoldenFox
    //procedure SetDataSource(const Value:TDataSource);      //Deleted by GoldenFox
    function  GetFieldText: string;
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property DataField: string read GetDataField{ write SetDataField};          //Deleted by GoldenFox
    property DataSource: TDataSource read GetDataSource{ write SetDataSource};  //Deleted by GoldenFox
  published
    property DataController: TD2FieldDataController read FDataController write SetDataController;   //Added by GoldenFox
    property TextAlign  default d2TextAlignNear;
  end;

TD2DBImage = class(TD2Image)
  private
    //FDataLink: TFieldDataLink;         //Deleted by GoldenFox
    FDataController: TD2FieldDataController;    //Added by GoldenFox
    procedure DataChange(Sender: TObject);
    procedure UpdateData(Sender: TObject);
    function  GetDataField: string;
    function  GetDataSource: TDataSource;
    procedure SetDataController(const AValue: TD2FieldDataController);   //Added by GoldenFox
    //procedure SetDataField(const Value:string);         //Deleted by GoldenFox
    //procedure SetDataSource(const Value:TDataSource);   //Deleted by GoldenFox
    function  GetFieldText: string;
  protected
    procedure DoBitmapChanged(Sender: TObject);  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property DataField: string read GetDataField{ write SetDataField};           //Deleted by GoldenFox
    property DataSource: TDataSource read GetDataSource{ write SetDataSource};   //Deleted by GoldenFox
  published
    property DataController: TD2FieldDataController read FDataController write SetDataController;   //Added by GoldenFox
  end;

TD2DBTextBox = class(TD2CustomTextBox)
  private
    //FDataLink: TFieldDataLink;         //Deleted by GoldenFox
    FDataController: TD2FieldDataController;    //Added by GoldenFox
    procedure SetDataController(const AValue: TD2FieldDataController);   //Added by GoldenFox
    procedure DataChange(Sender: TObject);
    function  GetDataField: string;
    function  GetDataSource: TDataSource;
    //procedure SetDataField(const Value:string);          //Deleted by GoldenFox
    //procedure SetDataSource(const Value:TDataSource);    //Deleted by GoldenFox
    function  GetFieldText: string;
    procedure UpdateData(Sender: TObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure Change;  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property DataField: string read GetDataField{ write SetDataField};              //Deleted by GoldenFox
    property DataSource: TDataSource read GetDataSource{ write SetDataSource};      //Deleted by GoldenFox
  published
    property DataController: TD2FieldDataController read FDataController write SetDataController;   //Added by GoldenFox
    property Password;
  end;

TD2DBMemo = class(TD2CustomMemo)
  private
    //FDataLink: TFieldDataLink;         //Deleted by GoldenFox
    FDataController: TD2FieldDataController;    //Added by GoldenFox
    procedure SetDataController(const AValue: TD2FieldDataController);   //Added by GoldenFox
    procedure DataChange(Sender: TObject);
    function  GetDataField: string;
    function  GetDataSource: TDataSource;
    //procedure SetDataField(const Value:string);          //Deleted by GoldenFox
    //procedure SetDataSource(const Value:TDataSource);    //Deleted by GoldenFox
    function  GetFieldText: string;
    procedure UpdateData(Sender: TObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override;
    procedure Change;  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    property DataField: string read GetDataField{ write SetDataField};           //Deleted by GoldenFox
    property DataSource: TDataSource read GetDataSource{ write SetDataSource};   //Deleted by GoldenFox
  published
    property DataController: TD2FieldDataController read FDataController write SetDataController;   //Added by GoldenFox
  end;

  //TD2DBGrid = class;

//TD2GridDataLink = class(TDataLink)
//  private
//    FGrid: TD2DBGrid;
//    FFieldCount: Integer;
//    FFieldMap: array of Integer;
//    FModified: Boolean;
//    FInUpdateData: Boolean;
//    FSparseMap: Boolean;
//    function GetDefaultFields: Boolean;
//    function GetFields(I: Integer): TField;
//  protected
//    procedure ActiveChanged;  override;
//    procedure BuildAggMap;
//    procedure DataSetChanged;  override;
//    procedure DataSetScrolled(Distance: Integer);  override;
//    procedure FocusControl(Field: TFieldRef);  override;
//    procedure EditingChanged;  override;
//    function  IsAggRow(Value:Integer): Boolean;  virtual;
//    procedure LayoutChanged;  override;
//    procedure RecordChanged(Field: TField);  override;
//    procedure UpdateData;  override;
//    function  GetMappedIndex(ColIndex: Integer): Integer;
//  public
//    constructor Create(AGrid: TD2DBGrid);
//    destructor Destroy;  override;
//    procedure Modified;
//    procedure Reset;
//    property DefaultFields: Boolean read GetDefaultFields;
//    property FieldCount: Integer read FFieldCount;
//    property Fields[I: Integer]: TField read GetFields;
//    property SparseMap: Boolean read FSparseMap write FSparseMap;
//    property Grid: TD2DBGrid read FGrid;
//  end;

TD2DBColumn = class(TD2Column)
  private
    FField: TField;
    FFieldName: String;
    procedure SetFieldName(const Value:String);
    function GetField: TField;
    procedure SetField(Value:TField);
    procedure LinkField;
  protected
    function GetData: Variant;  virtual;
    procedure SetData(Value:Variant);  virtual;
  public
    //constructor Create(AOwner: TComponent);  override;    //Deleted by GoldenFox
    destructor Destroy;  override;
    property  Field: TField read GetField write SetField;
  published
    property FieldName: String read FFieldName write SetFieldName;
  end;

TD2DBTextColumn = class(TD2DBColumn)
  protected
    procedure SetData(Value:Variant);  override;
    function GetData: Variant;  override;
end;

TD2DBCheckColumn = class(TD2DBColumn)
  private
  protected
    function CreateCellControl: TD2Control;  override;
    procedure DoCheckChanged(Sender: TObject);
    function GetData: Variant;  override;
  public
  published
  end;

TD2DBPopupColumn = class(TD2DBColumn)
  private
    FItems: TD2WideStrings;
    procedure SetItems(const Value:TD2WideStrings);
  protected
    function  CreateCellControl: TD2Control;  override;
    procedure DoPopupChanged(Sender: TObject);
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Items: TD2WideStrings read FItems write SetItems;
  end;

TD2DBImageColumn = class(TD2DBColumn)
  private
    FCurrent: TD2Bitmap;
  protected
    function CreateCellControl: TD2Control;  override;
    procedure DoImageChanged(Sender: TObject);
    procedure SetData(Value:Variant);  override;
    function GetData: Variant;  override;
  public
    destructor Destroy;  override;
  published
  end;

TD2DBProgressColumn = class(TD2DBColumn)
  private
    FMin:single;
    FMax:single;
  protected
    function CreateCellControl: TD2Control;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property Min:single read FMin write FMin;
    property Max:single read FMax write FMax;
  end;


{**********************************************************************
                          This part make by GoldenFox
**********************************************************************}

TD2GridDataController=class(TComponentDataLink)
  published
    property DataSource;
  end;

TD2CustomDBGrid = class(TD2CustomGrid)
  private
    FDataController: TD2GridDataController;  //
    FDisableMove:boolean;                //
    FEditValue:Variant;                  //
    FNeedUpdate:boolean;                 //
    function GetDataSource: TDataSource; //
    procedure SetDataSource(const Value:TDataSource); //
    function GetSelectedField: TField;                //
    procedure SetSelectedField(const Value:TField);   //
    procedure SetDataController(const AValue: TD2GridDataController);
    procedure UpdateRowCount;                         //
    procedure OnRecordChanged(Field:TField); virtual; //
    procedure OnDataSetChanged(aDataSet: TDataSet); virtual; //
    procedure OnEditingChanged(aDataSet: TDataSet); virtual; //
    procedure OnUpdateData(aDataSet: TDataSet); virtual;     //

    procedure OnDataSetOpen(aDataSet: TDataSet); virtual;    //
    procedure OnDataSetClose(aDataSet: TDataSet); virtual;   //
    procedure OnInvalidDataSet(aDataSet: TDataSet); virtual;
    procedure OnInvalidDataSource(aDataSet: TDataset); virtual;
    procedure OnLayoutChanged(aDataSet: TDataSet); virtual;     //изменился состав или порядок полей в DataSet
    procedure OnNewDataSet(aDataSet: TDataset); virtual;
    procedure OnDataSetScrolled(aDataSet:TDataSet; Distance: Integer); virtual;
  protected
    function  GetValue(Col, Row: integer): Variant;  override;  //
    procedure SetValue(Col, Row:integer; const Value:Variant);  override; //
    function  CanEditAcceptKey(Key: System.WideChar): Boolean;  override; //
    function  CanEditModify: Boolean;  override;  //
    procedure KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);  override; //
    procedure Reset;  override;  //
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;  //
    procedure Loaded;  override;  //
    procedure ColumnsLinkFields; //переопределить Field для каждой колонки
    procedure LinkActive(Value:Boolean);  //
    function  GetContentBounds: TD2Rect;  override;   //
    procedure SetSelected(const Value:integer);  override;   //
  public
    constructor Create(AOwner: TComponent);  override;  //
    destructor Destroy;  override;  //
    function ItemClass: string;  override;  //
    property SelectedField: TField read GetSelectedField write SetSelectedField;  //
    property DataSource: TDataSource read GetDataSource write SetDataSource;   //
    property DataController: TD2GridDataController read FDataController write SetDataController;
  published
    property AlternatingRowBackground;
    property CanFocused;
    property DisableFocusEffect;
    property MultiSelect;
    property ReadOnly;
    property RowCount;
    property RowHeight;
    property ScrollDuration;
    property ShowSelectedCell;
    property ShowVertLines;
    property ShowHorzLines;
    property ShowHeader;
    property TabOrder;
    property OnGetValue;
    property OnSetValue;
    property OnEdititingDone;
end;

TD2DBGrid = class(TD2CustomDBGrid)
  published
    property DataController;
end;

type
 TD2DockingAlign = (daNone, daClient, daBottom, daLeft, daRight, daTop);
 TD2DockingAllowAligns = set of TD2DockingAlign;
 TD2DockingMouseDownSide = (dmsTop, dmsTopLeft, dmsTopRight,
                            dmsBottom,dmsBottomLeft,dmsBottomRight,
                            dmsLeft, dmsRight, dmsNone);

TD2DockingTab = class(TD2TextControl)
private
  FAutoWidth:boolean;       //автоматический подбор ширины
  FIsSelected:boolean;      //флаг выделения закладки
  FLastPosition: TD2Point;   //последняя позиция закладки
  FMouseDownPos: TD2Point;   //положение мыши при нажатии
  FOldIndex: integer;        //индекс закладки до начала перемещения
  FPanel: TD2VisualObject;   //ссылка на панель
  FDisableSetText:boolean;      //флаг изменения текста закладки
  FTextControl: TD2Text;            //ссылка на текст
  FTimer: TD2Timer;          //ссылка на таймер закрытия
  FIsShowPanel:boolean;    //флаг видимости панели: true - панель выдвинута; false - скрыта
  function  GetAlign: TD2DockingAlign;
  procedure SetAlign(const Value:TD2DockingAlign);
  procedure SetAutoWidth(const Value:boolean);
protected
  procedure ApplyStyle;  override;   //применить стиль
  procedure DesignSelect;  override; //выбор закладки в дизайн-моде (показать/скрыть панель)
  procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  procedure MouseMove(Shift: TShiftState; X, Y, Dx, Dy:single);  override;
  procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y:single);  override;
  procedure MouseEnter;  override;
  procedure MouseLeave;  override;
  procedure Notification(AComponent: TComponent; Operation: TOperation);  override; //очистка FLayout при его удалении
  procedure SetText(const Value:WideString);  override; //
public
  constructor Create(AOwner: TComponent);  override;  //создать объект
  destructor Destroy;  override;                      //уничтожить объект
  procedure DoAutoWidth(AMinWidth:single);          //выполнить автоматический подбор ширины
  procedure DoTimer(Sender: TObject);
  procedure Realign;  override;
  procedure Select(ASelected:boolean);              //выделение закладки
  procedure ShowPanel(const Value:boolean);              //ture - выдвинуть панель; false - скрыть

published
  property Align: TD2DockingAlign read GetAlign write SetAlign;// write SetAlign;
  property AutoTranslate default true;              //флаг автоматического перевода текста на закладке
  property AutoWidth:boolean read FAutoWidth write SetAutoWidth default true;
  property Font;                                    //шрифт
  property IsSelected:boolean read FIsSelected;      //флаг выделения закладки
  property IsShowPanel:boolean read FIsShowPanel;  //флаг видимости панели: true - панель выдвинута; false - скрыта
  property Panel: TD2VisualObject read FPanel write FPanel;
  property TextAlign;        //горизонтальное выравнивание текста на закладке
  property VertTextAlign;    //вертикальное выравнивание текста на закладке
  property Text;             //текст на закладке
  property Resource;  //название ресурса
end;

TD2DockingPanel = class(TD2TextControl)
  private
    FTSplitter: TD2SplitLayout;   //ссылка на верхний сплиттер
    FTLSplitter: TD2SplitLayout;  //ссылка на верхний-левый сплиттер
    FTRSplitter: TD2SplitLayout;  //ссылка на верхний-правый сплиттер
    FBSplitter: TD2SplitLayout;   //ссылка на нижний сплиттер
    FBLSplitter: TD2SplitLayout;  //ссылка на нижний-левый сплиттер
    FBRSplitter: TD2SplitLayout;  //ссылка на нижний-правый сплиттер
    FLSplitter: TD2SplitLayout;   //ссылка на левый сплиттер
    FRSplitter: TD2SplitLayout;   //ссылка на правый сплиттер
    FCloseButton: TD2CloseButton; //ссылка на кнопку закрытия
    FContent: TD2Content;         //ссылка на клиентскую область
    //FContent: TD2ScrollContent;
    FDisableFixed:boolean;       //флаг запрета обрабтки изменения фиксации
    FDisableSetText:boolean;      //флаг изменения текста закладки
    FDragDistance: TD2Point;      //расстояние от мыши до точки 0,0 панели при перемещении
    FDragMousePos: TD2Point;      //последнее положение мыши при перемещении панели
    FDragTarget: TD2VisualObject; //ссылка на целевой объект при перемещении панели
    FIsNotDraged:boolean;        //флаг отсуствия перемещения панели
    FFixedCheckBox: TD2CheckBox;  //ссылка на кнопку фисации
    FFreeOnClose:boolean;        //true - уничтожить панель при закрытии; false - делать невидимой вместо закрытия
    FHeader: TD2Rectangle;        //ссылка на заголовок
    FShadow: TD2ShadowEffect;     //ссылка на тень
    FTab: TD2VisualObject;        //ссылка на закладку
    FTabAutoWidth:boolean;
    FAlign: TD2DockingAlign;                   //расположение панели
    FAllowDock: TD2DockingAllowAligns;         //разрешения на расположение панели
    FIsDockable:boolean;    //разрешено изменять положение
    FIsExpanded:boolean;    //панель развернута двойным щелчком
    FIsFixed:boolean;       //панель зафиксирована
    FMinHeight:single;  //минимальная высота окна
    FMinWidth:single;   //минимальная ширина окна
    FMouseDownSide: TD2DockingMouseDownSide; //активный сплитер
    FMouseDownPos: TD2Point;                //стартовые координаты мыши в активном сплиттере (нажатие мыши)
    FOldAlign: TD2DockingAlign;   //расположение панели до раскрытия на все окно
    FOldFixed:boolean;           //фиксация панели до распрытия на все окно
    FOldPos: TD2Point;            //положение неприкрепленного окна
    FOldSize: TD2Point;           //размеры неприкрепленного окна
    FOnAlignChange:TNotifyEvent;             //прерывание при изменении расположения
    FOnFixedChange:TNotifyEvent;    //прерывание при нажатии на кнопку фикасации
    FOnCloseClick:TNotifyEvent;       //прерывание при нажатии на кнопку закрытия
    FShowCloseButton:boolean;    //показать кнопку закрытия
    FShowFixedCheckBox:boolean;  //показать чек бокс фиксации окна
    FShowHeader:boolean;         //показать заголовок
    procedure DoCloseClick(Sender: TObject);     //нажатие на кнопку закрытия панели
    procedure DoFixedChange(Sender: TObject);    //нажатие на кнопку фикасации панели
    procedure DoHeaderDblClick(Sender: TObject); //двойной щелчок на заголовке панели
    procedure DoHeaderDragEnd(Sender: TObject);  //конец перемещения панели
    procedure DoSplitterMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y:single); //нажатие ЛКМ на сплиттере
    procedure DoSplitterMouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y:single);   //отпускание ЛКМ на сплиттере
    procedure DoSplitterMouseMove(Sender: TObject; Shift: TShiftState; X, Y, Dx, Dy:single);               //перемещение мыши на сплиттере
    procedure SetAlign(const Value:TD2DockingAlign);   //установить расположение панели
    procedure SetAllowDock(Value:TD2DockingAllowAligns); //установить разрешения на расположение панели
    procedure SetDragMode (const Value:TD2DragMode);
    procedure SetIsDockable (const Value:boolean);     //установить/снять разрешение изменять положение
    procedure SetIsFixed(const Value:boolean);           //установить/снять фиксацию панели
    procedure SetMinHeight(const Value:Single);        //установка минимальной высоты панели
    procedure SetMinWidth(const Value:Single);         //установка минимальной ширины панели
    procedure SetShowCloseButton(const Value:boolean); //показать/скрыть кнопку закрытия панели
    procedure SetShowFixedCheck(const Value:boolean);  //показать/скрыть чек бокс фиксации панели
    procedure SetShowHeader(const Value:boolean);      //показать/скрыть заголовок панели
    procedure SetTabAutoWidth (const Value:boolean);
  protected
    procedure ApplyAlign;  virtual;  //применить размещение
    procedure ApplyStyle;  override; //применить стиль
    procedure FreeStyle;  override;  //удалить стиль
    procedure DesignClick;  override;                     //двойной щелчок по панели в дизайн-моде (изменение фиксации)
    procedure DesignInsert;  override;
    procedure DesignSelect;  override;
    procedure TabInsert; //вставить закладку на связанном DockingPlace
    procedure TabDelete; //удалить закладку со связанного DockingPlace
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override; //очистка FLayout при его удалении
    procedure SetHeight(const Value:single);  override;   //установить высоту панели
    procedure SetText(const Value:WideString);  override; overload;  //
    procedure SetWidth(const Value:single);  override;    //установить ширину панели
    procedure SetVisible(const Value:boolean);  override; //установить видимость панели
    procedure SetMouseOverChildren(Sender: TObject; Value:Boolean);  override;
  public
    constructor Create(AOwner: TComponent);  override;  //создать объект
    destructor Destroy;  override;                      //разрушить объект
    procedure AddObject(AObject: TD2Object);  override; //добавить объект в окно
    procedure Realign;  override;                       //перестроить клиентскую область
    procedure DragMove(Sender: TD2VisualObject; Point: TD2Point);  virtual; //сместить панель
  published
    property Font;
    property TextAlign;
    property Text;
    property Resource;
    property AutoTranslate default true;
    property Align: TD2DockingAlign read FAlign write SetAlign default daNone;
    property AllowDock: TD2DockingAllowAligns read FAllowDock write SetAllowDock default [daNone, daClient, daBottom, daLeft, daRight, daTop];
    property DragMode write SetDragMode default d2DragManual;
    property FreeOnClose:boolean read FFreeOnClose write FFreeOnClose default true;
    property IsDockable:boolean read FIsDockable write SetIsDockable default true;
    property IsFixed:boolean read FIsFixed write SetIsFixed default true;
    property MinHeight:single read FMinHeight write SetMinHeight;
    property MinWidth:single read FMinWidth write SetMinWidth;
    property ShowCloseButton:boolean read FShowCloseButton write SetShowCloseButton default true;
    property ShowFixedCheckBox:boolean read FShowFixedCheckBox write SetShowFixedCheck default true;
    property ShowHeader:boolean read FShowHeader write SetShowHeader default true;
    property Tab: TD2VisualObject read FTab write FTab;
    property TabAutoWidth:boolean read FTabAutoWidth write SetTabAutoWidth default true;
    property OnAlignChange:TNotifyEvent read FOnAlignChange write FOnAlignChange;  //прерывание измения положения
    property OnCloseClick:TNotifyEvent read FOnCloseClick write FOnCloseClick;     //прерывание нажатия кнопки закрытия
    property OnFixedChange:TNotifyEvent read FOnFixedChange write FOnFixedChange;  //прерывание измения фиксации
  end;

TD2DockingPlace = class(TD2Control)
  private
    FAllowDockChildren: TD2DockingAllowAligns; //разрешения на расположение панелей
    FContent: TD2Content;         //ссылка на клиентскую область
    FDragPanel: TD2DockingPanel;  //ссылка на перемещаемую панель
    FDragRectLeft: TD2Rect;        //область для размещения панели слева
    FDragRectTop: TD2Rect;         //область для размещения панели сверху
    FDragRectRight: TD2Rect;       //область для размещения панели справа
    FDragRectBottom: TD2Rect;      //область для размещения панели снизу
    FDragRectClient: TD2Rect;      //область для размещения панели как клиента
    FIndexMaxLeft: integer;           //индекс последней левой закладки
    FIndexMaxTop: integer;            //индекс последнейверхней закладки
    FIndexMaxRight: integer;          //индекс последней правой закладки
    FIndexMaxBottom: integer;         //индекс последней нижней закладки
    FTabHeight:single;            //высота закладок (или ширина для вертикальных)
    FBackground: TD2VisualObject;  //ссылка на фон
    procedure SetAllowDockChildren(Value:TD2DockingAllowAligns); //установить разрешения на расположение панелей
    procedure SetTabHeight(const Value:single);   //установить высоту закладки
    procedure DoContentDragLeave(Sender: TObject);
    procedure DoContentDragEnter(Sender: TObject; const Data: TD2DragObject;
                                 const Point: TD2Point);
    procedure DoContentDragOver(Sender: TObject; const Data: TD2DragObject;
                                const Point: TD2Point; var Accept:boolean);
  protected
    procedure ApplyStyle;  override;       // применить стиль
    procedure FreeStyle;  override;        // удалить стиль

  public
    constructor Create(AOwner: TComponent);  override;  //создать объект
    destructor Destroy;  override;                      //уничтожить объект
    procedure HideAllPanels(ANotHide: TD2DockingTab);  //скрыть все открытые панели кроме параметра
    function ItemClass: string;  override;              //возвращает класс дочерних элементов
    procedure Realign;  override;
    procedure AddObject(AObject: TD2Object);  override;
    procedure SetDragRects;
  published
    property Resource;
    property AllowDockChildren: TD2DockingAllowAligns read FAllowDockChildren write SetAllowDockChildren default [daNone, daClient, daBottom, daLeft, daRight, daTop];
    property TabHeight:single read FTabHeight write SetTabHeight;
  end;
//=============================================================================
//======================= End part of make by GoldenFox =======================
//=============================================================================



{$IFDEF DARWIN}

type

TD2FilterQuartz = class(TD2Filter)
  private
  public
  published
    class function GetFileTypes: string;  override;
    class function GetImageSize(const AFileName: string): TD2Point;  override;
    function LoadFromFile(const AFileName: string; const Rotate:single; var Bitmap: TD2Bitmap):boolean;  override;
    function LoadThumbnailFromFile(const AFileName: string; const AFitWidth, AFitHeight:single; const UseEmbedded:boolean;var Bitmap: TD2Bitmap):boolean;  override;
    function SaveToFile(const AFileName: string; var Bitmap: TD2Bitmap; const Params: string = ''):boolean;  override;
    function LoadFromStream(const AStream: TStream; var Bitmap: TD2Bitmap):boolean;  override;
    function SaveToStream(const AStream: TStream; var Bitmap: TD2Bitmap; const Format: string; const Params: string = ''):boolean;  override;
  end;


TD2CanvasQuartz = class(TD2Canvas)
  private
    Func: CGFunctionRef;
    BitmapRef: CGImageRef;
    Callback: CGFunctionCallbacks;
    Shading: CGShadingRef;
    ColorSpace: CGColorSpaceRef;
  protected
    procedure ApplyFill(ARect: TD2Rect; const AOpacity:single);
    procedure DeApplyFill(ARect: TD2Rect; const AOpacity:single);
    procedure ApplyStroke(ARect: TD2Rect; const AOpacity:single);
    procedure FontChanged(Sender: TObject);  override;
    procedure UpdateBitmap(ABitmap: TD2Bitmap);
    procedure DoDestroyBitmap(Sender: TObject);
  public
    constructor Create(const AWidth, AHeight: integer);  override;
    constructor CreateFromBitmap(const ABitmap: TD2Bitmap);  override;
    destructor Destroy;  override;
    procedure FreeBuffer;  override;
    procedure ResizeBuffer(const AWidth, AHeight: integer);  override;
    procedure FlushBuffer(const X, Y:integer; const DC: Cardinal);  override;
    procedure FlushBufferRect(const X, Y:integer; const DC: Cardinal; const ARect: TD2Rect);  override;
    procedure Clear(const Color: cardinal);  override;
    procedure ClearRect(const ARect: TD2Rect; const AColor: TD2Color = 0);  override;
    class function GetBitmapScanline(Bitmap: TD2Bitmap; y: integer): PD2ColorArray;  override;
    procedure SetMatrix(const M: TD2Matrix);  override;
    procedure MultyMatrix(const M: TD2Matrix);  override;
    function SaveCanvas: cardinal;  override;
    procedure RestoreCanvas(const AState: cardinal);  override;
    procedure SetClipRects(const ARects: array of TD2Rect);  override;
    procedure IntersectClipRect(const ARect: TD2Rect);  override;
    procedure ExcludeClipRect(const ARect: TD2Rect);  override;
    procedure ResetClipRect;  override;
    procedure DrawLine(const APt1, APt2: TD2Point; const AOpacity:single);  override;
    procedure FillRect(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners; const AOpacity:single;
      const ACornerType: TD2CornerType = d2CornerRound);  override;
    procedure DrawRect(const ARect: TD2Rect; const xRadius, yRadius:single; const ACorners: TD2Corners; const AOpacity:single;
      const ACornerType: TD2CornerType = d2CornerRound);  override;
    procedure FillEllipse(const ARect: TD2Rect; const AOpacity:single);  override;
    procedure DrawEllipse(const ARect: TD2Rect; const AOpacity:single);  override;
    function PtInPath(const APoint: TD2Point; const ARect: TD2Rect; const APath: TD2PathData):boolean;  override;
    procedure FillPath(const APath: TD2PathData; const ARect: TD2Rect; const AOpacity:single);  override;
    procedure DrawPath(const APath: TD2PathData; const ARect: TD2Rect; const AOpacity:single);  override;
    procedure DrawBitmap(const ABitmap: TD2Bitmap; const SrcRect, DstRect: TD2Rect; const AOpacity:single; const HighSpeed: boolean = false);  override;
    procedure FillText(const ARect, AClipRect: TD2Rect; const AText: WideString; const WordWrap:boolean;
      const AOpacity:single; const ATextAlign: TD2TextAlign; const AVTextAlign: TD2TextAlign = d2TextAlignCenter);  override;
    procedure MeasureText(var ARect: TD2Rect; AClipRect: TD2Rect; const AText: WideString; const WordWrap:boolean; const ATextAlign: TD2TextAlign;
      const AVTextAlign: TD2TextAlign = d2TextAlignCenter);  override;
    function TextToPath(Path: TD2PathData; const ARect: TD2Rect; const AText: WideString; const WordWrap:boolean; const ATextAlign: TD2TextAlign;
      const AVTextAlign: TD2TextAlign = d2TextAlignCenter):boolean;  override;

  end;

function CGRectFromRect(const R: TD2Rect): CGRect;

{$ENDIF}

//=============================================================================
//=============== GLobal Functions ============================================
//=============================================================================

// Strings
function d2PointToString(R: TD2Point): Ansistring;
function d2StringToPoint(S: Ansistring): TD2Point;
function d2RectToString(R: TD2Rect): Ansistring;
function d2StringToRect(S: Ansistring): TD2Rect;
function d2ColorToStr(Value:TD2Color): string;
function d2StrToColor(Value:string): TD2Color;
function d2FloatToStr(Value:single): string;
function d2StrToFloat(Value:string):single;

// Geometry
procedure d2SinCos(const Theta:single; var Sin, Cos:single);
function  d2RadToDeg(const Degrees:single):single;
function  d2DegToRad(const Degrees:single):single;
function  d2NormalizeAngle(const angle: Single) :single;
function  d2Point(const X, Y:single): TD2Point;
function  d2MinPoint(P1, P2: TD2Point): TD2Point;
function  d2ScalePoint(P: TD2Point; dx, dy:single): TD2Point;
function  d2Rect(const ALeft, ATop, ARight, ABottom:single): TD2Rect;
function  d2NormalizeRect(const Pts: array of TD2Point): TD2Rect;
function  d2NormalizeRect2(const ARect: TD2Rect): TD2Rect;
function  d2RectWidth(const R: TD2Rect):single;
function  d2RectHeight(const R: TD2Rect):single;
function  d2RectCenter(var R: TD2Rect; Bounds: TD2Rect): TD2Rect;
function  d2FitRect(var R: TD2Rect; BoundsRect: TD2Rect):single;
function  d2IsRectEmpty(Rect: TD2Rect):boolean;
procedure d2OffsetRect(var R: TD2Rect; const Dx, Dy:single);
procedure d2MultiplyRect(var R: TD2Rect; const Dx, Dy:single);
procedure d2InflateRect(var R: TD2Rect; const Dx, Dy:single);
function  d2IntersectRect(const Rect1, Rect2: TD2Rect):boolean; overload;
function  d2IntersectRect(var DestRect: TD2Rect; const SrcRect1, SrcRect2: TD2Rect):boolean; overload;
function  d2PtInRect(const P: TD2Point; const Rect: TD2Rect):boolean; overload;
function  d2PtInRect(const PX,PY,RLeft,RRight,RTop,RBottom: Single):boolean; overload;
function  d2UnionRect(const ARect1, ARect2: TD2Rect): TD2Rect;
function  d2PointFromVector(const v: TD2Vector): TD2Point;
function  d2MatrixMultiply(const M1, M2: TD2Matrix): TD2Matrix;
function  d2MatrixDeterminant(const M: TD2Matrix):single;
procedure d2AdjointMatrix(var M: TD2Matrix);
procedure d2ScaleMatrix(var M: TD2Matrix; const factor:single);
procedure d2InvertMatrix(var M: TD2Matrix);
function  d2Vector(const x, y: Single; const w:single = 1.0) : TD2Vector; overload;
function  d2Vector(const P: TD2Point; const w:single = 1.0): TD2Vector; overload;
function  d2VectorTransform(const V: TD2Vector; const M: TD2Matrix): TD2Vector;
function  d2CreateRotationMatrix(angle:single): TD2Matrix;
function  d2VectorAdd(const v1: TD2Vector; const v2: TD2Vector): TD2Vector;
function  d2VectorSubtract(const v1: TD2Vector; const v2: TD2Vector): TD2Vector;
function  d2VectorNorm(const v : TD2Vector) :single;
function  d2VectorNormalize(const v: TD2Vector): TD2Vector;
function  d2VectorScale(const v: TD2Vector; factor : Single): TD2Vector;
function  d2VectorLength(const v : TD2Vector) :single;
function  d2VectorDotProduct(const V1, V2 : TD2Vector):single;
function  d2VectorAngleCosine(const V1, V2: TD2Vector):single;
function  d2VectorCrossProductZ(const V1, V2: TD2Vector):single;
function  d2VectorCombine2(const V1, V2: TD2Vector; const F1, F2: Single): TD2Vector;
function  d2VectorReflect(const V, N: TD2Vector): TD2Vector;
function  d2VectorAngle(const V, N: TD2Vector):single;

// Colors
function  d2AppendColor(start, stop: TD2Color): TD2Color;
function  d2SubtractColor(start, stop: TD2Color): TD2Color;
function  d2RGBtoBGR(const C: TD2Color): TD2Color;
function  d2ColorFromVCL(const C: TColor): TD2Color;
function  d2CorrectColor(const C: TD2Color): TD2Color;
function  d2PremultyAlpha(const C: TD2Color): TD2Color;
function  d2UnpremultyAlpha(const C: TD2Color): TD2Color;
function  d2Opacity(const C: TD2Color; const AOpacity:single): TD2Color;
function  d2Color(R, G, B: Byte; A: Byte = $FF): TD2Color;
function  d2HSLtoRGB(H, S, L: Single): TD2Color;
procedure d2RGBtoHSL(RGB: TD2Color; out H, S, L:single);
function  d2ChangeHSL(const C: TD2Color; dH, dS, dL:single): TD2Color;

// Animation
function d2InterpolateSingle(const start, stop, t:single):single;
function d2InterpolateRotation(start, stop, t : Single) :single;
function d2InterpolateColor(start, stop: TD2Color; t :single): TD2Color;
function d2InterpolateLinear(T, B, C, D: Double): Double;
function d2InterpolateSine(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateQuint(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateQuart(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateQuad(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateExpo(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateElastic(T, B, C, D, A, P: Double; aType: TD2AnimationType): Double;
function d2InterpolateCubic(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateCirc(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateBounce(T, B, C, D: Double; aType: TD2AnimationType): Double;
function d2InterpolateBack(T, B, C, D, S: Double; aType: TD2AnimationType): Double;

// Help functions

procedure d2ReverseBytes(p: Pointer; Count: integer);
procedure d2MoveLongword(const Src: Pointer; Dst: Pointer; Count: Integer);
procedure d2FillLongword(Src: Pointer; Count: Integer; Value:Longword);
procedure d2FillAlpha(Src: Pointer; Count: Integer; Alpha: byte);
procedure d2FillLongwordRect(Src: Pointer; W, H, X1, Y1, X2, Y2: Integer; Value:Longword);
function  d2GetToken(var S: Ansistring; Separators: Ansistring; Stop: Ansistring = ''): Ansistring;
function  d2WideGetToken(var Pos:integer; const S: WideString; Separators: WideString; Stop: WideString = ''): WideString;
function  d2MaxFloat(A1, A2:single):single;
function  d2MinFloat(A1, A2:single):single;
function  d2MinMax(x, mi, ma:single):single;

// Other
procedure Blur(const Canvas: TD2Canvas; const Bitmap: TD2Bitmap; const Radius:integer; UseAlpha: boolean = true);
function ComposeCaretPos(ALine, APos : integer) : TCaretPosition;

function MessagePopup(const ACaption, AMessage: WideString; AType: TD2MessageType;
                      Buttons: TD2MessageButtons; const AOwner: Id2Scene;
                      const Target: TD2VisualObject = nil;
                      const ADisableScene: boolean = true;
                      const ABitmap: TD2Bitmap = nil;
                      const AStyle: TD2Resources = nil):integer;


// Variants
function VarIsObject(Value:Variant):boolean;
function ObjectToVariant(const AObject: TObject): Variant;
function VariantToObject(const Value:Variant): TObject;
function VarIsEvent(Value:Variant):boolean;
function EventToVariant(const AMethod:TNotifyEvent): Variant;
function VariantToEvent(const Value:Variant):TNotifyEvent;

// Resources
procedure AddResource(const AObject: TD2Object);
procedure RemoveResource(const AObject: TD2Object);
function  FindResource(const AResource: string): TD2Object;

// Scenes
procedure AddScene(const AScene: Id2Scene);
procedure RemoveScene(const AScene: Id2Scene);

// Lang
procedure LoadLangFromFile(AFileName: string);
procedure LoadLangFromStrings(AStr: TD2WideStrings);
procedure ResetLang;
procedure UpdateLang;
//This function use to collect string which can be translated.
//Just place this function at Application start.
procedure CollectLangStart;
procedure CollectLangFinish;
// This function return Strings with collected text
function CollectLangStrings: TD2WideStrings;
function Translate(const AText: WideString): WideString;
function TranslateText(const AText: WideString): WideString;

{$IFDEF WINDOWS}
procedure SetD2DDefault;
procedure InitGDIP;
procedure FreeGDIP;
procedure UseDirect2DCanvas;
{$ENDIF}

// Objects Registration
procedure Registerd2Object(const Category: string; const AObject: TD2ObjectClass);
procedure Registerd2Objects(const Category: string; AClasses: array of TD2ObjectClass);
function  CreateObjectFromStream(AOwner: TComponent; const AStream: TStream): TD2Object;
function  CreateObjectFromBinStream(AOwner: TComponent; const AStream: TStream): TD2Object;
function  LoadObjectFromStream(AObject: TD2Object; const AStream: TStream): TD2Object;

// For Dialogs
function  DesignResources(AResource: TD2Resources; Current: string):boolean;
procedure SelectInDesign(AObject: TObject; AComp: TPersistent);
procedure ShowBrushDialog(const Brush: TD2Brush; const ShowStyles: TD2BrushStyles; const ShowBrushList: boolean = true);
procedure ShowGradientDialog(const Gradient: TD2Gradient);
function  ShowColorDialog(const Color: string): string;
procedure ShowDsgnImageList(ImgList: TD2ImageList);
procedure ShowDsgnLang(Lang: TD2Lang);

//=============================================================================
//=============== GLobal Variables ============================================
//=============================================================================

var
  GvarD2SceneCount: integer = 0;
  GvarD2aniThread: TD2Timer;
  GvarD2ObjectList: TStringList;
  GvarD2Designer: TD2Designer;
  GvarD2DefaultCanvasClass: TD2CanvasClass;
  GvarD2DefaultPrinterCanvasClass: TD2CanvasClass;
  GvarD2DefaultFilterClass: TD2FilterClass;
  GvarD2DefaultStyles: TD2Object;

  GvarD2LangDesigner: TD2LangDesigner;
  GvarD2BitmapEditor: TD2BitmapEditor;
  GvarD2StyleDesigner: TD2StyleDesigner;
  GvarD2PathDataDesigner: TD2PathDataDesigner;
  GvarD2frmDsgnImageList: TfrmDsgnImageList;
  GvarD2BrushDesign: TD2BrushDesign;

  GlobalDisableFocusEffect: boolean = false;
  CustomTranslateProc: TD2CustomTranslateProc;
//=============================================================================
//=============================================================================
//=============================================================================
implementation

uses math, typinfo,IntfGraphics;

{$R *.res}

const
  ModalResults: array[TD2MessageButton] of Integer = (
    mrYes, mrNo, mrOk, mrCancel, mrAbort, mrRetry, mrIgnore, mrAll, mrNoToAll, mrYesToAll, 0);
  MessageButtonNames: array[TD2MessageButton] of WideString = (
    'Yes', 'No', 'OK', 'Cancel', 'Abort', 'Retry', 'Ignore',
    'All', 'NoToAll', 'YesToAll', 'Help');

type

  TD2HackVisual = class(TD2VisualObject);
  TD2HackVisualObject = class(TD2VisualObject);
  TD2HackBackground = class(TD2Background);
  TD2HackObject = class(TD2VisualObject);
  THackComponent = class(TComponent);

  TD2StyleIDEDesigner = class(TD2Designer)
    private
    public
      procedure SelectObject(ADesigner: TComponent; AObject: TD2Object; MultiSelection: array of TD2Object);  override;
      procedure Modified(ADesigner: TComponent);  override;
      function UniqueName(ADesigner: TComponent; ClassName: string): string;  override;
      function IsSelected(ADesigner: TComponent; const AObject: TObject):boolean;  override;
      procedure AddObject(AObject: TD2Object);  override;
      procedure DeleteObject(AObject: TD2Object);  override;
    end;


var
  varD2PopupList: TList;
  varD2FTarget: TD2VisualObject = nil;
  varD2SceneList: TList;
  varD2NeedResetLang:boolean;
  varD2CollectLang, varD2Lang: TD2WideStrings;
  varD2ResourceList: TList;

//==============================================================
//==============================================================

{$IFDEF WINDOWS}
  {$I orca_scene2d_canvas_directx.inc}
  {$I orca_scene2d_canvas_dgiplus.inc}
{$ENDIF}

{$IFDEF UNIX}

   {$IFDEF DARWIN}
     {$I orca_scene2d_canvas_mac.inc}
   {$ELSE}
     {$I orca_scene2d_canvas_cairo.inc}
   {$ENDIF}

{$ENDIF}

//==============================================================
//==============================================================

procedure CloseAllPopups;
  var
    i:integer;
  begin
    if (varD2PopupList.Count > 0) then
    begin
      for i := varD2PopupList.Count - 1 downto 0 do
        TD2Popup(varD2PopupList[i]).ClosePopup;
    end;
  end;


procedure SelectInDesign(AObject: TObject; AComp: TPersistent);
begin
  if GvarD2BrushDesign = nil then
  begin
    GvarD2BrushDesign := TD2BrushDesign.Create(Application);
    try
    except
      GvarD2BrushDesign.Free;
      GvarD2BrushDesign := nil;
      raise;
    end;
  end;

  if AObject = nil then
  begin
    GvarD2BrushDesign.Comp := nil;
    GvarD2BrushDesign.Brush := nil;
  end;
  if AObject is TD2Brush then
  begin
    GvarD2BrushDesign.Comp := AComp;
    GvarD2BrushDesign.Brush := TD2Brush(AObject);
  end;
  if AObject is TD2Object then
  begin
    GvarD2BrushDesign.Comp := TPersistent(AObject);
    GvarD2BrushDesign.Brush := nil;
  end;

  GvarD2BrushDesign.Show;
end;

procedure ShowBrushDialog(const Brush: TD2Brush; const ShowStyles: TD2BrushStyles; const ShowBrushList: boolean);
var
  Dlg: TD2BrushDialog;
begin
  Dlg := TD2BrushDialog.Create(nil);
  Dlg.Brush := Brush;
  Dlg.ShowStyles := ShowStyles;
  Dlg.ShowBrushList := ShowBrushList;
  if Dlg.Execute then
    Brush.Assign(Dlg.Brush);
  Dlg.Free;
end;

procedure ShowGradientDialog(const Gradient: TD2Gradient);
var
  Dlg: TD2BrushDialog;
begin
  Dlg := TD2BrushDialog.Create(nil);
  Dlg.Brush.Style := d2BrushGradient;
  Dlg.Brush.Gradient := Gradient;
  Dlg.ShowStyles := [d2BrushGradient];
  Dlg.ShowBrushList := false;
  if Dlg.Execute then
    Gradient.Assign(Dlg.Brush.Gradient);
  Dlg.Free;
end;

function ShowColorDialog(const Color: string): string;
var
  Dlg: TD2BrushDialog;
begin
  Dlg := TD2BrushDialog.Create(nil);
  Dlg.Brush.Style := d2BrushSolid;
  Dlg.Brush.Color := Color;
  Dlg.ShowStyles := [d2BrushSolid];
  Dlg.ShowBrushList := false;
  if Dlg.Execute then
  begin
    Result := Dlg.Brush.Color;
  end;
  Dlg.Free;
end;

procedure ShowDsgnLang(Lang: TD2Lang);
begin
  GvarD2LangDesigner := TD2LangDesigner.Create(Application);
  with GvarD2LangDesigner do
  begin
    FLang := Lang;
    langList.Items.Assign(Lang.Resources);
    if langList.Items.Count > 0 then
      langList.ItemIndex := Lang.Resources.IndexOf(Lang.Lang);

    layoutAdd.Visible := langList.Items.Count = 0;
    layoutSelect.Visible := langList.Items.Count > 0;

    RebuildOriginalList;
    if ShowModal = mrOk then
    begin
      FLang.Lang := langList.Text;
    end;
  end;
  GvarD2LangDesigner.Free;
end;

procedure ShowDsgnImageList(ImgList: TD2ImageList);
var
  i:integer;
begin
  GvarD2frmDsgnImageList := TfrmDsgnImageList.Create(Application);
  with GvarD2frmDsgnImageList do
  begin
    for i := 0 to ImgList.Count - 1 do
      ImageList.AddBitmap('', ImgList.Images[i]);
    ImageList.ItemWidth := ImgList.Width;
    ImageList.ItemHeight := ImgList.Height;
    if ShowModal = mrOK then
    begin
      ImgList.Clear;
      for i := 0 to ImageList.Count - 1 do
        if ImageList.Images[i] <> nil then
        begin
          ImgList.Add(ImageList.Images[i].Bitmap);
        end;
    end;
  end;
  GvarD2frmDsgnImageList.Free;
end;


//-------------------------------------------------------------

function WeekOfYear(aDate : tDateTime) : byte;
var
  t,m,year : word;
  newyear : tDateTime;
  KW : word;
  wtag_ny : word;
begin
  DecodeDate (aDate, year,m,t); // calc year
  newyear := EncodeDate(year, 1, 1); // calc 1.1.year
  wtag_ny := ord(DayofWeek(newyear)); // DOW of 1.1.year
  KW := Trunc( ((aDate-newyear + ((wtag_ny + 1) Mod 7)-3) / 7)+1);
  if (KW = 0) then
  begin
    KW := 0;
  end;
  result := KW;
end;

function ReadString(S: TStream): WideString;
var
  L: Integer;
begin
  L := 0;
  S.Read(L, SizeOf(L));
  SetLength(Result, L);
  S.Read(Pointer(Result)^, L * 2);
end;

procedure WriteString(S: TStream; Value:WideString);
var
  L: Integer;
begin
  L := Length(Value);
  S.Write(L, SizeOf(L));
  S.Write(Pointer(Value)^, L * 2);
end;

function VarIsObject(Value:Variant):boolean;
var
  S: string;
begin
  S := Value;
  if (S <> '') and (Pos('d2obj', S) = 1) then
    Result := true
  else
    Result := false;
end;

function ObjectToVariant(const AObject: TObject): Variant;
begin
  Result := 'd2obj' + IntToStr(Integer(Pointer(AObject)));
end;

function VariantToObject(const Value:Variant): TObject;
var
  S: string;
begin
  S := Value;
  if (S <> '') and (Pos('d2obj', S) = 1) then
    Result := TObject(Pointer(StrToInt(Copy(S, 6, 10))))
  else
    Result := nil;
end;

function VarIsEvent(Value:Variant):boolean;
var
  S: string;
begin
  if VarIsStr(Value) then
  begin
    S := Value;
    if (S <> '') and (Pos('d2met', S) = 1) then
      Result := true
    else
      Result := false;
  end
  else
    Result := false
end;

function EventToVariant(const AMethod:TNotifyEvent): Variant;
begin
  Result := 'd2met' + IntToHex(Integer(TMethod(AMethod).Data), 8) + IntToHex(Integer(TMethod(AMethod).Code), 8);
end;

function VariantToEvent(const Value:Variant):TNotifyEvent;
var
  S: string;
begin
  S := Value;
  if (S <> '') and (Pos('d2met', S) = 1) then
  begin
    TMethod(Result).Data := Pointer(StrToInt('$' + Copy(S, 6, 8)));
    TMethod(Result).Code := Pointer(StrToInt('$' + Copy(S, 14, 8)));
  end
  else
  begin
    Result := nil;
  end;
end;


procedure Registerd2Object(const Category: string; const AObject: TD2ObjectClass);
begin
  if GvarD2ObjectList =  nil then
  begin
    GvarD2ObjectList := TStringList.Create;
  end;
  GvarD2ObjectList.InsertObject(0, Category, TObject(AObject));
  GvarD2ObjectList.Sort;

end;

procedure Registerd2Objects(const Category: string; AClasses: array of TD2ObjectClass);
var
  I: Integer;
begin
  for I := Low(AClasses) to High(AClasses) do
  begin
    Registerd2Object(Category, AClasses[I]);
    RegisterClass(AClasses[I]);
  end;
end;

procedure AddResource(const AObject: TD2Object);
begin
  if varD2ResourceList =  nil then
  begin
    varD2ResourceList := TList.Create;
    varD2ResourceList.Capacity := 100;
  end;
  if varD2ResourceList.IndexOf(AObject) < 0 then
    varD2ResourceList.Add(AObject);
end;

procedure RemoveResource(const AObject: TD2Object);
var
  Idx:integer;
begin
  if varD2ResourceList <> nil then
  begin
    Idx := varD2ResourceList.IndexOf(AObject);
    if Idx >= 0 then
      varD2ResourceList[Idx] := nil;
  end;
end;

function FindResource(const AResource: string): TD2Object;
var
  i:integer;
begin
  Result := nil;
  if varD2ResourceList <> nil then
    for i := varD2ResourceList.Count - 1 downto 0 do
      if (varD2ResourceList[i] <> nil) and TD2Object(varD2ResourceList[i]).Stored then
        if CompareText(TD2Object(varD2ResourceList[i]).ResourceName, AResource) = 0 then
        begin
          Result := TD2Object(varD2ResourceList[i]);
          Break;
        end;
end;

function CreateObjectFromStream(AOwner: TComponent; const AStream: TStream): TD2Object;
var
  Reader:TReader;
  //SavePos: Longint;
  //I: Integer;
  //Flags: TFilerFlags;
  ClassName: string;
  ObjClass: TD2ObjectClass;
  BinStream: TStream;
begin
  Result := nil;
  try
    BinStream := TMemoryStream.Create;
    try
      ObjectTextToBinary(AStream, BinStream);
      BinStream.Position := 0;

      Reader := TReader.Create(BinStream, 4096);
      Reader.Driver.BeginRootComponent;
      ClassName := Reader.Driver.ReadStr;
      ObjClass := TD2ObjectClass(GetClass(ClassName));
      Result := ObjClass.Create(AOwner);
      if Result <> nil then
      begin
        BinStream.Position := 0;
        Result.IntLoadFromBinStream(BinStream);
      end;
      Reader.Free;
    finally
      BinStream.Free;
    end;
  except
    Result := nil;
  end;
end;

function CreateObjectFromBinStream(AOwner: TComponent; const AStream: TStream): TD2Object;
var
  Reader:TReader;
  SavePos: Longint;
  //I: Integer;
  //Flags: TFilerFlags;
  ClassName: string;
  ObjClass: TD2ObjectClass;
begin
  Result := nil;
  try
    SavePos := AStream.Position;

    Reader := TReader.Create(AStream, 4096);
    Reader.Driver.BeginRootComponent;
    ClassName := Reader.Driver.ReadStr;
    ObjClass := TD2ObjectClass(GetClass(ClassName));
    Result := ObjClass.Create(AOwner);
    if Result <> nil then
    begin
      AStream.Position := SavePos;
      Result.IntLoadFromBinStream(AStream);
    end;
    Reader.Free;
  except
    Result := nil;
  end;
end;

function LoadObjectFromStream(AObject: TD2Object; const AStream: TStream): TD2Object;
var
  Reader:TReader;
  //SavePos: Longint;
  //I: Integer;
  //Flags: TFilerFlags;
  //ClassName: string;
  //ObjClass: TD2ObjectClass;
  BinStream: TStream;
begin
  Result := nil;
  try
    BinStream := TMemoryStream.Create;
    try
      ObjectTextToBinary(AStream, BinStream);
      BinStream.Position := 0;
      Result := AObject;
      if Result <> nil then
      begin
        BinStream.Position := 0;
        Result.IntLoadFromBinStream(BinStream);
      end;
      Reader.Free;
    finally
      BinStream.Free;
    end;
  except
    Result := nil;
  end;
end;

procedure AddScene(const AScene: Id2Scene);
begin
  if varD2SceneList = nil then  varD2SceneList := TList.Create;

  if varD2SceneList.IndexOf(Pointer(AScene)) < 0 then
    varD2SceneList.Add(Pointer(AScene));
end;

procedure RemoveScene(const AScene: Id2Scene);
begin
  varD2SceneList.Remove(Pointer(AScene));
end;


procedure CallLoaded(Obj: TD2Object);
var
  i:integer;
begin
  Obj.Loaded;
  for i := 0 to Obj.ChildrenCount - 1 do
    CallLoaded(Obj.Children[i]);
end;


procedure WriteValue(W:TWriter; Value:TValueType);
var
  b: byte;
begin
  b := Byte(Value);
  W.Write(b, 1);
end;

procedure WriteStr(W:TWriter; const Value:String);
var
  i:integer;
  b: byte;
begin
  i := Length(Value);
  if i > 255 then
    i := 255;
  b := i;
  W.Write(b, 1);
  if i > 0 then
    W.Write(Value[1], i);
end;

function ReadStr(R:TReader): string;
var
  L: Byte;
begin
  R.Read(L, SizeOf(Byte));
  System.SetString(Result, PChar(nil), L);
  R.Read(Result[1], L);
end;


function D2GetTickCount:single;
  var
    H, M, S, MS: word;
  begin
    DecodeTime(time, H, M, S, MS);
    Result := ((((H * 60 * 60) + (M * 60) + S) * 1000) + MS);
  end;

//------------------------------

procedure CholeskyDecomposition(const b: array of Single; const Count:integer;
  var Result: array of Single);
var
  Y, M1, M2: array of single;
  i, k, D, F:integer;
begin
  // calc Cholesky decomposition matrix
  D := 0;
  F := Count - 1;
  SetLength(M1, Count);

  SetLength(M2, Count - 1);
  M1[D] := sqrt(2);
  M2[D] := 1.0 / M1[D];
  for k := D + 1 to F - 1 do
  begin
    M1[K] := Sqrt(4 - M2[K-1] * M2[K-1]);
    M2[K] := 1.0 / M1[K];
  end;
  M1[F] := Sqrt(2 - M2[F-1] * M2[F-1]);

  SetLength(Y, Count);
  Y[D] := B[D] / M1[D];
  for i := D + 1 to F do
    Y[i] := (B[i]-Y[i-1]*M2[i-1]) / M1[i];

  Result[F] := Y[F] / M1[F];
  for i := F - 1 downto D do
    Result[i] := (Y[i] - Result[i + 1] * M2[i]) / M1[i];
end;

procedure CalcHermiteFactors(const Values: array of single; var Spline: TD2SplineMatrix);
var
  a, b, c, d :single;
  i, n : Integer;
  M1, M2: array of single;
begin
  if (Length(Values) > 0) then
  begin
    n := Length(Values) - 1;

    SetLength(M1, Length(Values));
    M1[0] := 3 * (Values[1] - Values[0]);
    M1[n] := 3 * (Values[n] - Values[n-1]);
    for i := 1 to n-1 do
      M1[I] := 3 * (Values[I+1] - Values[I-1]);

    SetLength(M2, Length(Values));
    CholeskyDecomposition(M1, Length(Values), M2);

    SetLength(Spline, n);
    for i := 0 to n - 1 do
    begin
      // calc koeef
      a := Values[I];
      b := M2[I];
      c := 3 * (Values[I+1] - Values[I])-2 * M2[I] - M2[I+1];
      d := -2 * (Values[I+1] - Values[I]) + M2[I] + M2[I+1];
      // calc spline
      Spline[I][3] := a + I * (I * (c - I * d) - b);
      Spline[I][2] := b + I * (3 * I * d - 2 * c);
      Spline[I][1] := c - 3 * I * d;
      Spline[I][0] := d;
    end;
  end;
end;

function HermitInterpolate(const Spline: TD2SplineMatrix; const x :single; const Count: Integer):single;
var
  i:integer;
begin
  if Length(Spline) > 0 then
  begin
    if x <= 0 then
      i := 0
    else
      if x > Count - 1 then
        i := Count - 1
      else
        i := trunc(x);
      if i = (Count - 1) then Dec(i);
      Result := ((Spline[i][0] * x + Spline[i][1]) * x + Spline[i][2]) * x + Spline[i][3];
  end
  else
    Result:=0;
end;


//================================================================

{$IFDEF WINDOWS}
  type

    PBlendFunction = ^TBlendFunction;
    _BLENDFUNCTION = packed record
      BlendOp: BYTE;
      BlendFlags: BYTE;
      SourceConstantAlpha: BYTE;
      AlphaFormat: BYTE;
    end;
    TBlendFunction = _BLENDFUNCTION;
    BLENDFUNCTION = _BLENDFUNCTION;

  const

    WS_EX_LAYERED = $00080000;
    LWA_COLORKEY = $00000001;
    LWA_ALPHA = $00000002;
    ULW_COLORKEY = $00000001;
    ULW_ALPHA = $00000002;
    ULW_OPAQUE = $00000004;

  var
    User32Lib: THandle;
    SetLayeredWindowAttributes: function (hwnd: HWND; crKey: COLORREF; bAlpha:BYTE; dwFlags:DWORD): BOOL; stdcall;
    UpdateLayeredWindow: function (hWnd: HWND; hdcDst: HDC; pptDst: PPOINT;
                                   psize: PSIZE; hdcSrc: HDC; pptSrc: PPOINT;
                                   crKey: COLORREF; pblend: PBlendFunction; dwFlags: DWORD): BOOL; stdcall;
    PrintWindow: function(hwnd: HWND; hdcBlt: HDC; nFlags: DWORD): BOOL; stdcall;



  function WndCallback(Ahwnd: HWND; uMsg: UINT; wParam: WParam; lParam: LParam): LRESULT; stdcall;
    var
      Win: TWinControl;
    begin
      Win := FindControl(Ahwnd);
      if (Win is TD2CustomScene) then
      begin
        if not (csDestroying in Win.ComponentState) then
        begin
          if (uMsg = WM_PAINT) or (uMsg = WM_ADDUPDATERECT) then
          begin
            Result := Win.Perform(uMsg, wParam, lParam);
            exit;
          end;
        end;
        result := CallWindowProcW(@TD2CustomScene(Win).PrevWndProc, Ahwnd, uMsg, WParam, LParam);
        Exit;
      end;

      result := CallWindowProcW(@DefWindowProcW, Ahwnd, uMsg, WParam, LParam);
    end;

procedure UseDirect2DCanvas;
begin
  SetD2DDefault;
end;

{$ENDIF}

{$IFDEF DARWIN}
  function WndEventHandler(inHandlerCallRef: EventHandlerCallRef; inEvent: EventRef; inUserData: Pointer): OSStatus; stdcall;
  var
    myContext: CGContextRef;
    myRect: CGRect;
    rgnCode: WindowRegionCode;
    rgn: RgnHandle;
    bool: longbool;
  begin
    Result := CallNextEventHandler(inHandlerCallRef, inEvent);
    Result := eventNotHandledErr;
    if GetEventClass(inEvent) = kEventClassControl then
    begin
      case GetEventKind(inEvent) of
        kEventControlDraw:
          begin
            GetEventParameter (inEvent,
                              kEventParamCGContextRef,
                              typeCGContextRef,
                              nil,
                              sizeof (CGContextRef),
                              nil,
                              @myContext);
            if myContext <> nil then
            begin
              myRect := CGRectFromRect(d2Rect(0, 0, TD2CustomScene(inUserData).Parent.Width, TD2CustomScene(inUserData).Parent.Height));
              CGContextClearRect(myContext, myRect);
            end;
            Result := noErr;
          end;
      end;
    end;
    if GetEventClass(inEvent) = kEventClassWindow then
    begin
      case GetEventKind(inEvent) of
        kEventWindowGetRegion:
          begin
            TD2CustomScene(inUserData).Invalidate;
            // which region code is being queried?
  {          GetEventParameter(inEvent, kEventParamWindowRegionCode, typeWindowRegionCode, nil, sizeof(rgnCode), nil, @rgnCode);
            // if it is the opaque region code then set the region to Empty and return noErr to stop the propagation
  	  if (rgnCode = kWindowOpaqueRgn) then
  	  begin
  	    GetEventParameter(inEvent, kEventParamRgnHandle, typeQDRgnHandle, nil, sizeof(rgn), nil, @rgn);
              SetEmptyRgn(rgn);
              Result := noErr;
  	  end;}
            Result := noErr;
          end;
      end;
    end;
  end;

  type
    byte8array = array [1..8] of byte;

  function RegionToRectsCallback(message: UInt16; rgn: RgnHandle; const rect_: byte8array; data: Pointer): OSStatus; cdecl;
  var
    R: TD2Rect;
  begin
    if (message = kQDRegionToRectsMsgParse) then
    begin
      {$ifdef FPC_BIG_ENDIAN}
      R := d2Rect(rect_[4] or (rect_[3] shl 8), rect_[2] or (rect_[1] shl 8), rect_[8] or (rect_[7] shl 8), rect_[6] or (rect_[5] shl 8));
      {$else}
      R := d2Rect(rect_[3] or (rect_[4] shl 8), rect_[1] or (rect_[2] shl 8), rect_[7] or (rect_[8] shl 8), rect_[5] or (rect_[6] shl 8));
      {$endif}
      SetLength(TD2CustomScene(data).FUpdateRects, Length(TD2CustomScene(data).FUpdateRects) + 1);
      TD2CustomScene(data).FUpdateRects[High(TD2CustomScene(data).FUpdateRects)] := R;
    end;
    Result := noErr;
  end;

  function CreateFileURLFromPasteboard(inPasteboard: PasteboardRef): TD2DragObject;
  var
    inIndex: CFIndex;
    inCount: ItemCount;
  	item: PasteboardItemID;
  	fileURL: CFURLRef;
    fileURLData: CFDataRef;
  	info: LSItemInfoRecord;
  	uti: CFStringRef;
  begin
    Fillchar(Result, sizeOf(Result), 0);

  	if PasteboardGetItemCount(inPasteboard, inCount) <> noErr then Exit;
    SetLength(Result.Files, inCount);
    for inIndex := 1 to inCount do
    begin
    	if PasteboardGetItemIdentifier(inPasteboard, inIndex, item) <> noErr then Exit;
    	if PasteboardCopyItemFlavorData(inPasteboard, item, kUTTypeFileURL, fileURLData) <> noErr then Exit;

    	// create the file URL with the dragged data
    	fileURL := CFURLCreateAbsoluteURLWithBytes( kCFAllocatorDefault, CFDataGetBytePtr( fileURLData ), CFDataGetLength( fileURLData ), kCFStringEncodingMacRoman, nil, true);
      if fileURL <> nil then
      begin
        uti := CFURLCopyFileSystemPath(fileURL, kCFURLPOSIXPathStyle);
        Result.Files[inIndex - 1] := CFStringToStr(uti);
      	CFRelease(uti);
    		CFRelease(fileURL);
      end;
      CFRelease(fileURLData);

      if inIndex = 1 then
        Result.Data := Result.Files[inIndex - 1];
    end;
  end;

  function CtrlEventHandler(inHandlerCallRef: EventHandlerCallRef;
                            inEvent: EventRef;
                            inUserData: Pointer): OSStatus; stdcall;
  var
    myContext: CGContextRef;
    myRect: CGRect;
    rgn: RgnHandle;
    bool: longbool;
    proc: RegionToRectsUPP;
    err: OSStatus;
    Part: ControlPartCode;
    drag: DragRef;
    pasteboard: PasteboardRef;
    str: string;
    mouseP: MacOSAll.Point;
    P: TD2Point;
    NewTarget: TD2VisualObject;
    Data: TD2DragObject;
    Accept:boolean;
  begin
    Result := CallNextEventHandler(inHandlerCallRef, inEvent);
    Result := eventNotHandledErr;
    if GetEventClass(inEvent) = kEventClassControl then
    begin
      case GetEventKind(inEvent) of
        kEventControlDraw:
          begin
            GetEventParameter (inEvent, kEventParamCGContextRef, typeCGContextRef, nil, sizeof (CGContextRef), nil, @myContext);
            GetEventParameter (inEvent, kEventParamRgnHandle, typeQDRgnHandle, nil, sizeof(rgn), nil, @rgn);
            if rgn <> nil then
            begin
              proc := NewRegionToRectsUPP(@RegionToRectsCallback);
              { clear rects }
              SetLength(TD2CustomScene(inUserData).FUpdateRects, 0);
              err := QDRegionToRects(rgn, kQDParseRegionFromBottomRight, proc, inUserData);
              DisposeRegionToRectsUPP(proc);
            end;
            { draw }
            if TD2CustomScene(inUserData).Canvas.FBuffered then
              TD2CustomScene(inUserData).Canvas.Parent := THandle(myContext)
            else
              TD2CustomScene(inUserData).Canvas.Handle := THandle(myContext);
  {          if TD2CustomScene(inUserData).Transparency then
              CGContextClearRect(myContext, CGRectFromRect(d2Rect(0, 0, TD2CustomScene(inUserData).Width, TD2CustomScene(inUserData).Height)));}
            TD2CustomScene(inUserData).Draw;
            if TD2CustomScene(inUserData).Canvas.FBuffered then
              TD2CustomScene(inUserData).Canvas.Parent := 0
            else
              TD2CustomScene(inUserData).Canvas.Handle := 0;
            Result := noErr;
          end;
        kEventControlDragEnter:
          begin
             bool := true;
             SetEventParameter(inEvent, kEventParamControlWouldAcceptDrop, typeBoolean, sizeof(bool), @bool);
             Result := noErr;
          end;
        kEventControlDragWithin:
          begin
            GetEventParameter(inEvent, kEventParamDragRef, typeDragRef, nil, sizeof(DragRef), nil, @drag);
            if drag <> nil then
            begin
              GetDragPasteboard(drag, pasteboard);
              if pasteboard <> nil then
              begin
                if TD2CustomScene(inUserData).Root = nil then Exit;

                Data := CreateFileURLFromPasteboard(pasteboard);

                GetDragMouse(drag, mouseP, mouseP);
                with TD2CustomScene(inUserData).ScreenToClient(Point(mouseP.h, mouseP.v)) do
                  P := d2Point(X, Y);
                NewTarget := TD2CustomScene(inUserData).Root.Visual.FindTarget(P, Data);

                {  8888
                if (NewTarget <> FTarget) then
                begin
                  if FTarget <> nil then
                     FTarget.DragLeave;
                  FTarget := NewTarget;
                  if FTarget <> nil then
                  begin
                    FTarget.DragEnter(Data, P);
                  end;
                end;
                if FTarget = nil then
                   Accept := false;      }

              end;
            end;
            Result := noErr;
          end;
        kEventControlDragLeave:
          begin
            {      8888
            if FTarget <> nil then
              FTarget.DragLeave;
            FTarget := nil;
            Result := noErr;  }
          end;
        kEventControlDragReceive:
          begin
            GetEventParameter(inEvent, kEventParamDragRef, typeDragRef, nil, sizeof(DragRef), nil, @drag);
            if drag <> nil then
            begin
              GetDragPasteboard(drag, pasteboard);
              if pasteboard <> nil then
              begin
                if TD2CustomScene(inUserData).Root = nil then Exit;

                Data := CreateFileURLFromPasteboard(pasteboard);
                {    8888
                if FTarget <> nil then
                  FTarget.DragDrop(Data, P);   }
              end;
            end;
            {    8888
            FTarget := nil;
            }
            Result := noErr;
          end;
      end;
    end;
  end;

  var
    EventKinds: array [0..20] of EventTypeSpec;
    WndEventHandlerUPP: EventHandlerUPP;
  {$ENDIF}

  {$ifdef WINDOWS}
  procedure GetLanguageIDs(var Lang, FallbackLang: string);
  var
    Buffer: array[1..4] of {$ifdef Wince}WideChar{$else}char{$endif};
    Country: string;
    UserLCID: LCID;
  begin
    //defaults
    Lang := '';
    FallbackLang:='';
    UserLCID := GetUserDefaultLCID;
    if GetLocaleInfo(UserLCID, LOCALE_SABBREVLANGNAME, @Buffer[1], 4)<>0 then
      FallbackLang := lowercase(copy(Buffer,1,2));
    if GetLocaleInfo(UserLCID, LOCALE_SABBREVCTRYNAME, @Buffer[1], 4)<>0 then begin
      Country := copy(Buffer,1,2);

      // some 2 letter codes are not the first two letters of the 3 letter code
      // there are probably more, but first let us see if there are translations
      if (Buffer='PRT') then Country:='PT';

      Lang := FallbackLang+'_'+Country;
    end;
  end;

  {$else}

  procedure GetLanguageIDs(var Lang, FallbackLang: string);
  begin
    lang := GetEnvironmentVariable('LC_ALL');
    if Length(lang) = 0 then
    begin
      lang := GetEnvironmentVariable('LC_MESSAGES');
      if Length(lang) = 0 then
      begin
        lang := GetEnvironmentVariable('LANG');
        if Length(lang) = 0 then
          exit;   // no language defined via environment variables
      end;
    end;
    FallbackLang := Copy(lang, 1, 2);
  end;
  {$endif}

//==============================================================
//==============================================================

{$I orca_scene2d_obj_imglist.inc}
{$I orca_scene2d_obj_animations.inc}
{$I orca_scene2d_obj_trackbars.inc}
{$I orca_scene2d_obj_comboboxies.inc}
{$I orca_scene2d_obj_panels.inc}
{$I orca_scene2d_obj_bars.inc}
{$I orca_scene2d_obj_labels.inc}
{$I orca_scene2d_obj_buttons.inc}
{$I orca_scene2d_obj_checkboxies.inc}
{$I orca_scene2d_obj_radiobuttons.inc}
{$I orca_scene2d_obj_scrollbars.inc}
{$I orca_scene2d_obj_indicators.inc}
{$I orca_scene2d_windows.inc}
{$I orca_scene2d_obj_images.inc}
{$I orca_scene2d_designers.inc}
{$I orca_scene2d_brushdialog.inc}
{$I orca_scene2d_obj_effects.inc}
{$I orca_scene2d_obj_calendars.inc}
{$I orca_scene2d_obj_compounds.inc}
{$I orca_scene2d_obj_grids.inc}
{$I orca_scene2d_obj_inspector.inc}
{$I orca_scene2d_obj_scrollboxes.inc}
{$I orca_scene2d_obj_layouts.inc}
{$I orca_scene2d_obj_listboxes.inc}
{$I orca_scene2d_obj_memo.inc}
{$I orca_scene2d_obj_shapes.inc}
{$I orca_scene2d_selections.inc}
{$I orca_scene2d_widestr.inc}
{$I orca_scene2d_fn_lang.inc}
{$I orca_scene2d_fn_strings.inc}
{$I orca_scene2d_fn_math.inc}
{$I orca_scene2d_fn_color.inc}
{$I orca_scene2d_storage.inc}
{$I orca_scene2d_elements.inc}
{$I orca_scene2d_brushes.inc}
{$I orca_scene2d_fonts.inc}
{$I orca_scene2d_bitmaps.inc}
{$I orca_scene2d_paths.inc}
{$I orca_scene2d_canvas.inc}
{$I orca_scene2d_obj_base.inc}
{$I orca_scene2d_obj_base_visible.inc}
{$I orca_scene2d_obj_controls.inc}
{$I orca_scene2d_resources.inc}
{$I orca_scene2d_lang.inc}
{$I orca_scene2d_actions.inc}
{$I orca_scene2d_scene.inc}
{$I orca_scene2d_popups.inc}
{$I orca_scene2d_obj_boxies.inc}
{$I orca_scene2d_obj_tab.inc}
{$I orca_scene2d_obj_tree.inc}
{$I orca_scene2d_obj_database.inc}
{$I orca_scene2d_obj_docking.inc} //Added by GoldenFox

//==============================================================
//==============================================================

initialization

{$IFDEF DARWIN}
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide, exOverflow, exUnderflow, exPrecision]);
{$ENDIF}

{$IFDEF WINDOWS}
  User32Lib := LoadLibrary(User32);
  if User32Lib <> 0 then
  begin
    @SetLayeredWindowAttributes := GetProcAddress(User32Lib, 'SetLayeredWindowAttributes');
    @UpdateLayeredWindow := GetProcAddress(User32Lib, 'UpdateLayeredWindow');
    @PrintWindow := GetProcAddress(User32Lib, 'PrintWindow');
  end;
  OleInitialize(nil);
{$ENDIF}
//.................................................................................................
  RegisterClasses([TD2CustomScene,
                   TD2Bitmap, TD2PathData, TD2Brush, TD2Bounds, TD2Position, TD2Gradient, TD2GradientPoints,
                   TD2GradientPoint, TD2Visual, TD2Resources, TD2Object, TD2Content, TD2Control,
                   TD2WideStrings, TD2WideStringList, TD2SelectionItem, TD2Thumb, TD2ExpanderButton,
                   TD2StringColumn, TD2TextCell, TD2CheckCell, TD2ProgressCell, TD2PopupCell,
                   TD2NavButton, TD2DBColumn, TD2DBCheckColumn, TD2DBPopupColumn,
                   TD2DBImageColumn, TD2DBProgressColumn,
{**************************************************************************************************
                       This part make by GoldenFox
***************************************************************************************************}
                   TD2DBTextColumn, TD2DockingTab, TD2Column,
                   TD2TextColumn, TD2CheckColumn, TD2ProgressColumn, TD2PopupColumn, TD2ImageColumn
//======================= End part of make by GoldenFox =======================

                   ]);

  Registerd2Objects('Resources', [TD2BrushObject, TD2PathObject, TD2BitmapObject]);

  Registerd2Objects('Layout', [TD2Frame,TD2Layout, TD2ScaledLayout, TD2GridLayout, TD2SplitLayout,TD2Nond2Layout]);

  Registerd2Objects('Popup', [TD2Popup, TD2PopupItem, TD2MessagePopup]);

  Registerd2Objects('Animations', [TD2ColorAnimation, TD2GradientAnimation, TD2FloatAnimation,
                                   TD2RectAnimation, TD2BitmapAnimation, TD2BitmapListAnimation,
                                   TD2ColorKeyAnimation, TD2FloatKeyAnimation, TD2PathAnimation, TD2PathSwitcher]);

  Registerd2Objects('Effects', [TD2ShadowEffect, TD2BlurEffect, TD2GlowEffect, TD2InnerGlowEffect,
                                TD2BevelEffect, TD2ReflectionEffect]);

  Registerd2Objects('Shapes', [TD2Line, TD2Rectangle, TD2SidesRectangle, TD2BlurRectangle, TD2RoundRect, TD2BlurRoundRect,
                               TD2Ellipse, TD2Circle, TD2Arc, TD2Pie, TD2Text, TD2Path, TD2Image, TD2PaintBox,
                               TD2ScrollArrowLeft, TD2ScrollArrowRight, TD2CalloutRectangle]);

  Registerd2Objects('Design', [TD2Selection, TD2SelectionPoint, TD2DesignFrame,TD2Inspector]);


  Registerd2Objects('Windows', [TD2Background, TD2SizeGrip, TD2CloseButton]);

  Registerd2Objects('Boxes', [TD2CheckBox, TD2PathCheckBox, TD2RadioButton, TD2GroupBox, TD2PopupBox,
                              TD2CalendarBox]);

  Registerd2Objects('Controls', [TD2Panel, TD2CalloutPanel, TD2Label, TD2ValueLabel,
                                 TD2ImageControl, TD2ProgressBar, TD2Track, TD2ScrollBar, TD2SmallScrollBar, TD2AniIndicator,
                                 TD2Expander, TD2TrackBar, TD2Splitter, TD2TabControl,TD2FramedVertScrollBox,
                                 TD2ScrollBox, TD2VertScrollBox, TD2FramedScrollBox]);

  Registerd2Objects('Tool and Status', [TD2StatusBar, TD2ToolBar, TD2ToolButton, TD2ToolPathButton]);

  Registerd2Objects('Buttons', [TD2Button, TD2RoundButton, TD2CircleButton, TD2BitmapButton, TD2PathButton, TD2SpeedButton, TD2CornerButton,
                                TD2ColorButton, TD2AngleButton, TD2BitmapStateButton, TD2PopupButton]);

  Registerd2Objects('HUD', [TD2HudPanel, TD2HudWindow, TD2HudButton, TD2HudSpeedButton, TD2HudAngleButton, TD2HudTrack, TD2HudTrackBar,
                            TD2HudScrollBar, TD2HudPopupBox, TD2HudLabel, TD2HudCheckBox, TD2HudRadioButton, TD2HudGroupBox, TD2HudCloseButton,
                            TD2HudStatusBar, TD2HudSizeGrip, TD2HudRoundButton, TD2HudCornerButton, TD2HudCircleButton,
                            TD2HudListBox, TD2HudHorzListBox, TD2HudComboBox, TD2HudStringListBox, TD2HudStringComboBox,
                            TD2HudImageListBox, TD2HudHorzImageListBox, TD2HudTextBox, TD2HudNumberBox,
                            TD2HudRoundTextBox, TD2HudSpinBox, TD2HudComboTextBox, TD2HudComboTrackBar,
                            TD2HudMemo, TD2HudHueTrackBar, TD2HudAlphaTrackBar, TD2HudBWTrackBar, TD2HudComboColorBox,
                            TD2HudTabControl
                            ]);


  Registerd2Objects('Lists', [TD2ListBox, TD2ComboBox, TD2StringListBox, TD2StringComboBox, TD2HorzListBox,
                              TD2ImageListBox, TD2HorzImageListBox]);
  Registerd2Objects('Items', [TD2ListBoxItem, TD2ImageListBoxItem,TD2TreeViewItem,
                              TD2TabItem, TD2HudTabItem, TD2HeaderItem]);

  Registerd2Objects('Trees', [TD2TreeView]);


  Registerd2Objects('Text Edits', [TD2TextBox, TD2RoundTextBox, TD2NumberBox, TD2SpinBox, TD2ComboTextBox, TD2ComboTrackBar, TD2TextBoxClearBtn]);


  Registerd2Objects('Text Edits', [TD2Memo, TD2CalendarTextBox]);


  Registerd2Objects('Colors', [TD2HueTrackBar, TD2AlphaTrackBar, TD2BWTrackBar, TD2ColorQuad, TD2ColorPicker,
                               TD2GradientEdit, TD2ColorBox, TD2ColorPanel, TD2ComboColorBox]);

  Registerd2Objects('Ext. Controls', [TD2IPhoneButton, TD2DockBar, TD2DropTarget, TD2ImageViewer]);

  Registerd2Objects('Math', [TD2PlotGrid]);

  Registerd2Objects('Compound', [TD2CompoundTrackBar, TD2CompoundAngleBar, TD2CompoundTextBox,
                                 TD2CompoundMemo, TD2CompoundNumberBox, TD2CompoundPopupBox, TD2CompoundColorButton,
                                 TD2CompoundImage, TD2Calendar]);


  Registerd2Objects('Grid', [TD2Grid, TD2StringGrid, TD2Header]);

  //Registerd2Objects('Grid Columns', [TD2Column, TD2CheckColumn, TD2ProgressColumn, TD2PopupColumn, TD2ImageColumn]);  //Deleted by GoldenFox

  Registerd2Objects('DB-Aware', [TD2DBNavigator, TD2DBGrid, TD2DBLabel, TD2DBImage, TD2DBTextBox, TD2DBMemo]);

  {**********************************************************************
                          This part make by GoldenFox
 **********************************************************************}
  Registerd2Objects('Docking', [TD2DockingPlace, TD2DockingPanel]);
//======================= End part of make by GoldenFox =======================
//.................................................................................................


{$IFDEF UNIX}
  {$IFDEF DARWIN}
    GvarD2DefaultCanvasClass := TD2CanvasQuartz;
    GvarD2DefaultFilterClass := TD2FilterQuartz;
  {$ELSE}
    GvarD2DefaultCanvasClass := TD2CanvasCairo;
    GvarD2DefaultFilterClass := TD2FilterGtk;
  {$ENDIF}
{$ENDIF}

{$IFDEF WINDOWS}
  LoadD2D;

  GvarD2DefaultFilterClass := TD2FilterGdiPlus;
  if GvarD2DefaultCanvasClass = nil then GvarD2DefaultCanvasClass := TD2CanvasGdiPlus;
  GvarD2DefaultPrinterCanvasClass := TD2CanvasGdiPlus;
  {$IFNDEF d2_no_init_gdip}
  InitGDIP;
  {$ENDIF}
{$ENDIF}

finalization

{$IFDEF WINDOWS}
  FreeD2D;

  {$IFNDEF d2_no_free_gdip}
    FreeGDIP;
  {$ENDIF}

  if User32Lib <> 0 then FreeLibrary(User32Lib);
{$ENDIF}

  if varD2PopupList <> nil then      FreeAndNil(varD2PopupList);
  if GvarD2aniThread <> nil then     FreeAndNil(GvarD2aniThread);
  if GvarD2DefaultStyles <> nil then FreeAndNil(GvarD2DefaultStyles);
  if GvarD2ObjectList <> nil then    FreeAndNil(GvarD2ObjectList);
  if varD2ResourceList <> nil then   FreeAndNil(varD2ResourceList);
  if varD2CollectLang <> nil then    FreeAndNil(varD2CollectLang);
  if varD2Lang <> nil then           FreeAndNil(varD2Lang);
  if varD2SceneList <> nil then      FreeAndNil(varD2SceneList);

end.
