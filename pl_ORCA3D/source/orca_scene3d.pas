{***************************************************
 ORCA 3D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit orca_scene3d;

{$MODE DELPHI}

{$I orca3d_define.inc}

interface

uses
  {$IFDEF UNIX}

    {$IFDEF DARWIN}
     macosall,
     CarbonProc,CarbonDef,CarbonPrivate,carboncanvas,
    {$ELSE}
     cairo,cairoXlib,xlib,x,xutil,gtk2def,gtk2proc,gtk2,gdk2,gdk2x,gdk2pixbuf,glib2,
    {$ENDIF}

  {$ENDIF}

  {$IFDEF WINDOWS}
    Windows,Messages,ShellAPI,ActiveX,CommCtrl,MMSystem,
  {$ENDIF}
  LCLProc,LCLIntf,LCLType,LMessages,LResources,types,
  Classes,SysUtils,Forms,Controls,Dialogs,Graphics,ExtCtrls,Menus,
  orca_scene2d;


//=============================================================================
//=============== GLobal const/Emu Types ======================================
//=============================================================================

const
  WM_ADDUPDATERECT=WM_USER + 501;

  cnEps    =0.0000000001;
  cnKeyTime=1 / 30;
  cnFloatCount:single=0;
  cnIntCount: integer=0;
  cnEPSILON2:single=1e-30;
  cnMaxBitmapSize: integer=2048;
  cnMaxMemoryStream=4096 * 1024 * 2;

type

  TD3Color=longword;

  PD3ColorRec=^TD3ColorRec;
  TD3ColorRec=packed record
    case longword of
      0: (Color: TD3Color);
      2: (HiWord,LoWord: word);
      {$ifdef  FPC_BIG_ENDIAN}
      3: (A,R,G,B: System.byte);
      {$else}
      3: (B,G,R,A: System.byte);
      {$endif}
  end;

  PD3ColorRecArray=^TD3ColorRecArray;
  TD3ColorRecArray=array [0..100] of TD3ColorRec;

  PD3ColorArray=^TD3ColorArray;
  TD3ColorArray=array [0..0] of TD3Color;

  PD3Color24=^TD3Color24;
  TD3Color24=packed record
    case longword of
      0: (R,G,B: byte);
  end;

  PD3Color24Array=^TD3Color24Array;
  TD3Color24Array=array [0..0] of TD3Color24;

  PD3Color32=^TD3Color32;
  TD3Color32=packed record
    case longword of
      0: (R,G,B,A: byte);
  end;

  PD3Color32Array=^TD3Color32Array;
  TD3Color32Array=array [0..0] of TD3Color32;

type

  TD3Point2D=packed record
    X:single;
    Y:single;
  end;

  TD3Point=packed record
    X:single;
    Y:single;
    Z:single;
  end;

  TD3Box=packed record
    Left,Top,Near,Right,Bottom,Far:single;
  end;


  TD3TexVectorType=array [0..1] of single;
  PD3TexVector=^TD3TexVector;
  TD3TexVector=packed record
    case integer of
      0: (V: TD3TexVectorType;);
      1: (S:single;T:single;);
  end;

  PD3TexVectorArray=^TD3TexVectorArray;
  TD3TexVectorArray=array[0..0] of TD3TexVector;

  TD3VectorType=array [0..3] of single;
  PD3Vector=^TD3Vector;
  TD3Vector=packed record
    case integer of
      0: (V: TD3VectorType;);
      1: (X:single;Y:single;Z:single;W:single;);
  end;

  PD3Vector3=^TD3Vector3;

  TD3Vector3=packed record
    X:single;
    Y:single;
    Z:single;
  end;

  PD3Face=^TD3Face;

  TD3Face=packed record
    P1: TD3Vector;
    P2: TD3Vector;
    P3: TD3Vector;
  end;

  TD3VectorInt=packed record
    X: integer;
    Y: integer;
    Z: integer;
    W: integer;
  end;

  PD3VectorArray=^TD3VectorArray;
  TD3VectorArray=array[0..0 shr 2] of TD3Vector;

  TD3MatrixArray=array [0..3] of TD3Vector;

  TD3Matrix=packed record
    case integer of
      0: (M: TD3MatrixArray;);
      1: (m11,m12,m13,m14:single;
          m21,m22,m23,m24:single;
          m31,m32,m33,m34:single;
          m41,m42,m43,m44:single);
  end;

  TD3Matrix3=packed record
    m11,m12,m13:single;
    m21,m22,m23:single;
    m31,m32,m33:single;
  end;

  TD3MatrixDouble=packed record
    m11,m12,m13,m14: double;
    m21,m22,m23,m24: double;
    m31,m32,m33,m34: double;
    m41,m42,m43,m44: double;
  end;

  PD3ByteArray=^TD3ByteArray;
  TD3ByteArray=array[0..MaxInt - 1] of byte;

  PD3WordArray=^TD3WordArray;
  TD3WordArray=array[0..0] of word;

  PD3SingleArray=^TD3SingleArray;
  TD3SingleArray=array[0..0] of single;

  PD3LongwordArray=^TD3LongwordArray;
  TD3LongwordArray=array[0..0] of longword;

  TD3AABB=record
    min,max: TD3Vector;
  end;

  TD3AABBCorners=array[0..7] of TD3Vector;

  TD3BSphere=record
    Center: TD3Vector;
    Radius:single;
  end;

  TD3Frustum=record
    pLeft,pTop,pRight,pBottom,pNear,pFar: TD3Vector;
  end;

  TD3SpaceContains=(d3NoOverlap,d3ContainsFully,d3ContainsPartially);

  PD3Quaternion=^TD3Quaternion;

  TD3Quaternion=record
    ImagPart: TD3Vector;
    RealPart:single;
  end;

type

  TD3VertexFormat=(vfVertex,vfVertexNormal,vfTexVertexNormal,
    vfTexVertex,vfColorVertex,vfColorVertexNormal,
    vfColorTexVertex,vfColorTexVertexNormal,vfNone);


  TD3Vertex=packed record
    x,y,z:single;
  end;
  PD3Vertex=^TD3Vertex;

  TD3VertexArray=array[0..100] of TD3Vertex;
  PD3VertexArray=^TD3VertexArray;

  TD3VertexNormal=packed record
    x,y,z:single;
    nx,ny,nz:single;
  end;
  PD3VertexNormal=^TD3VertexNormal;

  TD3VertexNormalArray=array[0..0] of TD3VertexNormal;
  PxdVertexNormalArray=^TD3VertexNormalArray;

  TD3TexVertexNormal=packed record
    x,y,z:single;
    nx,ny,nz:single;
    tu,tv:single;
  end;
  PD3TexVertexNormal=^TD3TexVertexNormal;

  TD3TexVertexNormalArray=array[0..0] of TD3TexVertexNormal;
  PD3TexVertexNormalArray=^TD3TexVertexNormalArray;

  TD3TexVertex=packed record
    x,y,z:single;
    tu,tv:single;
  end;
  PD3TexVertex=^TD3TexVertex;

  TD3TexVertexArray=array[0..0] of TD3TexVertex;
  PD3TexVertexArray=^TD3TexVertexArray;

  TD3ColorVertex=packed record
    x,y,z:single;
    color: longword;
  end;
  PD3ColorVertex=^TD3ColorVertex;

  TD3ColorVertexArray=array[0..1000] of TD3ColorVertex;
  PD3ColorVertexArray=^TD3ColorVertexArray;

  TD3ColorVertexNormal=packed record
    x,y,z:single;
    nx,ny,nz:single;
    color: longword;
  end;
  PD3ColorVertexNormal=^TD3ColorVertexNormal;

  TD3ColorVertexNormalArray=array[0..1000] of TD3ColorVertexNormal;
  PD3ColorVertexNormalArray=^TD3ColorVertexNormalArray;

  TD3ColorTexVertex=packed record
    x,y,z:single;
    color: longword;
    tu,tv:single;
  end;
  PD3ColorTexVertex=^TD3ColorTexVertex;

  TD3ColorTexVertexArray=array[0..1000] of TD3ColorTexVertex;
  PD3ColorTexVertexArray=^TD3ColorTexVertexArray;

  TD3ColorTexVertexNormal=packed record
    x,y,z:single;
    nx,ny,nz:single;
    color: longword;
    tu,tv:single;
  end;
  PD3ColorTexVertexNormal=^TD3ColorTexVertexNormal;

  TD3ColorTexVertexNormalArray=array[0..1000] of TD3ColorTexVertexNormal;
  PD3ColorTexVertexNormalArray=^TD3ColorTexVertexNormalArray;

const

  NullPoint: TD3Point=(x: 0; y: 0);

  XTexPoint: TD3TexVector=(S: 1; T: 0);
  YTexPoint: TD3TexVector=(S: 0; T: 1);
  XYTexPoint: TD3TexVector=(S: 1; T: 1);
  NullTexPoint: TD3TexVector=(S: 0; T: 0);
  MidTexPoint: TD3TexVector=(S: 0.5; T: 0.5);

  NullTextVector: TD3TexVector=(S: 0.0; T: 0.0);
  Nulld3Vector: TD3Vector=(X: 0.0; Y: 0.0; Z: 0.0; W: 0.0);

  IdentityQuaternion: TD3Quaternion=(ImagPart: (X: 0; Y: 0; Z: 0; W: 0); RealPart: 1);

  IdentityMatrix: TD3Matrix=(m11: 1.0; m12: 0.0; m13: 0.0; m14: 0.0;
    m21: 0.0; m22: 1.0; m23: 0.0; m24: 0.0;
    m31: 0.0; m32: 0.0; m33: 1.0; m34: 0.0;
    m41: 0.0; m42: 0.0; m43: 0.0; m44: 1.0; );

  XHmgVector: TD3Vector=(X: 1; Y: 0; Z: 0; W: 0);
  YHmgVector: TD3Vector=(X: 0; Y: 1; Z: 0; W: 0);
  ZHmgVector: TD3Vector=(X: 0; Y: 0; Z: 1; W: 0);
  WHmgVector: TD3Vector=(X: 0; Y: 0; Z: 0; W: 1);
  XYHmgVector: TD3Vector=(X: 1; Y: 1; Z: 0; W: 0);
  XYZHmgVector: TD3Vector=(X: 1; Y: 1; Z: 1; W: 0);
  XYZWHmgVector: TD3Vector=(X: 1; Y: 1; Z: 1; W: 1);
  NullHmgVector: TD3Vector=(X: 0; Y: 0; Z: 0; W: 0);

  Epsilon:single=1e-40;

const
  cPI:single=3.141592654;
  cPIdiv180:single=0.017453292;
  c180divPI:single=57.29577951;
  c2PI:single=6.283185307;
  cPIdiv2:single=1.570796326;
  cPIdiv4:single=0.785398163;
  c3PIdiv4:single=2.35619449;
  cInv2PI:single=1 / 6.283185307;
  cInv360:single=1 / 360;
  c180:single=180;
  c360:single=360;
  cOneHalf:single=0.5;

  MinSingle=1.5e-45;
  MaxSingle=3.4e+38;

  ClosePolygon: TD3Point=(X: $FFFF; Y: $FFFF; Z: $FFFF);

type
  TD3Polygon=array of TD3Point;
  PD3Polygon=^TD3Polygon;

  TD3TexMode=(d3TexModulate,d3TexReplace);
  TD3ShadeMode=(d3Flat,d3Gouraud);
  TD3FillMode=(d3Solid,d3Wireframe);
  TD3FontStyle=(d3FontRegular,d3FontBold,d3FontItalic,d3FontBoldItalic);
  TD3TextAlign=(d3TextAlignCenter,d3TextAlignNear,d3TextAlignFar);
  TD3Projection=(d3ProjectionCamera,d3ProjectionScreen);
  TD3Quality=(d3LowQuality,d3HighQuality,d3SuperHighQuality);
  TD3DragMode=(d3DragManual,d3DragAutomatic);
  TD3LightType=(d3LightDirectional,d3LightPoint,d3LightSpot);
  TD3Shape3DSide=(d3Shape3DFront,d3Shape3DBack,d3Shape3DLeft);
  TD3BlendingMode=(d3BlendAdditive,d3BlendBlend);
  TD3ParticleVelocityMode=(svmAbsolute,svmRelative);
  TD3ParticleDispersionMode=(sdmFast,sdmIsotropic);

  TD3LayerAlign=(
    d3LayerNone,
    d3LayerTop,
    d3LayerLeft,
    d3LayerRight,
    d3LayerBottom,
    d3LayerMostTop,
    d3LayerMostBottom,
    d3LayerMostLeft,
    d3LayerMostRight,
    d3LayerClient,
    d3LayerCenter,
    d3LayerContents);

  TD3RenderState=(
    rs2DScene,
    rs3DScene,
    rsLightOn,
    rsLightOff,
    rsZTestOn,
    rsZTestOff,
    rsZWriteOn,
    rsZWriteOff,
    rsFrontFace,
    rsBackFace,
    rsAllFace,
    rsBlendAdditive,
    rsBlendNormal,
    rsTexNearest,
    rsTexLinear,
    rsTexDisable,
    rsTexReplace,
    rsTexModulate,
    rsFrame,
    rsSolid,
    rsFlat,
    rsGouraud);

  TD3Align=(
    d3None,
    d3Scale,
    d3Contents,
    d3TopLeft,
    d3TopRight,
    d3BottomLeft,
    d3BottomRight,
    d3Top,
    d3Left,
    d3Right,
    d3Bottom,
    d3MostLeft,
    d3MostRight,
    d3Client,
    d3Center,
    d3VertCenter,
    d3HorzCenter,
    d3Horizontal,
    d3Vertical);


//=============================================================================
//=============== Classes =====================================================
//=============================================================================
type

  TD3Canvas=class;
  TD3Scene=class;
  TD3Object=class;
  TD3VisualObject=class;
  TD3ObjectClass=class of TD3Object;
  TD3Camera=class;
  TD3Light=class;
  TD3Dummy=class;
  TD3BitmapCollection=class;
  TD3BitmapList=class;

  TCanvasPaintEvent=procedure(Sender: TObject; const Canvas: TCanvas; const ARect: TRect; var Update: boolean) of object;

  TD3Position=class(TPersistent)
  private
    FOnChange:TNotifyEvent;
    FSave: TD3Vector;
    FY:single;
    FX:single;
    FZ:single;
    FDefaultValue:TD3Point;
    FOnChangeY:TNotifyEvent;
    FOnChangeX:TNotifyEvent;
    FOnChangeZ:TNotifyEvent;
    procedure SetPoint(const Value:TD3Point);
    procedure SetX(const Value:single);
    procedure SetY(const Value:single);
    procedure SetZ(const Value:single);
    function GetPoint: TD3Point;
    function GetVector: TD3Vector;
    procedure SetVector(const Value:TD3Vector);
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadPoint(Reader:TReader);
    procedure WritePoint(Writer:TWriter);
  public
    constructor Create(const ADefaultValue:TD3Point);  virtual;
    procedure Assign(Source: TPersistent);  override;
    procedure SetPointNoChange(const P: TD3Point);
    procedure SetVectorNoChange(const P: TD3Vector);
    function Empty:boolean;
    property Point: TD3Point read GetPoint write SetPoint;
    property Vector: TD3Vector read GetVector write SetVector;
    property DefaultValue:TD3Point read FDefaultValue write FDefaultValue;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnChangeX:TNotifyEvent read FOnChangeX write FOnChangeX;
    property OnChangeY:TNotifyEvent read FOnChangeY write FOnChangeY;
    property OnChangeZ:TNotifyEvent read FOnChangeZ write FOnChangeZ;
  published
    property X:single read FX write SetX stored False;
    property Y:single read FY write SetY stored False;
    property Z:single read FZ write SetZ stored False;
  end;

  TD3Bounds=class(TPersistent)
  private
    FRight:single;
    FBottom:single;
    FTop:single;
    FLeft:single;
    FOnChange:TNotifyEvent;
    FNear:single;
    FFar:single;
    procedure SetBottom(const Value:single);
    procedure SetLeft(const Value:single);
    procedure SetRight(const Value:single);
    procedure SetTop(const Value:single);
    procedure SetFar(const Value:single);
    procedure SetNear(const Value:single);
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadRect(Reader:TReader);
    procedure WriteRect(Writer:TWriter);
    procedure ReadNewRect(Reader:TReader);
    procedure WriteNewRect(Writer:TWriter);
  public
    constructor Create;  virtual;
    procedure Assign(Source: TPersistent);  override;
    function MarginEmpty:boolean;
    function Width:single;
    function Height:single;
    function Depth:single;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
  published
    property left:single read FLeft write SetLeft stored False;
    property top:single read FTop write SetTop stored False;
    property znear:single read FNear write SetNear stored False;
    property right:single read FRight write SetRight stored False;
    property bottom:single read FBottom write SetBottom stored False;
    property zfar:single read FFar write SetFar stored False;
  end;

TD3BitmapRect=class(TPersistent)
  private
    FRight:single;
    FBottom:single;
    FTop:single;
    FLeft:single;
    FOnChange:TNotifyEvent;
    procedure SetBottom(const Value:single);
    procedure SetLeft(const Value:single);
    procedure SetRight(const Value:single);
    procedure SetTop(const Value:single);
  protected
  public
    constructor Create();  virtual;
    procedure Assign(Source: TPersistent);  override;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    function empty:boolean;
  published
    property left:single read FLeft write SetLeft;
    property top:single read FTop write SetTop;
    property right:single read FRight write SetRight;
    property bottom:single read FBottom write SetBottom;
  end;

TD3Transform=class(TPersistent)
  private
    FMatrix: TD3Matrix;
    FRotateAngle:single;
    FPosition: TD3Position;
    FScale: TD3Position;
    FSkew: TD3Position;
    FRotateCenter: TD3Position;
    FOnChanged:TNotifyEvent;
  protected
    procedure MatrixChanged(Sender: TObject);
    property Skew: TD3Position read FSkew write FSkew;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property Matrix: TD3Matrix read FMatrix;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
  published
    property Position: TD3Position read FPosition write FPosition;
    property Scale: TD3Position read FScale write FScale;
    property RotateCenter: TD3Position read FRotateCenter write FRotateCenter;
  end;

TD3Bitmap=class(TPersistent)
  private
    FBits: PD3ColorArray;
    FCanvas: TD3Canvas;
    FHeight: integer;
    FWidth: integer;
    FHandle: cardinal;
    FNeedUpdate:boolean;
    FLocked:boolean;
    FCanvasList: TList;
    FOnChange:TNotifyEvent;
    function GetCanvas: TD3Canvas;
    procedure SetHeight(const Value:integer);
    procedure SetWidth(const Value:integer);
  protected
    procedure Recreate;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadBitmap(Stream: TStream);
    procedure WriteBitmap(Stream: TStream);
    procedure AssignTo(Dest: TPersistent);  override;
  public
    Handle: cardinal;
    constructor Create(const AWidth,AHeight: integer);  virtual;
    constructor CreateFromStream(const AStream: TStream);  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    procedure SetSize(const AWidth,AHeight: integer);
    procedure Clear(const AColor: TD3Color=0);
    procedure ClearRect(const ARect: TD2Rect; const AColor: TD3Color=0);
    function LockBitmapBits(var Bits: PD3ColorArray; const Write: boolean):boolean;
    procedure UnlockBitmapBits;
    procedure SetCanvas(ACanvas: TD3Canvas);
    procedure FillAlpha(const AAlpha: byte=$FF);
    procedure LoadFromFile(const AFileName: string; const Rotate:single=0);
    procedure LoadThumbnailFromFile(const AFileName: string; const AFitWidth,AFitHeight:single; const UseEmbedded: boolean=True);
    procedure SaveToFile(const AFileName: string; const Params: string='');
    procedure LoadFromStream(const AStream: TStream);
    procedure SaveToStream(const AStream: TStream);
    property Canvas: TD3Canvas read GetCanvas;
    property Width: integer read FWidth write SetWidth;
    property Height: integer read FHeight write SetHeight;
    property Bits: PD3ColorArray read FBits;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property CanvasList: TList read FCanvasList;
    property NeedUpdate: boolean read FNeedUpdate write FNeedUpdate;
  published
  end;

TD3Filter=class(TPersistent)
  published
    class function GetFileTypes: string;  virtual;
    class function GetImageSize(const AFileName: string): TD3Point;  virtual;
    function LoadFromFile(const AFileName: string; const Rotate:single; var Bitmap: TD3Bitmap):boolean;  virtual; abstract;
    function LoadThumbnailFromFile(const AFileName: string; const AFitWidth,AFitHeight:single; const UseEmbedded:boolean;
                                   var Bitmap: TD3Bitmap):boolean;  virtual; abstract;
    function SaveToFile(const AFileName: string; var Bitmap: TD3Bitmap; const Params: string=''):boolean;  virtual; abstract;
    function LoadFromStream(const AStream: TStream; var Bitmap: TD3Bitmap):boolean;  virtual; abstract;
    function SaveToStream(const AStream: TStream; var Bitmap: TD3Bitmap; const Format: string;
                          const Params: string=''):boolean;  virtual; abstract;
  end;

TD3FilterClass=class of TD3Filter;

TD3MeshData=class(TPersistent)
  private
    FNeedUpdate:boolean;
    FHandle: cardinal;
    FOnDestroyHandle:TNotifyEvent;
    FOnChanged:TNotifyEvent;
    function GetNormals: ansistring;
    function GetPoints: ansistring;
    function GetTexCoordinates: ansistring;
    procedure SetNormals(const Value:ansistring);
    procedure SetPoints(const Value:ansistring);
    procedure SetTexCoordinates(const Value:ansistring);
    function GetTriangleIndices: ansistring;
    procedure SetTriangleIndices(const Value:ansistring);
  protected
    procedure DefineProperties(Filer: TFiler);  override;
    procedure ReadMesh(Stream: TStream);
    procedure WriteMesh(Stream: TStream);
  public
    MeshVertices: array of TD3TexVertexNormal;
    MeshIndices: array of word;
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    procedure CalcNormals;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    property Handle: cardinal read FHandle write FHandle;
    property NeedUpdate: boolean read FNeedUpdate write FNeedUpdate;
    property OnDestroyHandle:TNotifyEvent read FOnDestroyHandle write FOnDestroyHandle;
  published
    property Normals: ansistring read GetNormals write SetNormals stored False;
    property Points: ansistring read GetPoints write SetPoints stored False;
    property TexCoordinates: ansistring read GetTexCoordinates write SetTexCoordinates stored False;
    property TriangleIndices: ansistring read GetTriangleIndices write SetTriangleIndices stored False;
  end;

TD3Material=class(TPersistent)
  private
    FOnChanged:TNotifyEvent;
    FAmbient: TD3Color;
    FBitmap: string;
    FModulation: TD3TexMode;
    FLighting:boolean;
    FTempBitmap: TD3Bitmap;
    FShadeMode: TD3ShadeMode;
    FFillMode: TD3FillMode;
    FBitmapTile,FBitmapTileY:single;
    FBitmapRect: TD3BitmapRect;
    procedure SetBitmap(const Value:string);
    procedure SetTempBitmap(const Value:TD3Bitmap);
    procedure SetDiffuse(const Value:string);
    function GetDiffuse: string;
    function GetAmbient: string;
    procedure SetAmbient(const Value:string);
    function GetBitmap: TD3Bitmap;
    procedure SetModulation(const Value:TD3TexMode);
    procedure SetLighting(const Value:boolean);
    procedure SetNativeDiffuse(const Value:TD3Color);
    procedure SetShadeMode(const Value:TD3ShadeMode);
    procedure SetFillMode(const Value:TD3FillMode);
    procedure SetNativeAmbient(const Value:TD3Color);
    procedure SetBitmapTile(const Value:single);
    procedure SetBitmapTileY(const Value:single);
    procedure SetBitmapRect(const Value:TD3BitmapRect);
  protected
    FDiffuse: TD3Color;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
    property NativeAmbient: TD3Color read FAmbient write SetNativeAmbient;
    property NativeDiffuse: TD3Color read FDiffuse write SetNativeDiffuse;
    property NativeBitmap: TD3Bitmap read GetBitmap;
    property TempBitmap: TD3Bitmap read FTempBitmap write SetTempBitmap;
  published
    property Diffuse: string read GetDiffuse write SetDiffuse;
    property Ambient: string read GetAmbient write SetAmbient;
    property Lighting: boolean read FLighting write SetLighting  default True;
    property Bitmap: string read FBitmap write SetBitmap;
    property BitmapMode: TD3TexMode read FModulation write SetModulation;
    property BitmapTileX:single read FBitmapTile write SetBitmapTile;
    property BitmapTileY:single read FBitmapTileY write SetBitmapTileY;
    property BitmapRect: TD3BitmapRect read FBitmapRect write SetBitmapRect;
    property FillMode: TD3FillMode read FFillMode write SetFillMode;
    property ShadeMode: TD3ShadeMode read FShadeMode write SetShadeMode  default d3Gouraud;
  end;

TD3Font=class(TPersistent)
  private
    FSize:single;
    FFamily: string;
    FStyle: TD3FontStyle;
    FOnChanged:TNotifyEvent;
    procedure SetFamily(const Value:string);
    procedure SetSize(const Value:single);
    procedure SetStyle(const Value:TD3FontStyle);
    function isFamilyStored:boolean;
    function isSizeStored:boolean;
  protected
    procedure AssignTo(Dest: TPersistent);  override;
  public
    constructor Create;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    property OnChanged:TNotifyEvent read FOnChanged write FOnChanged;
  published
    property Family: string read FFamily write SetFamily stored isFamilyStored;
    property Size:single read FSize write SetSize stored isSizeStored;
    property Style: TD3FontStyle read FStyle write SetStyle;
  end;


TD3Canvas=class(TPersistent)
  private
    FLighting:boolean;
    FAmbient: TD3Color;
    FMaterial: TD3Material;
    FFont: TD3Font;
    procedure SetMaterial(const M: TD3Material);
  protected
    FWidth,FHeight: integer;
    FBitmap: TD3Bitmap;
    FWnd: cardinal;
    FScene: TD3Scene;
    FLockable:boolean;
    FQuality: TD3Quality;
    FBitmaps: TList;
    FHandles: TList;
    FBuffered:boolean;
    {$IFDEF WINDOWS}
    FBufferDC: cardinal;
    FBufferBits: Pointer;
    FBufferHandle: cardinal;
    FBitmapInfo: TBitmapInfo;
    {$ENDIF}

    {$IFDEF UNIX}

     {$IFDEF DARWIN}
      FBufferBits: Pointer;
      FBufferHandle: CGContextRef;
     {$ELSE}
      FBufferBits: Pointer;
      FBufferHandle: Pointer;
     {$ENDIF}

    {$ENDIF}
    FRenderStates: array [TD3RenderState] of THandle;
    FChangeStateCount: integer;
    FCurrentBitmap: array [0..8] of TD3Bitmap;
    FCurrentVertexFormat: TD3VertexFormat;
    FCurrentStates: array [TD3RenderState] of boolean;
    FCurrentCamera: TD3Camera;
    FCurrentCameraMatrix: TD3Matrix;
    FCurrentCameraInvMatrix: TD3Matrix;
    FCurrentMatrix: TD3Matrix;
    FCurrentLights: TList;
    FPaintToMatrix: TD3Matrix;
    FSaveStates: array [TD3RenderState] of boolean;
    FSaveMatrix: TD3Matrix;
    procedure MaterialChanged(Sender: TObject);  virtual;
    procedure CreateRenderStateList(AState: TD3RenderState);  virtual; abstract;
    procedure ApplyRenderState(AState: TD3RenderState);  virtual; abstract;
    function GetProjectionMatrix: TD3Matrix;  virtual;
    function GetScreenMatrix: TD3Matrix;  virtual;
    procedure UpdateBitmap(Bitmap: TD3Bitmap);  virtual;
    procedure AddHandle(const Bitmap: TD3Bitmap; const Handle: cardinal);
    procedure ChangeHandle(const Bitmap: TD3Bitmap; const Handle: cardinal);
    procedure RemoveHandle(const Bitmap: TD3Bitmap);
    function GetHandle(const Bitmap: TD3Bitmap): cardinal;
    procedure DestroyBitmap(const Bitmap: TD3Bitmap);  virtual; abstract;
    procedure AssignTo(Dest: TPersistent);  override;
  public
    constructor Create(const AWnd: cardinal; const AWidth,AHeight: integer; const ALockable:boolean; const Quality: TD3Quality);  virtual;
    constructor CreateFromBitmap(const ABitmap: TD3Bitmap);  virtual;
    destructor Destroy;  override;
    procedure SetQuality(const Quality: TD3Quality);  virtual;
    procedure SaveToStream(S: TStream);
    procedure SaveToBits(Bits: Pointer);
    procedure Reset;
    function BeginScene:boolean;  virtual;
    procedure FlushBuffer;  virtual;
    procedure FreeBuffer;  virtual;
    procedure ResizeBuffer(const AWidth,AHeight: integer);  virtual;
    procedure Clear(const AColor: TD3Color);  virtual; abstract;
    procedure ClearARGB(const AWidth,AHeight: integer; const Bits: PD3ColorArray);  virtual; abstract;
    procedure CopyBits(const Bits: PD3ColorArray);  virtual; abstract;
    procedure SetMatrix(const M: TD3Matrix);  virtual;
    procedure SetCamera(const Camera: TD3Camera);  virtual;
    procedure SetLight(const Light: TD3Light);  virtual;
    procedure SetRenderState(const State: TD3RenderState);  virtual;
    procedure DrawLine(const StartPoint,EndPoint: TD3Vector; const Opacity:single);  virtual; abstract;
    procedure DrawRect(const StartPoint,EndPoint: TD3Vector; const Opacity:single);  virtual; abstract;
    procedure DrawCube(const Center,Size: TD3Vector; const Opacity:single);  virtual; abstract;
    procedure FillCube(const Center,Size: TD3Vector; const Opacity:single);  virtual; abstract;
    procedure FillMesh(const Center,Size: TD3Vector; const MeshData: TD3MeshData; const Opacity:single);  virtual; abstract;
    procedure FillColorTexVertexNormal(const Vertices: array of TD3ColorTexVertexNormal; const Indices: array of word;
                                       const Opacity:single);  virtual; abstract;
    procedure FillPolygon(const Center,Size: TD3Vector; const Rect: TD2Rect; const Points: TD2Polygon; const Opacity:single;
                          Front: boolean=True; Back: boolean=True; Left: boolean=True);  virtual; abstract;
    procedure FillRect(const Rect: TD2Rect; const Depth,Opacity:single);
    procedure DrawText(const AX,AY:single; const AText: WideString; const Opacity:single);  virtual; abstract;
    function MeasureText(const AText: WideString): TD3Point;  virtual; abstract;
    procedure Pick(x,y:single; const AProj: TD3Projection; var RayPos,RayDir: TD3Vector);
    function WorldToScreen(const AProj: TD3Projection; const P: TD3Point): TD3Point;
    procedure SetMaterialWithOpacity(M: TD3Material; Opacity:single);
    property Height: integer read FHeight;
    property Width: integer read FWidth;
    property Material: TD3Material read FMaterial write SetMaterial;
    property Font: TD3Font read FFont write FFont;
    property Ambient: TD3Color read FAmbient write FAmbient;
    property Lighting: boolean read FLighting write FLighting;
    property CurrentMatrix: TD3Matrix read FCurrentMatrix;
    property CurrentCamera: TD3Camera read FCurrentCamera;
    property CurrentCameraMatrix: TD3Matrix read FCurrentCameraMatrix;
    property CurrentCameraInvMatrix: TD3Matrix read FCurrentCameraInvMatrix;
    property Bitmap: TD3Bitmap read FBitmap;
  published
  end;

TD3CanvasClass=class of TD3Canvas;

TD3Physics=class(TPersistent)
  protected
    FSpaceWorld: integer;
    FScreenWorld: integer;
    FScene: TD3Scene;
  public
    constructor Create(const Scene: TD3Scene);  virtual;
    destructor Destroy;  override;
    function CreateWorld: integer;  virtual; abstract;
    procedure DestroyWorld(const AWorld: integer);  virtual; abstract;
    procedure UpdateWorld(const AWorld: integer; const DeltaTime:single);  virtual; abstract;
    function Collise(const AOwner: TD3VisualObject; var AList: TList): integer;  virtual; abstract;
    function ComplexCollise(const AOwner: TD3VisualObject; var AList: TList): integer;  virtual; abstract;
    function ObjectByBody(const Body: integer): TD3VisualObject;  virtual; abstract;
    function CreateBox(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  virtual; abstract;
    function CreateSphere(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  virtual; abstract;
    function CreateCone(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  virtual; abstract;
    function CreateCylinder(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  virtual; abstract;
    procedure DestroyBody(var ABody: integer);  virtual; abstract;
    procedure SetBodyMatrix(const Body: integer; const M: TD3Matrix);  virtual; abstract;
    function GetBodyMatrix(const Body: integer): TD3Matrix;  virtual; abstract;
    procedure AddForce(const AOwner: TD3VisualObject; const Force: TD3Vector);  virtual; abstract;
    procedure Explode(const AWorld: integer; const Position: TD3Vector; const Radius,Force:single);  virtual; abstract;
    procedure Wind(const AWorld: integer; const Dir: TD3Vector; const Force:single);  virtual; abstract;
    property ScreenWorld: integer read FScreenWorld;
    property SpaceWorld: integer read FSpaceWorld;
  end;

  TD3PhysicsClass=class of TD3Physics;

  TD3MouseEvent=procedure(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector) of object;
  TD3MouseMoveEvent=procedure(Sender: TObject; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector) of object;
  TD3MouseWheelEvent=procedure(Sender: TObject; Shift: TShiftState; WheelDelta: integer; MousePos: TD3Point; var Handled: boolean) of object;
  TD3KeyEvent=procedure(var Key: word; var KeyChar: System.widechar; Shift: TShiftState) of object;
  TD3ProcessTickEvent=procedure(Sender: TObject; time,deltaTime:single) of object;

TD3Object=class(TComponent)
  private
    FParent: TD3Object;
    FScene: TD3Scene;
    FStored:boolean;
    FResourceName: string;
    FTagFloat:single;
    FTagString: string;
    FTagObject: TObject;
    FOnTick: TD3ProcessTickEvent;
    procedure UpdateChildScene;
    procedure ReaderSetName(Reader:TReader; Component: TComponent; var Name: string);
    procedure ReaderError(Reader:TReader; const Message: string; var Handled: boolean);
    function GetChild(Index: integer): TD3Object;
    function GetChildrenCount: integer;
  protected
    FDisableEffect:boolean;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure SetParent(const Value:TD3Object);  virtual;
    procedure DoAniFinished(Sender: TObject);
    procedure SetResourceName(const Value:string);  virtual;
  public
    FChildren: TList;
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure SetParentComponent(Value:TComponent);  override;
    function  GetParentComponent: TComponent;  override;
    function  HasParent:boolean;  override;
    function  Clone(const AOwner: TComponent): TD3Object;
    procedure CloneChildFromStream(AStream: TStream);
    procedure ProcessTick(time,deltaTime:single);  virtual;
    procedure AddObject(AObject: TD3Object);  virtual;
    procedure RemoveObject(AObject: TD3Object);  virtual;
    procedure DeleteChildren;  virtual;
    procedure BringToFront;
    procedure SendToBack;
    procedure AddObjectsToList(const AList: TList);  virtual;
    function  FindObject(const AClass: TD3ObjectClass): TD3Object;
    procedure LoadFromStream(const AStream: TStream);
    procedure SaveToStream(const Stream: TStream);
    procedure LoadFromBinStream(const AStream: TStream);
    procedure SaveToBinStream(const AStream: TStream);
    function  FindResource(const AResource: string): TD3Object;
    procedure StartAnimation(const AName: string);  virtual;
    procedure StopAnimation(const AName: string);  virtual;
    procedure StartTriggerAnimation(AInstance: TD3Object; ATrigger: string);  virtual;
    procedure StopTriggerAnimation(AInstance: TD3Object);  virtual;
    procedure AnimateFloat(const APropertyName: string; const NewValue:single; Duration:single=0.2;
                           AType: TD2AnimationType=d2AnimationIn; AInterpolation: TD2InterpolationType=d2InterpolationLinear);
    procedure AnimateColor(const APropertyName: string; const NewValue:string; Duration:single=0.2;
                           AType: TD2AnimationType=d2AnimationIn; AInterpolation: TD2InterpolationType=d2InterpolationLinear);
    procedure AnimateFloatWait(const APropertyName: string; const NewValue:single; Duration:single=0.2;
                           AType: TD2AnimationType=d2AnimationIn; AInterpolation: TD2InterpolationType=d2InterpolationLinear);
    procedure AnimateColorWait(const APropertyName: string; const NewValue:string; Duration:single=0.2;
                           AType: TD2AnimationType=d2AnimationIn; AInterpolation: TD2InterpolationType=d2InterpolationLinear);
    procedure AnimateStop(const APropertyName: string);
    function IsVisual:boolean;
    function Visual: TD3VisualObject;
    property Scene: TD3Scene read FScene;
    property Stored: boolean read FStored write FStored;
    property TagObject: TObject read FTagObject write FTagObject;
    property TagFloat:single read FTagFloat write FTagFloat;
    property TagString: string read FTagString write FTagString;
    property ChildrenCount: integer read GetChildrenCount;
    property Children[Index: integer]: TD3Object read GetChild;
  published
    property Parent: TD3Object read FParent write SetParent stored False;
    property ResourceName: string read FResourceName write SetResourceName;
    property OnTick: TD3ProcessTickEvent read FOnTick write FOnTick;
  end;

TD3BitmapObject=class(TD3Object)
  private
    FBitmap: TD3Bitmap;
    procedure SetBitmap(const Value:TD3Bitmap);
  protected
    procedure SetName(const NewName: TComponentName);  override;
    procedure SetResourceName(const Value:string);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Bitmap: TD3Bitmap read FBitmap write SetBitmap;
  end;


TD3AniThread=class(TD2Timer)
  private
    FAniList: TList;
    FTime,FDeltaTime:single;
    procedure OneStep;
    procedure DoSyncTimer(Sender: TObject);
  public
    constructor Create;
    destructor Destroy;  override;
  end;

  TD3Animation=class(TD3Object)
  private
    FDuration:single;
    FDelay,FDelayTime:single;
    FInverse:boolean;
    FTrigger: string;
    FLoop:boolean;
    FPause:boolean;
    FOnFinish:TNotifyEvent;
    FInterpolation: TD2InterpolationType;
    FAnimationType: TD2AnimationType;
    FEnabled,FLoadEnabled:boolean;
    FAutoReverse:boolean;
    procedure SetEnabled(const Value:boolean);
  protected
    FRunning:boolean;
    FTime:single;
    function NormalizedTime:single;
    procedure ProcessAnimation;  virtual;
    procedure Loaded;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure ProcessTick(time,deltaTime:single);  override;
    procedure Start;  virtual;
    procedure Stop;  virtual;
    procedure StartTrigger(AInstance: TD3Object; ATrigger: string);  virtual;
    property Pause: boolean read FPause write FPause;
  published
    property AnimationType: TD2AnimationType read FAnimationType write FAnimationType  default d2AnimationIn;
    property AutoReverse: boolean read FAutoReverse write FAutoReverse  default False;
    property Duration:single read FDuration write FDuration;
    property Delay:single read FDelay write FDelay;
    property Enabled: boolean read FEnabled write SetEnabled  default False;
    property Interpolation: TD2InterpolationType read FInterpolation write FInterpolation  default d2InterpolationLinear;
    property Inverse: boolean read FInverse write FInverse;
    property Loop: boolean read FLoop write FLoop;
    property Trigger: string read FTrigger write FTrigger;
    property OnFinish:TNotifyEvent read FOnFinish write FOnFinish;
  end;


TD3CollisionEvent=procedure(Sender: TObject; CollisionObject: TD3VisualObject; Point,Normal: TD3Vector) of object;
TD3PaintEvent=procedure(Sender: TObject; const Canvas: TD3Canvas) of object;

  TD3DragObject=record
    Source: TObject;
    Files: array of WideString;
    Data: variant;
  end;

  TD3DragEnterEvent=procedure(Sender: TObject; const Data: TD3DragObject; const Point: TD3Point) of object;
  TD3DragOverEvent=procedure(Sender: TObject; const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point; var Accept: boolean) of object;
  TD3DragDropEvent=procedure(Sender: TObject; const Data: TD3DragObject; const Point: TD3Point) of object;
  TD3DragLeaveEvent=procedure(Sender: TObject) of object;

{ TD3VisualObject }

TD3VisualObject=class(TD3Object)
  private
    FVisible:boolean;
    FOnMouseUp: TD3MouseEvent;
    FOnMouseDown: TD3MouseEvent;
    FOnMouseMove: TD3MouseMoveEvent;
    FOnMouseWheel: TD3MouseWheelEvent;
    FOnClick:TNotifyEvent;
    FOnDblClick:TNotifyEvent;
    FMouseInObject:boolean;
    FHitTest:boolean;
    FClipChildren:boolean;
    FAutoCapture:boolean;
    FAlign: TD3Align;
    FLocked:boolean;
    FTempCanvas: TD3Canvas;
    FPosition: TD3Position;
    FQuaternion: TD3Quaternion;
    FRotateAngle: TD3Position;
    FScale: TD3Position;
    FSkew: TD3Position;
    FCanFocused:boolean;
    FIsMouseOver:boolean;
    FIsFocused:boolean;
    FRotateCenter: TD3Position;
    FZWrite:boolean;
    FProjection: TD3Projection;
    FVelocity: TD3Position;
    FDynamic:boolean;
    FCollider:boolean;
    FColliseTrack:boolean;
    FOnCollision: TD3CollisionEvent;
    FMargins: TD3Bounds;
    FPadding: TD3Bounds;
    FOnKeyUp: TD3KeyEvent;
    FOnKeyDown: TD3KeyEvent;
    FOnPaint,FOnBeforePaint: TD3PaintEvent;
    FDesignHide:boolean;
    FTwoSide:boolean;
    FDragMode: TD3DragMode;
    FDragDisableHighlight:boolean;
    FOnDragEnter: TD3DragEnterEvent;
    FOnDragDrop: TD3DragDropEvent;
    FOnDragEnd:TNotifyEvent;
    FOnDragLeave: TD3DragLeaveEvent;
    FOnDragOver: TD3DragOverEvent;
    FIsDragOver:boolean;
    FShowHint:boolean;
    FHint: WideString;
    FPopupMenu: TPopupMenu;
    FPressed,FDoubleClick:boolean;
    FCursor,FLoadCursor: TCursor;
    FShowContextMenu:boolean;
    function  GetInvertAbsoluteMatrix: TD3Matrix;
    procedure SetHitTest(const Value:boolean);
    procedure SetClipChildren(const Value:boolean);
    function  CheckHitTest(const AHitTest: boolean):boolean;
    procedure SetAlign(const Value:TD3Align);
    function  GetCanvas: TD3Canvas;
    procedure SetLocked(const Value:boolean);
    procedure SetTempCanvas(const Value:TD3Canvas);
    procedure SetOpacity(const Value:single);
    function  isOpacityStored:boolean;
    function  GetAbsolutePosition: TD3Vector;
    function  GetAbsoluteUp: TD3Vector;
    function  GetAbsoluteDirection: TD3Vector;
    function  GetAbsoluteRight: TD3Vector;
    procedure SetAbsolutePosition(Value:TD3Vector);
    function  GetScreenBounds: TD2Rect;
    procedure ReadQuaternion(Reader:TReader);
    procedure WriteQuaternion(Writer:TWriter);
    procedure SetZWrite(const Value:boolean);
    procedure SetProjection(const Value:TD3Projection);
    procedure SetCollider(const Value:boolean);
    procedure SetDynamic(const Value:boolean);
    procedure SetDesignHide(const Value:boolean);
    procedure MarginsChanged(Sender: TObject);
    procedure PaddingChanged(Sender: TObject);
  protected
    FHeight,FLastHeight:single;
    FWidth,FLastWidth:single;
    FDepth,FLastDepth:single;
    FLocalMatrix: TD3Matrix;
    FAbsoluteMatrix: TD3Matrix;
    FRecalcAbsolute:boolean;
    FDisableAlign:boolean;
    FCanResize,FCanRotate:boolean;
    FBody: integer;
    FDesignInteract:boolean;
    FOpacity,FAbsoluteOpacity:single;
    FRecalcOpacity:boolean;
    function GetHint: string;
    procedure SetHint(const Value:string);
    procedure Loaded;  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure SetVisible(const Value:boolean);  virtual;
    procedure SetHeight(const Value:single);  virtual;
    procedure SetWidth(const Value:single);  virtual;
    procedure SetDepth(const Value:single);  virtual;
    function  GetAbsoluteMatrix: TD3Matrix;  virtual;
    function  GetAbsoluteOpacity:single;  virtual;
    procedure RecalcOpacity;  virtual;
    procedure DesignSelect;  virtual;
    procedure DesignClick;  virtual;
    procedure Capture;
    procedure ReleaseCapture;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  virtual;
    procedure MouseMove(Shift: TShiftState; X,Y,d3,Dy:single; rayPos,rayDir: TD3Vector);  virtual;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  virtual;
    procedure MouseWheel(Shift: TShiftState; WheelDelta: integer; var Handled: boolean);  virtual;
    procedure KeyDown(var Key: word; var KeyChar: System.widechar; Shift: TShiftState);  virtual;
    procedure KeyUp(var Key: word; var KeyChar: System.widechar; Shift: TShiftState);  virtual;
    procedure MouseEnter;  virtual;
    procedure MouseLeave;  virtual;
    procedure EnterFocus;  virtual;
    procedure KillFocus;  virtual;
    procedure Click;  virtual;
    procedure DblClick;  virtual;
    procedure ContextMenu(const ScreenPosition: TD2Point);  virtual;
    procedure DragEnter(const Data: TD3DragObject; const Point: TD3Point);  virtual;
    procedure DragOver(const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point; var Accept: boolean);  virtual;
    procedure DragDrop(const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point);  virtual;
    procedure DragLeave;  virtual;
    procedure DragEnd;  virtual;
    function  DoHintShow(var Message: TLMessage):boolean;  virtual;
    procedure ApplyResource;  virtual;
    procedure PreRender;  virtual;
    procedure BeforePaint;  virtual;
    procedure AfterPaint;  virtual;
    procedure Paint;  virtual;
    procedure PaintChildren;  virtual;
    procedure MatrixChanged(Sender: TObject);  virtual;
    procedure RotateXChanged(Sender: TObject);  virtual;
    procedure RotateYChanged(Sender: TObject);  virtual;
    procedure RotateZChanged(Sender: TObject);  virtual;
    property MouseInObject: boolean read FMouseInObject write FMouseInObject;
    property Skew: TD3Position read FSkew write FSkew;
    property TempCanvas: TD3Canvas read FTempCanvas write SetTempCanvas;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure PaintToBitmap(ABitmap: TD3Bitmap; AWidth,AHeight: integer; ClearColor: TD3Color);
    procedure PaintToVxBitmap(ABitmap: TD2Bitmap; AWidth,AHeight: integer; ClearColor: TD3Color; AutoFit: boolean=False);
    procedure CreateHighQualitySnapshot(ABitmap: TD2Bitmap; AWidth,AHeight: integer; ClearColor: TD3Color; Quality: integer);
    procedure CreateTileSnapshot(ABitmap: TD3Bitmap; AWidth,AHeight,OffsetX,OffsetY: integer; Scale:single; ClearColor: TD3Color);
    procedure RecalcAbsolute;  virtual;
    function AbsoluteToLocal(P: TD3Point): TD3Point;  virtual;
    function LocalToAbsolute(P: TD3Point): TD3Point;  virtual;
    function AbsoluteToLocalVector(P: TD3Vector): TD3Vector;  virtual;
    function LocalToAbsoluteVector(P: TD3Vector): TD3Vector;  virtual;
    function ObjectByPoint(X,Y:single; AProjection: TD3Projection; var Distance:single): TD3VisualObject;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  virtual;
    procedure ResetRotateAngle;
    function FindTarget(Shift: TShiftState; const APoint: TD3Point; AProjection: TD3Projection; const Data: TD3DragObject; var Distance:single): TD3VisualObject;
    procedure ProcessTick(time,deltaTime:single);  override;
    // physics
    procedure Back;
    procedure Collision(CollisionObject: TD3VisualObject; Point,Normal: TD3Vector);  virtual;
    procedure CreateBody;  virtual;
    procedure DestroyBody;  virtual;
    procedure RecreateBody;
    procedure Spawn;  virtual;
    procedure AddForce(const Force: TD3Vector);
    procedure GetTabOrderList(const AList: TList);  virtual;
    procedure Realign;  virtual;
    procedure RealignLayers;
    procedure SetFocus;
    procedure Repaint;
    procedure Lock;
    property AbsoluteMatrix: TD3Matrix read GetAbsoluteMatrix;
    property LocalMatrix: TD3Matrix read FLocalMatrix;
    property AbsolutePosition: TD3Vector read GetAbsolutePosition write SetAbsolutePosition;
    property AbsoluteUp: TD3Vector read GetAbsoluteUp;
    property AbsoluteDirection: TD3Vector read GetAbsoluteDirection;
    property AbsoluteRight: TD3Vector read GetAbsoluteRight;
    property AbsoluteOpacity:single read GetAbsoluteOpacity;
    property InvertAbsoluteMatrix: TD3Matrix read GetInvertAbsoluteMatrix;
    property Canvas: TD3Canvas read GetCanvas;
    property DesignInteract: boolean read FDesignInteract;
    property Projection: TD3Projection read FProjection write SetProjection;
    property AutoCapture: boolean read FAutoCapture write FAutoCapture  default False;
    property RotateCenter: TD3Position read FRotateCenter write FRotateCenter;
    property ScreenBounds: TD2Rect read GetScreenBounds;
    property Align: TD3Align read FAlign write SetAlign;
    property Body: integer read FBody;
    property Collider: boolean read FCollider write SetCollider;
    property ColliseTrack: boolean read FColliseTrack write FColliseTrack;
    property Dynamic: boolean read FDynamic write SetDynamic;
    property Velocity: TD3Position read FVelocity write FVelocity;
    property Margins: TD3Bounds read FMargins write FMargins;
    property Padding: TD3Bounds read FPadding write FPadding;
    property CanFocused: boolean read FCanFocused write FCanFocused;
    property HintW: WideString read FHint write FHint;
  published
    property IsDragOver: boolean read FIsDragOver;
    property IsMouseOver: boolean read FIsMouseOver;
    property IsFocused: boolean read FIsFocused;
    property IsVisible: boolean read FVisible;
    property Cursor: TCursor read FCursor write FCursor  default crDefault;
    property DragMode: TD3DragMode read FDragMode write FDragMode;
    property DragDisableHighlight: boolean read FDragDisableHighlight write FDragDisableHighlight;
    property Position: TD3Position read FPosition write FPosition;
    property Scale: TD3Position read FScale write FScale;
    property RotateAngle: TD3Position read FRotateAngle write FRotateAngle;
    property Locked: boolean read FLocked write SetLocked;
    property Width:single read FWidth write SetWidth;
    property Height:single read FHeight write SetHeight;
    property Depth:single read FDepth write SetDepth;
    property DesignHide: boolean read FDesignHide write SetDesignHide;
    property Opacity:single read FOpacity write SetOpacity stored isOpacityStored;
    property HitTest: boolean read FHitTest write SetHitTest  default True;
    property Hint: string read GetHint write SetHint;
    property ShowHint: boolean read FShowHint write FShowHint  default False;
    property ShowContextMenu: boolean read FShowContextMenu write FShowContextMenu  default True;
    property TwoSide: boolean read FTwoSide write FTwoSide  default False;
    property Visible: boolean read FVisible write SetVisible  default True;
    property ZWrite: boolean read FZWrite write SetZWrite  default True;
    property OnDragEnter: TD3DragEnterEvent read FOnDragEnter write FOnDragEnter;
    property OnDragLeave: TD3DragLeaveEvent read FOnDragLeave write FOnDragLeave;
    property OnDragOver: TD3DragOverEvent read FOnDragOver write FOnDragOver;
    property OnDragDrop: TD3DragDropEvent read FOnDragDrop write FOnDragDrop;
    property OnDragEnd:TNotifyEvent read FOnDragEnd write FOnDragEnd;
    property OnClick:TNotifyEvent read FOnClick write FOnClick;
    property OnDblClick:TNotifyEvent read FOnDblClick write FOnDblClick;
    property OnMouseDown: TD3MouseEvent read FOnMouseDown write FOnMouseDown;
    property OnMouseMove: TD3MouseMoveEvent read FOnMouseMove write FOnMouseMove;
    property OnMouseUp: TD3MouseEvent read FOnMouseUp write FOnMouseUp;
    property OnMouseWheel: TD3MouseWheelEvent read FOnMouseWheel write FOnMouseWheel;
    property OnKeyDown: TD3KeyEvent read FOnKeyDown write FOnKeyDown;
    property OnKeyUp: TD3KeyEvent read FOnKeyUp write FOnKeyUp;
    property OnBeforePaint: TD3PaintEvent read FOnBeforePaint write FOnBeforePaint;
    property OnPaint: TD3PaintEvent read FOnPaint write FOnPaint;
    property OnCollision: TD3CollisionEvent read FOnCollision write FOnCollision;
  end;

TD3Camera=class(TD3VisualObject)
  private
    FSaveCamera: TD3Camera;
    FTarget: TD3VisualObject;
    procedure SetTarget(const Value:TD3VisualObject);
  protected
    procedure Paint;  override;
    procedure DesignClick;  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property HitTest  default False;
    property Target: TD3VisualObject read FTarget write SetTarget;
  end;

TD3Light=class(TD3VisualObject)
  private
    FEnabled:boolean;
    FLightType: TD3LightType;
    procedure SetLightType(const Value:TD3LightType);
  protected
    procedure PreRender;  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property HitTest  default False;
    property Enabled: boolean read FEnabled write FEnabled  default True;
    property LightType: TD3LightType read FLightType write SetLightType;
  end;

TD3Dummy=class(TD3VisualObject)
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
    property HitTest  default False;
  end;

TD3ProxyObject=class(TD3VisualObject)
  private
    FSourceObject: TD3VisualObject;
    procedure SetSourceObject(const Value:TD3VisualObject);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
    property SourceObject: TD3VisualObject read FSourceObject write SetSourceObject;
  end;

TD3CustomLayer=class(TD3VisualObject)
  private
    FOnLayerMouseMove: TMouseMoveEvent;
    FOnLayerMouseDown: TMouseEvent;
    FOnLayerMouseUp: TMouseEvent;
    FLayerAlign: TD3LayerAlign;
    FDisableLayerEvent:boolean;
    procedure SetLayerAlign(const Value:TD3LayerAlign);
  protected
    procedure BeforePaint;  override;
    procedure Paint;  override;
    procedure SetDepth(const Value:single);  override;
    procedure SetWidth(const Value:single);  override;
    procedure SetHeight(const Value:single);  override;
    procedure LayerMouseMove(Shift: TShiftState; X,Y:single);  virtual;
    procedure LayerMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single);  virtual;
    procedure LayerMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single);  virtual;
    procedure MouseMove(Shift: TShiftState; X,Y,d3,Dy:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
    property LayerAlign: TD3LayerAlign read FLayerAlign write SetLayerAlign;
  published
    property OnLayerMouseMove: TMouseMoveEvent read FOnLayerMouseMove write FOnLayerMouseMove;
    property OnLayerMouseDown: TMouseEvent read FOnLayerMouseDown write FOnLayerMouseDown;
    property OnLayerMouseUp: TMouseEvent read FOnLayerMouseUp write FOnLayerMouseUp;
  end;

TD3CustomBufferLayer=class(TD3CustomLayer)
  private
    FResolution: integer;
    FModulationColor: TD3Color;
    FOnUpdateBuffer:TNotifyEvent;
    function GetModulationColor: string;
    procedure SetModulationColor(const Value:string);
    procedure SetResolution(const Value:integer);
  protected
    FBuffer: TD3Bitmap;
    FPlane: TD3MeshData;
    FPlaneScreen: TD3MeshData;
    FLayerHeight: integer;
    FLayerWidth: integer;
    procedure BeforePaint;  override;
    procedure Paint;  override;
    procedure SetDepth(const Value:single);  override;
    procedure SetWidth(const Value:single);  override;
    procedure SetHeight(const Value:single);  override;
    procedure MouseMove(Shift: TShiftState; X,Y,d3,Dy:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
    function GetBitmap: TD3Bitmap;  virtual;
    procedure SetName(const NewName: TComponentName);  override;
    property OnUpdateBuffer:TNotifyEvent read FOnUpdateBuffer write FOnUpdateBuffer;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
    property LayerWidth: integer read FLayerWidth;
    property LayerHeight: integer read FLayerHeight;
    property Buffer: TD3Bitmap read FBuffer;
    property Resolution: integer read FResolution write SetResolution;
  published
    property ModulationColor: string read GetModulationColor write SetModulationColor;
  end;

TD3BufferLayer=class(TD3CustomBufferLayer)
  published
    property OnUpdateBuffer;
    property Resolution;
  end;

TD3Scene=class(TCustomControl{$IFDEF WINDOWS},IDropTarget{$ENDIF})
  private
    {$IFDEF WINDOWS}
    PrevWndProc: WNDPROC;
    FWStyle: string;
    {$ENDIF}

    FShift: TShiftState;
    FCanvas: TD3Canvas;
    FDisableUpdate:boolean;
    FChildren: TList;
    FDesignRoot,FSelected,FCaptured,FHovered,FFocused: TD3VisualObject;
    FSelection: array of TD3Object;
    FDesignGridLines: array of TD3VisualObject;
    FDesignPopup: TPopupMenu;
    FUnsnapMousePos,FMousePos,FDownPos: TD3Point;
    FMoving,FLeftFar,FRightTop,FRotateY,FRotateX,FRotateZ:boolean;
    FV1,FV2: TD3Vector;
    FResizeSize: TPoint;
    FDragging,FResizing:boolean;
    FDesignTime:boolean;
    FTransparency:boolean;
    FAllowDrag:boolean;
    FSnapToGrid:boolean;
    FSnapToLines:boolean;
    FSnapGridShow:boolean;
    FSnapGridSize:single;
    FDesignShowHint:boolean;
    FDesignAllowPanAndRotate:boolean;
    FInsertObject: string;
    FAlignRoot:boolean;
    FPopupPos: TPoint;
    FCloneFrame: TForm;
    FDrawing:boolean;
    FDrawingUpdateRect: array of TD2Rect;
    FCamera: TD3Camera;
    FDesignGrid: TD3VisualObject;
    FDesignCamera: TD3Camera;
    FDesignCameraZ: TD3Dummy;
    FDesignCameraX: TD3Dummy;
    FFill: TD3Color;
    FLighting:boolean;
    FAmbient: TD3Color;
    FQuality: TD3Quality;
    FBlendedList: TList;
    FRealTime:boolean;
    FRenderTime,FRenderCount,FFps,FTime,FDeltaTime:single;
    FPhysics: TD3Physics;
    FRealTimeSleep: integer;
    FInvisibleAtRuntime:boolean;
    FOnFlush:TNotifyEvent;
    FDesignHide:boolean;
    FDisableDraw:boolean;
    FDisableLayerAlign:boolean;
    FUsingDesignCamera:boolean;
    FShowTimer: TTimer;
    FLoadCursor: TCursor;
    FActiveControl: TD3VisualObject;
    {$ifdef WINDOWS}
    function GetDataObject: TD3DragObject;
    function DragEnter(const dataObj: IDataObject; grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    function DragOver(grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    function DragLeave: HResult; stdcall;
    function Drop(const dataObj: IDataObject; grfKeyState: DWORD; pt: TPoint; var dwEffect: DWORD): HResult; stdcall;
    procedure WMAddUpdateRect(var Msg: TMessage); message WM_ADDUPDATERECT;
    procedure WMEraseBkgnd(var Msg: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure WMPaint(var Msg: TWMPaint); message WM_PAINT;
    procedure CMMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
    {$endif}
    {$IFDEF UNIX}
    procedure Invalidate;  override;
    procedure WMEraseBkgnd(var Msg: TLMEraseBkgnd); message LM_ERASEBKGND;
    procedure CMMouseLeave(var Message: TLMessage); message CM_MOUSELEAVE;
    procedure WMPaint(var Msg: TLMPaint); message LM_PAINT;
    {$endif}
    procedure CMDesignHitTest(var Msg: TLMMouse); message CM_DESIGNHITTEST;
    procedure CMHintShow(var Message: TLMessage); message CM_HINTSHOW;
    procedure UpdateLayer;
    function GetCount: integer;
    procedure SetChildren(Index: integer; const Value:TD3Object);
    function GetChildrenObject(Index: integer): TD3Object;
    procedure SetSnapGridShow(const Value:boolean);
    procedure SetAllowPanAndRotate(const Value:boolean);
    procedure SetDesignHintShow(const Value:boolean);
    function SnapToGridValue(Value:single):single;
    procedure SetSnapGridSize(const Value:single);
    procedure SnapToGridLines(AllowChangePosition: boolean);
    function SnapPointToGridLines(const APoint: TD3Point): TD3Point;
    procedure ReadDesignCameraPos(Reader:TReader);
    procedure ReadDesignCameraZAngle(Reader:TReader);
    procedure ReadDesignCameraXAngle(Reader:TReader);
    procedure WriteDesignCameraPos(Writer:TWriter);
    procedure WriteDesignCameraXAngle(Writer:TWriter);
    procedure WriteDesignCameraZAngle(Writer:TWriter);
    procedure ReadDesignSnapGridShow(Reader:TReader);
    procedure WriteDesignSnapGridShow(Writer:TWriter);
    procedure ReadDesignShowHint(Reader:TReader);
    procedure WriteDesignShowHint(Writer:TWriter);
    procedure ReadDesignSnapToGrid(Reader:TReader);
    procedure WriteDesignSnapToGrid(Writer:TWriter);
    procedure ReadDesignSnapToLines(Reader:TReader);
    procedure WriteDesignSnapToLines(Writer:TWriter);
    procedure OpenDesignPopup;
    procedure doDesignPopupShowHint(Sender: TObject);
    procedure doDesignPopupShowGrid(Sender: TObject);
    procedure doDesignPopupLoadFromFile(Sender: TObject);
    procedure doDesignPopupAdd(Sender: TObject);
    procedure doDesignPopupDesignHide(Sender: TObject);
    procedure doDesignPasteFromClip(Sender: TObject);
    procedure doDesignPopupReorder(Sender: TObject);
    procedure doDesignPopupDel(Sender: TObject);
    procedure doDesignPopupCopy(Sender: TObject);
    procedure popupMeasureItem(Sender: TObject; ACanvas: TCanvas; var Width,Height: integer);
    //  procedure popupDrawItem(Sender: TObject; ACanvas: TCanvas; ARect: TRect; Selected: Boolean);
    function GetRoot: TD3Object;
    procedure SetFocused(const Value:TD3VisualObject);
    procedure DoDesignSelect(AObject: TObject);
    procedure SetFill(const Value:string);
    procedure SetAmbient(const Value:string);
    procedure SetQuality(const Value:TD3Quality);
    procedure SetRealTime(const Value:boolean);
    function GetFill: string;
    function GetAmbient: string;
    procedure SetActiveControl(AControl: TD3VisualObject);
    procedure SetSelected(Value:TD3VisualObject);
  protected
    procedure BeginVCLDrag(Source: TObject);
    procedure EndDragEvent(Sender,Target: TObject; X,Y: integer);
    procedure DoIdle(Sender: TObject; var Done: boolean);
    procedure CreateParams(var Params: TCreateParams);  override;
    procedure CreateHandle;  override;
    procedure CreateWnd;  override;
    procedure DestroyWnd;  override;
    procedure Loaded;  override;
    procedure Resize;  override;
    procedure Paint;  override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X,Y: integer);  override;
    procedure MouseMove(Shift: TShiftState; X,Y: integer);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X,Y: integer);  override;
    procedure NewKeyUp(var Key: word; var char: System.widechar; Shift: TShiftState);
    procedure NewKeyDown(var Key: word; var char: System.widechar; Shift: TShiftState);
    procedure DoDragOver(Sender,Source: TObject; X,Y: integer; State: TDragState; var Accept: boolean);
    procedure DoDragDrop(Sender,Source: TObject; X,Y: integer);
    procedure KeyUp(var Key: word; Shift: TShiftState);  override;
    procedure KeyDown(var Key: word; Shift: TShiftState);  override;
    procedure UTF8KeyPress(var UTF8Key: TUTF8Char);  override;
    function  DoMouseWheel(Shift: TShiftState; WheelDelta: integer; MousePos: TPoint):boolean;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function  ObjectByPoint(X,Y:single): TD3VisualObject;
    procedure DeleteChildren;
    procedure SetBounds(ALeft,ATop,AWidth,AHeight: integer);  override;
    procedure Draw;  virtual;
    procedure AddObject(AObject: TD3Object);
    procedure RemoveObject(AObject: TD3Object);
    procedure ProcessTick;
    procedure CreatePhysics;
    procedure BeginUpdate;
    procedure EndUpdate;
    procedure BeginDrag;
    procedure BeginResize;
    procedure AddUpdateRect(const R: TD2Rect);
    procedure InsertObject(const ClassName: string);
    procedure CreateEmbedded(const AWidth,AHeight: integer; AOnFlush:TNotifyEvent);
    procedure EmbeddedMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y: integer);
    procedure EmbeddedMouseMove(Shift: TShiftState; X,Y: integer);
    procedure EmbeddedMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y: integer);
    procedure EmbeddedKeyUp(var Key: word; var char: System.widechar; Shift: TShiftState);
    procedure EmbeddedKeyDown(var Key: word; var char: System.widechar; Shift: TShiftState);
    function  EmbeddedMouseWheel(Shift: TShiftState; WheelDelta: integer):boolean;
    property Canvas: TD3Canvas read FCanvas write FCanvas;
    property DesignTime: boolean read FDesignTime write FDesignTime stored False;
    property Count: integer read GetCount;
    property Root: TD3Object read GetRoot;
    property Children[Index: integer]: TD3Object read GetChildrenObject write SetChildren;
    property Selected: TD3VisualObject read FSelected write SetSelected;
    property Captured: TD3VisualObject read FCaptured;
    property Hovered: TD3VisualObject read FHovered;
    property Focused: TD3VisualObject read FFocused write SetFocused;
    property Physics: TD3Physics read FPhysics;
    property Time:single read FTime;
    property DeltaTime:single read FDeltaTime;
    property Fps:single read FFps;
    property IsDrawing: boolean read FDrawing;
    property OnFlush:TNotifyEvent read FOnFlush write FOnFlush;
    property DesignAllowPanAndRotate: boolean read FDesignAllowPanAndRotate write SetAllowPanAndRotate;
    property DesignGridShow: boolean read FSnapGridShow write SetSnapGridShow;
    property DesignShowHint: boolean read FDesignShowHint write SetDesignHintShow;
    property DesignSnapToGrid: boolean read FSnapToGrid write FSnapToGrid;
    property DesignSnapGridSize:single read FSnapGridSize write SetSnapGridSize;
    property DesignSnapToLines: boolean read FSnapToLines write FSnapToLines;
  published
    property Align;
    property AlignRoot: boolean read FAlignRoot write FAlignRoot  default True;
    property AllowDrag: boolean read FAllowDrag write FAllowDrag  default False;
    property ActiveControl: TD3VisualObject read FActiveControl write SetActiveControl;
    property Lighting: boolean read FLighting write FLighting  default True;
    property Quality: TD3Quality read FQuality write SetQuality;
    property FillColor: string read GetFill write SetFill;
    property AmbientColor: string read GetAmbient write SetAmbient;
    property Camera: TD3Camera read FCamera write FCamera;
    property Transparency: boolean read FTransparency write FTransparency  default False;
    property RealTime: boolean read FRealTime write SetRealTime  default False;
    property RealTimeSleep: integer read FRealTimeSleep write FRealTimeSleep;
    property InvisibleAtRuntime: boolean read FInvisibleAtRuntime write FInvisibleAtRuntime  default False;
    property UsingDesignCamera: boolean read FUsingDesignCamera write FUsingDesignCamera  default True;
    property TabStop;
    property OnMouseMove;
    property OnMouseDown;
    property OnMouseUp;
    property OnMouseWheel;
  end;

TD3BitmapStream=class(TCollectionItem)
  private
    FBitmap: TD3Bitmap;
    FName: string;
    procedure SetBitmap(const Value:TD3Bitmap);
    procedure SetName(const Value:string);
  protected
    function GetDisplayName: string;  override;
  public
    constructor Create(Collection: TCollection);  override;
    procedure Assign(Source: TPersistent);  override;
    destructor Destroy;  override;
  published
    property Bitmap: TD3Bitmap read FBitmap write SetBitmap;
    property Name: string read FName write SetName;
  end;


TD3BitmapCollection=class(TCollection)
  private
    FBitmapList: TD3BitmapList;
  protected
  public
    constructor Create(AOwner: TD3BitmapList);
    destructor Destroy;  override;
  end;


TD3BitmapList=class(TComponent)
  private
    FBitmaps: TD3BitmapCollection;
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Bitmaps: TD3BitmapCollection read FBitmaps write FBitmaps;
  end;

TOnDesignSelect=procedure(AObject:TObject) of object;

TD3Designer=class(TComponent)
  private
    FScenes: TList;
  protected
    procedure CallDesignSelect(AObject: TObject);
  public
    procedure SelectObject(ADesigner: TComponent; AObject: TD3Object; MultiSelection: array of TD3Object);  virtual; abstract;
    procedure Modified(ADesigner: TComponent);  virtual; abstract;
    function UniqueName(ADesigner: TComponent; ClassName: string): string;  virtual; abstract;
    function IsSelected(ADesigner: TComponent; const AObject: TObject):boolean;  virtual; abstract;
    procedure AddScene(const Scene: TD3Scene);  virtual;
    procedure RemoveScene(const Scene: TD3Scene);  virtual;
    procedure AddObject(AObject: TD3Object);  virtual;
    procedure DeleteObject(AObject: TD3Object);  virtual;
    procedure AddObject2D(AObject: TD2Object);  virtual;
  end;

TD3CustomCanvasLayer=class(TD3CustomBufferLayer)
  private
    FBitmap: TBitmap;
    FOnPaint: TCanvasPaintEvent;
    FOnMouseMove: TMouseMoveEvent;
    function GetLayerCanvas: TCanvas;
  protected
    procedure BeforePaint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure UpdateCanvas;
    property LayerCanvas: TCanvas read GetLayerCanvas;
  published
    property OnLayerPaint: TCanvasPaintEvent read FOnPaint write FOnPaint;
    property Resolution;
  end;

TD3CanvasLayer=class(TD3CustomCanvasLayer)
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
  end;


TD3GUICanvasLayer=class(TD3CustomCanvasLayer)
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property LayerAlign;
  end;

TD3ScreenCanvasLayer=class(TD3GUICanvasLayer)
  published
    property LayerAlign;
  end;

TD3Grid=class(TD3VisualObject)
  private
    FLineColor: TD3Color;
    FFrequency:single;
    FMarks:single;
    procedure SetLineColor(const Value:string);
    function GetLineColor: string;
    procedure SetFrequency(const Value:single);
    procedure SetMarks(const Value:single);
  protected
    procedure SetDepth(const Value:single);  override;
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
    property Marks:single read FMarks write SetMarks;
    property Frequency:single read FFrequency write SetFrequency;
    property LineColor: string read GetLineColor write SetLineColor;
  end;

TD3Shape=class(TD3VisualObject)
  private
    FMaterial: TD3Material;
    procedure SetMaterial(const Value:TD3Material);
  protected
    procedure MaterialChanged(Sender: TObject);
    procedure BeforePaint;  override;
    procedure AfterPaint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
    property Material: TD3Material read FMaterial write SetMaterial;
  end;

TD3StrokeCube=class(TD3Shape)
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  end;

TD3CustomMesh=class(TD3Shape)
  private
    FData: TD3MeshData;
    procedure SetData(const Value:TD3MeshData);
  protected
    procedure DoMeshChanged(Sender: TObject);
    procedure Paint;  override;
    property Data: TD3MeshData read FData write SetData;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  end;

TD3Cube=class(TD3CustomMesh)
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  end;

TD3Plane=class(TD3CustomMesh)
  protected
    procedure Paint;  override;
    procedure SetDepth(const Value:single);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  end;

TD3Mesh=class(TD3CustomMesh)
  published
    property Data;
  end;

TD3Sphere=class(TD3CustomMesh)
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure CreateBody;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
  end;

TD3Cylinder=class(TD3CustomMesh)
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
    procedure CreateBody;  override;
  end;

TD3RoundCube=class(TD3CustomMesh)
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
  end;

TD3Cone=class(TD3CustomMesh)
  public
    constructor Create(AOwner: TComponent);  override;
    procedure Paint;  override;
    procedure CreateBody;  override;
  end;

TD3Text=class(TD3Shape)
  private
    FFont: TD3Font;
    FText: WideString;
    procedure SetFont(const Value:TD3Font);
    procedure SetText(const Value:WideString); overload; // 5555
    procedure SetText(const Value:string); overload;     // 5555
    function GetText: WideString; overload;               // 5555
    function GetText: string;                             // 5555
  protected
    procedure FontChanged(Sender: TObject);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
    property TextW: WideString read FText write SetText;
  published
    property Font: TD3Font read FFont write SetFont;
    property Text: string read GetText write SetText;
  end;

TD3Image=class(TD3CustomMesh)
  private
    FPlane: TD3MeshData;
    FBitmap: TD3Bitmap;
    procedure SetHeight(const Value:single);
  protected
    procedure DoBitmapChanged(Sender: TObject);
    procedure Paint;  override;
    procedure SetDepth(const Value:single);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property Bitmap: TD3Bitmap read FBitmap write FBitmap;
  end;


TD3Shape3DSides=set of TD3Shape3DSide;

TD3Shape3D=class(TD3Shape)
  private
    FFlatness:single;
    FSides: TD3Shape3DSides;
    FMaterialLeft: TD3Material;
    FMaterialBack: TD3Material;
    FLeftSide2D:boolean;
    procedure SetFlatness(const Value:single);
    procedure SetSides(const Value:TD3Shape3DSides);
    procedure SetMaterialBack(const Value:TD3Material);
    procedure SetMaterialLeft(const Value:TD3Material);
  protected
    procedure BeforePaint;  override;
    procedure Paint;  override;
    procedure ShapeMouseMove(Shift: TShiftState; X,Y:single);  virtual;
    procedure ShapeMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single);  virtual;
    procedure ShapeMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single);  virtual;
    procedure MouseMove(Shift: TShiftState; X,Y,d3,Dy:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single; rayPos,rayDir: TD3Vector);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property Flatness:single read FFlatness write SetFlatness;
    property Sides: TD3Shape3DSides read FSides write SetSides;
    property MaterialBack: TD3Material read FMaterialBack write SetMaterialBack;
    property MaterialLeft: TD3Material read FMaterialLeft write SetMaterialLeft;
  end;

TD3Rectangle3D=class(TD3Shape3D)
  private
    FyRadius:single;
    FxRadius:single;
    FCorners: TD2Corners;
    FCornerType: TD2CornerType;
    procedure SetxRadius(const Value:single);
    procedure SetyRadius(const Value:single);
    function IsCornersStored:boolean;
    procedure SetCorners(const Value:TD2Corners);
    procedure SetCornerType(const Value:TD2CornerType);
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property xRadius:single read FxRadius write SetxRadius;
    property yRadius:single read FyRadius write SetyRadius;
    property Corners: TD2Corners read FCorners write SetCorners stored IsCornersStored;
    property CornerType: TD2CornerType read FCornerType write SetCornerType;
  end;

TD3Ellipse3D=class(TD3Shape3D)
  protected
    procedure Paint;  override;
  end;

TD3Text3D=class(TD3Shape3D)
  private
    FFont: TD3Font;
    FText: WideString;
    FWordWrap:boolean;
    FStretch:boolean;
    FVertTextAlign: TD2TextAlign;
    FHorzTextAlign: TD2TextAlign;
    procedure SetFont(const Value:TD3Font);
    procedure SetText(const Value:WideString); overload; // 5555
    procedure SetText(const Value:string); overload;     // 5555
    function GetText: WideString; overload;               // 5555
    function GetText: string; overload;                   // 5555
    procedure SetHorzTextAlign(const Value:TD2TextAlign);
    procedure SetStretch(const Value:boolean);
    procedure SetVertTextAlign(const Value:TD2TextAlign);
    procedure SetWordWrap(const Value:boolean);
  protected
    procedure FontChanged(Sender: TObject);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function GetTextBounds: TD2Rect;
    function GetPathBounds: TD2Rect;
    function GetPathLength:single;
    property TextW: WideString read FText write SetText;
  published
    property Font: TD3Font read FFont write SetFont;
    property HorzTextAlign: TD2TextAlign read FHorzTextAlign write SetHorzTextAlign  default d2TextAlignCenter;
    property VertTextAlign: TD2TextAlign read FVertTextAlign write SetVertTextAlign  default d2TextAlignCenter;
    property Text: string read GetText write SetText;
    property Stretch: boolean read FStretch write SetStretch  default False;
    property WordWrap: boolean read FWordWrap write SetWordWrap  default True;
  end;

TD3Path3D=class(TD3Shape3D)
  private
    FPath: TD2PathData;
    FWrapMode: TD2PathWrap;
    procedure SetPath(const Value:TD2PathData);
    procedure SetWrapMode(const Value:TD2PathWrap);
  protected
    procedure PathChanged(Sender: TObject);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Path: TD2PathData read FPath write SetPath;
    property WrapMode: TD2PathWrap read FWrapMode write SetWrapMode  default d2PathStretch;
  end;

TD3ColorAnimation=class(TD3Animation)
  private
    FStartColor: TD3Color;
    FStopColor: TD3Color;
    FPath,FPropertyName: ansistring;
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
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent;
    property StopValue:string read GetStopColor write SetStopColor;
    property PropertyName: ansistring read FPropertyName write FPropertyName;
  end;

TD3FloatAnimation=class(TD3Animation)
  private
    FStartFloat:single;
    FStopFloat:single;
    FPath,FPropertyName: ansistring;
    FInstance: TObject;
    FStartFromCurrent:boolean;
  protected
    procedure ProcessAnimation;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Start;  override;
  published
    property StartValue:single read FStartFloat write FStartFloat stored True;
    property StartFromCurrent: boolean read FStartFromCurrent write FStartFromCurrent;
    property StopValue:single read FStopFloat write FStopFloat stored True;
    property PropertyName: ansistring read FPropertyName write FPropertyName;
  end;

{ TD3CustomObjectLayer }

TD3CustomObjectLayer=class(TD3CustomBufferLayer,Id2Scene)
  private
    FCanvas: TD2Canvas;
    FDisableUpdate:boolean;
    FChildren: TList;
    FDesignRoot,FSelected,FCaptured,FHovered,FFocused: TD2VisualObject;
    FSelection: array of TD2Object;
    FDesignPlaceObject: TD2VisualObject;
    FDesignGridLines: array of TD2VisualObject;
    FDesignPopup: TPopupMenu;
    FDesignChangeSelection:TNotifyEvent;
    FUnsnapMousePos,FMousePos,FDownPos: TD2Point;
    FMoving,FLeftTop,FRightTop,FLeftBottom,FRightBottom,FTop,FBottom,FLeft,FRight,FRotate:boolean;
    FLeftTopHot,FRightTopHot,FLeftBottomHot,FRightBottomHot,FTopHot,FBottomHot,FLeftHot,FRightHot,FRotateHot:boolean;
    FResizeSize,FResizePos,FResizeStartPos,FDownSize: TD2Point;
    FDragging,FResizing:boolean;
    FDesignTime:boolean;
    FFill: TD2Brush;
    FTransparency:boolean;
    FAllowDrag:boolean;
    FSnapToGrid:boolean;
    FSnapToLines:boolean;
    FSnapGridShow:boolean;
    FSnapGridSize:single;
    FInsertObject: string;
    FAlignRoot:boolean;
    FDesignPopupEnabled:boolean;
    FPopupPos: TD2Point;
    FOpenInFrame: TD2Frame;
    FCloneFrame: TForm;
    FDrawing:boolean;
    FSaveIdle: TIdleEvent;
    FStyle: TD2Resources;
    FShowTimer: TTimer;
    FLoadCursor: TCursor;
    FActiveControl: TD2Control;
    FAnimatedCaret:boolean;
    procedure SetActiveControl(AControl: TD2Control);
    procedure DoShowTimer(Sender: TObject);
    function  GetCount: integer;
    procedure SetChildren(Index: integer; const Value:TD2Object);
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
    procedure RealignRoot;
    procedure OpenDesignPopup;
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
    procedure SetSelected(const Value:TD2VisualObject);
    procedure doDesignPopupPaste(Sender: TObject);
    function GetDisableUpdate:boolean;
    function GetDesignTime:boolean;
    function GetCanvas: TD2Canvas;
    function GetOwner: TComponent;
    function GetRealTime:boolean;
    function GetComponent: TComponent;
    function GetSelected: TD2VisualObject;
    function GetDeltaTime:single;
    procedure SetDisableUpdate(Value:boolean);
    function GetUpdateRectsCount: integer;
    function GetUpdateRect(const Index: integer): TD2Rect;
    procedure SetCaptured(const Value:TD2VisualObject);
    function GetCaptured: TD2VisualObject;
    function GetFocused: TD2VisualObject;
    procedure SetDesignRoot(const Value:TD2VisualObject);
    function GetMousePos: TD2Point;
    procedure BeginDrag;
    procedure BeginResize;
    function GetTransparency:boolean;
    function GetDesignPlaceObject: TD2VisualObject;
    function GetStyle: TD2Resources;
    function LocalToScreen(const Point: TD2Point): TD2Point;
    function GetActiveControl: TD2Control;
    procedure SetStyle(const Value:TD2Resources);
    procedure BeginVCLDrag(Source: TObject; ABitmap: TD2Bitmap);
    function GetAnimatedCaret:boolean;
    function ShowKeyboardForControl(AObject: TD2Object):boolean;
    function HideKeyboardForControl(AObject: TD2Object):boolean;
  protected
    FUpdateRects: array of TD2Rect;
    procedure Loaded;  override;
    procedure Draw;  virtual;
    procedure BeforePaint;  override;
    procedure Paint;  override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent);  override;
    procedure Notification(AComponent: TComponent; Operation: TOperation);  override;
    procedure DefineProperties(Filer: TFiler);  override;
    procedure LayerMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure LayerMouseMove(Shift: TShiftState; X,Y:single);  override;
    procedure LayerMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure MouseWheel(Shift: TShiftState; WheelDelta: integer; var Handled: boolean);  override;
    procedure KeyUp(var Key: word; var char: System.widechar; Shift: TShiftState);  override;
    procedure KeyDown(var Key: word; var char: System.widechar; Shift: TShiftState);  override;
    procedure DragEnter(const Data: TD3DragObject; const Point: TD3Point);  override;
    procedure DragOver(const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point; var Accept: boolean);  override;
    procedure DragDrop(const Data: TD3DragObject; Shift: TShiftState; const Point: TD3Point);  override;
    procedure DragLeave;  override;
    procedure DragEnd;  override;
    function DoHintShow(var Message: TLMessage):boolean;  override;
    function ObjectByPoint(X,Y:single): TD2VisualObject;
    procedure SetVisible(const Value:boolean);  override;
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    procedure MouseLeave;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure DeleteChildren;
    property Canvas: TD2Canvas read FCanvas;
    procedure UpdateResource;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
    procedure GetTabOrderList(const AList: TList);  override;
    procedure AddObject(AObject: TD2Object);
    procedure RemoveObject(AObject: TD2Object);
    procedure ProcessTick;
    procedure AddUpdateRect(R: TD2Rect);
    procedure InsertObject(const ClassName: string);
    property DesignTime: boolean read FDesignTime write FDesignTime stored False;
    property Count: integer read GetCount;
    property Root: TD2Object read GetRoot;
    property Children[Index: integer]: TD2Object read GetChildrenObject write SetChildren;
    property Selected: TD2VisualObject read FSelected write SetSelected;
    property Captured: TD2VisualObject read FCaptured;
    property Hovered: TD2VisualObject read FHovered;
    property Focused: TD2VisualObject read FFocused write SetFocused;
    property DisableUpdate: boolean read FDisableUpdate;
    property DesignPopupEnabled: boolean read FDesignPopupEnabled write FDesignPopupEnabled;
    property DesignSnapGridShow: boolean read FSnapGridShow write SetSnapGridShow;
    property DesignSnapToGrid: boolean read FSnapToGrid write FSnapToGrid;
    property DesignSnapToLines: boolean read FSnapToLines write FSnapToLines;
    property DesignChangeSelection:TNotifyEvent read FDesignChangeSelection write FDesignChangeSelection;
    property AlignRoot: boolean read FAlignRoot write FAlignRoot  default True;
    property AllowDrag: boolean read FAllowDrag write FAllowDrag;
    property DesignSnapGridSize:single read FSnapGridSize write SetSnapGridSize;
    property ShowHint  default True;
    property Transparency: boolean read FTransparency write FTransparency;
    property Fill: TD2Brush read FFill write SetFill;
    property Style: TD2Resources read FStyle write SetStyle;
  published
    property CanFocused  default True;
    property ActiveControl: TD2Control read FActiveControl write SetActiveControl;
    property AnimatedCaret: boolean read FAnimatedCaret write FAnimatedCaret  default True;
  end;

TD3ObjectLayer=class(TD3CustomObjectLayer)
  published
    property Align;
    property Body;
    property Collider;
    property ColliseTrack;
    property Dynamic;
    property Velocity;
    property Margins;
    property Padding;
    property Fill;
    property Style;
    property Resolution;
  end;


TD3GUIScene2DLayer=class(TD3CustomObjectLayer)
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property AllowDrag  default False;
    property LayerAlign;
    property Fill;
    property Style;
    property ZWrite  default False;
  end;


TD3d2Layer=class(TD3CustomObjectLayer)
  end;

TD3Screend2Layer=class(TD3GUIScene2DLayer)
  end;

TD3GUILayout=class(TD3CustomLayer)
  protected
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
  published
    property LayerAlign;
    property ZWrite  default False;
  end;

TD3GUIPlane=class(TD3CustomLayer)
  private
    FPlaneScreen: TD3MeshData;
    FMaterial: TD3Material;
    procedure SetMaterial(const Value:TD3Material);
    procedure MaterialChanged(Sender: TObject);
  protected
    procedure Paint;  override;
    procedure AfterPaint;  override;
    procedure BeforePaint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property Material: TD3Material read FMaterial write SetMaterial;
    property LayerAlign;
    property ZWrite  default False;
  end;

TD3GUIImage=class(TD3CustomLayer)
  private
    FBitmap: TD3Bitmap;
    FPlaneScreen: TD3MeshData;
    FWrapMode: TD2ImageWrap;
    procedure SetBitmap(const Value:TD3Bitmap);
    procedure SetWrapMode(const Value:TD2ImageWrap);
  protected
    procedure DoBitmapChanged(Sender: TObject);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
  published
    property LayerAlign;
    property Bitmap: TD3Bitmap read FBitmap write SetBitmap;
    property WrapMode: TD2ImageWrap read FWrapMode write SetWrapMode  default d2ImageStretch;
    property HitTest  default False;
    property ZWrite  default False;
  end;

TD3GUIText=class(TD3CustomObjectLayer)
  private
    FText: TD2Text;
    function GetText: WideString; overload;              // 5555
    function GetText: string; overload;                  // 5555
    procedure SetText(const Value:WideString); overload; // 5555
    procedure SetText(const Value:string); overload;     // 5555
    function GetFont: TD2Font;
    procedure SetFont(const Value:TD2Font);
    function GetBrush: TD2Brush;
    procedure SetBrush(const Value:TD2Brush);
  protected
  public
    constructor Create(AOwner: TComponent);  override;
    property TextW: WideString read GetText write SetText;
  published
    property LayerAlign;
    property Font: TD2Font read GetFont write SetFont;
    property Fill: TD2Brush read GetBrush write SetBrush;
    property Text: string read GetText write SetText;
    property HitTest  default False;
    property ZWrite  default False;
  end;

TD3GUISelection=class(TD3CustomLayer)
  private
    FMoving:boolean;
    FDownPos: TD2Point;
    FTopLeft,FTopRight,FBottomLeft,FBottomRight:boolean;
    FTopLeftHot,FTopRightHot,FBottomLeftHot,FBottomRightHot:boolean;
    FGripSize:single;
    FMinSize: integer;
    procedure SetGripSize(const Value:single);
    procedure SetMinSize(const Value:integer);
  protected
    procedure Paint;  override;
    procedure LayerMouseMove(Shift: TShiftState; X,Y:single);  override;
    procedure LayerMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure LayerMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure MouseLeave;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
  published
    property GripSize:single read FGripSize write SetGripSize;
    property MinSize: integer read FMinSize write SetMinSize  default 15;
  end;

TD3ScreenImage=class(TD3GUIImage)
  end;

TD3ScreenDummy=class(TD3GUILayout)
  end;

TD3TextBox3D=class(TD3Shape3D)
  private
    FOnChange:TNotifyEvent;
    FReadOnly:boolean;
    FSelStart: integer;
    FSelLength: integer;
    FCaretPosition: integer;
    FMaxLength: integer;
    FFirstVisibleChar: integer;
    FLMouseSelecting:boolean;
    FNeedChange:boolean;
    FDisableCaret:boolean;
    FPassword:boolean;
    FPopupMenu: TPopupMenu;
    FOnTyping:TNotifyEvent;
    FText: WideString;
    FFont: TD3Font;
    FBackground: string;
    FSelection: string;
    FShowBackground:boolean;
    procedure InsertText(const AText: WideString);
    function GetSelLength: integer;
    function GetSelStart: integer;
    function GetSelText: WideString;
    procedure SetSelLength(const Value:integer);
    procedure SetSelStart(const Value:integer);
    function GetSelRect: TD2Rect;
    procedure SetCaretPosition(const Value:integer);
    function GetCoordinatePosition(x:single): integer;
    procedure SetMaxLength(const Value:integer);
    function GetNextWordBeging(StartPosition: integer): integer;
    function GetPrivWordBeging(StartPosition: integer): integer;
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
    procedure SetFont(const Value:TD3Font);
    procedure SetBackground(const Value:string);
    procedure SetSelection(const Value:string);
    procedure SetShowBackground(const Value:boolean);
  protected
    procedure Change;  virtual;
    function GetPasswordCharWidth:single;
    function TextWidth(const Str: WideString):single;
    procedure SetText(const Value:WideString);  virtual; overload; // 5555
    procedure SetText(const Value:string);  virtual; overload;     // 5555
    function GetText: WideString;  virtual; overload;               // 5555
    function GetText: string;  virtual; overload;                   // 5555
    procedure KeyDown(var Key: word; var KeyChar: System.widechar; Shift: TShiftState);  override;
    procedure KeyUp(var Key: word; var KeyChar: System.widechar; Shift: TShiftState);  override;
    procedure ShapeMouseDown(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure ShapeMouseMove(Shift: TShiftState; X,Y:single);  override;
    procedure ShapeMouseUp(Button: TMouseButton; Shift: TShiftState; X,Y:single);  override;
    procedure FontChanged(Sender: TObject);
    procedure EnterFocus;  override;
    procedure KillFocus;  override;
    procedure ContextMenu(const ScreenPosition: TD2Point);  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    function RayCastIntersect(const RayPos,RayDir: TD3Vector; var Intersection: TD3Vector):boolean;  override;
    procedure Paint;  override;
    procedure ClearSelection;
    procedure CopyToClipboard;
    procedure CutToClipboard;
    procedure PasteFromClipboard;
    procedure SelectAll;
    function GetCharX(a: integer):single;
    property CaretPosition: integer read FCaretPosition write SetCaretPosition;
    property SelStart: integer read GetSelStart write SetSelStart;
    property SelLength: integer read GetSelLength write SetSelLength;
    property SelText: WideString read GetSelText;
    property MaxLength: integer read FMaxLength write SetMaxLength  default 0;
    property TextW: WideString read FText write SetText;
  published
    property CanFocused  default True;
    property Background: string read FBackground write SetBackground;
    property Selection: string read FSelection write SetSelection;
    property Cursor  default crIBeam;
    property Font: TD3Font read FFont write SetFont;
    property Password: boolean read FPassword write SetPassword;
    property ShowBackground: boolean read FShowBackground write SetShowBackground  default True;
    property Text: string read GetText write SetText;
    property ReadOnly: boolean read FReadOnly write FReadOnly;
    property OnChange:TNotifyEvent read FOnChange write FOnChange;
    property OnTyping:TNotifyEvent read FOnTyping write FOnTyping;
  end;

  TD3ParticleList=class;
  TD3ParticleEmitter=class;

  TD3Particle=class(TPersistent)
  private
    FTag: integer;
    FPosition: TD3Vector;
    FVelocity: TD3Vector;
    FTangentVel:single;
    FCentrifugalVel:single;
    FCreationTime:single;
    FCurrentTime:single;
    FRotationCenter: TD3Vector;
    FOwner: TD3ParticleList;
    FEmitter: TD3ParticleEmitter;
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    property Emitter: TD3ParticleEmitter read FEmitter write FEmitter;
    property Owner: TD3ParticleList read FOwner write FOwner;
    property Position: TD3Vector read FPosition write FPosition;
    property Velocity: TD3Vector read FVelocity write FVelocity;
    property Tag: integer read FTag write FTag;
  end;

  TD3ParticleArray=array [0..$FFFFFF shr 4] of TD3Particle;
  Pd3ParticleArray=^TD3ParticleArray;

TD3ParticleList=class(TPersistent)
  private
    FOwner: TD3ParticleEmitter;
    FItemList: TList;
    FDirectList: Pd3ParticleArray;
  protected
    function GetItems(index: integer): TD3Particle;
    procedure SetItems(index: integer; Value:TD3Particle);
    procedure AfterItemCreated(Sender: TObject);
  public
    constructor Create;  virtual;
    destructor Destroy;  override;
    property Owner: TD3ParticleEmitter read FOwner write FOwner;
    property Items[index: integer]: TD3Particle read GetItems write SetItems; default;
    function ItemCount: integer;
    function AddItem(AItem: TD3Particle): integer;
    procedure RemoveAndFreeItem(AItem: TD3Particle);
    function IndexOfItem(AItem: TD3Particle): integer;
    procedure Pack;
    property List: Pd3ParticleArray read FDirectList;
  end;

TD3ParticleReference=packed record
    particle: TD3Particle;
    distance: integer;
  end;

  PParticleReference=^TD3ParticleReference;
  TD3ParticleReferenceArray=packed array [0..$FFFFFF shr 4] of TD3ParticleReference;
  PParticleReferenceArray=^TD3ParticleReferenceArray;

  TRegion=packed record
    Count,capacity: integer;
    particleRef: PParticleReferenceArray;
    particleOrder: PPointerList;
  end;
  PRegion=^TRegion;

  TD3CreateParticleEvent=procedure(Sender: TObject; AParticle: TD3Particle) of object;

TD3ParticleKey=class(TCollectionItem)
  private
    FSpin:single;
    FScale:single;
    FColor: longword;
    FKey: longword;
    function GetColor: ansistring;
    procedure SetColor(const Value:ansistring);
    procedure SetKey(const Value:longword);
  public
    constructor Create(Collection: TCollection);  override;
    destructor Destroy;  override;
    function time:single;
  published
    property Key: longword read FKey write SetKey;
    property Color: ansistring read GetColor write SetColor;
    property Scale:single read FScale write FScale;
    property Spin:single read FSpin write FSpin;
  end;

TD3ParticleKeys=class(TCollection)
  private
    FEmitter: TD3ParticleEmitter;
    function GetKey(Index: integer): TD3ParticleKey;
  public
    constructor Create;  virtual;
    property Keys[Index: integer]: TD3ParticleKey read GetKey; default;
  end;

TD3ParticleEmitter=class(TD3Dummy)
  private
    FTimer: TD3Animation;
    FCurrentTime:single;
    FKeys: TD3ParticleKeys;
    FGravity: TD3Position;
    FPositionDispersion: TD3Position;
    FParticleInterval:single;
    FVelocityMode: TD3ParticleVelocityMode;
    FDispersionMode: TD3ParticleDispersionMode;
    FFollowToOwner:boolean;
    FLifeTime:single;
    FFriction:single;
    FDirectionAngle:single;
    FSpreadAngle:single;
    FVelocityMin:single;
    FCentrifugalVelMin:single;
    FTangentVelMin:single;
    FVelocityMax:single;
    FCentrifugalVelMax:single;
    FTangentVelMax:single;
    FParticles: TD3ParticleList;
    FOnCreateParticle: TD3CreateParticleEvent;
    FOldPosition: TD3Vector;
    FTimeRemainder:single;
    FRect: TD2Bounds;
    FblendingMode: TD3BlendingMode;
    FZTest:boolean;
    FBitmap: ansistring;
    FEditor: integer;
    MeshVertices: array of TD3ColorTexVertexNormal;
    MeshIndices: array of word;
    procedure SetParticleInterval(const Value:single);
    procedure SetParticles(const Value:TD3ParticleList);
    procedure SetGravity(const Value:TD3Position);
    procedure SetPositionDispersion(const Value:TD3Position);
    procedure SetRect(const Value:TD2Bounds);
    procedure SetBlendingMode(const Value:TD3BlendingMode);
    procedure SetZTest(const Value:boolean);
    procedure SetCentrifugalVelMax(const Value:single);
    procedure SetCentrifugalVelMin(const Value:single);
    procedure SetTangentVelMax(const Value:single);
    procedure SetTangentVelMin(const Value:single);
    procedure SetVelocityMax(const Value:single);
    procedure SetVelocityMin(const Value:single);
    procedure SetBitmap(const Value:ansistring);
    function GetColorMax: ansistring;
    function GetColorMin: ansistring;
    function GetScaleMax:single;
    function GetScaleMin:single;
    function GetSpinMax:single;
    function GetSpinMin:single;
    procedure SetColorMax(const Value:ansistring);
    procedure SetColorMin(const Value:ansistring);
    procedure SetScaleMax(const Value:single);
    procedure SetScaleMin(const Value:single);
    procedure SetSpinMax(const Value:single);
    procedure SetSpinMin(const Value:single);
    function GetParticleInterval:single;
  protected
    function FindKeys(const ATime:single; var Key1,Key2: TD3ParticleKey):boolean;
    procedure ProcessEffect(DeltaTime,Time:single);
    procedure SetLifeTime(const Value:single);
    procedure RenderParticle(const APart: TD3Particle; const Id3: integer; var Vertices: array of TD3ColorTexVertexNormal;
      var Indices: array of word);
    procedure Paint;  override;
  public
    constructor Create(AOwner: TComponent);  override;
    destructor Destroy;  override;
    procedure Assign(Source: TPersistent);  override;
    procedure KillAll;
    procedure Burst(aTime:single; ParticlesCount: integer);
    procedure ComputeCurColor(lifeTime:single; var curColor: TD3Color);
    function ComputeSizeScale(lifeTime:single; var sizeScale:single):boolean;
    function ComputeSpinAngle(lifeTime:single; var spinAngle:single):boolean;
    function CreateParticle: TD3Particle;  virtual;
    property Particles: TD3ParticleList read FParticles write SetParticles;
    property Keys: TD3ParticleKeys read FKeys write FKeys;
  published
    property Editor: integer read FEditor write FEditor stored False;
    property Bitmap: ansistring read FBitmap write SetBitmap;
    property BlendingMode: TD3BlendingMode read FblendingMode write SetBlendingMode  default d3BlendAdditive;
    property Gravity: TD3Position read FGravity write SetGravity;
    property PositionDispersion: TD3Position read FPositionDispersion write SetPositionDispersion;
    property ParticlePerSecond:single read GetParticleInterval write SetParticleInterval;
    property VelocityMode: TD3ParticleVelocityMode read FVelocityMode write FVelocityMode  default svmAbsolute;
    property FollowToOwner: boolean read FFollowToOwner write FFollowToOwner;
    property DispersionMode: TD3ParticleDispersionMode read FDispersionMode write FDispersionMode  default sdmFast;
    property DirectionAngle:single read FDirectionAngle write FDirectionAngle;
    property SpreadAngle:single read FSpreadAngle write FSpreadAngle;
    property Friction:single read FFriction write FFriction;
    property Rect: TD2Bounds read FRect write SetRect;
    property LifeTime:single read FLifeTime write SetLifeTime;
    property CentrifugalVelMin:single read FCentrifugalVelMin write SetCentrifugalVelMin;
    property CentrifugalVelMax:single read FCentrifugalVelMax write SetCentrifugalVelMax;
    property TangentVelMin:single read FTangentVelMin write SetTangentVelMin;
    property TangentVelMax:single read FTangentVelMax write SetTangentVelMax;
    property VelocityMin:single read FVelocityMin write SetVelocityMin;
    property VelocityMax:single read FVelocityMax write SetVelocityMax;
    property ColorBegin: ansistring read GetColorMin write SetColorMin;
    property ScaleBegin:single read GetScaleMin write SetScaleMin;
    property SpinBegin:single read GetSpinMin write SetSpinMin;
    property ColorEnd: ansistring read GetColorMax write SetColorMax;
    property ScaleEnd:single read GetScaleMax write SetScaleMax;
    property SpinEnd:single read GetSpinMax write SetSpinMax;
    property OnCreateParticle: TD3CreateParticleEvent read FOnCreateParticle write FOnCreateParticle;
    property TwoSide  default True;
    property ZWrite  default False;
  end;

TD3DummyPhysics=class(TD3Physics)
  public
    constructor Create(const Scene: TD3Scene);  override;
    destructor Destroy;  override;
    function CreateWorld: integer;  override;
    function Collise(const AOwner: TD3VisualObject; var AList: TList): integer;  override;
    function ComplexCollise(const AOwner: TD3VisualObject; var AList: TList): integer;  override;
    function ObjectByBody(const Body: integer): TD3VisualObject;  override;
    function CreateBox(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  override;
    function CreateSphere(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  override;
    function CreateCone(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  override;
    function CreateCylinder(const AOwner: TD3VisualObject; const ASize: TD3Vector): integer;  override;
    function GetBodyMatrix(const Body: integer): TD3Matrix;  override;
    procedure DestroyWorld(const AWorld: integer);  override;
    procedure UpdateWorld(const AWorld: integer; const DeltaTime:single);  override;
    procedure DestroyBody(var ABody: integer);  override;
    procedure AddForce(const AOwner: TD3VisualObject; const Force: TD3Vector);  override;
    procedure Explode(const AWorld: integer; const Position: TD3Vector; const Radius,Force:single);  override;
    procedure Wind(const AWorld: integer; const Dir: TD3Vector; const Force:single);  override;
    procedure SetBodyMatrix(const Body: integer; const M: TD3Matrix);  override;
  end;

//=============================================================================
//=============== GLobal Functions ============================================
//=============================================================================

// Math functions

function  MinInt(A1,A2: integer): integer;
function  MinFloat(A1,A2:single):single;
function  MaxInt(A1,A2: integer): integer;
function  MaxFloat(A1,A2:single):single;

function  d3InterpolateSingle(const start,stop,t:single):single;
function  d3InterpolateRotation(start,stop,t:single):single;
function  d3InterpolateColor(start,stop: TD3Color; t:single): TD3Color;

function  d3AppendColor(start,stop: TD3Color): TD3Color;
function  d3SubtractColor(start,stop: TD3Color): TD3Color;

function  d3TexVector(const u,v:single): TD3TexVector;
function  d3Vector(const x,y,z:single; const w:single=1.0): TD3Vector; overload;
function  d3Vector(const Point: TD3Point; const w:single=1.0): TD3Vector; overload;

function  MidPoint(const p1,p2: TD3Vector): TD3Vector;

function  BSphere(center: TD3Vector; radius:single): TD3BSphere;

function  AABB(min,max: TD3Vector): TD3AABB;
procedure AABBInclude(var bb: TD3AABB; const p: TD3Vector);
procedure AABBTransform(var bb: TD3AABB; const m: TD3Matrix);
function  IntersecTD3AABBsAbsolute(const aabb1,aabb2: TD3AABB):boolean;

procedure SeTD3Vector(var V: TD3Vector; const x,y,z:single; const w:single=1.0);
function  d3VectorNorm(const v: TD3Vector):single;
procedure Normalized3Vector(var v: TD3Vector);
function  d3VectorNormalize(const v: TD3Vector): TD3Vector;
function  d3VectorAdd(const v1: TD3Vector; const v2: TD3Vector): TD3Vector;
procedure Addd3Vector(var v1: TD3Vector; const v2: TD3Vector);
procedure Combined3Vector(var v1: TD3Vector; const v2: TD3Vector; f:single);
function  d3VectorReflect(const V,N: TD3Vector): TD3Vector;
function  d3VectorAddScale(const v1: TD3Vector; const v2:single): TD3Vector;
function  d3VectorSubtract(const v1: TD3Vector; const v2: TD3Vector): TD3Vector;
function  d3VectorScale(const v: TD3Vector; factor:single): TD3Vector;
function  PointProject(const p,origin,direction: TD3Vector):single;

function  VectorDistance2(const v1,v2: TD3Vector):single;
function  d3VectorLength(const v: TD3Vector):single;
function  d3MatrixMultiply(const M1,M2: TD3Matrix): TD3Matrix;
function  d3MatrixDeterminant(const M: TD3Matrix):single;
procedure AdjoinTD3Matrix(var M: TD3Matrix);
procedure Scaled3Matrix(var M: TD3Matrix; const factor:single);
procedure InvertMatrix(var M: TD3Matrix);
procedure Transposed3Matrix(var M: TD3Matrix);
function  d3VectorCrossProduct(const V1,V2: TD3Vector): TD3Vector;
function  d3VectorDotProduct(const V1,V2: TD3Vector):single;
function  d3VectorAngleCosine(const V1,V2: TD3Vector):single;
function  d3VectorTransform(const V: TD3Vector; const M: TD3Matrix): TD3Vector;
function  d3CalcPlaneNormal(const p1,p2,p3: TD3Vector): TD3Vector;
procedure RotateVector(var vector: TD3Vector; const axis: TD3Vector; angle:single);
function  d3CreateRotationMatrix(const anAxis: TD3Vector; angle:single): TD3Matrix;
function  d3CreateYawPitchRollMatrix(const y,p,r:single): TD3Matrix;

// Geometry
function GetToken(var S: ansistring; Separators: ansistring; Stop: ansistring=''): ansistring;
function d3GetToken(var Pos: integer; const S: ansistring; Separators: ansistring; Stop: ansistring=''): ansistring;
function d3FloatToStr(Value:single): ansistring;
function d3StrToFloat(Value:ansistring):single;
function d3Point(X,Y,Z:single): TD3Point; overload;
function d3Point(const P: TD3Vector): TD3Point; overload;
function d3PointToString(R: TD3Point): ansistring;
function d3StringToPoint(S: ansistring): TD3Point;
function d3Opacity(const C: TD3Color; const AOpacity:single): TD3Color;
function d3Color(R,G,B: byte; A: byte=$FF): TD3Color;
function d3ColorToStr(Value:TD3Color): ansistring;
function d3StrToColor(Value:ansistring): TD3Color;
function VertexSize(const AFormat: TD3VertexFormat): integer;
function Vertex(x,y,z:single): TD3Vertex;
function VertexNormal(x,y,z,nx,ny,nz:single): TD3VertexNormal;
function TexVertexNormal(x,y,z,nx,ny,nz,tu,tv:single): TD3TexVertexNormal;
function ColorVertex(x,y,z:single; color: TD3Color): TD3ColorVertex;
function ColorVertexNormal(x,y,z:single; nx,ny,nz:single; color: TD3Color): TD3ColorVertexNormal;
function TexVertex(x,y,z:single; tu,tv:single): TD3TexVertex;
function ColorTexVertex(x,y,z:single; color: TD3Color; tu,tv:single): TD3ColorTexVertex;
function ColorTexVertexNormal(x,y,z:single; nx,ny,nz:single; color: TD3Color; tu,tv:single): TD3ColorTexVertexNormal;
function VertexTod3Vector(V: TD3Vertex): TD3Vector;
function d3VectortoVertex(V: TD3Vector): TD3Vertex;

// Matrix
function MatrixOrthoLH(w,h,zn,zf:single): TD3Matrix;
function MatrixOrthoOffCenterLH(l,r,b,t,zn,zf:single): TD3Matrix;
function MatrixOrthoOffCenterRH(l,r,b,t,zn,zf:single): TD3Matrix;
function MatrixPerspectiveFovRH(flovy,aspect,zn,zf:single): TD3Matrix;
function MatrixPerspectiveFovLH(flovy,aspect,zn,zf:single): TD3Matrix;
function MatrixPerspectiveOffCenterLH(l,r,b,t,zn,zf:single): TD3Matrix;
function MatrixLookAtRH(const Eye,At,Up: TD3Vector): TD3Matrix;
function MatrixLookAtDirRH(const Pos,Dir,Up: TD3Vector): TD3Matrix;
function MakeShadowMatrix(const planePoint,planeNormal,lightPos: TD3Vector): TD3Matrix;
function MakeReflectionMatrix(const planePoint,planeNormal: TD3Vector): TD3Matrix;

// Quaternion
procedure NormalizeQuaternion(var q: TD3Quaternion);
function QuaternionToMatrix(quat: TD3Quaternion): TD3Matrix;
function QuaternionMultiply(const qL,qR: TD3Quaternion): TD3Quaternion;
function QuaternionFromAngleAxis(const angle:single; const axis: TD3Vector): TD3Quaternion;
function d3QuaternionFromMatrix(const mat: TD3Matrix): TD3Quaternion;
function RSqrt(v:single):single;
function ISqrt(i: integer): integer;

// Angles
function DegToRad(const Degrees:single):single;
function RadToDeg(const Degrees:single):single;
procedure SinCos(const Theta:single; var Sin,Cos:single);
function ArcCos(const x:single):single;
function ArcSin(const x:single):single;
function ArcTan2(const a,b:single):single;
function NormalizeDegAngle(angle:single):single;
function RoundInt(v:single):single;
function IsPowerOf2(Value:integer):boolean;
function RoundDownToPowerOf2(Value:integer): integer;
function RoundUpToPowerOf2(Value:integer): integer;
function Power(const Base,Exponent:single):single; overload;
function Power(Base:single; Exponent: integer):single; overload;

// Intersection

function RayCastPlaneIntersect(const rayStart,rayVector: TD3Vector; const planePoint,planeNormal: TD3Vector;
                               var intersectPoint: TD3Vector):boolean;
function RayCastIntersectsSphere(const rayStart,rayVector: TD3Vector;
                                 const sphereCenter: TD3Vector; const sphereRadius:single):boolean;
function RayCastSphereIntersect(const rayStart,rayVector: TD3Vector;
                                const sphereCenter: TD3Vector; const sphereRadius:single; var i1,i2: TD3Vector): integer;
function RayCastTriangleIntersect(const rayStart,rayVector: TD3Vector; const p1,p2,p3: TD3Vector;
                                  intersectPoint: PD3Vector=nil; intersectNormal: PD3Vector=nil):boolean;


// Resoruces
procedure AddResource(const AObject: TD3Object);
procedure RemoveResource(const AObject: TD3Object);
function FindResource(const AResource: string): TD3Object;

// Other
function GetBitmapByName(const Name: string): TD3Bitmap;
function GetBitmapParent(const Name: string): TObject;

// Objects Registration
procedure Registerd3Object(const Category: string; const AObject: TD3ObjectClass);
procedure Registerd3Objects(const Category: string; AClasses: array of TD3ObjectClass);
function CreateObject3DFromStream(AOwner: TComponent; const AStream: TStream): TD3Object;

//=============================================================================
//=============== GLobal Variables ============================================
//=============================================================================

var
  GvarD3Designer: TD3Designer;
  GvarD3DefaultCanvasClass: TD3CanvasClass;
  GvarD3DefaultFilterClass: TD3FilterClass;
  GvarD3DefaultPhysicsClass: TD3PhysicsClass;
  GvarD3ObjectList: TStringList;
  GvarD3BitmapList: TStringList;
  GvarD3DefaultStyles: TD3Object;

//=============================================================================
//=============================================================================
//=============================================================================
implementation

uses
  Math,typinfo,IntfGraphics,

{$IFDEF ORCA_3D_USE_DIRECTX9}              // Use DirectX9 API
  DXTypes, D3DX9, Direct3D9,
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL}                 // Use OPENGL API
 {$IFDEF UNIX}
  dynlibs,
  gl,glext,glx,
  pango,
  {$ENDIF}
  {$IFDEF DARWIN}
  dynlibs,
  agl,gl,glext,
  MacOsAll,CarbonPrivate,CarbonDef,
  {$ENDIF}
  Contnrs,
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL_DLG}         // Use OPENGL_DLG API
 {$IFDEF UNIX}

   {$IFDEF DARWIN}
    dynlibs,dglOpenGL,
    AGL,
   {$ELSE}
    dynlibs,dglOpenGL,
    pango,
   {$ENDIF}

 {$ENDIF}
  Contnrs,
{$ENDIF}

{$IFDEF ORCA_3D_USE_MESA}              // Use MESA
  dynlibs,ctypes,
  Contnrs,
  orca_api_mesa,
{$ENDIF}

clipbrd;

//-----------------------------------------------------------------

{$I orca_scene3d_res.inc}

const

  X=0;
  Y=1;
  Z=2;
  W=3;

  cZero:single=0.0;
  cOne:single=1.0;
  cOneDotFive:single=0.5;

type
  TParentControl=class(TWinControl);

var
  User32Lib: THandle=0;

  VarD3SceneTarget: TD3VisualObject=nil;
  VarD3AniThread: TD3AniThread=nil;
  VarD3ResourceList: TList=nil;

  RotateGripDist:single=1.8;
  GripSize:single=0.18;


procedure Registerd3Object(const Category: string; const AObject: TD3ObjectClass);
begin
  if GvarD3ObjectList=nil then
    GvarD3ObjectList := TStringList.Create;
  GvarD3ObjectList.InsertObject(0,Category,TObject(AObject));
end;

procedure Registerd3Objects(const Category: string; AClasses: array of TD3ObjectClass);
var
  I: integer;
begin
  for I := Low(AClasses) to High(AClasses) do
  begin
    Registerd3Object(Category,AClasses[I]);
    RegisterClass(AClasses[I]);
  end;
end;

function GetBitmapByName(const Name: string): TD3Bitmap;
var
  Id3: integer;
begin
  if (GvarD3BitmapList <> nil) and (Name <> '') then
  begin
    Id3 := GvarD3BitmapList.IndexOf(Name);
    if Id3 >= 0 then
    begin
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapStream then
        Result := TD3BitmapStream(GvarD3BitmapList.Objects[Id3]).Bitmap
      else
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapObject then
        Result := TD3BitmapObject(GvarD3BitmapList.Objects[Id3]).Bitmap
      else
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3CustomBufferLayer then
        Result := TD3CustomBufferLayer(GvarD3BitmapList.Objects[Id3]).Buffer;
    end
    else
      Result := nil;
  end
  else
    Result := nil;
end;

function GetBitmapParent(const Name: string): TObject;
var
  Id3: integer;
begin
  if (GvarD3BitmapList <> nil) and (Name <> '') then
  begin
    Id3 := GvarD3BitmapList.IndexOf(Name);
    if Id3 >= 0 then
    begin
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapStream then
        Result := TD3BitmapCollection(TD3BitmapStream(GvarD3BitmapList.Objects[Id3]).Collection).FBitmapList
      else
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3BitmapObject then
        Result := TD3BitmapObject(GvarD3BitmapList.Objects[Id3]).Scene
      else
      if TObject(GvarD3BitmapList.Objects[Id3]) is TD3CustomBufferLayer then
        Result := TD3CustomBufferLayer(GvarD3BitmapList.Objects[Id3]).Scene;
    end
    else
      Result := nil;
  end
  else
    Result := nil;
end;


procedure AddResource(const AObject: TD3Object);
begin
  if VarD3ResourceList=nil then
    VarD3ResourceList := TList.Create;

  VarD3ResourceList.Add(AObject);
end;

procedure RemoveResource(const AObject: TD3Object);
begin
  if VarD3ResourceList <> nil then
    VarD3ResourceList.Remove(AObject);
end;

function FindResource(const AResource: string): TD3Object;
var
  i: integer;
begin
  Result := nil;
  if VarD3ResourceList <> nil then
    for i := 0 to VarD3ResourceList.Count - 1 do
      if CompareText(TD3Object(VarD3ResourceList[i]).ResourceName,AResource)=0 then
      begin
        Result := TD3Object(VarD3ResourceList[i]);
        Break;
      end;
end;

function CreateObject3DFromStream(AOwner: TComponent; const AStream: TStream): TD3Object;
var
  Reader:TReader;
  SavePos: longint;
  I: integer;
  Flags: TFilerFlags;
  ClassName: string;
  ObjClass: TD3ObjectClass;
  BinStream: TStream;
begin
  Result := nil;
  try
    BinStream := TMemoryStream.Create;
    try
      ObjectTextToBinary(AStream,BinStream);
      BinStream.Position := 0;

      Reader := TReader.Create(BinStream,4096);

      Reader.Driver.BeginRootComponent;
      ClassName := Reader.Driver.ReadStr;


      ObjClass := TD3ObjectClass(GetClass(ClassName));
      Result := ObjClass.Create(AOwner);
      if Result <> nil then
      begin
        BinStream.Position := 0;
        Result.LoadFromBinStream(BinStream);
      end;
      Reader.Free;
    finally
      BinStream.Free;
    end;
  except
    Result := nil;
  end;
end;

{$IFDEF WINDOWS}
type

  PBlendFunction=^TBlendFunction;

  _BLENDFUNCTION=packed record
    BlendOp: byte;
    BlendFlags: byte;
    SourceConstantAlpha: byte;
    AlphaFormat: byte;
  end;
  TBlendFunction=_BLENDFUNCTION;
  BLENDFUNCTION=_BLENDFUNCTION;

const

  WS_EX_LAYERED=$00080000;
  LWA_COLORKEY=$00000001;
  LWA_ALPHA=$00000002;
  ULW_COLORKEY=$00000001;
  ULW_ALPHA=$00000002;
  ULW_OPAQUE=$00000004;

var
  SetLayeredWindowAttributes: function(hwnd: HWND; crKey: COLORREF; bAlpha: byte; dwFlags: DWORD): BOOL; stdcall;
  UpdateLayeredWindow: function(hWnd: HWND; hdcDst: HDC; pptDst: PPOINT; psize: PSIZE; hdcSrc: HDC; pptSrc: PPOINT;
  crKey: COLORREF; pblend: PBlendFunction; dwFlags: DWORD): BOOL; stdcall;
  PrintWindow: function(hwnd: HWND; hdcBlt: HDC; nFlags: DWORD): BOOL; stdcall;

{$ENDIF}

{$IFDEF DARWIN}
function WndEventHandler(inHandlerCallRef: EventHandlerCallRef; inEvent: EventRef;
  inUserData: Pointer): OSStatus; stdcall;
var
  myContext: CGContextRef;
  myRect: CGRect;
  rgnCode: WindowRegionCode;
  rgn: RgnHandle;
begin
  Result := CallNextEventHandler(inHandlerCallRef,inEvent);
  Result := eventNotHandledErr;
  if GetEventClass(inEvent)=kEventClassControl then
  begin
    case GetEventKind(inEvent) of
      kEventControlDraw:
      begin
        GetEventParameter(inEvent,kEventParamCGContextRef,typeCGContextRef,nil,
          sizeof(CGContextRef),nil,@myContext);
        if myContext <> nil then
        begin
          myRect.origin.x := 0;
          myRect.origin.y := 0;
          myRect.size.Width := 10000;
          myRect.size.Height := 10000;
          CGContextClearRect(myContext,myRect);
        end;
        Result := noErr;
      end;
    end;
  end;
  if GetEventClass(inEvent)=kEventClassWindow then
  begin
    case GetEventKind(inEvent) of
      kEventWindowGetRegion:
      begin
        //          TD3Scene(inUserData).Invalidate;
        // which region code is being queried?
        GetEventParameter(inEvent,kEventParamWindowRegionCode,typeWindowRegionCode,nil,sizeof(rgnCode),nil,@rgnCode);
        // if it is the opaque region code then set the region to Empty and return noErr to stop the propagation
        if (rgnCode=kWindowOpaqueRgn) then
        begin
          GetEventParameter(inEvent,kEventParamRgnHandle,typeQDRgnHandle,nil,sizeof(rgn),nil,@rgn);
          SetEmptyRgn(rgn);
          Result := noErr;
        end;
        Result := noErr;
      end;
    end;
  end;
end;

function CreateFileURLFromPasteboard(inPasteboard: PasteboardRef): TD3DragObject;
var
  inIndex: CFIndex;
  inCount: ItemCount;
  item: PasteboardItemID;
  fileURL: CFURLRef;
  fileURLData: CFDataRef;
  info: LSItemInfoRecord;
  uti: CFStringRef;
begin
  Fillchar(Result,sizeOf(Result),0);

  if PasteboardGetItemCount(inPasteboard,inCount) <> noErr then
    Exit;
  SetLength(Result.Files,inCount);
  for inIndex := 1 to inCount do
  begin
    if PasteboardGetItemIdentifier(inPasteboard,inIndex,item) <> noErr then
      Exit;
    if PasteboardCopyItemFlavorData(inPasteboard,item,kUTTypeFileURL,fileURLData) <> noErr then
      Exit;

    // create the file URL with the dragged data
    fileURL := CFURLCreateAbsoluteURLWithBytes(kCFAllocatorDefault,CFDataGetBytePtr(fileURLData),CFDataGetLength(
      fileURLData),kCFStringEncodingMacRoman,nil,True);
    if fileURL <> nil then
    begin
      uti := CFURLCopyFileSystemPath(fileURL,kCFURLPOSIXPathStyle);
      Result.Files[inIndex - 1] := CFStringToStr(uti);
      CFRelease(uti);
      CFRelease(fileURL);
    end;
    CFRelease(fileURLData);

    if inIndex=1 then
      Result.Data := Result.Files[inIndex - 1];
  end;
end;

function CtrlEventHandler(inHandlerCallRef: EventHandlerCallRef; inEvent: EventRef; inUserData: Pointer): OSStatus; stdcall;
var
  myContext: CGContextRef;
  myRect: CGRect;
  rgn: RgnHandle;
  proc: RegionToRectsUPP;
  err: OSStatus;
  Part: ControlPartCode;
  drag: DragRef;
  pasteboard: PasteboardRef;
  str: string;
  mouseP: MacOSAll.Point;
  P: TD3Point;
  NewTarget: TD3VisualObject;
  Data: TD3DragObject;
  Accept:boolean;
  Distace:single;
  bool: longbool;
begin
  Result := CallNextEventHandler(inHandlerCallRef,inEvent);
  Result := eventNotHandledErr;
  if GetEventClass(inEvent)=kEventClassControl then
  begin
    case GetEventKind(inEvent) of
      kEventControlDraw:
      begin
        GetEventParameter(inEvent,kEventParamCGContextRef,typeCGContextRef,nil,
          sizeof(CGContextRef),nil,@myContext);
        if myContext <> nil then
        begin
          myRect.origin.x := 0;
          myRect.origin.y := 0;
          myRect.size.Width := 10000;
          myRect.size.Height := 10000;
          CGContextClearRect(myContext,myRect);
        end;
        Result := noErr;
      end;
      kEventControlDragEnter:
      begin
        bool := True;
        SetEventParameter(inEvent,kEventParamControlWouldAcceptDrop,typeBoolean,sizeof(bool),@bool);
       // d3SceneTarget := nil;     // 8888
        Result := noErr;
      end;
      kEventControlDragWithin:
      begin
        GetEventParameter(inEvent,kEventParamDragRef,typeDragRef,nil,sizeof(DragRef),nil,@drag);
        if drag <> nil then
        begin
          GetDragPasteboard(drag,pasteboard);
          if pasteboard <> nil then
          begin
            if TD3Scene(inUserData).Root=nil then
              Exit;

            Data := CreateFileURLFromPasteboard(pasteboard);

            GetDragMouse(drag,mouseP,mouseP);
            with TD3Scene(inUserData).ScreenToClient(Point(mouseP.h,mouseP.v)) do
              P := d3Point(X,Y,0);
            Distace := $FFFF;
            NewTarget := TD3Scene(inUserData).Root.Visual.FindTarget(P,d3ProjectionScreen,Data,Distace);
            if NewTarget=nil then
              NewTarget := TD3Scene(inUserData).Root.Visual.FindTarget(P,d3ProjectionCamera,Data,Distace);

          {  8888
            if (NewTarget <> d3SceneTarget) then
            begin
              if d3SceneTarget <> nil then  d3SceneTarget.DragLeave;
              d3SceneTarget := NewTarget;
              if d3SceneTarget <> nil then
              begin
                d3SceneTarget.DragEnter(Data,d3SceneTarget.AbsoluteToLocal(P));
              end;
            end; }

          end;
        end;
        Result := noErr;
      end;
      kEventControlDragLeave:
      begin
        {   8888
        if d3SceneTarget <> nil then
          d3SceneTarget.DragLeave;
        d3SceneTarget := nil;    }
        Result := noErr;
      end;

      kEventControlDragReceive:
      begin
        GetEventParameter(inEvent,kEventParamDragRef,typeDragRef,nil,sizeof(DragRef),nil,@drag);
        if drag <> nil then
        begin
          GetDragPasteboard(drag,pasteboard);
          if pasteboard <> nil then
          begin
            if TD3Scene(inUserData).Root=nil then
              Exit;

            GetDragMouse(drag,mouseP,mouseP);
            with TD3Scene(inUserData).ScreenToClient(Point(mouseP.h,mouseP.v)) do
              P := d3Point(X,Y,0);
            Data := CreateFileURLFromPasteboard(pasteboard);
            {  8888
            if d3SceneTarget <> nil then
              d3SceneTarget.DragDrop(Data,d3SceneTarget.AbsoluteToLocal(P));
              }
          end;
         // end;
        end;
        //d3SceneTarget := nil; // 8888
        Result := noErr;
      end;


    end;
  end;
end;

var
  EventKinds: array [0..20] of EventTypeSpec;
  WndEventHandlerUPP: EventHandlerUPP;

{$ENDIF}

{$IFDEF WINDOWS}
function WndCallback(Ahwnd: HWND; uMsg: UINT; wParam: WParam; lParam: LParam): LRESULT; stdcall;
var
  Win: TWinControl;

begin

  Win := FindControl(Ahwnd);
  if (Win is TD3Scene) then
  begin
    if not (csDestroying in Win.ComponentState) then
    begin
      if (uMsg=WM_ADDUPDATERECT) or (uMsg=WM_PAINT) then
      begin
        Result := Win.Perform(uMsg,wParam,lParam);
        exit;
      end;
    end;
    Result := CallWindowProcW(@TD3Scene(Win).PrevWndProc,Ahwnd,uMsg,WParam,LParam);
    Exit;
  end;

  Result := CallWindowProcW(@DefWindowProc,Ahwnd,uMsg,WParam,LParam);
end;

{$ENDIF}

type

  TD3AlignInfo=record
    AlignList: TList;
    ControlIndex: integer;
    Align: TD3Align;
    Scratch: integer;
  end;

//==============================================================

{$I orca_scene3d_positions.inc}
{$I orca_scene3d_bounds.inc}
{$I orca_scene3d_transforms.inc}
{$I orca_scene3d_filters.inc}
{$I orca_scene3d_materials.inc}
{$I orca_scene3d_physics.inc}
{$I orca_scene3d_mesh.inc}
{$I orca_scene3d_fonts.inc}
{$I orca_scene3d_obj_base.inc}
{$I orca_scene3d_obj_base_visible.inc}
{$I orca_scene3d_obj_oblects.inc}
{$I orca_scene3d_designers.inc}
{$I orca_scene3d_scene.inc}
{$I orca_scene3d_bitmaps.inc}
{$I orca_scene3d_animations.inc}
{$I orca_scene3d_layers.inc}
{$I orca_scene3d_canvas.inc}
{$I orca_scene3d_objects.inc}
{$I orca_scene3d_math.inc}
{$I orca_scene3d_particles.inc}

//==============================================================
//==============================================================
//Platform things

{$IFDEF ORCA_3D_USE_DIRECTX9}
 {$I orca_scene3d_canvas_directx9.inc}
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL}
 {$I orca_scene3d_canvas_gl.inc}
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL_DLG}
 {$I orca_scene3d_canvas_gl_dgl.inc}
{$ENDIF}

{$IFDEF ORCA_3D_USE_MESA}
 {$I orca_scene3d_canvas_mesa.inc}
{$ENDIF}

//==============================================================
//==============================================================

initialization

{$IFDEF WINDOWS}
  User32Lib := LoadLibrary(User32);
  if User32Lib <> 0 then
  begin
    @SetLayeredWindowAttributes := GetProcAddress(User32Lib,'SetLayeredWindowAttributes');
    @UpdateLayeredWindow := GetProcAddress(User32Lib,'UpdateLayeredWindow');
    @PrintWindow := GetProcAddress(User32Lib,'PrintWindow');
  end;
{$ENDIF}

  //.................................................................................................
  RegisterClasses([TD3Bitmap,TD3Position,TD3MeshData,TD3Material,TD3BitmapRect,TD3Bounds]);
  RegisterClasses([TD3Scene,TD3Object,TD3CustomLayer,TD3CustomBufferLayer]);
  RegisterClasses([TD3BitmapStream,TD3BitmapCollection,TD3BitmapList]);
  RegisterClasses([TD3Shape,TD3Shape3D]);
  RegisterClasses([TD3d2Layer,TD3Screend2Layer]);
  RegisterClasses([TD3ScreenImage,TD3ScreenDummy]);

  Registerd3Objects('Scene',[TD3Camera,TD3Light,TD3Dummy,TD3ProxyObject]);
  Registerd3Objects('Resources',[TD3BitmapObject]);
  Registerd3Objects('Shapes',[TD3CanvasLayer,TD3BufferLayer,TD3ObjectLayer,TD3Plane,
    TD3Cube,TD3Mesh,TD3Sphere,TD3Cylinder,TD3RoundCube,TD3Cone,TD3Text,TD3Image,TD3Grid,TD3StrokeCube]);
  Registerd3Objects('3D',[TD3Text3D,TD3Path3D,TD3Rectangle3D,TD3Ellipse3D]);
  Registerd3Objects('Animations',[TD3ColorAnimation,TD3FloatAnimation]);
  Registerd3Objects('GUI',[TD3GUICanvasLayer,TD3GUILayout,TD3GUIImage,TD3GUIPlane,
    TD3GUIText,TD3GUISelection,TD3GUIScene2DLayer]);
  Registerd3Objects('Controls',[TD3TextBox3D]);
  Registerd3Objects('Dynamics',[TD3ParticleEmitter]);
  //.................................................................................................

{$IFDEF WINDOWS}
  OleInitialize(nil);
{$ENDIF}

{$IFDEF ORCA_3D_USE_DIRECTX9}
  GvarD3DefaultFilterClass := TD3FilterGdiPlus;
  GvarD3DefaultCanvasClass := TD3DirectXCanvas;
  CalcMaxBitmapSize;
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL}
  GvarD3DefaultFilterClass := TD3FilterOpenGL;
  GvarD3DefaultCanvasClass := TD3CanvasOpenGL;
{$ENDIF}
{$IFDEF ORCA_3D_USE_OPENGL_DLG}
  GvarD3DefaultFilterClass := TD3FilterOpenGLDGL;
  GvarD3DefaultCanvasClass := TD3CanvasOpenGLDGL;
{$ENDIF}

{$IFDEF ORCA_3D_USE_MESA}
  Set8087CW($133F);
  LoadOpenGL(ExtractFilePath(ParamStr(0)) + 'opengl32.dll');
  LoadOSMesa(ExtractFilePath(ParamStr(0)) + 'osmesa32.dll');
{$ENDIF}

  // Set Default Physics to Dummy one
  GVarD3DefaultPhysicsClass := TD3DummyPhysics;

finalization

{$IFDEF ORCA_3D_USE_DIRECTX9}
  SAFE_RELEASE(Enum);
{$ENDIF}

{$IFDEF ORCA_3D_USE_OPENGL}

{$ENDIF}
{$IFDEF ORCA_3D_USE_OPENGL_DLG}

{$ENDIF}

{$IFDEF ORCA_3D_USE_MESA}
  FreeOSMesa;
  FreeOpenGL;
{$ENDIF}

  if VarD3AniThread <> nil then
    FreeAndNil(VarD3AniThread);
  if GvarD3ObjectList <> nil then
    FreeAndNil(GvarD3ObjectList);
  if GvarD3BitmapList <> nil then
    FreeAndNil(GvarD3BitmapList);
  if VarD3ResourceList <> nil then
    FreeAndNil(VarD3ResourceList);
end.
