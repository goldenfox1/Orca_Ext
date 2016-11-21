{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


var
  Res:HResult;

const
  TargetMode:TD2D1_RenderTargetType=D2D1_RENDER_TARGET_TYPE_default;

type

TD2HackBitmap=class(TD2Bitmap);

var
  Factory:ID2D1Factory;
  DWriteFactory:IDWriteFactory;
  ImagingFactory:IWICImagingFactory;

type

TD2FilterWIC=class(TD2Filter)
  private
  published
    class function GetFileTypes:string;  override;
    class function GetImageSize(const AFileName:string):TD2Point;  override;
    function LoadFromFile(const AFileName:string; const Rotate:single; var Bitmap:TD2Bitmap):boolean;  override;
    function SaveToFile(const AFileName:string; var Bitmap:TD2Bitmap; const Params:string=''):boolean;  override;
    function LoadThumbnailFromFile(const AFileName:string; const AFitWidth,AFitHeight:single; const UseEmbedded:boolean;
                                   var Bitmap:TD2Bitmap):boolean;  override;
    function LoadFromStream(const AStream:TStream; var Bitmap:TD2Bitmap):boolean;  override;
    function SaveToStream(const AStream:TStream; var Bitmap:TD2Bitmap; const Format:string;
                          const Params:string=''):boolean;  override;
  end;

TD2CanvasD2D=class(TD2Canvas)
  private
    FLastState   :integer;
    FBufferHandle:cardinal;
    FBitmapInfo  :TBitmapInfo;
    RenderTarget :ID2D1RenderTarget;
    Brush        :ID2D1Brush;
    StrokeBrush  :ID2D1Brush;
    StrokeStyle  :ID2D1StrokeStyle;
    Layer        :ID2D1Layer;
    FSaved       :Pointer;
    WICBitmap    :IWICBitmap;
    procedure CreateResources;
    procedure DisposeResources;
  protected
    procedure ApplyFill(ARect:TD2Rect; const AOpacity:single);
    procedure ApplyStroke(ARect:TD2Rect; const AOpacity:single);
    procedure FontChanged(Sender:TObject);  override;
    procedure IntFillPath(P:ID2D1Geometry; R:TD2Rect; Opacity:single);
    procedure IntFillRect(R:TD2Rect; Opacity:single);
  public
    constructor Create(const AWidth,AHeight:integer);  override;
    constructor CreateFromBitmap(const ABitmap:TD2Bitmap);  override;
    destructor Destroy;  override;
    class function GetBitmapScanline(Bitmap:TD2Bitmap; y:integer):PD2ColorArray;  override;
    function  BeginScene:boolean;  override;
    procedure EndScene;  override;
    procedure FreeBuffer;  override;
    procedure ResizeBuffer(const AWidth,AHeight:integer);  override;
    procedure FlushBuffer(const X,Y:integer; const DC:Cardinal);  override;
    procedure FlushBufferRect(const X,Y:integer; const DC:Cardinal; const ARect:TD2Rect);  override;
    procedure Clear(const Color:TD2Color);  override;
    procedure ClearRect(const ARect:TD2Rect; const AColor:TD2Color=0);  override;
    procedure SetMatrix(const M:TD2Matrix);  override;
    procedure MultyMatrix(const M:TD2Matrix);  override;
    function  SaveCanvas:cardinal;  override;
    procedure RestoreCanvas(const AState:cardinal);  override;
    procedure SetClipRects(const ARects:array of TD2Rect);  override;
    procedure IntersectClipRect(const ARect:TD2Rect);  override;
    procedure ExcludeClipRect(const ARect:TD2Rect);  override;
    procedure ResetClipRect;  override;
    procedure DrawLine(const APt1,APt2:TD2Point; const AOpacity:single);  override;
    procedure FillRect(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners; const AOpacity:single;
                       const ACornerType:TD2CornerType=d2CornerRound);  override;
    procedure DrawRect(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners; const AOpacity:single;
                       const ACornerType:TD2CornerType=d2CornerRound);  override;
    procedure FillEllipse(const ARect:TD2Rect; const AOpacity:single);  override;
    procedure DrawEllipse(const ARect:TD2Rect; const AOpacity:single);  override;
    procedure FillText(const ARect,AClipRect:TD2Rect; const AText:WideString; const WordWrap:boolean;
                       const AOpacity:single; const ATextAlign:TD2TextAlign; const AVTextAlign:TD2TextAlign=d2TextAlignCenter);  override;
    procedure MeasureText(var ARect:TD2Rect; AClipRect:TD2Rect; const AText:WideString; const WordWrap:boolean; const ATextAlign:TD2TextAlign;
                          const AVTextAlign:TD2TextAlign=d2TextAlignCenter);  override;
    function  TextToPath(Path:TD2PathData; const ARect:TD2Rect; const AText:WideString; const WordWrap:boolean; const ATextAlign:TD2TextAlign;
                         const AVTextAlign:TD2TextAlign=d2TextAlignCenter):boolean;  override;
    function  PtInPath(const APoint:TD2Point; const ARect:TD2Rect; const APath:TD2PathData):boolean;  override;
    procedure FillPath(const APath:TD2PathData; const ARect:TD2Rect; const AOpacity:single);  override;
    procedure DrawPath(const APath:TD2PathData; const ARect:TD2Rect; const AOpacity:single);  override;
    procedure DrawBitmap(const ABitmap:TD2Bitmap; const SrcRect,DstRect:TD2Rect; const AOpacity:single; const HighSpeed:boolean=false);  override;
    procedure DrawThumbnail(const ABitmap:TD2Bitmap; const Width,Height:single);  override;
  end;

//==================================================================
//==================================================================


procedure SAFE_RELEASE(var i);
begin
  if IUnknown(i) <> nil then IUnknown(i):= nil;
end;

{ TD2CustomTreeGrid }

constructor TD2CustomTreeGrid.Create(AOwner: TComponent);
begin
  inherited;
  FResource:='gridstyle';
end;

function TD2CustomTreeGrid.ItemClass: string;
begin
  Result:='TD2TGTextColumn;TD2TGCheckColumn;TD2TGPopupColumn;TD2TGImageColumn;TD2TGProgressColumn';
end;

//==================== TD2FilterWIC =======================================

class function TD2FilterWIC.GetFileTypes:string;
begin
  Result:='*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.gif;*.ico;*.wmp'
end;

class function TD2FilterWIC.GetImageSize(const AFileName:string):TD2Point;
var
  dec:IWICBitmapDecoder;
  frame:IWICBitmapFrameDecode;
  W,H:cardinal;
  fn:WideString;
begin
  W:=0; H:=0;
  fn:=AFileName;
  ImagingFactory.CreateDecoderFromFilename(PWideChar(fn),nil,$FFFFFFFF,WICDecodeMetadataCacheOnDemand,dec);

  if Assigned(dec) then
  begin
    dec.GetFrame(0,frame);

    if Assigned(frame) then
    begin
      frame.GetSize(W,H);
    end;

  end;

  Result:=d2Point(W,H);
end;

function TD2FilterWIC.LoadFromFile(const AFileName:string; const Rotate:single; var Bitmap:TD2Bitmap):boolean;
var
  dec:IWICBitmapDecoder;
  frame:IWICBitmapFrameDecode;
  conv:IWICFormatConverter;
  W,H:cardinal;
  fn:WideString;
begin
  fn:=AFileName;
  Result:=false;
  ImagingFactory.CreateDecoderFromFilename(PWideChar(fn),nil,$FFFFFFFF,WICDecodeMetadataCacheOnDemand,dec);

  if Assigned(dec) then
  begin
    dec.GetFrame(0,frame);
    if Assigned(frame) then
    begin
      ImagingFactory.CreateFormatConverter(conv);
      Res:=conv.Initialize(frame,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,nil,0,0);

      if Res=0 then
      begin
        conv.GetSize(W,H);

        Bitmap.SetSize(W,H);
        Bitmap.Clear($FFFF0000);
        Res:=conv.CopyPixels(nil,W * 4,W * H * 4,PByte(Bitmap.Startline));
        Result:=true;
      end;

    end;
  end;
end;

function TD2FilterWIC.LoadFromStream(const AStream:TStream; var Bitmap:TD2Bitmap):boolean;
var
  dec:IWICBitmapDecoder;
  mem:TMemoryStream;
  stream:IWICStream;
  frame:IWICBitmapFrameDecode;
  conv:IWICFormatConverter;
  W,H:cardinal;
begin
  Result:=false;
  mem:=TMemoryStream.Create;
  mem.CopyFrom(AStream,AStream.Size);
  ImagingFactory.CreateStream(stream);
  stream.InitializeFromMemory(mem.Memory,mem.Size);

  ImagingFactory.CreateDecoderFromStream(stream,nil,WICDecodeMetadataCacheOnDemand,dec);
  if Assigned(dec) then
  begin
    dec.GetFrame(0,frame);
    if Assigned(frame) then
    begin
      ImagingFactory.CreateFormatConverter(conv);
      Res:=conv.Initialize(frame,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,nil,0,0);
      if Res=0 then
      begin
        conv.GetSize(W,H);

        Bitmap.SetSize(W,H);
        Res:=conv.CopyPixels(nil,W * 4,W * H * 4,PByte(Bitmap.Startline));
        Result:=true;
      end;
    end;
  end;
  mem.Free;
end;

function TD2FilterWIC.LoadThumbnailFromFile(const AFileName:string; const AFitWidth,AFitHeight:single;
                                            const UseEmbedded:boolean; var Bitmap:TD2Bitmap):boolean;
var
  bmp:IWICBitmapSource;
  dec:IWICBitmapDecoder;
  scale:single;
  scaler:IWICBitmapScaler;
  frame:IWICBitmapFrameDecode;
  conv:IWICFormatConverter;
  W,H:cardinal;
  R:TD2Rect;
  fn:WideString;
begin
  fn:=AFileName;
  Result:=false;
  ImagingFactory.CreateDecoderFromFilename(PWideChar(fn),nil,$FFFFFFFF,WICDecodeMetadataCacheOnDemand,dec);
  if Assigned(dec) then
  begin
    dec.GetFrame(0,frame);
    if UseEmbedded then
      frame.GetThumbnail(bmp);
    if Assigned(bmp) then
    begin
      ImagingFactory.CreateFormatConverter(conv);
      Res:=conv.Initialize(bmp,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,nil,0,0);
      if Res=0 then
      begin
        conv.GetSize(W,H);

        Bitmap.SetSize(W,H);
        Bitmap.Clear($FFFF0000);
        Res:=conv.CopyPixels(nil,W * 4,W * H * 4,PByte(Bitmap.Startline));
        Result:=true;
      end;
    end else
    begin

      if Assigned(frame) then
      begin
        frame.GetSize(W,H);
        R:=d2Rect(0,0,W,H);
        scale:=d2FitRect(R,d2Rect(0,0,AFitWidth,AFitHeight));
        if scale=0 then scale:=0.001;
        if scale < 1 then scale:=1;

        ImagingFactory.CreateBitmapScaler(scaler);
        Res:=scaler.Initialize(frame,round(W / scale),round(H / scale),WICBitmapInterpolationModeLinear);

        ImagingFactory.CreateFormatConverter(conv);
        Res:=conv.Initialize(scaler,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,nil,0,0);

        if Res=0 then
        begin
          conv.GetSize(W,H);

          Bitmap.SetSize(W,H);
          Bitmap.Clear($FFFF0000);
          Res:=conv.CopyPixels(nil,W * 4,W * H * 4,PByte(Bitmap.Startline));
          Result:=true;
        end;

      end;
    end;
  end;
end;

function TD2FilterWIC.SaveToFile(const AFileName:string; var Bitmap:TD2Bitmap; const Params:string):boolean;
var
  bmp:IWICBitmap;
  enc:IWICBitmapEncoder;
  encoderType:TGuid;
  stream:IWICStream;
  frame:IWICBitmapFrameEncode;
  conv:IWICFormatConverter;
  props:IPropertyBag2;
  fn:WideString;
  pformat:WICPixelFormatGUID;
begin
  fn:=AFileName;
  Result:=false;
  encoderType:=GUID_ContainerFormatPng;

  if (LowerCase(ExtractFileExt(AFileName))='.jpg') or (LowerCase(ExtractFileExt(AFileName))='.jpeg') then
    encoderType:=GUID_ContainerFormatJpeg;

  if (LowerCase(ExtractFileExt(AFileName))='.bmp') then
    encoderType:=GUID_ContainerFormatBmp;

  if (LowerCase(ExtractFileExt(AFileName))='.png') then
    encoderType:=GUID_ContainerFormatPng;

  if (LowerCase(ExtractFileExt(AFileName))='.tif') or (LowerCase(ExtractFileExt(AFileName))='.tiff') then
    encoderType:=GUID_ContainerFormatTiff;

  if (LowerCase(ExtractFileExt(AFileName))='.gif') then
    encoderType:=GUID_ContainerFormatGif;

  if (LowerCase(ExtractFileExt(AFileName))='.wmp') then
    encoderType:=GUID_ContainerFormatWmp;

  ImagingFactory.CreateEncoder(encoderType,nil,enc);

  if Assigned(enc) then
  begin
    ImagingFactory.CreateStream(stream);
    stream.InitializeFromFilename(PWideChar(fn),$FFFFFFFF);
    enc.Initialize(stream,WICBitmapEncoderNoCache);
    enc.CreateNewFrame(frame,props);
    if Assigned(frame) then
    begin

      Res:=frame.Initialize(props);

      Res:=frame.SetSize(Bitmap.Width,Bitmap.Height);
      pformat:=GUID_WICPixelFormat32bppPBGRA;
      Res:=frame.SetPixelFormat(pformat);
      if IsEqualGuid(pformat,GUID_WICPixelFormat32bppPBGRA) then
      begin
        Res:=frame.WritePixels(Bitmap.Height,Bitmap.Width * 4,Bitmap.Width * Bitmap.Height * 4,PByte(Bitmap.Startline));
        Res:=frame.Commit;
        Res:=enc.Commit;
      end
      else
      begin
        ImagingFactory.CreateBitmapFromMemory(Bitmap.Width,Bitmap.Height,GUID_WICPixelFormat32bppPBGRA,
          Bitmap.Width * 4,Bitmap.Width * Bitmap.Height * 4,PByte(Bitmap.Startline),bmp);
        ImagingFactory.CreateFormatConverter(conv);
        Res:=conv.Initialize(bmp,pformat,WICBitmapDitherTypeNone,nil,0,0);
        Res:=frame.WriteSource(bmp,nil);
        Res:=frame.Commit;
        Res:=enc.Commit;
      end;
    end;
  end;
end;

function TD2FilterWIC.SaveToStream(const AStream:TStream;
  var Bitmap:TD2Bitmap; const Format,Params:string):boolean;
var
  bmp:IWICBitmap;
  enc:IWICBitmapEncoder;
  encoderType:TGuid;
  stream:IWICStream;
  frame:IWICBitmapFrameEncode;
  conv:IWICFormatConverter;
  props:IPropertyBag2;
  pformat:WICPixelFormatGUID;
  adapter:TStreamAdapter;
begin
  Result:=false;

  encoderType:=GUID_ContainerFormatPng;

  if LowerCase(Format)='jpeg' then encoderType:=GUID_ContainerFormatJpeg;
  if LowerCase(Format)='jpg' then encoderType:=GUID_ContainerFormatJpeg;
  if LowerCase(Format)='png' then encoderType:=GUID_ContainerFormatPng;
  if LowerCase(Format)='bmp' then encoderType:=GUID_ContainerFormatBmp;
  if LowerCase(Format)='tif' then encoderType:=GUID_ContainerFormatTiff;
  if LowerCase(Format)='tiff' then encoderType:=GUID_ContainerFormatTiff;
  if LowerCase(Format)='gif' then encoderType:=GUID_ContainerFormatGif;
  if LowerCase(Format)='wmp' then encoderType:=GUID_ContainerFormatWmp;

  ImagingFactory.CreateEncoder(encoderType,nil,enc);

  if Assigned(enc) then
  begin
    ImagingFactory.CreateStream(stream);
    adapter:=TStreamAdapter.Create(AStream);
    IUnknown(adapter)._AddRef;
    stream.InitializeFromIStream(adapter);
    enc.Initialize(stream,WICBitmapEncoderNoCache);
    enc.CreateNewFrame(frame,props);

    if Assigned(frame) then
    begin
      frame.Initialize(props);
      Res:=frame.SetSize(Bitmap.Width,Bitmap.Height);
      pformat:=GUID_WICPixelFormat32bppPBGRA;
      Res:=frame.SetPixelFormat(pformat);

      if IsEqualGuid(pformat,GUID_WICPixelFormat32bppPBGRA) then
      begin
        Res:=frame.WritePixels(Bitmap.Height,Bitmap.Width * 4,Bitmap.Width * Bitmap.Height * 4,PByte(Bitmap.Startline));
        Res:=frame.Commit;
        Res:=enc.Commit;
      end  else
      begin
        ImagingFactory.CreateBitmapFromMemory(Bitmap.Width,Bitmap.Height,GUID_WICPixelFormat32bppPBGRA,
          Bitmap.Width * 4,Bitmap.Width * Bitmap.Height * 4,PByte(Bitmap.Startline),bmp);
        ImagingFactory.CreateFormatConverter(conv);
        Res:=conv.Initialize(bmp,pformat,WICBitmapDitherTypeNone,nil,0,0);
        Res:=frame.WriteSource(bmp,nil);
        Res:=frame.Commit;
        Res:=enc.Commit;
      end;

    end;
    IUnknown(adapter)._Release;
  end;
end;


const
  D2DERR_RECREATE_TARGET=HResult($8899000C);

var
    D2DLibrary:Cardinal=0;
    DWriteLibrary:Cardinal=0;

  procedure LoadD2D;
  const
    DLL_DWrite='DWrite.dll';
    DLL_D2D1='d2d1.dll';
  begin
    D2DLibrary:=LoadLibrary(PChar(DLL_D2D1));
    if D2DLibrary <> 0 then
    begin
      D2D1CreateFactory:=GetProcAddress(D2DLibrary,'D2D1CreateFactory');
    end;
    DWriteLibrary:=LoadLibrary(PChar(DLL_DWrite));
    if DWriteLibrary <> 0 then
    begin
      DWriteCreateFactory:=GetProcAddress(DWriteLibrary,'DWriteCreateFactory');
    end
  end;

  procedure FreeD2D;
  begin
    if (D2DLibrary <> 0) then
    begin
      D2D1CreateFactory:=nil;
      FreeLibrary(D2DLibrary);
      D2DLibrary:=0;
    end;
    if DWriteLibrary <> 0 then
    begin
      DWriteCreateFactory:=nil;
      FreeLibrary(DWriteLibrary);
      DWriteLibrary:=0;
    end
  end;

procedure SetD2DDefault;
begin
  if D2DLibrary <> 0 then
  begin
    GvarD2DefaultCanvasClass:=TD2CanvasD2D;
    GvarD2DefaultFilterClass:=TD2FilterWIC;
  end;
end;

function D2iRect(const R:TD2Rect):TD2D1_RectF;
begin
  Result:=TD2D1_RectF(R);
end;

function D2iColor(const AColor:TD2Color; const Opacity:single):TD2D1_ColorF;
begin
  with TD2ColorRec(AColor) do
    Result:=D2D1ColorF(R / $FF,G / $FF,B / $FF,(A / $FF) * Opacity);
end;

function D2iPoint(X,Y:single):TD2D1_Point2F;
begin
  Result.X:=X;
  Result.Y:=Y;
end;

function D2iSize(W,H:cardinal):TD2D_SizeU;
begin
  Result.Width:=W;
  Result.Height:=H;
end;

function D2iMatrix(M:TD2Matrix):TD2D1_Matrix3X2F;
begin
  Result._11:=M.m11;
  Result._12:=M.m12;
  Result._21:=M.m21;
  Result._22:=M.m22;
  Result._31:=M.m31;
  Result._32:=M.m32;
end;

function D2iBezier(x1,y1,x2,y2,x3,y3:single):TD2D1_BezierSegment;
begin
  Result.Point1.X:=x1;
  Result.Point1.Y:=y1;
  Result.Point2.X:=x2;
  Result.Point2.Y:=y2;
  Result.Point3.X:=x3;
  Result.Point3.Y:=y3;
end;

function D2iEllipse(R:TD2Rect):TD2D1_Ellipse;
begin
  Result.Point.x:=(R.Right + R.left) / 2;
  Result.Point.y:=(R.bottom + R.top) / 2;
  Result.RadiusX:=(R.Right - R.left) / 2;
  Result.RadiusY:=(R.bottom - R.top) / 2;
end;

function D2iFontStyle(Style:TD2FontStyle):TDWrite_FontStyle;
begin
  case Style of
    d2FontRegular:Result:=DWRITE_FONT_STYLE_NORMAL;
    d2FontBold:Result:=DWRITE_FONT_STYLE_NORMAL;
    d2FontItalic:Result:=DWRITE_FONT_STYLE_OBLIQUE;
    d2FontBoldItalic:Result:=DWRITE_FONT_STYLE_OBLIQUE;
    d2FontUnderline:Result:=DWRITE_FONT_STYLE_NORMAL;
    d2FontStrikeout:Result:=DWRITE_FONT_STYLE_NORMAL;
  end;
end;

function D2iFontWeight(Style:TD2FontStyle):TDWrite_FontWeight;
begin
  case Style of
    d2FontRegular:Result:=DWRITE_FONT_WEIGHT_NORMAL;
    d2FontBold:Result:=DWRITE_FONT_WEIGHT_BOLD;
    d2FontItalic:Result:=DWRITE_FONT_WEIGHT_NORMAL;
    d2FontBoldItalic:Result:=DWRITE_FONT_WEIGHT_BOLD;
    d2FontUnderline:Result:=DWRITE_FONT_WEIGHT_NORMAL;
    d2FontStrikeout:Result:=DWRITE_FONT_WEIGHT_NORMAL;
  end;
end;

function BitmapProp(DXGI:TDXGI_Format; AlphaMode:TD2D1_AlphaMode):TD2D1_BitmapProperties;
begin
  Result.PixelFormat.Format:=DXGI;
  Result.PixelFormat.AlphaMode:=AlphaMode;
  Result.DpiX:=0;
  Result.DpiY:=0;
end;

//================= TD2CanvasD2D ================================

const
  SavedCount=200;

type

  TSaveData=record
    GdiIndex:integer;
    Matrix:TD2Matrix;
    AbsoluteMatrix,InvertMatrix:TD2Matrix;
    StrokeThickness:single;
    Stroke:TD2Brush;
    Fill:TD2Brush;
  end;

  PSaveDataArray=^TSaveDataArray;
  TSaveDataArray=array [0..100] of TSaveData;

var
  D2DLoadcount:integer=0;

procedure AddD2DRef;
begin
  if Factory=nil then
  begin
    D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,ID2D1Factory,nil,Factory);
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,IDWriteFactory,IUnknown(DWriteFactory));
    CoCreateInstance(CLSID_WICImagingFactory,nil,CLSCTX_INPROC_SERVER or CLSCTX_LOCAL_SERVER,IUnknown,ImagingFactory);
  end;
  D2DLoadcount:=D2DLoadcount + 1;
end;

procedure DecD2DRef;
begin
  D2DLoadcount:=D2DLoadcount - 1;
  if D2DLoadcount=0 then
  begin
    SAFE_RELEASE(ImagingFactory);
    SAFE_RELEASE(DWriteFactory);
    SAFE_RELEASE(Factory);
  end;
end;

constructor TD2CanvasD2D.Create(const AWidth,AHeight:integer);
begin
  AddD2DRef;
  inherited;
  GetMem(FSaved,SizeOf(TSaveData) * SavedCount);
end;

constructor TD2CanvasD2D.CreateFromBitmap(const ABitmap:TD2Bitmap);
begin
  AddD2DRef;
  inherited;
  FBitmap:=ABitmap;
//  UpdateBitmap(FBitmap);
  GetMem(FSaved,SizeOf(TSaveData) * SavedCount);

  if Assigned(ImagingFactory) then
  begin
    ImagingFactory.CreateBitmapFromMemory(FBitmap.Width,FBitmap.Height,
                                          GUID_WICPixelFormat32bppPBGRA,
                                          FBitmap.Width * 4,
                                          FBitmap.Width * FBitmap.Height * 4,
                                          PByte(FBitmap.Startline),
                                          WICBitmap);
    CreateResources;
  end;
end;

destructor TD2CanvasD2D.Destroy;
begin
  FreeMem(FSaved,SizeOf(TSaveData) * 100);
  SAFE_RELEASE(Brush);
  SAFE_RELEASE(StrokeBrush);
  SAFE_RELEASE(StrokeStyle);
  inherited;
  DecD2DRef;
end;

procedure TD2CanvasD2D.FreeBuffer;
begin
  DisposeResources;
  if FBuffered then
  begin

    if FBufferHandle=0 then Exit;
    if FHandle <> 0 then DeleteDC(FHandle);

    FHandle:=0;

    if FBufferHandle <> 0 then DeleteObject(FBufferHandle);

    FBufferHandle:=0;
  end;
end;

procedure TD2CanvasD2D.CreateResources;
var
  SizeInPixels:D2D1_SIZE_U;
begin

  if RenderTarget=nil then
  begin
    SizeInPixels.width:=FWidth;
    SizeInPixels.height:=FHeight;

    if FBitmap <> nil then
    begin
      Factory.CreateWicBitmapRenderTarget(WICBitmap,D2D1RenderTargetProperties(TargetMode,
                                         D2D1PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
                                         RenderTarget);
    end else
    begin

      if FBuffered then
      begin

        if FHandle=0 then
        begin
          if Assigned(ImagingFactory) then
          begin
            ImagingFactory.CreateBitmapFromMemory(FBitmap.Width,FBitmap.Height,GUID_WICPixelFormat32bppPBGRA,
              FBitmap.Width * 4,FBitmap.Width * FBitmap.Height * 4,PByte(FBitmap.Startline),WICBitmap);
            Factory.CreateWicBitmapRenderTarget(WICBitmap,D2D1RenderTargetProperties(TargetMode,D2D1PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
              RenderTarget);
          end;
        end
        else
        begin
          Factory.CreateDCRenderTarget(D2D1RenderTargetProperties(TargetMode,D2D1PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
            ID2D1DCRenderTarget(RenderTarget));
          ID2D1DCRenderTarget(RenderTarget).BindDC(FHandle,Rect(0,0,FWidth,FHeight));
        end;

      end else
        Factory.CreateHWndRenderTarget(D2D1RenderTargetProperties(TargetMode,
                                       D2D1PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
                                       D2D1HwndRenderTargetProperties(FParent,SizeInPixels),
                                       ID2D1HwndRenderTarget(RenderTarget));
    end;

    RenderTarget.SetDpi(96,96);
    RenderTarget.SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
  end
end;

procedure TD2CanvasD2D.DisposeResources;
begin
  SAFE_RELEASE(StrokeBrush);
  SAFE_RELEASE(Brush);
  SAFE_RELEASE(Layer);
  SAFE_RELEASE(RenderTarget);
  SAFE_RELEASE(WICBitmap);
end;

procedure TD2CanvasD2D.Clear(const Color:TD2Color);
begin
  RenderTarget.Clear(D2iColor(Color,1));
end;

procedure TD2CanvasD2D.ClearRect(const ARect:TD2Rect; const AColor:TD2Color);
begin
  //...
end;

procedure TD2CanvasD2D.ResizeBuffer(const AWidth,AHeight:integer);
var
  Size:D2D1_SIZE_U;
begin
  if Assigned(FScene) and (FScene.GetTransparency) and not FBuffered then
  begin
    FBuffered:=true;
  end;

  if (AWidth=FWidth) and (AHeight=FHeight) then Exit;

  if (FParent <> 0) and not (FBuffered) and (RenderTarget <> nil) then
  begin
    FWidth:=AWidth;
    FHeight:=AHeight;
    if FWidth <= 0 then FWidth:=1;
    if FHeight <= 0 then FHeight:=1;
    Size.Width:=AWidth;
    Size.Height:=AHeight;
    ID2D1HwndRenderTarget(RenderTarget).Resize(Size);
    Exit;
  end;

  FreeBuffer;
  FWidth:=AWidth;
  FHeight:=AHeight;
  if FWidth <= 0 then FWidth:=1;
  if FHeight <= 0 then FHeight:=1;
  FResized:=true;

  if FBuffered then
  begin

    with FBitmapInfo.bmiHeader do
    begin
      biSize:=SizeOf(TBitmapInfoHeader);
      biPlanes:=1;
      biBitCount:=32;
      biCompression:=BI_RGB;
      biWidth:=AWidth;
      if biWidth <= 0 then biWidth:=1;
      biHeight:=-AHeight;
      if biHeight >= 0 then biHeight:=-1;
    end;

    FBufferHandle:=CreateDIBSection(0,FBitmapInfo,DIB_RGB_COLORS,Pointer(FBufferBits),0,0);
    if FBufferBits=nil then
      raise Exception.Create('Can''t allocate the DIB handle');

    FHandle:=CreateCompatibleDC(0);
    if FHandle=0 then
    begin
      DeleteObject(FBufferHandle);
      FHandle:=0;
      FBufferBits:=nil;
      raise Exception.Create('Can''t create compatible DC');
    end;

    if SelectObject(FHandle,FBufferHandle)=0 then
    begin
      DeleteDC(FHandle);
      DeleteObject(FBufferHandle);
      FHandle:=0;
      FBufferHandle:=0;
      FBufferBits:=nil;
      raise Exception.Create('Can''t select an object into DC');
    end;

  end;

end;

procedure TD2CanvasD2D.FlushBuffer(const X,Y:integer;
  const DC:Cardinal);
begin
  if FBufferHandle=0 then Exit;
  if DC=0 then Exit;
  Windows.BitBlt(DC,X,Y,FWidth,FHeight,FHandle,0,0,SRCCOPY);
end;

procedure TD2CanvasD2D.FlushBufferRect(const X,Y:integer;
  const DC:Cardinal; const ARect:TD2Rect);
var
  R:TRect;
begin
  if FBufferHandle=0 then Exit;
  if DC=0 then Exit;
  R:=Rect(trunc(ARect.left),trunc(ARect.top),trunc(ARect.right) + 1,trunc(ARect.bottom) + 1);
  with R do
  begin
    Windows.BitBlt(DC,X + R.left,Y + R.top,R.right - R.left,R.bottom - R.top,FHandle,R.left,R.top,SRCCOPY);
  end;
end;

function TD2CanvasD2D.BeginScene:boolean;
begin
  CreateResources;
  Result:=inherited BeginScene and (RenderTarget <> nil);
  if Result then
  begin
    FLastState:=-1;
    if (FParent <> 0) and not (FBuffered) then
      Result:=(LongWord(ID2D1HwndRenderTarget(RenderTarget).CheckWindowState) and LongWord(D2D1_WINDOW_STATE_OCCLUDED)=0);
    RenderTarget.BeginDraw;
  end;
end;

procedure TD2CanvasD2D.EndScene;
var
  Res:HResult;
begin
  inherited ;
  if RenderTarget <> nil then
  begin
    if Assigned(Layer) then
    begin
      RenderTarget.PopLayer;
      SAFE_RELEASE(Layer);
    end;
    Res:=RenderTarget.EndDraw;
    if Res=D2DERR_RECREATE_TARGET then
    begin
      DisposeResources;
    end
    else
      if Res <> 0 then
      begin
        //Res:=10;
      end;
    if WICBitmap <> nil then
    begin
      Res:=WICBitmap.CopyPixels(nil,Width * 4,Width * Height * 4,PByte(FBitmap.Startline));
    end;
  end;
end;

class function TD2CanvasD2D.GetBitmapScanline(Bitmap:TD2Bitmap; y:integer):PD2ColorArray;
begin
  if (y >= 0) and (y < Bitmap.Height) and (Bitmap.StartLine <> nil) then
    Result:=@PD2ColorArray(Bitmap.StartLine)[(y) * Bitmap.Width]
  else
    Result:=nil;
end;

procedure TD2CanvasD2D.SetMatrix(const M:TD2Matrix);
begin
  FMatrix:=M;
  RenderTarget.SetTransform(D2iMatrix(FMatrix));
end;

procedure TD2CanvasD2D.MultyMatrix(const M:TD2Matrix);
// var T:TD2Matrix;
begin
  inherited;
{  RenderTarget.GetTransform(D2D_MATRIX_3X2_F(T));
  M:=d2MatrixMultipy(T,M);
  RenderTarget.SetTransform(D2iMatrix(T));}
end;

function TD2CanvasD2D.SaveCanvas:cardinal;
var
  i:integer;
  B:ID2D1DrawingStateBlock;
begin
  Result:=InvalideCanvasState;
  if RenderTarget=nil then Exit;

  if Length(FSaveData) > 0 then
    for i:=0 to High(FSaveData) do
      if FSaveData[i].Index=InvalideCanvasState then
      begin
        Result:=i;
        Break;
      end;

  if Result=InvalideCanvasState then
  begin
    SetLength(FSaveData,Length(FSaveData) + 1);
    Result:=High(FSaveData);
  end;

  Factory.CreateDrawingStateBlock(nil,nil,B);
  B._AddRef;
  RenderTarget.SaveDrawingState(B);

  FLastState:=Result;
  FSaveData[Result].Index:=Cardinal(B);
  FSaveData[Result].Matrix:=FMatrix;
  FSaveData[Result].Stroke:=TD2Brush.Create(d2BrushSolid,InvalideCanvasState);
  FSaveData[Result].Stroke.Assign(Stroke);
  FSaveData[Result].Fill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FSaveData[Result].Fill.Assign(Fill);
  FSaveData[Result].StrokeThickness:=FStrokeThickness;
  FSaveData[Result].StrokeCap:=StrokeCap;
  FSaveData[Result].StrokeJoin:=StrokeJoin;
  FSaveData[Result].StrokeDash:=StrokeDash;

  if StrokeDash <> d2DashSolid then
  begin
    SetLength(FSaveData[Result].Dash,Length(FDash));
    Move(FDash[0],FSaveData[Result].Dash[0],SizeOf(FDash[0]) * Length(FDash));
  end;

  FSaveData[Result].DashOffset:=FDashOffset;
  FSaveData[Result].Font:=TD2Font.Create;
  FSaveData[Result].Font.Assign(Font);
  FSaveData[Result].Data:=nil;
end;

procedure TD2CanvasD2D.RestoreCanvas(const AState:cardinal);
var
  B:ID2D1DrawingStateBlock;
begin
  if RenderTarget=nil then Exit;

  if (AState >= 0) and (AState < Length(FSaveData)) then
  begin
    B:=ID2D1DrawingStateBlock(FSaveData[AState].Index);
    RenderTarget.RestoreDrawingState(B);
    B._Release;
    SAFE_RELEASE(B);

    if FSaveData[AState].Data <> nil then
    begin
      RenderTarget.PopLayer;
      SAFE_RELEASE(ID2D1Layer(FSaveData[AState].Data));
      FSaveData[AState].Data:=nil;
    end;

    FSaveData[AState].Index:=InvalideCanvasState;

    FMatrix:=FSaveData[AState].Matrix;
    Stroke.Assign(FSaveData[AState].Stroke);
    FSaveData[AState].Stroke.Free;
    Fill.Assign(FSaveData[AState].Fill);
    FSaveData[AState].Fill.Free;
    SetMatrix(FMatrix);
    FStrokeThickness:=FSaveData[AState].StrokeThickness;
    FStrokeCap:=FSaveData[AState].StrokeCap;
    FStrokeJoin:=FSaveData[AState].StrokeJoin;
    FStrokeDash:=FSaveData[AState].StrokeDash;

    if StrokeDash <> d2DashSolid then
    begin
      SetLength(FDash,Length(FSaveData[AState].Dash));
      Move(FSaveData[AState].Dash[0],FDash[0],SizeOf(FDash[0]) * Length(FDash));
    end;

    FDashOffset:=FSaveData[AState].DashOffset;
    Font.Assign(FSaveData[AState].Font);
    FSaveData[AState].Font.Free;
  end
end;

procedure TD2CanvasD2D.SetClipRects(const ARects:array of TD2Rect);
var
  i:integer;
  Geoms:array of ID2D1Geometry;
  FClipGeom:ID2D1GeometryGroup;
  LayerPar:TD2D1_LayerParameters;
  R:TD2Rect;
begin

  if Length(ARects) > 0 then
  begin
    RenderTarget.SetTransform(D2iMatrix(IdentityMatrix));

    for i:=0 to High(ARects) do
    begin
      R:=ARects[i];
      RenderTarget.PushAxisAlignedClip(D2iRect(R),D2D1_ANTIALIAS_MODE_ALIASED);
      RenderTarget.Clear(D2iColor(0,0));
      RenderTarget.PopAxisAlignedClip;
    end;

    SAFE_RELEASE(FClipGeom);
    SetLength(Geoms,Length(ARects));

    for i:=0 to High(ARects) do
    begin
      Factory.CreateRectangleGeometry(D2iRect(ARects[i]),ID2D1RectangleGeometry(Geoms[i]));
    end;

    Factory.CreateGeometryGroup(D2D1_FILL_MODE_WINDING,@Geoms[0],Length(Geoms),FClipGeom);

    for i:=0 to High(ARects) do
    begin
      SAFE_RELEASE(Geoms[i]);
    end;

    RenderTarget.CreateLayer(nil,Layer);
    LayerPar.ContentBounds:=D2iRect(d2Rect(0,0,FWidth,FHeight));
    LayerPar.GeometricMask:=FClipGeom;
    LayerPar.MaskAntialiasMode:=D2D1_ANTIALIAS_MODE_ALIASED;
    LayerPar.MaskTransform:=D2iMatrix(IdentityMatrix);
    LayerPar.Opacity:=1;
    LayerPar.OpacityBrush:=nil;
    LayerPar.LayerOptions:=D2D1_LAYER_OPTIONS_NONE;
    RenderTarget.PushLayer(LayerPar,Layer);
    SAFE_RELEASE(FClipGeom);
  end;

end;

procedure TD2CanvasD2D.IntersectClipRect(const ARect:TD2Rect);
var
  Geom:ID2D1Geometry;
  LayerPar:TD2D1_LayerParameters;
begin
  if FLastState < 0 then Exit;
  Factory.CreateRectangleGeometry(D2iRect(ARect),ID2D1RectangleGeometry(Geom));

  if Assigned(Geom) then
  begin
    RenderTarget.CreateLayer(nil,ID2D1Layer(FSaveData[FLastState].Data));
    LayerPar.ContentBounds:=D2iRect(d2Rect(0,0,100000,400000));
    LayerPar.GeometricMask:=Geom;
    LayerPar.MaskAntialiasMode:=D2D1_ANTIALIAS_MODE_ALIASED;
    LayerPar.MaskTransform:=D2iMatrix(IdentityMatrix);
    LayerPar.Opacity:=1;
    LayerPar.OpacityBrush:=nil;
    LayerPar.LayerOptions:=D2D1_LAYER_OPTIONS_NONE;
    RenderTarget.PushLayer(LayerPar,ID2D1Layer(FSaveData[FLastState].Data));
    SAFE_RELEASE(Geom);
  end;

end;

procedure TD2CanvasD2D.ExcludeClipRect(const ARect:TD2Rect);
var
  i:integer;
  Geoms:array [0..3] of ID2D1Geometry;
  R:TD2Rect;
  RR:array [0..3] of TD2Rect;
  GeomGroup:ID2D1GeometryGroup;
  LayerPar:TD2D1_LayerParameters;
begin
  if FLastState < 0 then Exit;

  R:=ARect;
  RR[0]:=d2Rect(0,0,R.Left,FHeight);
  RR[1]:=d2Rect(R.Right,0,FWidth,FHeight);
  RR[2]:=d2Rect(R.Left,0,R.Right,R.Top);
  RR[3]:=d2Rect(R.Left,R.Bottom,R.Right,FHeight);

  for i:=0 to High(RR) do
  begin
    Factory.CreateRectangleGeometry(D2iRect(RR[i]),ID2D1RectangleGeometry(Geoms[i]));
  end;

  Factory.CreateGeometryGroup(D2D1_FILL_MODE_WINDING,@Geoms[0],Length(Geoms),GeomGroup);

  for i:=0 to High(RR) do
  begin
    SAFE_RELEASE(Geoms[i]);
  end;

  if Assigned(GeomGroup) then
  begin
    RenderTarget.CreateLayer(nil,ID2D1Layer(FSaveData[FLastState].Data));
    LayerPar.ContentBounds:=D2iRect(d2Rect(0,0,FWidth,FHeight));
    LayerPar.GeometricMask:=GeomGroup;
    LayerPar.MaskAntialiasMode:=D2D1_ANTIALIAS_MODE_ALIASED;
    LayerPar.MaskTransform:=D2iMatrix(IdentityMatrix);
    LayerPar.Opacity:=1;
    LayerPar.OpacityBrush:=nil;
    LayerPar.LayerOptions:=D2D1_LAYER_OPTIONS_NONE;
    RenderTarget.PushLayer(LayerPar,ID2D1Layer(FSaveData[FLastState].Data));
    SAFE_RELEASE(GeomGroup);
  end;
end;

procedure TD2CanvasD2D.ResetClipRect;
begin
  //....
end;

procedure TD2CanvasD2D.ApplyFill(ARect:TD2Rect; const AOpacity:single);
var
  i:integer;
  count:integer;
  B:TD2Bitmap;
  M:TD2Matrix;
  grad:array [0..100] of TD2D1_GradientStop;
  gradcol:ID2D1GradientStopCollection;
  gradbrushprop:TD2D1_LinearGradientBrushProperties;
  rgradbrushprop:TD2D1_RadialGradientBrushProperties;
  bitmapbrushprop:TD2D1_BitmapBrushProperties;
  brushprop:TD2D1_BrushProperties;
  d2dbmp:ID2D1Bitmap;
begin
  SAFE_RELEASE(Brush);

  if (FFill.Style=d2BrushResource) and (FFill.Resource <> nil) and (FFill.Resource.Brush <> nil) then
    FFill.Assign(FFill.Resource.Brush);

  with FFill do
  begin

    case Style of
      d2BrushSolid:
        begin
          RenderTarget.CreateSolidColorBrush(D2iColor(FFill.SolidColor,AOpacity),nil,ID2D1SolidColorBrush(Brush));
        end;

      d2BrushGradient:
        begin
          if Gradient.Points.Count > 1 then
          begin
            count:=0;
            if Gradient.Points[0].Offset > 0 then
            begin
              grad[count].Color:=D2iColor(d2Opacity(Gradient.Points[0].IntColor,AOpacity),1);
              grad[count].Position:=0;
              count:=count + 1;
            end;
            for i:=0 to Gradient.Points.Count - 1 do
            begin
              grad[i + count].Color:=D2iColor(d2Opacity(Gradient.Points[i].IntColor,AOpacity),1);
              grad[i + count].Position:=Gradient.Points[i].Offset;
            end;
            if Gradient.Points[Gradient.Points.Count - 1].Offset < 1 then
            begin
              count:=count + 1;
              grad[Gradient.Points.Count + count - 1].Color:=D2iColor(d2Opacity(Gradient.Points[Gradient.Points.Count - 1].IntColor,AOpacity),1);
              grad[Gradient.Points.Count + count - 1].Position:=1;
            end;

            if Gradient.Style=d2LinearGradient then
            begin
              RenderTarget.CreateGradientStopCollection(@grad[0],Gradient.Points.Count + count,D2D1_GAMMA_2_2,D2D1_EXTEND_MODE_CLAMP,gradcol);
              gradbrushprop.StartPoint:=D2iPoint(ARect.Left + Gradient.StartPosition.X * ARect.Right,ARect.Top + Gradient.StartPosition.Y * ARect.Bottom);
              gradbrushprop.EndPoint:=D2iPoint(ARect.Left + Gradient.StopPosition.X * ARect.Right,ARect.Top + Gradient.StopPosition.Y * ARect.Bottom);
              RenderTarget.CreateLinearGradientBrush(gradbrushprop,nil,gradcol,ID2D1LinearGradientBrush(Brush));
              gradcol:=nil;
            end
            else
            begin
              for i:=0 to Gradient.Points.Count + count - 1 do
                grad[i].Position:=1 - grad[i].Position;

              RenderTarget.CreateGradientStopCollection(@grad[0],Gradient.Points.Count + count,D2D1_GAMMA_2_2,D2D1_EXTEND_MODE_CLAMP,gradcol);
              rgradbrushprop.GradientOriginOffset:=TD2D1_Point2F(d2Point(0,0));
              rgradbrushprop.Center:=TD2D1_Point2F(d2Point(Gradient.RadialTransform.RotateCenter.X * d2RectWidth(ARect),
                Gradient.RadialTransform.RotateCenter.Y * d2RectHeight(ARect)));
              rgradbrushprop.RadiusX:=d2RectWidth(ARect) / 2;
              rgradbrushprop.RadiusY:=d2RectHeight(ARect) / 2;
              RenderTarget.CreateRadialGradientBrush(rgradbrushprop,nil,gradcol,ID2D1RadialGradientBrush(Brush) );
              gradcol:=nil;
            end;
          end
          else
            RenderTarget.CreateSolidColorBrush(D2iColor(0,0),nil,ID2D1SolidColorBrush(Brush));
        end;

      d2BrushResource:
        begin
        end;

      d2BrushVisual:
        begin
        end;

      d2BrushBitmap:
        begin
          B:=Bitmap.Bitmap;
          if (B <> nil) and (B.ResourceBitmap <> nil) then
            B:=B.ResourceBitmap;
          if (B <> nil) and (B.Width > 0) and (B.Height > 0) then
          begin
            bitmapbrushprop.InterpolationMode:=D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
            brushprop.Opacity:=AOpacity;
            M:=IdentityMatrix;
            case Bitmap.WrapMode of
              d2WrapTile:
                begin
                  bitmapbrushprop.ExtendModeX:=D2D1_EXTEND_MODE_WRAP;
                  bitmapbrushprop.ExtendModeY:=D2D1_EXTEND_MODE_WRAP;
                end;
              d2WrapTileOriginal:
                begin
                  bitmapbrushprop.ExtendModeX:=D2D1_EXTEND_MODE_CLAMP;
                  bitmapbrushprop.ExtendModeY:=D2D1_EXTEND_MODE_CLAMP;
                end;
              d2WrapTileStretch:
                begin
                  bitmapbrushprop.ExtendModeX:=D2D1_EXTEND_MODE_WRAP;
                  bitmapbrushprop.ExtendModeY:=D2D1_EXTEND_MODE_WRAP;
                  M.m11:=(d2RectWidth(ARect) + (StrokeThickness / 2)) / B.Width;
                  M.m22:=(d2RectHeight(ARect) + (StrokeThickness / 2)) / B.Height;
                end;
            end;
            brushprop.Transform:=D2iMatrix(M);
            Res:=RenderTarget.CreateBitmap(D2iSize(B.Width,B.Height),B.StartLine,
              B.Width * 4,BitmapProp(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),d2dbmp);
            RenderTarget.CreateBitmapBrush(
              d2dbmp,
              @bitmapbrushprop,
              @brushprop,
              ID2D1BitmapBrush(Brush));
          end
          else
            RenderTarget.CreateSolidColorBrush(D2iColor(0,0),nil,ID2D1SolidColorBrush(Brush));
        end;

    else
      RenderTarget.CreateSolidColorBrush(D2iColor(0,0),nil,ID2D1SolidColorBrush(Brush));
    end;
  end;
end;

procedure TD2CanvasD2D.ApplyStroke(ARect:TD2Rect; const AOpacity:single);
var
  StyleProp:TD2D1_StrokeStyleProperties;
begin
  SAFE_RELEASE(StrokeBrush);
  SAFE_RELEASE(StrokeStyle);

  if (FStroke.Style=d2BrushResource) and
     (FStroke.Resource <> nil) and
     (FStroke.Resource.Brush <> nil) then
       FStroke.Assign(FStroke.Resource.Brush);

  with FStroke do
  begin

    case Style of
      d2BrushSolid,d2BrushGradient,d2BrushBitmap:
        begin
          RenderTarget.CreateSolidColorBrush(D2iColor(FStroke.SolidColor,AOpacity),nil,ID2D1SolidColorBrush(StrokeBrush));
        end;

    else
      RenderTarget.CreateSolidColorBrush(D2iColor(0,0),nil,ID2D1SolidColorBrush(StrokeBrush));
    end;
  end;

  case StrokeCap of
    d2CapFlat:StyleProp.DashCap:=D2D1_CAP_STYLE_SQUARE;
    d2CapRound:StyleProp.DashCap:=D2D1_CAP_STYLE_ROUND;
  end;

  StyleProp.StartCap:=StyleProp.DashCap;
  StyleProp.EndCap:=StyleProp.DashCap;

  case StrokeJoin of
    d2JoinMiter:StyleProp.LineJoin:=D2D1_LINE_JOIN_MITER;
    d2JoinRound:StyleProp.LineJoin:=D2D1_LINE_JOIN_ROUND;
    d2JoinBevel:StyleProp.LineJoin:=D2D1_LINE_JOIN_BEVEL;
  end;

  StyleProp.MiterLimit:=10;
  StyleProp.DashOffset:=FDashOffset;
  StyleProp.DashStyle:=TD2D1_DashStyle(StrokeDash);

  if StrokeDash=d2DashCustom then
    Factory.CreateStrokeStyle(StyleProp,@FDash[0],Length(FDash),StrokeStyle) else
    Factory.CreateStrokeStyle(StyleProp,nil,0,StrokeStyle);

end;

procedure TD2CanvasD2D.FontChanged(Sender:TObject);
begin
  //....
end;

procedure TD2CanvasD2D.DrawLine(const APt1,APt2:TD2Point; const AOpacity:single);
begin
  if FStroke.Style <> d2BrushNone then
  begin
    ApplyStroke(d2Rect(APt1.X,APt1.Y,APt2.X,APt2.Y),AOpacity);
    RenderTarget.DrawLine(D2iPoint(APt1.X,APt1.Y),D2iPoint(APt2.X,APt2.Y),StrokeBrush,StrokeThickness,StrokeStyle);
  end;
end;

procedure TD2CanvasD2D.IntFillRect(R:TD2Rect; Opacity:single);
begin
  if FFill.Style <> d2BrushNone then
  begin

    if FFill.Style=d2BrushVisual then
    begin end
    else
    begin
      ApplyFill(R,Opacity);
      RenderTarget.FillRectangle(D2iRect(R),Brush);
    end;

  end;
end;

procedure TD2CanvasD2D.IntFillPath(P:ID2D1Geometry; R:TD2Rect;
  Opacity:single);
begin
  if FFill.Style <> d2BrushNone then
  begin

    if FFill.Style=d2BrushVisual then
    begin end
    else
    begin
      ApplyFill(R,Opacity);
      RenderTarget.FillGeometry(P,Brush);
    end;

  end;
end;

procedure TD2CanvasD2D.DrawRect(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners;
                                const AOpacity:single; const ACornerType:TD2CornerType=d2CornerRound);
var
  Geometry:ID2D1PathGeometry;
  Path:ID2D1GeometrySink;
  x1,x2,y1,y2:single;
  R:TD2Rect;
begin
  if FStroke.Style <> d2BrushNone then
  begin
    R:=ARect;
    ApplyStroke(R,AOpacity);

    if (xRadius < Epsilon) and (yRadius < Epsilon) then
    begin
      RenderTarget.DrawRectangle(D2iRect(ARect),StrokeBrush,StrokeThickness,StrokeStyle);
    end else
    begin
      x1:=xRadius;

      if d2RectWidth(R) - (x1 * 2) < 0 then
        x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));

      x2:=x1 * CurveKappaInv;
      y1:=yRadius;

      if d2RectHeight(R) - (y1 * 2) < 0 then
        y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));

      y2:=y1 * CurveKappaInv;
      Factory.CreatePathGeometry(Geometry);
      Geometry.Open(Path);
      Path.BeginFigure(D2iPoint(R.Left,R.Top + y1),D2D1_FIGURE_BEGIN_FILLED);

      if d2CornerTopLeft in ACorners then
      begin

        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Left + x1,R.Top));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Left + x2,R.Top + y1,R.Left + x1,R.Top + y2,R.Left + x1,R.Top));
          d2CornerInnerLine :begin
                               Path.AddLine(D2iPoint(R.Left + x2,R.Top + y1));
                               Path.AddLine(D2iPoint(R.Left + x1,R.Top + y2));
                               Path.AddLine(D2iPoint(R.Left + x1,R.Top));
                             end;
        else
          Path.AddBezier(D2iBezier(R.Left,R.Top + (y2),R.Left + x2,R.Top,R.Left + x1,R.Top))
        end;

      end else
      begin
        Path.AddLine(D2iPoint(R.Left,R.Top));
        Path.AddLine(D2iPoint(R.Left + x1,R.Top));
      end;

      Path.AddLine(D2iPoint(R.Right - x1,R.Top));

      if d2CornerTopRight in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Right,R.Top + y1));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Right - x1,R.Top + y2,R.Right - x2,R.Top + y1,R.Right,R.Top + y1));
          d2CornerInnerLine :begin
                               Path.AddLine(D2iPoint(R.Right - x1,R.Top + y2));
                               Path.AddLine(D2iPoint(R.Right - x2,R.Top + y1));
                               Path.AddLine(D2iPoint(R.Right,R.Top + y1));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Right - x2,R.Top,R.Right,R.Top + (y2),R.Right,R.Top + y1))
        end;

      end else
      begin
        Path.AddLine(D2iPoint(R.Right,R.Top));
        Path.AddLine(D2iPoint(R.Right,R.Top + y1));
      end;

      Path.AddLine(D2iPoint(R.Right,R.Bottom - y1));

      if d2CornerBottomRight in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Right - x2,R.Bottom - y1,R.Right - x1,R.Bottom - y2,R.Right - x1,R.Bottom));
          d2CornerInnerLine :begin
                                Path.AddLine(D2iPoint(R.Right - x2,R.Bottom - y1));
                                Path.AddLine(D2iPoint(R.Right - x1,R.Bottom - y2));
                                Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Right,R.Bottom - (y2),R.Right - x2,R.Bottom,R.Right - x1,R.Bottom))
        end;

      end else
      begin
        Path.AddLine(D2iPoint(R.Right,R.Bottom));
        Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
      end;

      Path.AddLine(D2iPoint(R.Left + x1,R.Bottom));

      if d2CornerBottomLeft in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Left + x1,R.Bottom - y2,R.Left + x2,R.Bottom - y1,R.Left,R.Bottom - y1));
          d2CornerInnerLine :begin
                                Path.AddLine(D2iPoint(R.Left + x1,R.Bottom - y2));
                                Path.AddLine(D2iPoint(R.Left + x2,R.Bottom - y1));
                                Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Left + x2,R.Bottom,R.Left,R.Bottom - (y2),R.Left,R.Bottom - y1))
        end;
      end else
      begin
        Path.AddLine(D2iPoint(R.Left,R.Bottom));
        Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
      end;

      Path.EndFigure(D2D1_FIGURE_END_CLOSED);
      Path.Close;
      SAFE_RELEASE(Path);
      RenderTarget.DrawGeometry(Geometry,StrokeBrush,StrokeThickness,StrokeStyle);
      SAFE_RELEASE(Geometry);
    end;
  end;
end;

procedure TD2CanvasD2D.FillRect(const ARect:TD2Rect; const xRadius,yRadius:single; const ACorners:TD2Corners; const AOpacity:single;
                                const ACornerType:TD2CornerType=d2CornerRound);
var
  Geometry:ID2D1PathGeometry;
  Path:ID2D1GeometrySink;
  x1,x2,y1,y2:single;
  R:TD2Rect;
begin
  if FFill.Style <> d2BrushNone then
  begin
    if ((xRadius=0) and (yRadius=0)) or (ACorners=[]) then
    begin
      IntFillRect(ARect,AOpacity);
    end else
    begin
      R:=ARect;
      x1:=xRadius;

      if d2RectWidth(R) - (x1 * 2) < 0 then
        x1:=(xRadius * (d2RectWidth(R) / (x1 * 2)));

      x2:=x1 * CurveKappaInv;
      y1:=yRadius;

      if d2RectHeight(R) - (y1 * 2) < 0 then
        y1:=(yRadius * (d2RectHeight(R) / (y1 * 2)));

      y2:=y1 * CurveKappaInv;
      Factory.CreatePathGeometry(Geometry);
      Geometry.Open(Path);
      Path.BeginFigure(D2iPoint(R.Left,R.Top + y1),D2D1_FIGURE_BEGIN_FILLED);

      if d2CornerTopLeft in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Left + x1,R.Top));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Left + x2,R.Top + y1,R.Left + x1,R.Top + y2,R.Left + x1,R.Top));
          d2CornerInnerLine :begin
                                Path.AddLine(D2iPoint(R.Left + x2,R.Top + y1));
                                Path.AddLine(D2iPoint(R.Left + x1,R.Top + y2));
                                Path.AddLine(D2iPoint(R.Left + x1,R.Top));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Left,R.Top + (y2),R.Left + x2,R.Top,R.Left + x1,R.Top))
        end;
      end
      else
      begin
        Path.AddLine(D2iPoint(R.Left,R.Top));
        Path.AddLine(D2iPoint(R.Left + x1,R.Top));
      end;
      Path.AddLine(D2iPoint(R.Right - x1,R.Top));
      if d2CornerTopRight in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Right,R.Top + y1));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Right - x1,R.Top + y2,R.Right - x2,R.Top + y1,R.Right,R.Top + y1));
          d2CornerInnerLine :begin
                                  Path.AddLine(D2iPoint(R.Right - x1,R.Top + y2));
                                  Path.AddLine(D2iPoint(R.Right - x2,R.Top + y1));
                                  Path.AddLine(D2iPoint(R.Right,R.Top + y1));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Right - x2,R.Top,R.Right,R.Top + (y2),R.Right,R.Top + y1))
        end;
      end
      else
      begin
        Path.AddLine(D2iPoint(R.Right,R.Top));
        Path.AddLine(D2iPoint(R.Right,R.Top + y1));
      end;
      Path.AddLine(D2iPoint(R.Right,R.Bottom - y1));
      if d2CornerBottomRight in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Right - x2,R.Bottom - y1,R.Right - x1,R.Bottom - y2,R.Right - x1,R.Bottom));
          d2CornerInnerLine :begin
                               Path.AddLine(D2iPoint(R.Right - x2,R.Bottom - y1));
                               Path.AddLine(D2iPoint(R.Right - x1,R.Bottom - y2));
                               Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Right,R.Bottom - (y2),R.Right - x2,R.Bottom,R.Right - x1,R.Bottom))
        end;
      end
      else
      begin
        Path.AddLine(D2iPoint(R.Right,R.Bottom));
        Path.AddLine(D2iPoint(R.Right - x1,R.Bottom));
      end;
      Path.AddLine(D2iPoint(R.Left + x1,R.Bottom));
      if d2CornerBottomLeft in ACorners then
      begin
        case ACornerType of
          d2CornerBevel     :Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
          d2CornerInnerRound:Path.AddBezier(D2iBezier(R.Left + x1,R.Bottom - y2,R.Left + x2,R.Bottom - y1,R.Left,R.Bottom - y1));
          d2CornerInnerLine :begin
                                Path.AddLine(D2iPoint(R.Left + x1,R.Bottom - y2));
                                Path.AddLine(D2iPoint(R.Left + x2,R.Bottom - y1));
                                Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
                              end;
        else
          Path.AddBezier(D2iBezier(R.Left + x2,R.Bottom,R.Left,R.Bottom - (y2),R.Left,R.Bottom - y1))
        end;
      end
      else
      begin
        Path.AddLine(D2iPoint(R.Left,R.Bottom));
        Path.AddLine(D2iPoint(R.Left,R.Bottom - y1));
      end;

      Path.EndFigure(D2D1_FIGURE_END_CLOSED);
      Path.Close;
      SAFE_RELEASE(Path);
      IntFillPath(Geometry,ARect,AOpacity);
      SAFE_RELEASE(Geometry);
    end;
  end;
end;

procedure TD2CanvasD2D.DrawEllipse(const ARect:TD2Rect; const AOpacity:single);
//var
//  R:TD2Rect;
begin
  if FStroke.Style <> d2BrushNone then
  begin
    ApplyStroke(ARect,AOpacity);
    RenderTarget.DrawEllipse(D2iEllipse(ARect),StrokeBrush,StrokeThickness,StrokeStyle);
  end;
end;

procedure TD2CanvasD2D.FillEllipse(const ARect:TD2Rect; const AOpacity:single);
var
  Geometry:ID2D1EllipseGeometry;
begin
  if FFill.Style <> d2BrushNone then
  begin
    if FFill.Style <> d2BrushVisual then
    begin
      ApplyFill(ARect,AOpacity);
      RenderTarget.FillEllipse(D2iEllipse(ARect),Brush);
    end
    else
    begin
      Factory.CreateEllipseGeometry(D2iEllipse(ARect),Geometry);
      IntFillPath(Geometry,ARect,AOpacity);
      SAFE_RELEASE(Geometry);
    end;
  end;
end;

procedure TD2CanvasD2D.FillText(const ARect,AClipRect:TD2Rect; const AText:WideString; const WordWrap:boolean;
  const AOpacity:single; const ATextAlign:TD2TextAlign; const AVTextAlign:TD2TextAlign=d2TextAlignCenter);
var
  TextRange:TDWrite_TextRange;
  TextLayout:IDWriteTextLayout;
  TextFormat:IDWriteTextFormat;
  //R:TD2Rect;
  //S:TD2FontStyle;
  WS:WideString;
begin
  if (FFill.Style <> d2BrushNone) and (AText <> '') then
  begin
    WS:=FFont.Family;
    DWriteFactory.CreateTextFormat(PWideChar(WS),nil,D2iFontWeight(FFont.Style),
                                   D2iFontStyle(FFont.Style),DWRITE_FONT_STRETCH_NORMAL,FFont.Size,'en-us',TextFormat);

    DWriteFactory.CreateTextLayout(PWideChar(AText),Length(AText),TextFormat,d2RectWidth(ARect),d2RectHeight(ARect),TextLayout);

    TextRange.startPosition:=0;
    TextRange.length:=Length(AText);

    if not WordWrap then
      TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP)  else
      TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);

    if FFont.Style=d2FontStrikeout then
      TextLayout.SetStrikethrough(True,TextRange);

    if FFont.Style=d2FontUnderline then
      TextLayout.SetUnderline(True,TextRange);


    case AVTextAlign of
      d2TextAlignCenter:TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
      d2TextAlignNear:  TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
      d2TextAlignFar:   TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    end;

    case ATextAlign of
      d2TextAlignCenter:TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
      d2TextAlignNear:  TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
      d2TextAlignFar:   TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    end;

    ApplyFill(ARect,AOpacity);

    RenderTarget.DrawTextLayout(D2iPoint(ARect.Left,ARect.Top),TextLayout,Brush,D2D1_DRAW_TEXT_OPTIONS_CLIP);
    SAFE_RELEASE(TextFormat);
    SAFE_RELEASE(TextLayout);
  end;
end;

procedure TD2CanvasD2D.MeasureText(var ARect:TD2Rect;
  AClipRect:TD2Rect; const AText:WideString; const WordWrap:boolean;
  const ATextAlign,AVTextAlign:TD2TextAlign);
var
  TextRange:TDWrite_TextRange;
  TextLayout:IDWriteTextLayout;
  TextMetrics:TDWrite_TextMetrics;
  TextFormat:IDWriteTextFormat;
  //R:TD2Rect;
  //S:TD2FontStyle;
  WS:WideString;
begin
  if not WordWrap then
    ARect.Right:=ARect.Left;
  if Length(AText)=0 then Exit;

  WS:=FFont.Family;
  DWriteFactory.CreateTextFormat(PWideChar(WS),nil,D2iFontWeight(FFont.Style),
    D2iFontStyle(FFont.Style),DWRITE_FONT_STRETCH_NORMAL,FFont.Size,'en-us',TextFormat);

  DWriteFactory.CreateTextLayout(PWideChar(AText),Length(AText),TextFormat,d2RectWidth(ARect),d2RectHeight(ARect),TextLayout);

  TextRange.startPosition:=0;
  TextRange.length:=Length(AText);

  if not WordWrap then
    TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP)
  else
    TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);

  if FFont.Style=d2FontStrikeout then
    TextLayout.SetStrikethrough(True,TextRange);

  if FFont.Style=d2FontUnderline then
    TextLayout.SetUnderline(True,TextRange);

  case AVTextAlign of
    d2TextAlignCenter:TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    d2TextAlignNear: TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    d2TextAlignFar:  TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
  end;

  case ATextAlign of
    d2TextAlignCenter:TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    d2TextAlignNear: TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    d2TextAlignFar:  TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
  end;

  TextLayout.GetMetrics(TextMetrics);
  ARect:=d2Rect( ARect.Left + TextMetrics.Left,
                   ARect.Top + TextMetrics.Top,
                   ARect.Left + TextMetrics.Left + TextMetrics.widthIncludingTrailingWhitespace,
                   ARect.Top + TextMetrics.Top + TextMetrics.height );

  SAFE_RELEASE(TextFormat);
  SAFE_RELEASE(TextLayout);
end;

procedure TD2CanvasD2D.DrawThumbnail(const ABitmap:TD2Bitmap; const Width,Height:single);
//var
//  xscale:single;
begin
//  UpdateBitmap(ABitmap);
  if ABitmap.Handle=0 then Exit;
  //xscale:=Width / ABitmap.Width;
  if FBitmap <> nil then
  begin
//    Fgraphics.ScaleTransform(xscale,xscale);
//    RenderTarget.DrawBitmap(ID2D1Bitmap(ABitmap.Handle),@DR,AOpacity,IntMode,@SR);
  end;
end;

procedure TD2CanvasD2D.DrawBitmap(const ABitmap:TD2Bitmap;
  const SrcRect,DstRect:TD2Rect; const AOpacity:single; const HighSpeed:boolean=false);
var
  SR,DR:TD2D1_RectF;
  IntMode:TD2D1_BitmapInterpolationMode;
  B:ID2D1Bitmap;
begin
  if HighSpeed then
    IntMode:=D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
  else
    IntMode:=D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

  SR:=D2iRect(SrcRect);
  DR:=D2iRect(DstRect);

  Res:=RenderTarget.CreateBitmap(D2iSize(ABitmap.Width,ABitmap.Height),ABitmap.StartLine,
    ABitmap.Width * 4,BitmapProp(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),B);
  RenderTarget.DrawBitmap(B,@DR,AOpacity,IntMode,@SR);
end;

procedure TD2CanvasD2D.DrawPath(const APath:TD2PathData; const ARect:TD2Rect; const AOpacity:single);
var
  i:integer;
  B:TD2Rect;
  W,H,NewW,NewH:single;
  CP1,CP2:TD2Point;
  Geometry:ID2D1PathGeometry;
  Path:ID2D1GeometrySink;
  Closed:boolean;
begin
  if FStroke.Style=d2BrushNone then Exit;
  if APath.IsEmpty then Exit;

  ApplyStroke(ARect,AOpacity);

  B:=APath.GetBounds;
  W:=d2RectWidth(B);
  H:=d2RectHeight(B);
  NewW:=d2RectWidth(ARect);
  NewH:=d2RectHeight(ARect);

  Factory.CreatePathGeometry(Geometry);
  Geometry.Open(Path);
  i:=0;
  Closed:=false;
  while i < Length(APath.PathData) do
  begin
    case APath.PathData[i].Kind of
      d2PathPointMoveTo:
        begin
          if (i > 0) and (APath.PathData[i - 1].Kind <> d2PathPointClose) then
            Path.EndFigure(D2D1_FIGURE_END_OPEN);
          Path.BeginFigure(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH),D2D1_FIGURE_BEGIN_FILLED);
        end;
      d2PathPointLineTo:
        begin
          Path.AddLine(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
        end;
      d2PathPointCurveTo:
        begin
          CP1:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
          Inc(i);
          CP2:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
          Inc(i);
          Path.AddBezier(D2iBezier(CP1.X,
            CP1.Y,
            CP2.X,
            CP2.Y,
            ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
        end;
      d2PathPointClose:
        begin
          Path.EndFigure(D2D1_FIGURE_END_CLOSED);
          Closed:=true;
        end;
    end;
    inc(i);
  end;

  if not Closed then
    Path.EndFigure(D2D1_FIGURE_END_OPEN);

  Res:=Path.Close;
  SAFE_RELEASE(Path);
  RenderTarget.DrawGeometry(Geometry,StrokeBrush,StrokeThickness,StrokeStyle);
  SAFE_RELEASE(Geometry);
end;

procedure TD2CanvasD2D.FillPath(const APath:TD2PathData; const ARect:TD2Rect; const AOpacity:single);
var
  i:integer;
  B:TD2Rect;
  W,H,NewW,NewH:single;
  CP1,CP2:TD2Point;
  Geometry:ID2D1PathGeometry;
  Path:ID2D1GeometrySink;
  Closed:boolean;
begin
//exit;
  if FFill.Style=d2BrushNone then Exit;
  if APath.IsEmpty then Exit;

  B:=APath.GetBounds;
  W:=d2RectWidth(B);
  H:=d2RectHeight(B);
  NewW:=d2RectWidth(ARect);
  NewH:=d2RectHeight(ARect);

  Factory.CreatePathGeometry(Geometry);
  Geometry.Open(Path);
  i:=0;
  Closed:=false;
  while i < Length(APath.PathData) do
  begin
    case APath.PathData[i].Kind of
      d2PathPointMoveTo:
        begin
          if (i > 0) and (APath.PathData[i - 1].Kind <> d2PathPointClose) then
            Path.EndFigure(D2D1_FIGURE_END_OPEN);
          Path.BeginFigure(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH),D2D1_FIGURE_BEGIN_FILLED);
        end;
      d2PathPointLineTo:
        begin
          Path.AddLine(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
        end;
      d2PathPointCurveTo:
        begin
          CP1:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
          Inc(i);
          CP2:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
          Inc(i);
          Path.AddBezier(D2iBezier(CP1.X,
            CP1.Y,
            CP2.X,
            CP2.Y,
            ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
            ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
        end;
      d2PathPointClose:
        begin
          Path.EndFigure(D2D1_FIGURE_END_CLOSED);
          Closed:=true;
        end;
    end;
    inc(i);
  end;
  if not Closed then
    Path.EndFigure(D2D1_FIGURE_END_OPEN);

  Path.Close;
  SAFE_RELEASE(Path);
  IntFillPath(Geometry,ARect,AOpacity);
  SAFE_RELEASE(Geometry);
end;

function TD2CanvasD2D.PtInPath(const APoint:TD2Point; const ARect:TD2Rect; const APath:TD2PathData):boolean;
var
  i:integer;
  B:TD2Rect;
  W,H,NewW,NewH:single;
  CP1,CP2:TD2Point;
  Geometry:ID2D1PathGeometry;
  Path:ID2D1GeometrySink;
  Cont:LongBool;
  Closed:boolean;
begin
  Result:=false;
  if not d2PtInRect(APoint,ARect) then
    Result:=false
  else
  begin
    if APath.IsEmpty then Exit;
    B:=APath.GetBounds;
    W:=d2RectWidth(B);
    H:=d2RectHeight(B);
    NewW:=d2RectWidth(ARect);
    NewH:=d2RectHeight(ARect);

    Factory.CreatePathGeometry(Geometry);
    Geometry.Open(Path);
    i:=0;
    Closed:=false;
    while i < Length(APath.PathData) do
    begin
      case APath.PathData[i].Kind of
        d2PathPointMoveTo:
          begin
            Path.BeginFigure(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
              ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH),D2D1_FIGURE_BEGIN_FILLED);
          end;
        d2PathPointLineTo:
          begin
            Path.AddLine(D2iPoint(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
              ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
          end;
        d2PathPointCurveTo:
          begin
            CP1:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
              ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
            Inc(i);
            CP2:=d2Point(ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
              ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH);
            Inc(i);
            Path.AddBezier(D2iBezier(CP1.X,
              CP1.Y,
              CP2.X,
              CP2.Y,
              ARect.Left + (APath.PathData[i].Point.X - B.Left) / W * NewW,
              ARect.Top + (APath.PathData[i].Point.Y - B.Top) / H * NewH));
          end;
        d2PathPointClose:
          begin
            Path.EndFigure(D2D1_FIGURE_END_CLOSED);
            Closed:=true;
          end;
      end;
      inc(i);
    end;
    if not Closed then
      Path.EndFigure(D2D1_FIGURE_END_OPEN);

    Path.Close;
    SAFE_RELEASE(Path);
    Geometry.FillContainsPoint(D2iPoint(APoint.X,APoint.Y),nil,1,Cont);
    SAFE_RELEASE(Geometry);

    Result:=Cont;
  end;
end;


//============= TD2Sink ==============================

type
  TD2Sink=class(TInterfacedPersistent,ID2D1SimplifiedGeometrySink)
  private
    Path:TD2PathData;
    procedure SetSegmentFlags(VertexFlags:TD2D1_PathSegment); stdcall;
    procedure SetFillMode(FillMode:TD2D1_FillMode); stdcall;
    procedure AddLines(Points:PTD2D1_Point2F; PointsCount:LongWord); stdcall;
    procedure AddBeziers(Beziers:PTD2D1_BezierSegment; BeziersCount:LongWord); stdcall;
    procedure BeginFigure(StartPoint:TD2D1_Point2F;FigureBegin:TD2D1_FigureBegin); stdcall;
    procedure EndFigure(FigureEnd:TD2D1_FigureEnd); stdcall;
    function Close:HResult; stdcall;
  end;


procedure TD2Sink.SetSegmentFlags(VertexFlags:TD2D1_PathSegment);
begin
  //.............
end;

procedure TD2Sink.SetFillMode(FillMode:TD2D1_FillMode);    stdcall;
begin
  //.............
end;

procedure TD2Sink.AddBeziers(Beziers:PTD2D1_BezierSegment; BeziersCount:LongWord); stdcall;
var
  i:integer;
begin
  for i:=0 to BeziersCount - 1 do
  begin
    Path.CurveTo(TD2Point(Beziers.Point1),TD2Point(Beziers.Point2),TD2Point(Beziers.Point3));
    Inc(Beziers);
  end;
end;

procedure TD2Sink.AddLines(Points:PTD2D1_Point2F; PointsCount:LongWord);  stdcall;
var
  i:integer;
begin
  for i:=0 to PointsCount - 1 do
  begin
    Path.LineTo(TD2Point(Points^));
    Inc(Points);
  end;
end;

procedure TD2Sink.BeginFigure(StartPoint:TD2D1_Point2F; FigureBegin:TD2D1_FigureBegin);  stdcall;
begin
  Path.MoveTo(TD2Point(StartPoint));
end;

procedure TD2Sink.EndFigure(FigureEnd:TD2D1_FigureEnd);  stdcall;
begin
  Path.ClosePath;
end;

function TD2Sink.Close:HResult;  stdcall;
begin
  Result:=S_OK;
end;


//============== TD2TextRendering ================================================
type

  TD2TextRendering=class(TInterfacedPersistent,IDWriteTextRenderer)
  private
    Canvas:TD2CanvasD2D;
    function IsPixelSnappingDisabled(ClientDrawingContext:Pointer; out IsDisabled:LongBool):HResult; stdcall;
    function GetCurrentTransform(ClientDrawingContext:Pointer; out Transform:TDWrite_Matrix):HResult; stdcall;
    function GetPixelsPerDip(ClientDrawingContext:Pointer; out PixelsPerDip:Single):HResult; stdcall;
    function DrawGlyphRun(ClientDrawingContext:Pointer;BaselineOriginX:Single; BaselineOriginY:Single;
                          MeasuringMode:TDWrite_MeasuringMode; const GlyphRun:TDWrite_GlyphRun;
                          const GlyphRunDescription:TDWrite_GlyphRunDescription;
                          ClientDrawingEffect:IUnknown):HResult; stdcall;
    function DrawUnderline(ClientDrawingContext:Pointer; BaselineOriginX:Single; BaselineOriginY:Single;
                           const Underline:TDWrite_Underline; ClientDrawingEffect:IUnknown):HResult; stdcall;
    function DrawStrikethrough(ClientDrawingContext:Pointer; BaselineOriginX:Single; BaselineOriginY:Single;
                               const Strikethrough:TDWrite_Strikethrough; ClientDrawingEffect:IUnknown):HResult; stdcall;
    function DrawInlineObject(ClientDrawingContext:Pointer; OriginX:Single; OriginY:Single; InlineObject:IDWriteInlineObject;
                              IsSideways:LongBool; IsRightToLeft:LongBool; ClientDrawingEffect:IUnknown):HResult; stdcall;
  end;


function TD2TextRendering.IsPixelSnappingDisabled(ClientDrawingContext:Pointer; out IsDisabled:LongBool):HResult; stdcall;
begin
  IsDisabled:=true;
  Result:=S_OK;
end;

function TD2TextRendering.GetPixelsPerDip(ClientDrawingContext:Pointer;
  out PixelsPerDip:single):HResult;
begin
  PixelsPerDip:=1;
  Result:=S_OK;
end;

function TD2TextRendering.GetCurrentTransform(ClientDrawingContext:Pointer; out Transform:TDWrite_Matrix):HResult; stdcall;
begin
  Transform:=TDWrite_Matrix(D2iMatrix(IdentityMatrix));
  Result:=S_OK;
end;

function TD2TextRendering.DrawGlyphRun(ClientDrawingContext:Pointer;BaselineOriginX:Single; BaselineOriginY:Single;
                          MeasuringMode:TDWrite_MeasuringMode; const GlyphRun:TDWrite_GlyphRun;
                          const GlyphRunDescription:TDWrite_GlyphRunDescription;
                          ClientDrawingEffect:IUnknown):HResult; stdcall;
var
  pPathGeometry:ID2D1PathGeometry;
  pSink:ID2D1GeometrySink;
  pSimSink:TD2Sink;
begin
  Res:=Factory.CreatePathGeometry(pPathGeometry);
  if (SUCCEEDED(Res)) then
    Res:=pPathGeometry.Open(pSink);
  if (SUCCEEDED(Res)) then
  begin
    Res:=glyphRun.fontFace.GetGlyphRunOutline(
        glyphRun.fontEmSize,
        glyphRun.glyphIndices,
        glyphRun.glyphAdvances,
        glyphRun.glyphOffsets,
        glyphRun.glyphCount,
        glyphRun.isSideways,
        false {glyphRun.bidiLevel},
        pSink
        );
  end;
  if (SUCCEEDED(Res)) then
    Res:=pSink.Close();

  pSimSink:=TD2Sink.Create;
  pSimSink.Path:=TD2PathData(ClientDrawingContext);
  Res:=pPathGeometry.Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES,nil,0,pSimSink);
  pSimSink.Close;
  pSimSink.Path.Offset(BaselineOriginX,BaselineOriginY);
  pSimSink.Free;

  SAFE_RELEASE(pSink);
  SAFE_RELEASE(pPathGeometry);
end;

function TD2TextRendering.DrawInlineObject(ClientDrawingContext:Pointer; OriginX:Single; OriginY:Single; InlineObject:IDWriteInlineObject;
                                           IsSideways:LongBool; IsRightToLeft:LongBool; ClientDrawingEffect:IUnknown):HResult; stdcall;
begin
  Result:=S_OK;
end;

function TD2TextRendering.DrawStrikethrough(ClientDrawingContext:Pointer; BaselineOriginX:Single; BaselineOriginY:Single;
                               const Strikethrough:TDWrite_Strikethrough; ClientDrawingEffect:IUnknown):HResult; stdcall;
begin
  Result:=S_OK;
end;

function TD2TextRendering.DrawUnderline(ClientDrawingContext:Pointer; BaselineOriginX:Single; BaselineOriginY:Single;
                           const Underline:TDWrite_Underline; ClientDrawingEffect:IUnknown):HResult; stdcall;
begin
  Result:=S_OK;
end;


//------------ TD2CanvasD2D ----------------------

function TD2CanvasD2D.TextToPath(Path:TD2PathData; const ARect:TD2Rect;
                      const AText:WideString; const WordWrap:boolean; const ATextAlign,
                      AVTextAlign:TD2TextAlign):boolean;
var
  TextRange:TDWrite_TextRange;
  TextLayout:IDWriteTextLayout;
  TextFormat:IDWriteTextFormat;
  //R:TD2Rect;
  //S:TD2FontStyle;
  WS:WideString;
  MyRenderer:TD2TextRendering;
begin
  if (AText <> '') then
  begin
    Path.Clear;

    WS:=FFont.Family;
    DWriteFactory.CreateTextFormat(PWideChar(WS),nil,D2iFontWeight(FFont.Style),
      D2iFontStyle(FFont.Style),DWRITE_FONT_STRETCH_NORMAL,FFont.Size,'en-us',TextFormat);

    DWriteFactory.CreateTextLayout(PWideChar(AText),Length(AText),
                                   TextFormat,d2RectWidth(ARect),d2RectHeight(ARect),TextLayout);

    TextRange.startPosition:=0;
    TextRange.length:=Length(AText);

    if not WordWrap then
      TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP) else
      TextLayout.SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);

    if FFont.Style=d2FontStrikeout then
      TextLayout.SetStrikethrough(True,TextRange);

    if FFont.Style=d2FontUnderline then
      TextLayout.SetUnderline(True,TextRange);

    case AVTextAlign of
      d2TextAlignCenter:TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
      d2TextAlignNear:  TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
      d2TextAlignFar:   TextLayout.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    end;

    case ATextAlign of
      d2TextAlignCenter: TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
      d2TextAlignNear:   TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
      d2TextAlignFar:    TextLayout.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    end;

    MyRenderer:=TD2TextRendering.Create;
    MyRenderer.Canvas:=Self;
    TextLayout.Draw(Path,MyRenderer,0,0);
    MyRenderer.Free;

    SAFE_RELEASE(TextFormat);
    SAFE_RELEASE(TextLayout);
  end;
end;    
     
                 
