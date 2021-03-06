{**********************************************************************
 Package pl_OpenGL.pkg
 This unit is part of CodeTyphon Studio (http://www.pilotlogic.com/)
***********************************************************************}

unit OpenGLPanel;

{$mode objfpc}{$H+}

{$I OpenGLPanel.inc}

interface

uses
  Classes, SysUtils, LCLProc, Forms, Controls, LCLType, LCLIntf, LResources,
  Graphics, LMessages, WSLCLClasses, WSControls,
{$IFDEF UseGtkGLX}
  OpenGLPanel_gtk;
{$ENDIF}
{$IFDEF UseGtk2GLX}
  OpenGLPanel_gtk;
{$ENDIF}
{$IFDEF UseCarbonAGL}
  OpenGLPanel_carbon;
{$ENDIF}
{$IFDEF UseCocoaNS}
  OpenGLPanel_cocoans;
{$ENDIF}    
{$IFDEF UseWin32WGL}
  OpenGLPanel_win;
{$ENDIF}
{$IFDEF UseQTGLX}
  OpenGLPanel_qt;
{$ENDIF}
{$IFDEF UseQT5GLX}
  OpenGLPanel_qt5;
{$ENDIF}

const
  DefaultDepthBits = 24;
  
type
  TOpenGlCtrlMakeCurrentEvent = procedure(Sender: TObject; var Allow: boolean) of object;

  TCustomOpenGLPanel = class(TWinControl)
  private
    FAutoResizeViewport: boolean;
    FCanvas: TCanvas; // only valid at designtime
    FDebugContext: boolean;
    //FDoubleBuffered: boolean;
    FFrameDiffTime: integer;
    FOnMakeCurrent: TOpenGlCtrlMakeCurrentEvent;
    FOnPaint: TNotifyEvent;
    FCurrentFrameTime: integer; // in msec
    FLastFrameTime: integer; // in msec
    fOpenGLMajorVersion: Cardinal;
    fOpenGLMinorVersion: Cardinal;
    FRGBA: boolean;
    {$IFDEF HasRGBBits}
    FRedBits, FGreenBits, FBlueBits,
    {$ENDIF}
    FMultiSampling, FAlphaBits, FDepthBits, FStencilBits, FAUXBuffers: Cardinal;
    FSharedOpenGLControl: TCustomOpenGLPanel;
    FSharingOpenGlControls: TList;
    function GetSharingControls(Index: integer): TCustomOpenGLPanel;
    procedure SetAutoResizeViewport(const AValue: boolean);
    procedure SetDebugContext(AValue: boolean);
    procedure SetDoubleBuffered(const AValue: boolean);
    procedure SetOpenGLMajorVersion(AValue: Cardinal);
    procedure SetOpenGLMinorVersion(AValue: Cardinal);
    procedure SetRGBA(const AValue: boolean);
    {$IFDEF HasRGBBits}
    procedure SetRedBits(const AValue: Cardinal);
    procedure SetGreenBits(const AValue: Cardinal);
    procedure SetBlueBits(const AValue: Cardinal);
    {$ENDIF}
    procedure SetMultiSampling(const AMultiSampling: Cardinal);
    procedure SetAlphaBits(const AValue: Cardinal);
    procedure SetDepthBits(const AValue: Cardinal);
    procedure SetStencilBits(const AValue: Cardinal);
    procedure SetAUXBuffers(const AValue: Cardinal);
    procedure SetSharedControl(const AValue: TCustomOpenGLPanel);
  protected
    procedure WMPaint(var Message: TLMPaint); message LM_PAINT;
    procedure WMSize(var Message: TLMSize); message LM_SIZE;
    procedure UpdateFrameTimeDiff;
    procedure OpenGLAttributesChanged;
  public
    constructor Create(TheOwner: TComponent); override;
    destructor Destroy; override;
    Procedure Paint; virtual;
    procedure RealizeBounds; override;
    procedure DoOnPaint; virtual;
    procedure SwapBuffers; virtual;
    function  MakeCurrent(SaveOldToStack: boolean = false): boolean; virtual;
    function  ReleaseContext: boolean; virtual;
    function  RestoreOldOpenGLControl: boolean;
    function  SharingControlCount: integer;
    property  SharingControls[Index: integer]: TCustomOpenGLPanel read GetSharingControls;
    procedure Invalidate; override;
    procedure EraseBackground(DC: HDC); override;
  public
    property FrameDiffTimeInMSecs: integer read FFrameDiffTime;
    property OnMakeCurrent: TOpenGlCtrlMakeCurrentEvent read FOnMakeCurrent  write FOnMakeCurrent;
    property OnPaint: TNotifyEvent read FOnPaint write FOnPaint;
    property SharedControl: TCustomOpenGLPanel read FSharedOpenGLControl write SetSharedControl;
    property AutoResizeViewport: boolean read FAutoResizeViewport  write SetAutoResizeViewport default false;
    property DoubleBuffered: boolean read FDoubleBuffered write SetDoubleBuffered default true;
    property DebugContext: boolean read FDebugContext write SetDebugContext default false; // create context with debugging enabled. Requires OpenGLMajorVersion!
    property RGBA: boolean read FRGBA write SetRGBA default true;
    {$IFDEF HasRGBBits}
    property RedBits: Cardinal read FRedBits write SetRedBits default 8;
    property GreenBits: Cardinal read FGreenBits write SetGreenBits default 8;
    property BlueBits: Cardinal read FBlueBits write SetBlueBits default 8;
    {$ENDIF}
    property OpenGLMajorVersion: Cardinal read fOpenGLMajorVersion write SetOpenGLMajorVersion default 0;
    property OpenGLMinorVersion: Cardinal read fOpenGLMinorVersion write SetOpenGLMinorVersion default 0;
    { Number of samples per pixel, for OpenGL multi-sampling (anti-aliasing).

      Value <= 1 means that we use 1 sample per pixel, which means no anti-aliasing.
      Higher values mean anti-aliasing. Exactly which values are supported
      depends on GPU, common modern GPUs support values like 2 and 4.
      
      If this is > 1, and we will not be able to create OpenGL
      with multi-sampling, we will fallback to normal non-multi-sampled context.
      You can query OpenGL values GL_SAMPLE_BUFFERS_ARB and GL_SAMPLES_ARB 
      (see ARB_multisample extension) to see how many samples have been 
      actually allocated for your context. }
    property MultiSampling: Cardinal read FMultiSampling write SetMultiSampling default 1;

    property AlphaBits: Cardinal read FAlphaBits write SetAlphaBits default 0;
    property DepthBits: Cardinal read FDepthBits write SetDepthBits default DefaultDepthBits;
    property StencilBits: Cardinal read FStencilBits write SetStencilBits default 0;
    property AUXBuffers: Cardinal read FAUXBuffers write SetAUXBuffers default 0;
  end;


  TOpenGLPanel = class(TCustomOpenGLPanel)
  published
    property Align;
    property Anchors;
    property AutoResizeViewport;
    property BorderSpacing;
    property Enabled;
    {$IFDEF HasRGBBits}
    property RedBits;
    property GreenBits;
    property BlueBits;
    {$ENDIF}
    property OpenGLMajorVersion;
    property OpenGLMinorVersion;
    property MultiSampling;
    property AlphaBits;
    property DepthBits;
    property StencilBits;
    property AUXBuffers;
    property OnChangeBounds;
    property OnClick;
    property OnConstrainedResize;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMakeCurrent;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnPaint;
    property OnResize;
    property OnShowHint;
    property PopupMenu;
    property ShowHint;
    property Visible;
  end;

  TWSOpenGLControl = class(TWSWinControl)
  published
    class function CreateHandle(const AWinControl: TWinControl;  const AParams: TCreateParams): HWND; override;
    class procedure DestroyHandle(const AWinControl: TWinControl); override;
  end;


implementation

var
  OpenGLControlStack: TList = nil;

//=========================== TCustomOpenGLPanel ====================

function TCustomOpenGLPanel.GetSharingControls(Index: integer): TCustomOpenGLPanel;
begin
  Result:=TCustomOpenGLPanel(FSharingOpenGlControls[Index]);
end;

procedure TCustomOpenGLPanel.SetAutoResizeViewport(const AValue: boolean);
begin
  if FAutoResizeViewport=AValue then exit;
  FAutoResizeViewport:=AValue;
  if AutoResizeViewport
  and ([csLoading,csDestroying]*ComponentState=[])
  and IsVisible and HandleAllocated
  and MakeCurrent then
    LOpenGLViewport(0,0,Width,Height);
end;

procedure TCustomOpenGLPanel.SetDebugContext(AValue: boolean);
begin
  if FDebugContext=AValue then Exit;
  FDebugContext:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetDoubleBuffered(const AValue: boolean);
begin
  if FDoubleBuffered=AValue then exit;
  FDoubleBuffered:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetOpenGLMajorVersion(AValue: Cardinal);
begin
  if fOpenGLMajorVersion=AValue then Exit;
  fOpenGLMajorVersion:=AValue;
end;

procedure TCustomOpenGLPanel.SetOpenGLMinorVersion(AValue: Cardinal);
begin
  if fOpenGLMinorVersion=AValue then Exit;
  fOpenGLMinorVersion:=AValue;
end;

procedure TCustomOpenGLPanel.SetRGBA(const AValue: boolean);
begin
  if FRGBA=AValue then exit;
  FRGBA:=AValue;
  OpenGLAttributesChanged;
end;

{$IFDEF HasRGBBits}
procedure TCustomOpenGLPanel.SetRedBits(const AValue: Cardinal);
begin
  if FRedBits=AValue then exit;
  FRedBits:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetGreenBits(const AValue: Cardinal);
begin
  if FGreenBits=AValue then exit;
  FGreenBits:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetBlueBits(const AValue: Cardinal);
begin
  if FBlueBits=AValue then exit;
  FBlueBits:=AValue;
  OpenGLAttributesChanged;
end;
{$ENDIF}

procedure TCustomOpenGLPanel.SetMultiSampling(const AMultiSampling: Cardinal);
begin
  if FMultiSampling=AMultiSampling then exit;
  FMultiSampling:=AMultiSampling;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetAlphaBits(const AValue: Cardinal);
begin
  if FAlphaBits=AValue then exit;
  FAlphaBits:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetDepthBits(const AValue: Cardinal);
begin
  if FDepthBits=AValue then exit;
  FDepthBits:=AValue;
  OpenGLAttributesChanged;
end;

procedure TCustomOpenGLPanel.SetStencilBits(const AValue: Cardinal);
begin
  if FStencilBits=AValue then exit;
  FStencilBits:=AValue;
  OpenGLAttributesChanged;
end;
procedure TCustomOpenGLPanel.SetAUXBuffers(const AValue: Cardinal);
begin
  if FAUXBuffers=AValue then exit;
  FAUXBuffers:=AValue;
  OpenGLAttributesChanged;
end;
procedure TCustomOpenGLPanel.SetSharedControl(const AValue: TCustomOpenGLPanel);
begin
  if FSharedOpenGLControl=AValue then exit;
  if AValue=Self then
    Raise Exception.Create('A control can not be shared by itself.');
  // unshare old
  if (AValue<>nil) and (AValue.SharedControl<>nil) then
    Raise Exception.Create('Target control is sharing too. A sharing control can not be shared.');
  if FSharedOpenGLControl<>nil then
    FSharedOpenGLControl.FSharingOpenGlControls.Remove(Self);
  // share new
  if (AValue<>nil) and (csDestroying in AValue.ComponentState) then
    FSharedOpenGLControl:=nil
  else begin
    FSharedOpenGLControl:=AValue;
    if (FSharedOpenGLControl<>nil) then begin
      if FSharedOpenGLControl.FSharingOpenGlControls=nil then
        FSharedOpenGLControl.FSharingOpenGlControls:=TList.Create;
      FSharedOpenGLControl.FSharingOpenGlControls.Add(Self);
    end;
  end;
  // recreate handle if needed
  if HandleAllocated and (not (csDesigning in ComponentState)) then
    ReCreateWnd(Self);
end;

procedure TCustomOpenGLPanel.WMPaint(var Message: TLMPaint);
begin
  Include(FControlState, csCustomPaint);
  inherited WMPaint(Message);
  //debugln('TCustomGTKGLAreaControl.WMPaint A ',dbgsName(Self),' ',dbgsName(FCanvas));
  if (csDesigning in ComponentState) and (FCanvas<>nil) then begin
    with FCanvas do begin
      if Message.DC <> 0 then
        Handle := Message.DC;
      Brush.Color:=clLtGray;
      Pen.Color:=clRed;
      Rectangle(0,0,Self.Width,Self.Height);
      MoveTo(0,0);
      LineTo(Self.Width,Self.Height);
      MoveTo(0,Self.Height);
      LineTo(Self.Width,0);
      if Message.DC <> 0 then
        Handle := 0;
    end;
  end else begin
    Paint;
  end;
  Exclude(FControlState, csCustomPaint);
end;

procedure TCustomOpenGLPanel.WMSize(var Message: TLMSize);
begin
  if (Message.SizeType and Size_SourceIsInterface)>0 then
    DoOnResize;
end;

procedure TCustomOpenGLPanel.UpdateFrameTimeDiff;
begin
  FCurrentFrameTime:=integer(GetTickCount);
  if FLastFrameTime=0 then
    FLastFrameTime:=FCurrentFrameTime;
  // calculate time since last call:
  FFrameDiffTime:=FCurrentFrameTime-FLastFrameTime;
  // if the counter is reset restart:
  if (FFrameDiffTime<0) then FFrameDiffTime:=1;
  FLastFrameTime:=FCurrentFrameTime;
end;

procedure TCustomOpenGLPanel.OpenGLAttributesChanged;
begin
  if HandleAllocated
  and ([csLoading,csDesigning,csDestroying]*ComponentState=[]) then
    RecreateWnd(Self);
end;

procedure TCustomOpenGLPanel.EraseBackground(DC: HDC);
begin
  if DC=0 then ;
  // everything is painted, so erasing the background is not needed
end;

constructor TCustomOpenGLPanel.Create(TheOwner: TComponent);
begin
  inherited Create(TheOwner);
  FDoubleBuffered:=true;
  FRGBA:=true;
  {$IFDEF HasRGBBits}
  FRedBits:=8;
  FGreenBits:=8;
  FBlueBits:=8;
  {$ENDIF}
  fOpenGLMajorVersion:=0;
  fOpenGLMinorVersion:=0;
  FMultiSampling:=1;
  FDepthBits:=DefaultDepthBits;
  ControlStyle:=ControlStyle-[csSetCaption];
  if (csDesigning in ComponentState) then begin
    FCanvas := TControlCanvas.Create;
    TControlCanvas(FCanvas).Control := Self;
  end else
    FCompStyle:=csNonLCL;
  SetInitialBounds(0, 0, 120, 120);
end;

destructor TCustomOpenGLPanel.Destroy;
begin
  if FSharingOpenGlControls<>nil then begin
    while SharingControlCount>0 do
      SharingControls[SharingControlCount-1].SharedControl:=nil;
    FreeAndNil(FSharingOpenGlControls);
  end;
  SharedControl:=nil;
  if OpenGLControlStack<>nil then begin
    OpenGLControlStack.Remove(Self);
    if OpenGLControlStack.Count=0 then
      FreeAndNil(OpenGLControlStack);
  end;
  FCanvas.Free;
  FCanvas:=nil;
  inherited Destroy;
end;

procedure TCustomOpenGLPanel.Paint;
begin
  if IsVisible and HandleAllocated then begin
    UpdateFrameTimeDiff;
    if ([csDesigning,csDestroying]*ComponentState=[]) then begin
      if not MakeCurrent then exit;
      if AutoResizeViewport then
        LOpenGLViewport(0,0,Width,Height);
    end;
    //LOpenGLClip(Handle);
    DoOnPaint;
  end;
end;

procedure TCustomOpenGLPanel.RealizeBounds;
begin
  if IsVisible and HandleAllocated
  and ([csDesigning,csDestroying]*ComponentState=[])
  and AutoResizeViewport then begin
    if MakeCurrent then
      LOpenGLViewport(0,0,Width,Height);
  end;
  inherited RealizeBounds;
end;

procedure TCustomOpenGLPanel.DoOnPaint;
begin
  if Assigned(OnPaint) then OnPaint(Self);
end;

procedure TCustomOpenGLPanel.SwapBuffers;
begin
  LOpenGLSwapBuffers(Handle);
end;

function TCustomOpenGLPanel.MakeCurrent(SaveOldToStack: boolean): boolean;
var
  Allowed: Boolean;
begin
  if csDesigning in ComponentState then exit(false);
  if Assigned(FOnMakeCurrent) then begin
    Allowed:=true;
    OnMakeCurrent(Self,Allowed);
    if not Allowed then begin
      Result:=False;
      exit;
    end;
  end;
  // make current
  Result:=LOpenGLMakeCurrent(Handle);
  if Result and SaveOldToStack then begin
    // on success push on stack
    if OpenGLControlStack=nil then
      OpenGLControlStack:=TList.Create;
    OpenGLControlStack.Add(Self);
  end;
end;

function TCustomOpenGLPanel.ReleaseContext: boolean;
begin
  Result:=false;
  if not HandleAllocated then exit;
  Result:=LOpenGLReleaseContext(Handle);
end;

function TCustomOpenGLPanel.RestoreOldOpenGLControl: boolean;
var
  RestoredControl: TCustomOpenGLPanel;
begin
  Result:=false;
  // check if the current context is on stack
  if (OpenGLControlStack=nil) or (OpenGLControlStack.Count=0) then exit;
  // pop
  OpenGLControlStack.Delete(OpenGLControlStack.Count-1);
  // make old control the current control
  if OpenGLControlStack.Count>0 then begin
    RestoredControl:=
      TCustomOpenGLPanel(OpenGLControlStack[OpenGLControlStack.Count-1]);
    if (not LOpenGLMakeCurrent(RestoredControl.Handle)) then
      exit;
  end else begin
    FreeAndNil(OpenGLControlStack);
  end;
  Result:=true;
end;

function TCustomOpenGLPanel.SharingControlCount: integer;
begin
  if FSharingOpenGlControls=nil then
    Result:=0
  else
    Result:=FSharingOpenGlControls.Count;
end;

procedure TCustomOpenGLPanel.Invalidate;
begin
  if csCustomPaint in FControlState then exit;
  inherited Invalidate;
end;

// =================== TWSOpenGLControl =======================================

class function TWSOpenGLControl.CreateHandle(const AWinControl: TWinControl;
  const AParams: TCreateParams): HWND;
var
  OpenGlControl: TCustomOpenGLPanel;
  AttrControl: TCustomOpenGLPanel;
begin
  if csDesigning in AWinControl.ComponentState then begin
    // do not use "inherited CreateHandle", because the LCL changes the hierarchy at run time
    Result:=TWSWinControlClass(ClassParent).CreateHandle(AWinControl,AParams);
  end
  else begin
    OpenGlControl:=AWinControl as TCustomOpenGLPanel;
    if OpenGlControl.SharedControl<>nil then
      AttrControl:=OpenGlControl.SharedControl
    else
      AttrControl:=OpenGlControl;
    Result:=LOpenGLCreateContext(OpenGlControl,WSPrivate,
                                 OpenGlControl.SharedControl,
                                 AttrControl.DoubleBuffered,
                                 {$IFDEF HasRGBA}
                                 AttrControl.RGBA,
                                 {$ENDIF}
                                 {$IFDEF HasDebugContext}
                                 AttrControl.DebugContext,
                                 {$ENDIF}
                                 {$IFDEF HasRGBBits}
                                 AttrControl.RedBits,
                                 AttrControl.GreenBits,
                                 AttrControl.BlueBits,
                                 {$ENDIF}
                                 {$IFDEF UsesModernGL}
                                 AttrControl.OpenGLMajorVersion,
                                 AttrControl.OpenGLMinorVersion,
                                 {$ENDIF}
                                 AttrControl.MultiSampling,
                                 AttrControl.AlphaBits,
                                 AttrControl.DepthBits,
                                 AttrControl.StencilBits,
                                 AttrControl.AUXBuffers,
                                 AParams);
  end;
end;

class procedure TWSOpenGLControl.DestroyHandle(const AWinControl: TWinControl);
begin
  LOpenGLDestroyContextInfo(AWinControl);
  // do not use "inherited DestroyHandle", because the LCL changes the hierarchy at run time
  TWSWinControlClass(ClassParent).DestroyHandle(AWinControl);
end;

//==================================================================
initialization
  RegisterWSComponent(TCustomOpenGLPanel,TWSOpenGLControl);
end.
