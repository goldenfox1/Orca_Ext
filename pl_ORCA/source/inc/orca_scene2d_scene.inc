{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2CustomScene ================================================

constructor TD2CustomScene.Create(AOwner:TComponent);
begin
  inherited;
  AddScene(Self);
  ShowHint:=true;
  FAnimatedCaret:=true;
  FDesignTime:=csDesigning in ComponentState;
  ControlStyle:=ControlStyle - [csAcceptsControls] + [csActionClient];
  OnDragOver:=DoDragOver;
  OnDragDrop:=DoDragDrop;
  ControlStyle:=ControlStyle + [csCaptureMouse,csOpaque,csDoubleClicks];
  FSnapToLines:=true;
  FAlignRoot:=true;
  FCanvas:=GvarD2DefaultCanvasClass.Create(Width,Height);
  FCanvas.Scene:=Self;
  FFill:=TD2Brush.Create(d2BrushNone,$FF000000);
  FFill.OnChanged:=FillChanged;
  DesignPopupEnabled:=true;
  FSnapGridSize:=1;

  //Align:=alClient;
  Width :=200;
  Height:=200;

  if GvarD2Designer <> nil then
    GvarD2Designer.AddScene(Self);

  GvarD2SceneCount:=GvarD2SceneCount + 1;
end;

destructor TD2CustomScene.Destroy;
begin
  if FHovered <> nil then
  begin
    TD2VisualObject(FHovered).RemoveFreeNotify(Self);
    FHovered:=nil;
  end;
  if FFocused <> nil then
  begin
    TD2VisualObject(FFocused).RemoveFreeNotify(Self);
    FFocused:=nil;
  end;
  if FStyle <> nil then
    FStyle.RemoveSceneUpdater(Self);
  if GvarD2SceneCount=0 then
  begin
    if GvarD2aniThread <> nil then
    begin
      GvarD2aniThread.Free;
    end;
    GvarD2aniThread:=nil;
  end;
  if FDesignPopup <> nil then
    FreeAndnil(FDesignPopup);
  if GvarD2Designer <> nil then
    GvarD2Designer.RemoveScene(Self);
  GvarD2SceneCount:=GvarD2SceneCount - 1;
  if FOpenInFrame <> nil then
    FOpenInFrame.SceneObject:=nil;
  DeleteChildren;
  if FChildren <> nil then
    FreeAndNil(FChildren);
  FreeAndNil(FFill);
  FreeAndNil(FCanvas);
  RemoveScene(Self);
  inherited;
end;

procedure TD2CustomScene.CreateHandle;
{$IFDEF UNIX}
 {$IFNDEF DARWIN}
var
  screen:PGdkScreen;
  colormap:PGdkColormap;
 {$ENDIF}
{$ENDIF}
begin
  inherited;
  {$IFDEF WINDOWS}
  PrevWndProc:=Windows.WNDPROC(SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)));
  {$ENDIF}
  {$IFNDEF UCL}
  {$IFDEF WINDOWS}
  SetWindowLongW(Handle,GWL_WNDPROC,GetWindowLong(Handle,GWL_WNDPROC));
  Canvas.Parent:=Handle;
  {$ENDIF}
  {$IFDEF UNIX}
     {$IFNDEF DARWIN}
       Canvas.Parent:=PtrInt(GetFixedWidget(pgtkwidget(Handle)));
     {$ENDIF}
  {$ENDIF}
  {$ENDIF}
  Canvas.ResizeBuffer(Width,Height);
  RealignRoot;
  AddUpdateRect(d2Rect(0,0,Width,Height));
  { Add Hook }
  {$IFNDEF UCL}
  if FTransparency and not (GetDesignTime) then
  begin
    if Parent is TCustomForm then
    begin
      {$IFDEF UNIX}

       {$IFDEF DARWIN}
         WndEventHandlerUPP:=NewEventHandlerUPP(EventHandlerProcPtr(Pointer(@WndEventHandler)));
         EventKinds[0].eventClass:=kEventClassControl;
         EventKinds[0].eventKind:=kEventControlDraw;
         InstallEventHandler(GetControlEventTarget(TCarbonWindow(TCustomForm(Parent).Handle).Widget),WndEventHandlerUPP,1,@EventKinds[0],Self,nil);
         EventKinds[0].eventClass:=kEventClassWindow;
         EventKinds[0].eventKind:=kEventWindowGetRegion;
         InstallEventHandler(GetWindowEventTarget(TCarbonWindow(TCustomForm(Parent).Handle).Window),WndEventHandlerUPP,1,@EventKinds[0],Self,nil);
         SetWindowAlpha(TCarbonWindow(TCustomForm(Parent).Handle).Window,0.999);
         ReshapeCustomWindow(TCarbonWindow(TCustomForm(Parent).Handle).Window);
       {$ELSE}
         screen:=gtk_widget_get_screen(pgtkwidget(Parent.Handle));
         colormap:=gdk_screen_get_rgba_colormap(screen);
         if colormap <> nil then
          gtk_widget_set_colormap(pgtkwidget(Parent.Handle),colormap);
         { Set Paint by App }
         gtk_widget_set_app_paintable(pgtkwidget(Parent.Handle),true);
       {$ENDIF}

      {$ENDIF}

      {$IFDEF WINDOWS}
      if FTransparency and not (GetDesignTime) and (Parent is TCustomForm) then
        SetWindowLong(Parent.Handle,GWL_EXSTYLE,GetWindowLong(Parent.Handle,GWL_EXSTYLE) or WS_EX_LAYERED);
      UpdateLayer;
      AddUpdateRect(d2Rect(0,0,$FFFF,$FFFF));
      {$ENDIF}
    end
  end;
  {$IFDEF DARWIN}
  SetControlDragTrackingEnabled(TCarbonWidget(Handle).Widget,true);
  if Parent is TCustomForm then
    SetAutomaticControlDragTrackingEnabledForWindow(TCarbonWindow(TCustomForm(Parent).Handle).Window,true);
  WndEventHandlerUPP:=NewEventHandlerUPP(EventHandlerProcPtr(Pointer(@CtrlEventHandler)));
  EventKinds[0].eventClass:=kEventClassControl;
  EventKinds[0].eventKind:=kEventControlDraw;
  EventKinds[1].eventClass:=kEventClassControl;
  EventKinds[1].eventKind:=kEventControlDragEnter;
  EventKinds[2].eventClass:=kEventClassControl;
  EventKinds[2].eventKind:=kEventControlDragReceive;
  EventKinds[3].eventClass:=kEventClassControl;
  EventKinds[3].eventKind:=kEventControlDragWithin;
  EventKinds[4].eventClass:=kEventClassControl;
  EventKinds[4].eventKind:=kEventControlDragLeave;
  InstallEventHandler(GetControlEventTarget(TCarbonWidget(Handle).Widget),WndEventHandlerUPP,5,@EventKinds[0],Self,nil);
  {$ELSE}
  {$ENDIF}
  {$ENDIF}
end;


procedure TD2CustomScene.CreateWnd;
begin
  inherited ;
  {$IFNDEF UCL}
  {$IFDEF WINDOWS}
  RegisterDragDrop(Handle,Self);
  {$ENDIF}
  {$ENDIF}
end;

procedure TD2CustomScene.DestroyWnd;
begin
  {$ifdef windows}
  if Canvas <> nil then
    Canvas.Handle:=0;
  {$endif}
  {$IFNDEF UCL}
  {$IFdef WINDOWS}
  if HandleAllocated then
    RevokeDragDrop(Handle);
  if PtrInt(SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)))=PtrInt(@WndCallback) then
    SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@PrevWndProc));
  {$ENDIF}
  {$ENDIF}
  inherited ;
end;

procedure TD2CustomScene.Loaded;
begin
  inherited;
  FLoadCursor:=Cursor;
  if FSnapToLines then
    FSnapToGrid:=false;
  FShowTimer:=TD2Timer.Create(Self);
  FShowTimer.Interval:=1;
  FShowTimer.OnTimer:=DoShowTimer;
end;

procedure TD2CustomScene.InitiateAction;
  procedure TraverseClients(Container:TD2VisualObject);
  var
    I:integer;
    Control:TD2VisualObject;
  begin
    if Container.CheckParentVisible then
      for I:=0 to Container.ChildrenCount - 1 do
      begin
        if not Container.Children[I].IsVisual then Continue;
        Control:=Container.Children[I].Visual;
        if (Control is TD2Control) and Control.Visible and (TD2Control(Control).Action <> nil) then
          TD2Control(Control).InitiateAction;
        TraverseClients(Control);
      end;
  end;
begin
  inherited ;
  if Root <> nil then
    TraverseClients(TD2VisualObject(Root));
end;

procedure TD2CustomScene.CreateEmbedded(const AWidth,AHeight:integer; AOnFlush:TNotifyEvent);
begin
  FOnFlush:=AOnFlush;
  Width:=AWidth;
  Height:=AHeight;
  Canvas.ResizeBuffer(AWidth,AHeight);
  RealignRoot;
  AddUpdateRect(d2Rect(0,0,AWidth,AHeight));
end;

procedure TD2CustomScene.EmbeddedMouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:integer);
begin
  MouseDown(Button,Shift,X,Y);
end;

procedure TD2CustomScene.EmbeddedMouseMove(Shift:TShiftState; X,Y:integer);
begin
  MouseMove(Shift,X,Y);
end;

procedure TD2CustomScene.EmbeddedMouseUp(Button:TMouseButton; Shift:TShiftState; X,Y:integer);
begin
  MouseUp(Button,Shift,X,Y);
end;

function TD2CustomScene.EmbeddedMouseWheel(Shift:TShiftState; WheelDelta:integer):boolean;
var
  MousePos:TPoint;
begin
  Result:=DoMouseWheel(Shift,WheelDelta,MousePos);
end;

procedure TD2CustomScene.EmbeddedKeyUp(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
begin
  UnicodeKeyUp(Key,Char,Shift);
end;

procedure TD2CustomScene.EmbeddedKeyDown(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
begin
  UnicodeKeyDown(Key,Char,Shift);
end;

procedure TD2CustomScene.DoShowTimer(Sender:TObject);
begin
  FShowTimer.Enabled:=false;
  {$IFDEF DARWIN}
  HIViewSetNeedsDisplay(HiViewRef(TCarbonWidget(Handle).Widget),true);
  {$ENDIF}
  {$IFdef WINDOWS}
  AddUpdateRect(d2Rect(0,0,$FFFF,$FFFF));
  {$ENDIF}
  FShowTimer.Free;
end;

procedure TD2CustomScene.UpdateBuffer;
begin
  {$IFDEF DARWIN}
  HIViewSetNeedsDisplay(HiViewRef(TCarbonWidget(Handle).Widget),true);
  {$ENDIF}
  {$IFdef WINDOWS}
  SetLength(FUpdateRects,1);
  FUpdateRects[0]:=d2Rect(0,0,Width,Height);
  Draw;
  {$ENDIF}
end;

procedure TD2CustomScene.UpdateLayer;
{$IFNDEF UCL}
{$IFDEF WINDOWS}
var
  Blend:TBLENDFUNCTION;
  Origin,Size,BitmapOrigin:Windows.TPoint;
  i,j:integer;
  SaveBits:PD2ColorRecArray;
{$ENDIF}
{$ENDIF}
begin
  {$IFNDEF UCL}
  {$IFDEF WINDOWS}
  if (GetDesignTime) then Exit;
  if not (Owner is TWinControl) then Exit;
  if Parent.Handle=0 then Exit;

  Origin:=Point(Parent.Left + Left,Parent.Top + Top);
  Size:=Point(Width,Height);
  { Update }
  with Blend do
  begin
    BlendOp:=AC_SRC_OVER;
    AlphaFormat:=$01; //AC_SRC_ALPHA;
    BlendFlags:=0;
    SourceConstantAlpha:=$FF;
  end;
  BitmapOrigin:=Point(0,0);

  { VCL }
  if ControlCount > 0 then
  begin
    // save alpha
    GetMem(SaveBits,Width * Height * 4);
    d2MoveLongword(Canvas.FBufferBits,SaveBits,Width * Height);
    // paint
    PaintControls(Canvas.Handle,nil);
    // restore alpha
    for j:=0 to Height - 1 do
      for i:=0 to Width - 1 do
        PD2ColorRecArray(Canvas.FBufferBits)[i + (j * Width)].A:=SaveBits[i + (j * Width)].A;
    FreeMem(SaveBits,Width * Height * 4);
  end;

  UpdateLayeredWindow(Parent.Handle,0,@Origin,@Size,Canvas.Handle,@BitmapOrigin,$00000000,@Blend,ULW_ALPHA);
  {$ENDIF}
  {$ENDIF}
end;

procedure TD2CustomScene.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  i:integer;
begin
  inherited;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]).Stored then
        Proc(FChildren[i]);
end;

procedure TD2CustomScene.AddUpdateRect(R:TD2Rect);
{$IFdef WINDOWS}

var
  WR:TRect;
{$ENDIF}

{$IFDEF UNIX}
  {$IFNDEF DARWIN}
var
  GR:TGdkRectangle;
  {$ENDIF}
{$ENDIF}
begin
  if FDisableUpdate then Exit;
  if csLoading in ComponentState then Exit;
  if not Assigned(FOnFlush) and not HandleAllocated then Exit;
  if csDestroying in ComponentState then Exit;
  if (Canvas.FBuffered) and (Canvas.FBufferBits=nil) then Exit;
  if (d2RectWidth(R)=0) or (d2RectHeight(R)=0) then Exit;

  R:=d2Rect(Trunc(R.Left),Trunc(R.Top),Trunc(R.Right) + 1,Trunc(R.Bottom) + 1);
  if not d2IntersectRect(R,d2Rect(0,0,Width,Height)) then Exit;

  if Assigned(FOnFlush) then
  begin
    { not use WM_PAINT in embedded mode }
    {$IFdef WINDOWS}
    SetLength(FUpdateRects,Length(FUpdateRects) + 1);
    FUpdateRects[High(FUpdateRects)]:=R;
    PostMessage(Handle,WM_ADDUPDATERECT,0,0); // only for layered
    {$ENDIF}
    Exit;
  end;

{$IFDEF UNIX}

  {$IFDEF DARWIN}
    HIViewSetNeedsDisplayInRect(HiViewRef(TCarbonWidget(Handle).Widget),CGRectFromRect(R),true);
  {$ELSE}
    GR.x:=Trunc(R.Left);
    GR.y:=Trunc(R.Top);
    GR.width:=Trunc(d2RectWidth(R));
    GR.height:=Trunc(d2RectHeight(R));
    SetLength(FUpdateRects,Length(FUpdateRects) + 1);
    FUpdateRects[High(FUpdateRects)]:=R;
    if HandleAllocated and GDK_IS_WINDOW(pgtkwidget(Handle)^.window) then
     gdk_window_invalidate_rect(pgtkwidget(Handle)^.window,@GR,true);
   {$ENDIF}

{$ENDIF}

{$IFDEF WINDOWS}
   if Transparency and (Parent is TCustomForm) and not FDEsigntime then
   begin
     SetLength(FUpdateRects,Length(FUpdateRects) + 1);
     FUpdateRects[High(FUpdateRects)]:=R;
     PostMessage(Handle,WM_ADDUPDATERECT,0,0); // only for layered
   end
   else
   begin
     WR:=Rect(Trunc(R.Left),Trunc(R.Top),Trunc(R.Right),Trunc(R.Bottom));
     Windows.InvalidateRect(Handle,@WR,false);
   end;
{$ENDIF}
end;

//============================================================= 8888
{$IFDEF WINDOWS}
type
  TParentControl=class(TWinControl);

procedure GetControls(X,Y,W,H:integer;
                      Control:TCustomControl; Dest:TCanvas);
var
  I,Count,SaveIndex:integer;
  DC:HDC;
  R,SelfR,CtlR:TRect;
  Ctrl:TControl;
begin
  Count:=Control.ControlCount;
  DC:=Dest.Handle;
  SelfR:=Bounds(0,0,W,H);
  // Copy images of controls
  for I:=0 to Count - 1 do
  begin
    Ctrl:=Control.Controls[I];
    if (Ctrl <> nil) and (Ctrl is TCustomControl)
    then
      begin
        with Ctrl do
        begin
          CtlR:=Bounds(X + Left,Y + Top,Width,Height);
          if Bool(IntersectRect(R,SelfR,CtlR)) and Visible then
          begin
            SaveIndex:=SaveDC(DC);
            SetViewportOrgEx(DC,Left + X,Top + Y,nil);
            IntersectClipRect(DC,0,0,Width,Height);
            Perform(WM_PAINT,DC,0);
            RestoreDC(DC,SaveIndex);
            if TCustomControl(Ctrl).ControlCount <> 0
            then
              GetControls(Left + X,Top + Y,W,H,
              TCustomControl(Ctrl),Dest);
          end;
       end;
    end;
  end;
end;

procedure DrawParentImage(Control:TControl; DestDC:HDC);
var
  I,Count,X,Y,SaveIndex:integer;
  Dest:TCanvas;
  R,SelfR,CtlR:TRect;
  Ctrl:TControl;
begin
  if Control.Parent=nil then Exit;
  Count:=Control.Parent.ControlCount;
  Dest:=TCanvas.Create;
  try
    Dest.Handle:=DestDC;
    SelfR:=Bounds(Control.Left,Control.Top,Control.Width,Control.Height);
    X:=-Control.Left; Y:=-Control.Top;
    // Copy parent control image
    if Control.Parent is TForm then
    begin
        SaveIndex:=SaveDC(DestDC);
        SetViewportOrgEx(DestDC,X,Y,nil);
        IntersectClipRect(DestDC,0,0,Control.Parent.ClientWidth,
           Control.Parent.ClientHeight);
        if (Control.Parent is TForm) and
            (TForm(Control.Parent).FormStyle=fsMDIForm)
         then
           begin
             SendMessage(TForm(Control.Parent).ClientHandle,WM_ERASEBKGND,DestDC,0);
           end
        else
          SendMessage(Control.Parent.Handle,WM_ERASEBKGND,DestDC,0);
        RestoreDC(DestDC,SaveIndex);
      end
    else
      begin
        SaveIndex:=SaveDC(DestDC);
        SetViewportOrgEx(DestDC,X,Y,nil);
        IntersectClipRect(DestDC,0,0,Control.Parent.ClientWidth,
           Control.Parent.ClientHeight);
        TParentControl(Control.Parent).Perform(WM_ERASEBKGND,DestDC,0);
        TParentControl(Control.Parent).Perform(WM_PAINT,DestDC,0);
        RestoreDC(DestDC,SaveIndex);
      end;

    // Copy images of controls
    for I:=0 to Count - 1 do
    begin
      Ctrl:=Control.Parent.Controls[I];
      if Ctrl=Control then Break;
      if (Ctrl <> nil) and
         ((Ctrl is TGraphicControl) or (Ctrl is TCustomControl))
      then
        with Ctrl do
        begin
          CtlR:=Bounds(Left,Top,Width,Height);
          if Bool(IntersectRect(R,SelfR,CtlR)) and Visible then
          begin
            SaveIndex:=SaveDC(DestDC);
            SetViewportOrgEx(DestDC,Left + X,Top + Y,nil);
            IntersectClipRect(DestDC,0,0,Width,Height);
            Perform(WM_PAINT,DestDC,0);
            RestoreDC(DestDC,SaveIndex);
            if Ctrl is TCustomControl then
              GetControls(Left + X,Top + Y,
                Control.Width,Control.Height,
                TCustomControl(Ctrl),Dest);
          end;
       end;
    end;
  finally
    Dest.Handle:=0;
    Dest.Free;
  end;
end;
{$ENDIF}

procedure TD2CustomScene.Draw;
var
  i,j:integer;
  R:TD2Rect;
  Rgn,NRgn:Cardinal;
  ScaleMatrix:TD2Matrix;
begin
  if not (FDesignTime) and (FOpenInFrame <> nil) then Exit;
  if FDrawing then Exit;
  if Length(FUpdateRects) > 0 then
  begin
    FDrawing:=true;
    try
      { Split rects if rects too more }
      if (Length(FUpdateRects) > 20) then
      begin
        for i:=1 to High(FUpdateRects) do
          FUpdateRects[0]:=d2UnionRect(FUpdateRects[0],FUpdateRects[i]);
        SetLength(FUpdateRects,1);
      end;
      { draw back }
      if Canvas.BeginScene then
      begin
        Canvas.ResetClipRect;
        ScaleMatrix:=IdentityMatrix;
        Canvas.SetMatrix(ScaleMatrix);
        Canvas.SetClipRects(FUpdateRects);

        if FTransparency and not (Parent is TCustomForm) then
        begin
          if FFill.Style=d2BrushNone then
          begin
            { apply cliprgn }
            {$IFdef WINDOWS}
            j:=SaveDC(Canvas.Handle);
            with FUpdateRects[0] do
              Rgn:=CreateRectRgn(trunc(left),trunc(top),trunc(right),trunc(bottom));
            for i:=1 to High(FUpdateRects) do
            begin
              with FUpdateRects[i] do
                NRgn:=CreateRectRgn(trunc(left),trunc(top),trunc(right),trunc(bottom));
              CombineRgn(Rgn,Rgn,NRgn,RGN_OR);
              DeleteObject(NRgn);
            end;
            SelectClipRgn(Canvas.Handle,Rgn);
            DrawParentImage(Self,Canvas.Handle);
            RestoreDC(Canvas.Handle,j);
            {$ENDIF}
          end
          else
          begin
            Canvas.Fill.Assign(FFill);
            Canvas.FillRect(d2Rect(-1,-1,Width + 1,Height + 1),0,0,AllCorners,1);
          end;
        end
        else
        begin
          if (FFill.Style=d2BrushNone) or ((FFill.SolidColor and $FF000000=0) and (FFill.Style=d2BrushSolid)) then
          begin
            for i:=0 to High(FUpdateRects) do
            begin
              if FTransparency then
                Canvas.ClearRect(FUpdateRects[i],0)
              else
                Canvas.ClearRect(FUpdateRects[i],FFill.SolidColor and $FFFFFF);
            end;
          end
          else
          begin
            Canvas.Fill.Assign(FFill);
            Canvas.FillRect(d2Rect(-1,-1,Width + 1,Height + 1),0,0,AllCorners,1);
          end;
        end;
        { reset }
        Canvas.StrokeThickness:=1;
        Canvas.StrokeCap:=d2CapFlat;
        Canvas.StrokeJoin:=d2JoinMiter;
        Canvas.StrokeDash:=d2DashSolid;
        Canvas.Stroke.Style:=d2BrushSolid;
        Canvas.Fill.Style:=d2BrushSolid;
        if FChildren <> nil then
          for i:=0 to FChildren.Count - 1 do
          begin
            if not (TD2Object(FChildren[i]).IsVisual) then Continue;
            if not TD2VisualObject(FChildren[i]).Visible then Continue;

            ScaleMatrix:=IdentityMatrix;
            for j:=0 to High(FUpdateRects) do
              if d2IntersectRect(FUpdateRects[j],TD2VisualObject(FChildren[i]).UpdateRect) then
              begin
                Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,TD2VisualObject(FChildren[i]).AbsoluteMatrix));
                TD2VisualObject(FChildren[i]).BeforePaint;
                TD2VisualObject(FChildren[i]).Paint;
                TD2VisualObject(FChildren[i]).AfterPaint;
                TD2VisualObject(FChildren[i]).PaintChildren;
                if Assigned(TD2VisualObject(FChildren[i]).OnPaint) then
                begin
                  Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,TD2VisualObject(FChildren[i]).AbsoluteMatrix));
                  TD2VisualObject(FChildren[i]).OnPaint(TD2VisualObject(FChildren[i]),Canvas,TD2VisualObject(FChildren[i]).LocalRect);
                end;
                Break;
              end;
          end;

        { grid }
        if FSnapGridShow and (FSnapGridSize <> 0) then
        begin
          ScaleMatrix:=IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);
          Canvas.Stroke.Style:=d2BrushSolid;
          Canvas.StrokeThickness:=1;
  {        for i:=Trunc((-FDesignScroll.X) / (FSnapGridSize)) - 1 to Trunc((-FDesignScroll.X + Width) / (FSnapGridSize)) + 1 do
          begin
            if i mod 5=0 then
              Canvas.Stroke.SolidColor:=$50505050
            else
              Canvas.Stroke.SolidColor:=$50303030;
            Canvas.DrawLine(d2Point(i * FSnapGridSize + 0.5,-FDesignScroll.Y + 0.5),d2Point(i * FSnapGridSize + 0.5,-FDesignScroll.Y + Height + 0.5),1);
          end;
          for j:=Trunc((-FDesignScroll.Y) / (FSnapGridSize)) - 1 to Trunc((-FDesignScroll.Y + Height) / (FSnapGridSize)) + 1 do
          begin
            if j mod 5=0 then
              Canvas.Stroke.SolidColor:=$50505050
            else
              Canvas.Stroke.SolidColor:=$50303030;
            Canvas.DrawLine(d2Point(-FDesignScroll.X + 0.5,j * FSnapGridSize + 0.5),d2Point(-FDesignScroll.X + Width + 0.5,j * FSnapGridSize + 0.5),1)
          end;}
        end;
        if (FSelected <> nil) and not FSelected.DisableDesignResize then
        begin
          Canvas.Fill.Style:=d2BrushSolid;
          Canvas.Fill.SolidColor:=$FFFFFFFF;
          Canvas.StrokeThickness:=1;
          Canvas.Stroke.Style:=d2BrushSolid;
          Canvas.Stroke.SolidColor:=$FF1072C5;
          ScaleMatrix:=orca_scene2d.IdentityMatrix;
          Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,FSelected.AbsoluteMatrix));
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.StrokeDash:=d2DashDash;
          Canvas.DrawRect(R,0,0,AllCorners,1);
          Canvas.StrokeDash:=d2DashSolid;
          begin
          { rotate }
          if FRotateHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.DrawLine(d2Point((R.Left + R.Right) / 2,R.Top),d2Point((R.Left + R.Right) / 2,R.Top - cnRotSize),1);
          Canvas.Fillellipse(d2Rect((R.Left + R.Right) / 2 - (cnGripSize),R.Top - cnRotSize - (cnGripSize),
            (R.Left + R.Right) / 2 +(cnGripSize),R.Top - cnRotSize + (cnGripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect((R.Left + R.Right) / 2 - (cnGripSize),R.Top - cnRotSize - (cnGripSize),
            (R.Left + R.Right) / 2 +(cnGripSize),R.Top - cnRotSize + (cnGripSize)),Opaque);
          { angles }
          if FLeftTopHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Left - (cnGripSize),R.Top - (cnGripSize),R.Left + (cnGripSize),R.Top + (cnGripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Left - (cnGripSize),R.Top - (cnGripSize),R.Left + (cnGripSize),R.Top + (cnGripSize)),Opaque);

          if FRightTopHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Right - (cnGripSize),R.Top - (cnGripSize),R.Right + (cnGripSize),R.Top + (cnGripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Right - (cnGripSize),R.Top - (cnGripSize),R.Right + (cnGripSize),R.Top + (cnGripSize)),Opaque);

          if FLeftBottomHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.Fillellipse(d2Rect(R.Left - (cnGripSize),R.Bottom - (cnGripSize),R.Left + (cnGripSize),R.Bottom + (cnGripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Left - (cnGripSize),R.Bottom - (cnGripSize),R.Left + (cnGripSize),R.Bottom + (cnGripSize)),Opaque);

          if FRightBottomHot then
            Canvas.Fill.SolidColor:=$FFFF0000
          else
            Canvas.Fill.SolidColor:=$FFFFFFFF;
          R:=FSelected.BoundsRect;
          d2InflateRect(R,-0.5,-0.5);
          Canvas.FillEllipse(d2Rect(R.Right - (cnGripSize),R.Bottom - (cnGripSize),R.Right + (cnGripSize),R.Bottom + (cnGripSize)),Opaque);
          Canvas.DrawEllipse(d2Rect(R.Right - (cnGripSize),R.Bottom - (cnGripSize),R.Right + (cnGripSize),R.Bottom + (cnGripSize)),Opaque);
          { lines }
          if FSelected.Width > cnGripSize * 4 then
          begin
            if FTopHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left + d2RectWidth(R)/2 - (cnGripSize),R.Top - (cnGripSize),R.Left + d2RectWidth(R)/2 + (cnGripSize),R.Top + (cnGripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left + d2RectWidth(R)/2 - (cnGripSize),R.Top - (cnGripSize),R.Left + d2RectWidth(R)/2 + (cnGripSize),R.Top + (cnGripSize)),0,0,[],Opaque);
            if FBottomHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left + d2RectWidth(R)/2 - (cnGripSize),R.Bottom - (cnGripSize),R.Left + d2RectWidth(R)/2 + (cnGripSize),R.Bottom + (cnGripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left + d2RectWidth(R)/2 - (cnGripSize),R.Bottom - (cnGripSize),R.Left + d2RectWidth(R)/2 + (cnGripSize),R.Bottom + (cnGripSize)),0,0,[],Opaque);
          end;
          if FSelected.Height > cnGripSize * 4 then
          begin
            if FLeftHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Left - (cnGripSize),R.Top + d2RectHeight(R)/2 - (cnGripSize),R.Left + (cnGripSize),R.Top + d2RectHeight(R)/2 + (cnGripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Left - (cnGripSize),R.Top + d2RectHeight(R)/2 - (cnGripSize),R.Left + (cnGripSize),R.Top + d2RectHeight(R)/2 + (cnGripSize)),0,0,[],Opaque);
            if FRightHot then
              Canvas.Fill.SolidColor:=$FFFF0000
            else
              Canvas.Fill.SolidColor:=$FFFFFFFF;
            R:=FSelected.BoundsRect;
            d2InflateRect(R,-0.5,-0.5);
            Canvas.FillRect(d2Rect(R.Right - (cnGripSize),R.Top + d2RectHeight(R)/2 - (cnGripSize),R.Right + (cnGripSize),R.Top + d2RectHeight(R)/2 + (cnGripSize)),0,0,[],Opaque);
            Canvas.DrawRect(d2Rect(R.Right - (cnGripSize),R.Top + d2RectHeight(R)/2 - (cnGripSize),R.Right + (cnGripSize),R.Top + d2RectHeight(R)/2 + (cnGripSize)),0,0,[],Opaque);
          end;
          { grid lines }
          if (FMoving or FLeftTop or FRightTop or FLeftBottom or FRightBottom or FTop or FBottom or FLeft or FRight) and
             (Length(FDesignGridLines) > 0) and (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
          begin
            ScaleMatrix:=orca_scene2d.IdentityMatrix;
            Canvas.SetMatrix(d2MatrixMultiply(ScaleMatrix,TD2VisualObject(FSelected.Parent).AbsoluteMatrix));
            Canvas.StrokeDash:=d2DashDash;
            for i:=0 to High(FDesignGridLines) do
            begin
              if (FDesignGridLines[i].Position.Y + round(FDesignGridLines[i].Height / 2))=(FSelected.Position.Y + round(FSelected.Height / 2)) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,Trunc(FSelected.Position.Y + (FSelected.Height / 2)) + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,Trunc(FSelected.Position.Y + (FSelected.Height / 2)) + 0.5),1);
              end;
              if (FDesignGridLines[i].Position.X + round(FDesignGridLines[i].Width / 2))=(FSelected.Position.X + round(FSelected.Width / 2)) then
              begin
                Canvas.DrawLine(d2Point(Trunc(FSelected.Position.X + (FSelected.Width / 2)) + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(Trunc(FDesignGridLines[i].Position.X + (FDesignGridLines[i].Width / 2)) + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X=FDesignGridLines[i].Position.X) or (FSelected.Position.Y=FDesignGridLines[i].Position.Y) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FSelected.Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X + FSelected.Width=FDesignGridLines[i].Position.X) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + 0.5,FSelected.Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y + FSelected.Height=FDesignGridLines[i].Position.Y) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.X=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),1);
              end;
              if (FSelected.Position.X + FSelected.Width=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width) then
              begin
                Canvas.DrawLine(d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FSelected.Position.Y + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 0.5,FDesignGridLines[i].Position.Y + 0.5),1);
              end;
              if (FSelected.Position.Y + FSelected.Height=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height) then
              begin
                Canvas.DrawLine(d2Point(FSelected.Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),
                  d2Point(FDesignGridLines[i].Position.X + 0.5,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 0.5),1);
              end;
            end;
            Canvas.StrokeDash:=d2DashSolid;
          end;
          end;
          { place message }
          if FDesignPlaceObject <> nil then
          begin
            ScaleMatrix:=orca_scene2d.IdentityMatrix;
            Canvas.SetMatrix(ScaleMatrix);

            R:=FDesignPlaceObject.AbsoluteRect;
            Canvas.Stroke.SolidColor:=$FF5B91DE;
            Canvas.DrawRect(R,0,0,AllCorners,1);
            Canvas.Font.Family:='Tahoma';
            Canvas.Font.Style:=d2FontRegular;
            Canvas.Font.Size:=9;
            R.Bottom:=R.Top;
            R.Top:=R.Bottom - 19;
            R.Right:=R.Left + 160;
            Canvas.Fill.SolidColor:=$FF5B91DE;
            Canvas.FillRect(R,0,0,AllCorners,1);
            Canvas.Fill.SolidColor:=$FFFFFFFF;
            d2InflateRect(R,-2,-2);
            if FDesignPlaceObject.Name <> '' then
              Canvas.FillText(R,R,'ALT-drag to place into [' + FDesignPlaceObject.Name + ']',false,1,d2TextAlignNear,d2TextAlignCenter)
            else
              Canvas.FillText(R,R,'ALT-drag to place into [' + FDesignPlaceObject.ClassName + ']',false,1,d2TextAlignNear,d2TextAlignCenter);
          end;
        end;
        { design modes }
        if FDesignTime then
        begin
          ScaleMatrix:=IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);

          Canvas.Stroke.SolidColor:=$FF5B91DE;
          Canvas.Font.Family:='Tahoma';
          Canvas.Font.Style:=d2FontRegular;
          Canvas.Font.Size:=9;
          R:=d2Rect(0,0,200,17);
        end;
        { debug }
        if ShowUpdateRects then
        with Canvas do
        begin
          ResetClipRect;
          ScaleMatrix:=IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);
          Stroke.Style:=d2BrushSolid;
          Stroke.Color:='#9000FF00';
          StrokeThickness:=1;
          Fill.Style:=d2BrushNone;
          for i:=0 to High(FUpdateRects) do
          begin
            R:=FUpdateRects[i];
            DrawRect(FUpdateRects[i],0,0,AllCorners,0.5);
          end;
        end;
        {$IFDEF UPDATERECT}
        if not ShowUpdateRects then
        with Canvas do
        begin
          ResetClipRect;
          ScaleMatrix:=IdentityMatrix;
          Canvas.SetMatrix(ScaleMatrix);
          Stroke.Style:=d2BrushSolid;
          Stroke.Color:='#A000FF00';
          StrokeThickness:=1;
          Fill.Style:=d2BrushNone;
          for i:=0 to High(FUpdateRects) do
          begin
            R:=FUpdateRects[i];
            DrawRect(FUpdateRects[i],0,0,AllCorners,0.5);
          end;
        end;
        {$ENDIF}
        Canvas.EndScene;
      end;

      if Assigned(OnFlush) then
        OnFlush(Self)
      else
      begin
        { buffer }
        {$IFDEF WINDOWS}
        if (not FTransparency) or (FTransparency and not (Parent is TCustomForm)) or (FDesignTime) then
        {$ENDIF}
        begin
          for i:=0 to High(FUpdateRects) do
          begin
            R:=FUpdateRects[i];
            {$IFDEF WINDOWS}
            Canvas.FlushBufferRect(0,0,FDC,FUpdateRects[i]);
            {$ELSE}
            Canvas.FlushBufferRect(0,0,0,R);
            {$ENDIF}
          end;
        end;
        { Transparancy }
        {$IFDEF WINDOWS}
        if FTransparency and (Parent is TCustomForm) then
          UpdateLayer;
        {$ENDIF}
      end;
    finally
      setLength(FUpdateRects,0);
      FDrawing:=false;
    end;
  end;
end;

{ Drag and Drop }

type
  THackControl=class(TCustomControl);

procedure TD2CustomScene.BeginVCLDrag(Source:TObject; ABitmap:TD2Bitmap);
begin
  VCLDragSource:=TCustomControl.Create(Self);
  VCLDragSource.Parent:=Self;
  VCLDragSource.Width:=0;
  VCLDragSource.Height:=0;
  VCLDragSource.Tag:=PtrInt(Source);
  VCLDragSource.BeginDrag(true,-1);
  THackControl(VCLDragSource).OnEndDrag:=EndDragEvent;
end;

procedure TD2CustomScene.EndDragEvent(Sender,Target:TObject; X,Y:integer);
begin
  if TObject(Pointer(VCLDragSource.Tag)) is TD2VisualObject then
    TD2VisualObject(Pointer(VCLDragSource.Tag)).DragEnd;
  VCLDragSource.Tag:=0;
end;

procedure TD2CustomScene.DoDragOver(Sender,Source:TObject; X,Y:integer; State:TDragState;
  var Accept:boolean);
var
  P:TD2Point;
  NewTarget:TD2VisualObject;
  Data:TD2DragObject;
begin
  Accept:=false;
  if Root=nil then Exit;

  FillChar(Data,SizeOf(Data),0);
  if (VCLDragSource <> nil) and (VCLDragSource.Tag <> 0) then
    Data.Source:=TD2Object(VCLDragSource.Tag)
  else
    Data.Source:=Source;

  P:=d2Point(X,Y);
  NewTarget:=Root.Visual.FindTarget(P,Data);

  if (VCLDragSource <> nil) and (NewTarget=TD2Object(VCLDragSource.Tag)) then
  begin
    if varD2FTarget <> nil then
      TD2VisualObject(varD2FTarget).DragLeave;
    varD2FTarget:=nil;
    Accept:=false;
    Exit;
  end;

  if varD2FTarget <> nil then
  begin
    TD2VisualObject(varD2FTarget).DragOver(Data,P,Accept);
  end;
  if (NewTarget <> varD2FTarget) then
  begin
    if varD2FTarget <> nil then
      TD2VisualObject(varD2FTarget).DragLeave;
    varD2FTarget:=NewTarget;
    if varD2FTarget <> nil then
    begin
      TD2VisualObject(varD2FTarget).DragEnter(Data,P);
    end;
  end;
  if varD2FTarget=nil then Accept:=false;
end;

procedure TD2CustomScene.DoDragDrop(Sender,Source:TObject;
  X,Y:integer);
var
  Data:TD2DragObject;
begin
  if varD2FTarget <> nil then
  begin
    FillChar(Data,SizeOf(Data),0);
    if (VCLDragSource <> nil) and (VCLDragSource.Tag <> 0) then
      Data.Source:=TD2Object(VCLDragSource.Tag)
    else
      Data.Source:=Source;
    TD2VisualObject(varD2FTarget).DragDrop(Data,d2Point(X,Y));
  end;
  varD2FTarget:=nil;
end;

procedure TD2CustomScene.Paint;
begin
{$IFDEF UNIX}

  {$IFDEF DARWIN}
    // nothing...
  {$ELSE}
  Canvas.Handle:=PtrInt(gdk_cairo_create(TGtkDeviceContext(inherited Canvas.Handle).Drawable));

  if FTransparency then
  begin
    cairo_set_operator(Pcairo_t(Canvas.Handle),CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(Pcairo_t(Canvas.Handle),0,0,0,0);
    cairo_paint(Pcairo_t(Canvas.Handle));
    cairo_set_operator(Pcairo_t(Canvas.Handle),CAIRO_OPERATOR_OVER);
  end;
//  if Length(FUpdateRects)=0 then
  begin
    // update all area
    SetLength(FUpdateRects,1);
    FUpdateRects[0]:=d2Rect(0,0,Width,Height);
  end;
  Draw;
  cairo_destroy(Pcairo_t(Canvas.Handle));
  Canvas.Handle:=0;
  {$ENDIF}

{$ENDIF}
end;

function TD2CustomScene.ObjectByPoint(X,Y:single):TD2VisualObject;
var
  i:integer;
  Obj,NewObj:TD2Object;
begin
  Result:=nil;
  for i:=Count - 1 downto 0 do
  begin
    Obj:=Children[i];
    if not (Obj.IsVisual) then Exit;
    if not TD2VisualObject(Obj).Visible and not (FDesignTime) then Continue;

    NewObj:=TD2VisualObject(Obj).ObjectByPoint(X,Y);
    if NewObj <> nil then
    begin
      Result:=TD2VisualObject(NewObj);
      Exit;
    end;
  end;
end;

procedure TD2CustomScene.CMShowingChanged(var Message:TLMessage );
begin
  inherited ;
  AddUpdateRect(d2Rect(0,0,Width,Height));
end;

procedure TD2CustomScene.CMDesignHitTest(var Msg:TLMMouse );
var
  Obj:TD2VisualObject;
  P:TD2Point;
begin
  inherited ;

  if (FChildren=nil) or (FChildren.Count=0) then
  begin
    Msg.Result:=1;
    Exit;
  end;

  P:=d2Point(Msg.XPos,Msg.YPos);
  if (FMoving or FLeftTop or FRightTop or FLeftBottom or FRightBottom) then
  begin
    Msg.Result:=1;
    Exit;
  end;
  Obj:=ObjectByPoint(P.X,P.Y);
  if Obj=nil then
  begin
    if (FSelected <> nil) and (d2PtInRect(d2Point(P.X,P.Y),FSelected.AbsoluteRect)) then
      Msg.Result:=1
    else
      Msg.Result:=0
  end
  else
  begin
    Msg.Result:=1;
  end;
end;

procedure TD2CustomScene.CMHintShow(var Message:TLMessage);
var
  Obj:TD2VisualObject;
begin
  inherited ;
  with TCMHintShow(Message).HintInfo^ do
  begin
    Obj:=ObjectByPoint(CursorPos.X,CursorPos.Y);
    if (Obj <> nil) and (Obj.ShowHint) then
    begin
      HintStr:=Obj.Hint;
      with Obj.AbsoluteRect do
        CursorRect:=Rect(Trunc(Left),Trunc(Top),Trunc(Right),Trunc(Bottom));
    end
    else
    begin
      HintStr:='';
    end;
  end;
end;


procedure TD2CustomScene.UnicodeKeyDown(var Key:Word; var Char:System.WideChar;
  Shift:TShiftState);
var
  List:TList;
  i,CurIdx:integer;
  Found:boolean;
  O:TComponent;
begin
  { dialog key }
  if (Key=VK_ESCAPE) or (Key=VK_RETURN) or (Key=VK_LEFT) or (Key=VK_RIGHT) or (Key=VK_UP) or (Key=VK_DOWN) or
     (Key=VK_HOME) or (Key=VK_END) or (Key=VK_ADD) or (Key=VK_SUBTRACT) then
  begin
    TD2VisualObject(Root).DialogKey(Key,Shift);
    if Key=0 then Exit;
  end;
  { modal }
  if (Key=VK_ESCAPE) then
  begin
    O:=Owner;
    while O <> nil do
    begin
      if (O is TCustomForm) and (fsModal in TCustomForm(O).FormState) then
      begin
        TCustomForm(O).ModalResult:=mrCancel;
        Key:=0;
        Break;
      end;
      O:=O.Owner;
    end;
    if Key=0 then Exit;
  end;
  { change focus }
  if (Key=VK_TAB) and (Root <> nil) then
  begin
    Key:=0;
    List:=TList.Create;
    TD2VisualObject(Root).GetTabOrderList(List,true);

    Found:=false;
    if ssShift in Shift then
    begin
      { second search in first part of list }
      if FFocused <> nil then
        CurIdx:=List.IndexOf(FFocused) - 1
      else
        CurIdx:=List.Count - 1;
      if CurIdx > 0 then
          for i:=CurIdx - 1 downto 0 do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
      { first search in last part of list }
      if not Found then
        if (List.Count > 2) and (CurIdx < List.Count) then
          for i:=List.Count - 1 downto CurIdx do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Break;
            end;
    end
    else
    begin
      if FFocused <> nil then
        CurIdx:=List.IndexOf(FFocused) + 1
      else
        CurIdx:=0;
      { first search in last part of list }
      if (List.Count > 2) and (CurIdx < List.Count) then
        for i:=CurIdx to List.Count - 1 do
          if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
          begin
            TD2VisualObject(List[i]).SetFocus;
            Found:=true;
            Break;
          end;
      { second search in first part of list }
      if not Found then
        if CurIdx > 0 then
          for i:=0 to CurIdx - 1 do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Break;
            end;
    end;
    List.Free;
    Exit;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    TD2VisualObject(FFocused).KeyDown(Key,Char,Shift);
  end;
end;

procedure TD2CustomScene.UnicodeKeyUp(var Key:Word; var Char:System.WideChar;
  Shift:TShiftState);
begin
  if FDesignTime or (FDesignTime) then
  begin
    if (Key=VK_DELETE) and (FSelected <> nil) then
    begin
      FSelected.Free;
      FSelected:=nil;
    end;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    TD2VisualObject(FFocused).KeyUp(Key,Char,Shift);
  end;
end;

procedure TD2CustomScene.DeleteChildren;
var
  Child:TD2Object;
begin
  if Assigned(FChildren) then
  begin
    while FChildren.Count > 0 do
    begin
      Child:=TD2Object(FChildren[0]);
      FChildren.Delete(0);
      TD2VisualObject(Child).FParent:=nil;
      TD2VisualObject(Child).FScene:=nil;
      Child.Free;
    end;
    FreeAndNil(FChildren);
  end;
end;

procedure TD2CustomScene.AddObject(AObject:TD2Object);
begin
  if AObject=nil then Exit;
  if AObject.Parent <> nil then
    AObject.Parent:=nil;
  if FChildren=nil then
    FChildren:=TList.Create;
  FChildren.Add(AObject);
  TD2VisualObject(AObject).SetNewScene(Self);
  if AObject.IsVisual and not (csDestroying in ComponentState) then
  begin
    TD2VisualObject(AObject).RecalcUpdateRect;
    TD2VisualObject(AObject).RecalcAbsolute;
    TD2VisualObject(AObject).Repaint;
    RealignRoot;
  end;
end;

procedure TD2CustomScene.RemoveObject(AObject:TD2Object);
begin
  if FChildren <> nil then
  begin
    Invalidate;
    TD2VisualObject(AObject).SetNewScene(nil);
    Notification(AObject,opRemove);
    FChildren.Remove(AObject);
  end;
end;

function TD2CustomScene.GetCount:integer;
begin
  if FChildren <> nil then
    Result:=FChildren.Count
  else
    Result:=0;
end;

function TD2CustomScene.GetChildrenObject(Index:integer):TD2Object;
begin
  if FChildren <> nil then
    Result:=TD2Object(FChildren[Index])
  else
    Result:=nil;
end;

procedure TD2CustomScene.SetChildren(Index:integer; const Value:TD2Object);
begin
end;

procedure TD2CustomScene.BeginDrag;
var
  P:TPoint;
begin
  if (Parent <> nil) and (Parent.Tag=$FFFF) then Exit; // Disable for popup

  FDragging:=true;

  GetCursorPos(P);
  P:=TWinControl(Owner).ScreenToClient(P);
  FDownPos:=d2Point(P.X,P.Y);
  MouseCapture;
end;

procedure TD2CustomScene.BeginResize;
var
  P:TPoint;
begin
  FResizing:=true;
  GetCursorPos(P);
  P:=TWinControl(Owner).ScreenToClient(P);
  FDownPos:=d2Point(P.X,P.Y);
  FResizeSize:=Point(TWinControl(Owner).Width,TWinControl(Owner).Height);
  MouseCapture;
end;

function TD2CustomScene.SnapToGridValue(Value:single):single;
begin
  if (DesignSnapToGrid) and (DesignSnapGridSize <> 0) then
    Result:=Trunc(Value / DesignSnapGridSize) * DesignSnapGridSize
  else
    Result:=Value;
end;

procedure TD2CustomScene.AddUpdateRectsFromGridLines;
  procedure IntAddUpdateRect(const R:TD2Rect);
  var
    i:integer;
  begin
    for i:=0 to High(FUpdateRects) do
      with FUpdateRects[i] do
        if (R.Left=Left) and (R.Top=Top) and (R.Right=Right) and (R.Bottom=Bottom) then
        begin
          Exit;
        end;
    AddUpdateRect(R);
  end;
var
  i:integer;
begin
  { Add grip lines }
  if FDesignTime and (FSelected <> nil) and not FSelected.DisableDesignResize and
     (FMoving or FLeftTop or FRightTop or FLeftBottom or FRightBottom or FTop or FBottom or FLeft or FRight) and
     (Length(FDesignGridLines) > 0) and (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
  begin
    for i:=0 to High(FDesignGridLines) do
    begin
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FSelected.Position.Y + (FSelected.Height / 2) - 1,
        FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + (FDesignGridLines[i].Height / 2) + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X + (FSelected.Width / 2) - 1,FSelected.Position.Y - 1,
        FDesignGridLines[i].Position.X + (FDesignGridLines[i].Width / 2) + 1,FDesignGridLines[i].Position.Y + 1)));

      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FSelected.Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X - 1,FSelected.Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y - 1,FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width - 1,FSelected.Position.Y - 1,
          FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height - 1,
          FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FDesignGridLines[i].Position.X + FDesignGridLines[i].Width - 1,FSelected.Position.Y - 1,
          FDesignGridLines[i].Position.X + FDesignGridLines[i].Width + 1,FDesignGridLines[i].Position.Y + 1)));
      IntAddUpdateRect(d2NormalizeRect2(d2Rect(FSelected.Position.X - 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height - 1,
          FDesignGridLines[i].Position.X + 1,FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height + 1)));
    end; 
  end;
end;

procedure TD2CustomScene.SnapToGridLines(AllowChangePosition:boolean);
  procedure AddGridLine(const Obj:TD2VisualObject);
  var
    i:integer;
  begin
    for i:=0 to High(FDesignGridLines) do
      if FDesignGridLines[i]=Obj then Exit;
    SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
    FDesignGridLines[High(FDesignGridLines)]:=Obj;
  end;
const
  SnapLineSize=2;
var
  i:integer;
begin
  if (DesignSnapToLines) and (FSelected.Parent <> nil) then
    for i:=0 to FSelected.Parent.ChildrenCount - 1 do
    begin
      if TD2Object(FSelected.Parent.Children[i])=FSelected then Continue;
      if not TD2Object(FSelected.Parent.Children[i]).isVisual then Continue;
      with TD2VisualObject(FSelected.Parent.Children[i]) do
      begin
        if (Abs((Position.Y + round(Height / 2)) - (FSelected.Position.Y + round(FSelected.Height / 2))) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + round(Height / 2)) - (FSelected.Position.Y + round(FSelected.Height / 2)));
            FSelected.Position.Y:=(Position.Y + round(Height / 2) - round(FSelected.Height / 2));
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + round(Width / 2)) - (FSelected.Position.X + round(FSelected.Width / 2))) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + round(Width / 2)) - (FSelected.Position.X + round(FSelected.Width / 2)));
            FSelected.Position.X:=(Position.X + round(Width / 2) - round(FSelected.Width / 2));
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.X - FSelected.Position.X) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + (Position.X - FSelected.Position.X);
            FSelected.Position.X:=Position.X;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.Y - FSelected.Position.Y) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + (Position.Y - FSelected.Position.Y);
            FSelected.Position.Y:=Position.Y;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.X - (FSelected.Position.X + FSelected.Width)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + (Position.X - (FSelected.Position.X + FSelected.Width));
            FSelected.Position.X:=Position.X - FSelected.Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs(Position.Y - (FSelected.Position.Y + FSelected.Height)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + (Position.Y - (FSelected.Position.Y + FSelected.Height));
            FSelected.Position.Y:=Position.Y - FSelected.Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + Width) - FSelected.Position.X) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + Width) - FSelected.Position.X);
            FSelected.Position.X:=Position.X + Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.Y + Height) - FSelected.Position.Y) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + Height) - FSelected.Position.Y);
            FSelected.Position.Y:=Position.Y + Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.X + Width) - (FSelected.Position.X + FSelected.Width)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.X:=FMousePos.X + ((Position.X + Width) - (FSelected.Position.X + FSelected.Width));
            FSelected.Position.X:=Position.X + Width - FSelected.Width;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
        if (Abs((Position.Y + Height) - (FSelected.Position.Y + FSelected.Height)) < SnapLineSize) then
        begin
          if AllowChangePosition then
          begin
            FMousePos.Y:=FMousePos.Y + ((Position.Y + Height) - (FSelected.Position.Y + FSelected.Height));
            FSelected.Position.Y:=Position.Y + Height - FSelected.Height;
          end;
          AddGridLine(TD2VisualObject(FSelected.Parent.Children[i]));
          Continue;
        end;
      end;
    end;
  AddUpdateRectsFromGridLines;
end;

function TD2CustomScene.SnapPointToGridLines(const APoint:TD2Point):TD2Point;
var
  i:integer;
begin
  Result:=APoint;
  if not DesignSnapToLines then Exit;
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  SnapToGridLines(false);
  if Length(FDesignGridLines) > 0 then
  begin
    Result:=FSelected.LocalToAbsolute(APoint);
    Result:=TD2VisualObject(FSelected.Parent).AbsoluteToLocal(Result);
    for i:=0 to High(FDesignGridLines) do
    begin
      if Abs(Result.X - FDesignGridLines[i].Position.X) < (4) then
        Result.X:=FDesignGridLines[i].Position.X;
      if Abs(Result.Y - FDesignGridLines[i].Position.Y) < (4) then
        Result.Y:=FDesignGridLines[i].Position.Y;
      if Abs(Result.X - (FDesignGridLines[i].Position.X + FDesignGridLines[i].Width)) < (4) then
        Result.X:=FDesignGridLines[i].Position.X + FDesignGridLines[i].Width;
      if Abs(Result.Y - (FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height)) < (4) then
        Result.Y:=FDesignGridLines[i].Position.Y + FDesignGridLines[i].Height;
    end;
    Result:=TD2VisualObject(FSelected.Parent).LocalToAbsolute(Result);
    Result:=FSelected.AbsolutetoLocal(Result);
  end;
end;

procedure TD2CustomScene.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:integer);
var
  P:TD2Point;
  R:TD2Rect;
  Obj:TD2VisualObject;
  SG:Id2SizeGrip;
  i:integer;
begin
  inherited;
  if not (FDesignTime) and not Assigned(FOnFlush) and not (csDesigning in ComponentState) then
    SetFocus;
  { translate coord }
  FUnsnapMousePos:=d2Point(x,y);
  FMousePos:=d2Point(SnapToGridValue(x),SnapToGridValue(y));
  FDownPos:=FMousePos;
  SetLength(FDesignGridLines,0);

  if FDesignTime then
  begin
    { Create root }
    if ((FChildren=nil) or (FChildren.Count=0)) then
    begin
      Obj:=TD2Background.Create(Owner);
      if GvarD2Designer <> nil then
        Obj.Name:=GvarD2Designer.UniqueName(Owner,'Root');
      AddObject(Obj);
      RealignRoot;
    end;
    { Popup }
    if Button=mbRight then
      OpenDesignPopup;
    { Resize }
    if (FSelected <> nil) and (not FSelected.DisableDesignResize) then
    begin
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(FSelected.Width / 2 - (cnGripSize),- cnRotSize - (cnGripSize),
          (FSelected.Width) / 2 +(cnGripSize),- cnRotSize + (cnGripSize));
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRotate:=true;
        FMoving:=false;
        Exit;
      end;
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(-cnGripSize,-cnGripSize,cnGripSize,cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeftTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right -cnGripSize,-cnGripSize,R.Right + cnGripSize,cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRightTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-cnGripSize,R.Bottom - cnGripSize,cnGripSize,R.Bottom + cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeftBottom:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right - cnGripSize,R.Bottom - cnGripSize,R.Right + cnGripSize,R.Bottom + cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRightBottom:=true;
        FMoving:=false;
        Exit;
      end;

      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - cnGripSize,-cnGripSize,d2RectWidth(R)/2 + cnGripSize,cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FTop:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - cnGripSize,R.Bottom - cnGripSize,d2RectWidth(R)/2 + cnGripSize,R.Bottom + cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FBottom:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-cnGripSize,d2RectHeight(R)/2 - cnGripSize,cnGripSize,d2RectHeight(R)/2 + cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FLeft:=true;
        FMoving:=false;
        Exit;
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right-cnGripSize,d2RectHeight(R)/2 - cnGripSize,R.Right + cnGripSize,d2RectHeight(R)/2 + cnGripSize);
      if d2PtInRect(P,R) then
      begin
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,FSelected,[]);
        if Assigned(FDesignChangeSelection) then
          FDesignChangeSelection(Self);
        FRight:=true;
        FMoving:=false;
        Exit;
      end;
    end;
    { Change Selected }
    Obj:=ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y);
    if (Obj <> nil) and (Obj=FSelected) and (ssDouble in Shift) then
    begin
      TD2VisualObject(Obj).DesignClick;
    end;
    if (Obj <> nil) then
    begin
      if (ssCtrl in Shift) and (Obj <> Selected) then
      begin
        { check is exists }
        for i:=0 to High(FSelection) do
          if FSelection[i]=Obj then
          begin
            FSelection[i]:=Selected;
            Obj:=nil;
          end;
        if Obj <> nil then
        begin
          SetLength(FSelection,Length(FSelection) + 1);
          FSelection[High(FSelection)]:=Obj;
        end;
        if GvarD2Designer <> nil then
          GvarD2Designer.SelectObject(Owner,Selected,FSelection);
        Exit;
      end;
      SetLength(FSelection,0);
      if FSelected <> nil then
      begin
        TD2VisualObject(FSelected).RecalcUpdateRect;
        FSelected.Repaint;
      end;
      FSelected:=Obj;
      TD2VisualObject(FSelected).DesignSelect;
      { Select in IDE }
      if GvarD2Designer <> nil then
        GvarD2Designer.SelectObject(Owner,Obj,[]);
      if Assigned(FDesignChangeSelection) then
        FDesignChangeSelection(Self);
      { }
      TD2VisualObject(FSelected).RecalcUpdateRect;
      FSelected.Repaint;

      if (Obj=FSelected) then
        FMoving:=true;
    end;
    Exit;
  end;
  { event }
  if not FDesignTime then
  begin
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
      if (TD2VisualObject(Obj).QueryInterface(Id2SizeGrip,SG)=0) and (Assigned(SG)) then
      begin
        BeginResize;
      end
      else
      begin
        P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
        TD2VisualObject(Obj).MouseDown(Button,Shift,P.X,P.Y);
        if (TD2VisualObject(Obj).FDragMode=d2DragAutomatic) then
        begin
          TD2VisualObject(Obj).BeginAutoDrag;
        end;
      end;
    end
    else
      BeginDrag;
  end;
end;

procedure TD2CustomScene.MouseMove(Shift:TShiftState; X,Y:integer);
var
  R:TD2Rect;
  P,P1:TD2Point;
  Obj:TD2VisualObject;
  SG:Id2SizeGrip;
  NewCursor:TCursor;
  i:integer;
begin
  inherited ;
  NewCursor:=FLoadCursor;
  { drag }
  if FDragging then
  begin
    with TWinControl(Owner) do
      SetBounds(Round(Left + (X - FDownPos.X)),Round(Top + (Y - FDownPos.Y)),Width,Height);
    Exit;
  end;
  if FResizing then
  begin
    FResizeSize.X:=Round(FResizeSize.X + (X - FUnsnapMousePos.X));
    FResizeSize.Y:=Round(FResizeSize.Y + (Y - FUnsnapMousePos.Y));
    with TWinControl(Owner) do
      SetBounds(Left,Top,FResizeSize.X,FResizeSize.Y);
    Cursor:=crSizeNWSE;
    FUnsnapMousePos:=d2Point(x,y);
    Exit;
  end;
  { translate coord }
  FMousePos:=d2Point(SnapToGridValue(x),SnapToGridValue(y));
  FUnsnapMousePos:=d2Point(x,y);

  if FDesignTime then
  begin
    { change cursor }
    if (FSelected <> nil) then
    begin
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(FSelected.Width / 2 - (cnGripSize),- cnRotSize - (cnGripSize),
        (FSelected.Width) / 2 +(cnGripSize),- cnRotSize + (cnGripSize));
      if FRotateHot <> d2PtInRect(P,R) then
      begin
        FRotateHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      P:=FSelected.AbsoluteToLocal(d2Point(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      R:=d2Rect(-cnGripSize,-cnGripSize,cnGripSize,cnGripSize);
      if FLeftTopHot <> d2PtInRect(P,R) then
      begin
        FLeftTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right -cnGripSize,-cnGripSize,R.Right + cnGripSize,cnGripSize);
      if FRightTopHot <> d2PtInRect(P,R) then
      begin
        FRightTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-cnGripSize,R.Bottom - cnGripSize,cnGripSize,R.Bottom + cnGripSize);
      if FLeftBottomHot <> d2PtInRect(P,R) then
      begin
        FLeftBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right - cnGripSize,R.Bottom - cnGripSize,R.Right + cnGripSize,R.Bottom + cnGripSize);
      if FRightBottomHot <> d2PtInRect(P,R) then
      begin
        FRightBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;

      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - cnGripSize,-cnGripSize,d2RectWidth(R)/2 + cnGripSize,cnGripSize);
      if FTopHot <> d2PtInRect(P,R) then
      begin
        FTopHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(d2RectWidth(R)/2 - cnGripSize,R.Bottom - cnGripSize,d2RectWidth(R)/2 + cnGripSize,R.Bottom + cnGripSize);
      if FBottomHot <> d2PtInRect(P,R) then
      begin
        FBottomHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(-cnGripSize,d2RectHeight(R)/2 - cnGripSize,cnGripSize,d2RectHeight(R)/2 + cnGripSize);
      if FLeftHot <> d2PtInRect(P,R) then
      begin
        FLeftHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
      R:=FSelected.BoundsRect;
      R:=d2Rect(R.Right-cnGripSize,d2RectHeight(R)/2 - cnGripSize,R.Right + cnGripSize,d2RectHeight(R)/2 + cnGripSize);
      if FRightHot <> d2PtInRect(P,R) then
      begin
        FRightHot:=d2PtInRect(P,R);
        R.TopLeft:=FSelected.LocaltoAbsolute(R.TopLeft);
        R.BottomRight:=FSelected.LocaltoAbsolute(R.BottomRight);
        d2InflateRect(R,2,2);
        AddUpdateRect(R);
      end;
    end;
    { resize and move }
    if (ssLeft in Shift) and (FSelected <> nil) and (FMoving) then
    begin
      P:=d2Point(FUnsnapMousePos.X - FDownPos.X,FUnsnapMousePos.Y - FDownPos.Y);
      if (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
      begin
        with TD2VisualObject(FSelected.Parent).AbsoluteToLocalVector(d2Vector(P.X,P.Y)) do
          P:=d2Point(X,Y);
      end
      else
      begin
        with FSelected.AbsoluteToLocalVector(d2Vector(P.X,P.Y)) do
          P:=d2Point(X,Y);
      end;
      FSelected.Position.X:=SnapToGridValue(FSelected.Position.X + P.X);
      FSelected.Position.Y:=SnapToGridValue(FSelected.Position.Y + P.Y);
      { lines grid }
      SnapToGridLines(true);
      { check place }
      TD2VisualObject(FSelected).FLocked:=true;
      Obj:=TD2VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
      TD2VisualObject(FSelected).FLocked:=false;
      { select }
      if (Obj <> nil) and (Obj <> FSelected.Parent) and (Obj <> FSelected) then
      begin
        if FDesignPlaceObject <> nil then
          FDesignPlaceObject.Repaint;
        FDesignPlaceObject:=Obj;
        if FDesignPlaceObject <> nil then
        begin
          TD2VisualObject(FDesignPlaceObject).RecalcUpdateRect;
          FDesignPlaceObject.Repaint;
        end;
        if (ssAlt in Shift) then
        begin
          P:=FSelected.LocalToAbsolute(d2Point(0,0));
          FSelected.Parent:=FDesignPlaceObject;
          P:=FDesignPlaceObject.AbsoluteToLocal(P);
          FSelected.Position.X:=P.X;
          FSelected.Position.Y:=P.Y;
        end;
      end
      else
      begin
        if FDesignPlaceObject <> nil then
          FDesignPlaceObject.Repaint;
        FDesignPlaceObject:=nil;
      end;
    end;
    if (ssLeft in Shift) and (FSelected <> nil) then
    begin
      if (ssLeft in Shift) and (FSelected <> nil) and (FRotate) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P1:=FSelected.AbsoluteToLocal(FDownPos);
        if d2VectorCrossProductZ(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
          d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2))) < 0
        then
          FSelected.RotateAngle:=FSelected.RotateAngle + d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
            d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2)))))
        else
          FSelected.RotateAngle:=FSelected.RotateAngle - d2RadToDeg(ArcCos(d2VectorAngleCosine(d2Vector(P.X - (FSelected.Width / 2),P.Y - (FSelected.Height / 2)),
            d2Vector(P1.X - (FSelected.Width / 2),P1.Y - (FSelected.Height / 2)))));
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FLeftTop) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(P.X,P.Y,
          R.Right,R.Bottom);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FRightTop) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(R.Left,P.Y,
          P.X,R.Bottom);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FLeftBottom) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(P.X,R.Top,
          R.Right,P.Y);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FRightBottom) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(R.Left,R.Top,P.X,P.Y);
      end;

      if (ssLeft in Shift) and (FSelected <> nil) and (FTop) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(R.Left,P.Y,R.Right,R.Bottom);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FBottom) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(R.Left,R.Top,R.Right,P.Y);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FLeft) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(P.X,R.Top,R.Right,R.Bottom);
      end;
      if (ssLeft in Shift) and (FSelected <> nil) and (FRight) then
      begin
        R:=FSelected.BoundsRect;
        P:=FSelected.AbsoluteToLocal(FMousePos);
        P:=SnapPointToGridLines(P);
        FSelected.BoundsRect:=d2Rect(R.Left,R.Top,P.X,R.Bottom);
      end;
      FDownPos:=FMousePos;
    end;
    Exit;
  end;
  { event }
  if not FDesignTime then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2VisualObject(FCaptured).MouseMove(Shift,P.X,P.Y,0,0);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
      if (Obj.ShowHint) then
        Hint:=Obj.Hint
      else
        Hint:='';

      if ((TD2VisualObject(Obj).QueryInterface(Id2SizeGrip,SG)=0) and Assigned(SG)) then
        NewCursor:=crSizeNWSE
      else
        NewCursor:=Obj.Cursor;

      if (Obj <> FHovered) then
      begin
        if FHovered <> nil then
        begin
          TD2VisualObject(FHovered).MouseInObject:=false;
          TD2VisualObject(FHovered).MouseLeave;
          TD2VisualObject(FHovered).RemoveFreeNotify(Self);
        end;
        FHovered:=Obj;
        TD2VisualObject(FHovered).MouseInObject:=true;
        TD2VisualObject(FHovered).MouseEnter;
        TD2VisualObject(FHovered).AddFreeNotify(Self);
      end;

      P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2VisualObject(Obj).MouseMove(Shift,P.X,P.Y,0,0);
    end
    else
    begin
      if FHovered <> nil then
      begin
        TD2VisualObject(FHovered).MouseInObject:=false;
        TD2VisualObject(FHovered).MouseLeave;
        TD2VisualObject(FHovered).RemoveFreeNotify(Self);
        FHovered:=nil;
      end;
    end;
  end;
  Cursor:=NewCursor;
  FDownPos:=FMousePos;
end;

procedure TD2CustomScene.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:integer);
var
  P:TD2Point;
  Obj:TD2VisualObject;
begin
  inherited;

  if FDesignPlaceObject <> nil then
    FDesignPlaceObject.Repaint;
  FDesignPlaceObject:=nil;
  AddUpdateRectsFromGridLines;
  SetLength(FDesignGridLines,0);
  if (FSelected <> nil) and FMoving then
  begin
    if (FSelected.Parent <> nil) and (TD2Object(FSelected.Parent).IsVisual) then
      TD2VisualObject(FSelected.Parent).Realign;
    if (GvarD2Designer <> nil) then
      GvarD2Designer.Modified(Owner);
    if FSelected.Parent=nil then
      RealignRoot;
  end;
  if (FSelected <> nil) and (FLeftTop or FLeftBottom or FLeftBottom or FRightBottom) then
  begin
    if (FSelected.Parent <> nil) and (TD2Object(FSelected.Parent).IsVisual) then
      TD2VisualObject(FSelected.Parent).Realign;
    if (GvarD2Designer <> nil) then
      GvarD2Designer.Modified(Owner);
    if FSelected.Parent=nil then
      RealignRoot;
  end;
  FMoving:=false;
  FLeftTop:=false;
  FLeftBottom:=false;
  FRightTop:=false;
  FRightBottom:=false;
  FTop:=false;
  FBottom:=false;
  FLeft:=false;
  FRight:=false;
  FRotate:=false;
  { drag }
  if FDragging then
  begin
    FDragging:=false;
//    ReleaseCapture;
  end;
  if FResizing then
  begin
    FResizing:=false;
//    ReleaseCapture;
  end;
  { event }
  if not FDesignTime then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2VisualObject(FCaptured).MouseUp(Button,Shift,P.X,P.Y);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) then
    begin
      P:=Obj.AbsoluteToLocal(d2Point(FMousePos.X,FMousePos.Y));
      TD2VisualObject(Obj).MouseUp(Button,Shift,P.X,P.Y);
    end;
  end;
end;

function TD2CustomScene.DoMouseWheel(Shift:TShiftState; WheelDelta:integer;
   MousePos:TPoint):boolean;
var
  Obj:TD2VisualObject;
begin
  Result:=inherited DoMouseWheel(Shift,WheelDelta,MousePos);
  { event }
  if not FDesignTime then
  begin
    {$IFDEF DARWIN}
//    WheelDelta:=WheelDelta * 40;
    {$ENDIF}
    if (FCaptured <> nil) then
    begin
      TD2VisualObject(FCaptured).MouseWheel(Shift,WheelDelta,Result);
      Exit;
    end;
    Obj:=TD2VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    while (Obj <> nil) do
    begin
      TD2VisualObject(Obj).MouseWheel(Shift,WheelDelta,Result);
      if Result then Break;
      if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
        Obj:=TD2VisualObject(Obj.Parent)
      else
        Obj:=nil;
    end;
  end;
end;

procedure TD2CustomScene.KeyDown(var Key:Word; Shift:TShiftState);
var
  List:TList;
  i,CurIdx:integer;
  Found:boolean;
  K:Word;
  O:TComponent;
  Ch,Char:System.WideChar;
begin
  inherited;
  FShift:=Shift;
  if ssMeta in Shift then
  begin
    K:=0;
    case Key of
      67:// ctrl+c
      begin
        Ch:='c';
        UnicodeKeyDown(K,Ch,[ssCtrl]);
        UnicodeKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      86:// ctrl+v
      begin
        Ch:='v';
        UnicodeKeyDown(K,Ch,[ssCtrl]);
        UnicodeKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      88:// ctrl+x
      begin
        Ch:='x';
        UnicodeKeyDown(K,Ch,[ssCtrl]);
        UnicodeKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      99:// ctrl+z
      begin
        Ch:='z';
        UnicodeKeyDown(K,Ch,[ssCtrl]);
        UnicodeKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
    end;
  end;
  { dialog key }
  if (Key=VK_ESCAPE) or (Key=VK_RETURN) or (Key=VK_LEFT) or (Key=VK_RIGHT) or (Key=VK_UP) or (Key=VK_DOWN) or
     (Key=VK_HOME) or (Key=VK_END) or (Key=VK_ADD) or (Key=VK_SUBTRACT) then
  begin
    TD2VisualObject(Root).DialogKey(Key,Shift);
    if Key=0 then Exit;
  end;
  { modal }
  if (Key=VK_ESCAPE) then
  begin
    O:=Owner;
    while O <> nil do
    begin
      if (O is TCustomForm) and (fsModal in TCustomForm(O).FormState) then
      begin
        TCustomForm(O).ModalResult:=mrCancel;
        Key:=0;
        Break;
      end;
      O:=O.Owner;
    end;
    if Key=0 then Exit;
  end;
  { change focus }
  if (Key=VK_TAB) and (Root <> nil) then
  begin
    Key:=0;
    List:=TList.Create;
    TD2VisualObject(Root).GetTabOrderList(List,true);

    Found:=false;
    if ssShift in Shift then
    begin
      { second search in first part of list }
      if FFocused <> nil then
        CurIdx:=List.IndexOf(FFocused) - 1
      else
        CurIdx:=List.Count - 1;
      if CurIdx > 0 then
          for i:=CurIdx - 1 downto 0 do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
      { first search in last part of list }
      if not Found then
        if (List.Count > 2) and (CurIdx < List.Count) then
          for i:=List.Count - 1 downto CurIdx do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Break;
            end;
    end
    else
    begin
      if FFocused <> nil then
        CurIdx:=List.IndexOf(FFocused) + 1
      else
        CurIdx:=0;
      { first search in last part of list }
      if (List.Count > 2) and (CurIdx < List.Count) then
        for i:=CurIdx to List.Count - 1 do
          if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
          begin
            TD2VisualObject(List[i]).SetFocus;
            Found:=true;
            Break;
          end;
      { second search in first part of list }
      if not Found then
        if CurIdx > 0 then
          for i:=0 to CurIdx - 1 do
            if TD2Object(List[i]).isVisual and (TD2VisualObject(List[i]).CheckParentVisible) and (TD2VisualObject(List[i]).CanFocused) and (TD2VisualObject(List[i]).AbsoluteEnabled) then
            begin
              TD2VisualObject(List[i]).SetFocus;
              Break;
            end;
    end;
    List.Free;
    Exit;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    Char:=#0;
    FFocused.KeyDown(Key,Char,Shift);
  end;

end;

procedure TD2CustomScene.UTF8KeyPress(var UTF8Key:TUTF8Char);
var
  S:System.WideChar;
  K:word;
  C:System.WideChar;
begin
  K:=0;
  C:=UTF8ToUTF16(UTF8Key)[1];
  case C of
        #3:// ctrl+c
          begin
            C:='c';
            UnicodeKeyDown(K,C,FShift);
            UnicodeKeyUp(K,C,FShift);
          end;
        #$16:// ctrl+v
          begin
            C:='v';
            UnicodeKeyDown(K,C,FShift);
            UnicodeKeyUp(K,C,FShift);
          end;
        #$18:// ctrl+x
          begin
            C:='x';
            UnicodeKeyDown(K,C,FShift);
            UnicodeKeyUp(K,C,FShift);
          end;
        #$1A:// ctrl+z
          begin
            C:='z';
            UnicodeKeyDown(K,C,FShift);
            UnicodeKeyUp(K,C,FShift);
          end;
  else
    UnicodeKeyDown(K,C,FShift);
    UnicodeKeyUp(K,C,FShift);
  end;
  UTF8Key:='';
end;

procedure TD2CustomScene.KeyPress(var Key:char);
begin
  inherited KeyPress(Key);
end;

procedure TD2CustomScene.KeyUp(var Key:Word; Shift:TShiftState);
var
  Char:system.WideChar;
begin
  inherited;
  FShift:=[];
  if FDesignTime then
  begin
    if (Key=VK_DELETE) and (FSelected <> nil) then
    begin
      FSelected.Free;
      FSelected:=nil;
    end;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    Char:=#0;
    FFocused.KeyUp(Key,char,Shift);
  end;
end;


procedure TD2CustomScene.RealignRoot;
begin
  if (FChildren <> nil) and (FChildren.Count > 0) and (TD2Object(FChildren[0]).isVisual) then
    with TD2VisualObject(FChildren[0]) do
    begin
      if (RotateAngle=90) or (RotateAngle=-90) or (RotateAngle=-270) or (RotateAngle=270) then
        SetBounds((Self.Width - Self.Height) / 2,-(Self.Width - Self.Height) / 2,Self.Height / Scale.Y,Self.Width / Scale.X)
      else
        SetBounds(0,0,Self.Width / Scale.X,Self.Height / Scale.Y);
    end;
end;

procedure TD2CustomScene.SetBounds(ALeft,ATop,AWidth,AHeight:integer);
begin
  inherited;
  if (Canvas <> nil) and (HandleAllocated) then
  begin
    Canvas.ResizeBuffer(AWidth,AHeight);
    RealignRoot;
  end;
end;

procedure TD2CustomScene.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FActiveControl) then
    FActiveControl:=nil;
  if (Operation=opRemove) and (AComponent=FCaptured) then
    FCaptured:=nil;
  if (Operation=opRemove) and (AComponent=FSelected) then
    FSelected:=nil;
  if (Operation=opRemove) and (AComponent=FHovered) then
    FHovered:=nil;
  if (Operation=opRemove) and (AComponent=FFocused) then
    FFocused:=nil;
  if (Operation=opRemove) and (AComponent=FDesignPlaceObject) then
    FDesignPlaceObject:=nil;
  if (Operation=opRemove) and (AComponent=FStyle) then
    Style:=nil;
  if (Operation=opRemove) and (AComponent=FOpenInFrame) then
    FOpenInFrame:=nil;
end;

procedure TD2CustomScene.SetFill(const Value:TD2Brush);
begin
  FFill.Assign(Value);
end;

procedure TD2CustomScene.FillChanged(Sender:TObject);
begin
  SetLength(FUpdateRects,0);
  AddUpdateRect(d2Rect(0,0,Width,Height));
end;

procedure TD2CustomScene.SetSnapGridShow(const Value:boolean);
begin
  if FSnapGridShow <> Value then
  begin
    FSnapGridShow:=Value;
    SetLength(FUpdateRects,0);
    AddUpdateRect(d2Rect(0,0,Width,Height));
  end;
end;

procedure TD2CustomScene.SetSnapGridSize(const Value:single);
begin
  if FSnapGridSize <> Value then
  begin
    FSnapGridSize:=Value;
    if FSnapGridSize < 0.01 then
      FSnapGridSize:=0.01;
    if FsnapGridShow then
      Repaint;
  end;
end;

procedure TD2CustomScene.InsertObject(const ClassName:string);  // 7777
var
  P:TPoint;
  Obj:TD2Object;
  OldSel:TD2VisualObject;
  InsertPos:TD2Point;
begin
  if GetClass(ClassName) <> nil then
  begin
    if GetClass(ClassName).InheritsFrom(TD2Object) then
    begin
      if (FSelected=nil) and (Root <> nil) and (Root.IsVisual) then
        FSelected:=Root.Visual;
      if FSelected <> nil then
      begin
        try
          Obj:=TD2ObjectClass(GetClass(ClassName)).Create(Owner);
          OldSel:=FSelected;
          FSelected.AddObject(Obj);

          if GvarD2Designer <> nil then
          begin
            Obj.Name:=GvarD2Designer.UniqueName(Owner,Obj.ClassName);
            GvarD2Designer.AddObject(Obj);
          end;

          if GvarD2Designer <> nil then
          begin
            GvarD2Designer.SelectObject(Owner,Obj,[]);
            if Assigned(FDesignChangeSelection) then FDesignChangeSelection(Self);
            GvarD2Designer.Modified(Owner);
          end;

          //.....
          if Obj.IsVisual then
          begin
            if GetPropInfo(Obj.ClassInfo,'Text',[tkString,tkLString,tkWString]) <> nil then
              SetStrProp(Obj,'Text',Copy(Obj.ClassName,4,Length(Obj.ClassName)));

            if (Owner is TWinControl) and (FPopupPos.X > 0) then
            begin
              if Pos('Item',Obj.ClassName)=0 then
              begin
                P:=ScreenToClient(FPopupPos);
                InsertPos:=OldSel.AbsoluteToLocal(d2Point(P.X,P.Y));

                TD2VisualObject(Obj).Position.X:=InsertPos.X;
                TD2VisualObject(Obj).Position.Y:=InsertPos.Y;

              end;
            end;
            FPopupPos:=Point(-1,-1);
            FSelected:=TD2VisualObject(Obj);
            TD2VisualObject(FSelected).DesignInsert;
            TD2VisualObject(FSelected).DesignSelect;
          end;

          if OldSel.IsVisual then OldSel.Visual.Realign;

        finally
        end;
      end
      else
        if (FChildren=nil) or (FChildren.Count=0) then
        begin
          // insert root object
           try
            Obj:=TD2ObjectClass(GetClass(ClassName)).Create(Owner);
            if GvarD2Designer <> nil then
              Obj.Name:=GvarD2Designer.UniqueName(Owner,Obj.ClassName);
            AddObject(Obj);
            if GvarD2Designer <> nil then
            begin
              GvarD2Designer.SelectObject(Owner,Obj,[]);
              if Assigned(FDesignChangeSelection) then  FDesignChangeSelection(Self);
              GvarD2Designer.Modified(Owner);
            end;
            if Obj.IsVisual then
            begin
              if (Owner is TWinControl) and (FPopupPos.X > 0) then
              begin
                P:=TWinControl(Owner).ScreenToClient(FPopupPos);
                InsertPos:=d2Point(P.X,P.Y);
                TD2VisualObject(Obj).Position.X:=P.X;
                TD2VisualObject(Obj).Position.Y:=P.Y;
              end;
              FPopupPos:=Point(-1,-1);
              FSelected:=TD2VisualObject(Obj);
              TD2VisualObject(FSelected).DesignSelect;
            end;
          finally
          end;
        end;
    end
    else
    begin
      FInsertObject:=ClassName;
    end;
  end
  else
    FInsertObject:='';
end;

procedure TD2CustomScene.Resize;
begin
  inherited;
end;

procedure TD2CustomScene.doDesignPopupDesignHide(Sender:TObject);
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    FSelected.DesignHide:=not FSelected.DesignHide;
    if FSelected.DesignHide and (FSelected.Parent.IsVisual) then
    begin
      FSelected:=TD2VisualObject(FSelected.Parent); 
    end;
    AddUpdateRect(d2Rect(0,0,Width,Height));
  end;
end;

procedure TD2CustomScene.doDesignPopupDel(Sender:TObject);
var
  Obj:TD2VisualObject;
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    Obj:=FSelected;
    if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
    begin
      FSelected:=TD2VisualObject(Obj.Parent);
      TD2VisualObject(FSelected).DesignSelect;
    end
    else
    begin
      FSelected:=TD2VisualObject(Root);
      if FSelected <> nil then
        TD2VisualObject(FSelected).DesignSelect;
    end;
    Obj.Free;
  end;
end;

procedure TD2CustomScene.doDesignPopupAddItem(Sender:TObject);
begin
  if FSelected <> nil then
  begin
    InsertObject(TMenuItem(Sender).Hint);
  end
end;

function StripHotkey(S:string):string;
begin
  Result:=S;
end;

procedure TD2CustomScene.doDesignPopupAdd(Sender:TObject);
var
  S:string;
begin
  S:=StripHotkey(TMenuItem(Sender).Caption);
  if (S <> '') then
    S:='TD2' + S;
  InsertObject(S);
end;

procedure TD2CustomScene.doDesignPopupReorder(Sender:TObject);
begin
  if FSelected=nil then Exit;

  if StripHotkey(TMenuItem(Sender).Caption)='Bring to front' then
    FSelected.BringToFront;
  if StripHotkey(TMenuItem(Sender).Caption)='Send to back' then
    FSelected.SendToBack;
end;

procedure TD2CustomScene.doDesignPopupGrid(Sender:TObject);
begin
  case TMenuItem(Sender).Tag of
    1:FSnapGridShow:=TMenuItem(Sender).Checked;
    2:
      begin
        FSnapToGrid:=TMenuItem(Sender).Checked;
        FSnapToLines:=not FSnapToGrid;
      end;
    3:
      begin
        FSnapToLines:=TMenuItem(Sender).Checked;
        FSnapToGrid:=not FSnapToLines;
      end;
    4:
      begin
        FSnapToLines:=false;
        FSnapToGrid:=false;
      end;
  end;
  SetLength(FUpdateRects,0);
  AddUpdateRect(d2Rect(0,0,Width,Height));
end;

var
  ClipboardText:AnsiString;

procedure TD2CustomScene.doDesignPopupCopy(Sender:TObject);
var
  S:TStringStream;
begin
  if FSelected <> nil then
  begin
    S:=TStringStream.Create('');
    FSelected.SaveToStream(S);
    ClipboardText:=S.DataString;
    S.Free;
  end;
end;

procedure TD2CustomScene.doDesignPopupPaste(Sender:TObject);
 procedure SetUniqueName(Obj:TD2Object);
 var
   i:integer;
 begin
   Obj.Name:=GvarD2Designer.UniqueName(Owner,Obj.ClassName);
   if Obj.IsVisual and Obj.Visual.Locked then Exit;
   for i:=0 to Obj.ChildrenCount - 1 do
     SetUniqueName(Obj.Children[i]);
 end;
var
  S:TStringStream;
  Result:TD2Object;
begin
  if ClipboardText <> '' then
  begin
    S:=TStringStream.Create(ClipboardText);
    Result:=CreateObjectFromStream(Owner,S);
    if FSelected <> nil then
      Result.Parent:=FSelected
    else
      Result.Parent:=Root;
    if GvarD2Designer <> nil then
    begin
      SetUniqueName(Result);
      GvarD2Designer.SelectObject(Owner,Result,[]);
    end;
    S.Free;
  end;
end;

procedure TD2CustomScene.doDesignPopupLoadFromFile(Sender:TObject);
var
  S:TStream;
  Result:TD2Object;
  Open:TOpenDialog;
begin
  Open:=TOpenDialog.Create(nil);
  Open.Filter:='d2Scene Files|*.d2scene;*.d2';
  if Open.Execute then
  begin
    S:=TFileStream.Create(Open.FileName,fmOpenRead);
    Result:=CreateObjectFromStream(Owner,S);
    if FSelected <> nil then
      Result.Parent:=FSelected
    else
      Result.Parent:=Root;
    if GvarD2Designer <> nil then
      Result.Name:=GvarD2Designer.UniqueName(Owner,Result.ClassName);
    S.Free;
  end;
  Open.Free;
end;

procedure TD2CustomScene.doDesignPopupCreateStyle(Sender:TObject);
var
  R:TD2Resources;
  CurrentRes,Link:TD2VisualObject;
  LinkedList:TList;
  i:integer;
begin
  { Edit Style }
  if FSelected=nil then Exit;
  if not Assigned(FSelected.Scene) then Exit;

  if (FSelected.Scene.GetStyle=nil) then
  begin
    // Create style
    R:=TD2Resources.Create(FSelected.Scene.GetOwner);
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
    FSelected.Scene.SetStyle(R);
  end
  else
    R:=FSelected.Scene.GetStyle;

  if (TD2Control(FSelected).Resource='') and (GvarD2Designer <> nil) then
  begin
    TD2Control(FSelected).Resource:=GvarD2Designer.UniqueName(Owner,FSelected.Name + 'Style');
  end;
  if (R.Root=nil) then
  begin
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
  end;
  if (R.Root.FindResource(TD2Control(FSelected).Resource)=nil) then
  begin
    // Clone Resource from Current
    CurrentRes:=TD2Control(FSelected).GetResourceObject;
    if CurrentRes=nil then Exit; 
    CurrentRes.Align:=vaNone;
    CurrentRes.Parent:=R.Root;
    if GvarD2Designer <> nil then
    begin
      CurrentRes.ResourceName:=GvarD2Designer.UniqueName(Owner,FSelected.Name + 'Style');
      TD2Control(FSelected).Resource:=CurrentRes.ResourceName;
    end
    else
    begin
      CurrentRes.ResourceName:=FSelected.Name;
      TD2Control(FSelected).Resource:=CurrentRes.ResourceName;
    end;

    // Add linked object to resource - like ListBox with ScrollBar
    LinkedList:=TList.Create;
    TD2Control(CurrentRes).AddControlsToList(LinkedList);
    for i:=0 to LinkedList.Count - 1 do
    begin
      if R.Root.FindResource(TD2Control(LinkedList[i]).Resource)=nil then
      begin
        Link:=TD2Control(LinkedList[i]).GetResourceObject;
        Link.Align:=vaNone;
        Link.Parent:=R.Root;
        if GvarD2Designer <> nil then
        begin
          Link.ResourceName:=GvarD2Designer.UniqueName(Owner,FSelected.Name + TD2Control(LinkedList[i]).ResourceName);
          TD2Control(LinkedList[i]).Resource:=Link.ResourceName;
        end;
      end;
    end;
    LinkedList.Free;
  end;

  if GvarD2Designer <> nil then
  begin
    // Edit Style
    GvarD2Designer.EditStyle(R,TD2Control(FSelected).Resource);
  end;
end;

procedure TD2CustomScene.doDesignPopupEditStyle(Sender:TObject);
var
  R:TD2Resources;
  CurrentRes,Link:TD2VisualObject;
  LinkedList:TList;
  i:integer;
  StyleName:string;
begin
  { Edit Style }
  if FSelected=nil then Exit;
  if not Assigned(FSelected.Scene) then Exit;

  if (FSelected.Scene.GetStyle=nil) then
  begin
    // Create style
    R:=TD2Resources.Create(FSelected.Scene.GetOwner);
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
    FSelected.Scene.SetStyle(R);
  end
  else
    R:=FSelected.Scene.GetStyle;

  // Clone Resource from Current
  StyleName:=TD2Control(FSelected).Resource;
  if StyleName='' then
  begin
    StyleName:=FSelected.ClassName + 'Style';
    Delete(StyleName,1,3);
  end;
  if (R.Root=nil) then
  begin
    R.Root:=TD2Background.Create(nil);
    if GvarD2Designer <> nil then
      R.Name:=GvarD2Designer.UniqueName(Owner,R.ClassName);
  end;
  if (R.Root.FindResource(StyleName)=nil) then
  begin
    CurrentRes:=TD2Control(FSelected).GetResourceObject;
    if CurrentRes=nil then Exit;
    CurrentRes.Align:=vaNone;
    CurrentRes.Parent:=R.Root;

    // Add linked object to resource - like ListBox with ScrollBar
    LinkedList:=TList.Create;
    TD2Control(CurrentRes).AddControlsToList(LinkedList);
    for i:=0 to LinkedList.Count - 1 do
    begin
      StyleName:=TD2Control(LinkedList[i]).Resource;
      if StyleName='' then
      begin
        StyleName:=TD2Control(LinkedList[i]).ClassName + 'Style';
        Delete(StyleName,1,3);
      end;
      if R.Root.FindResource(StyleName)=nil then
      begin
        Link:=TD2Control(LinkedList[i]).GetResourceObject;
        Link.Align:=vaNone;
        Link.ResourceName:=StyleName;
        Link.Parent:=R.Root;
      end;
    end;
    LinkedList.Free;
  end;

  if GvarD2Designer <> nil then
  begin
    // Edit Style
    StyleName:=TD2Control(FSelected).Resource;
    if StyleName='' then
    begin
      StyleName:=FSelected.ClassName + 'Style';
      Delete(StyleName,1,3);
    end;
    GvarD2Designer.EditStyle(R,StyleName);
  end;
end;

procedure TD2CustomScene.doDesignTabOrderBtnClick(Sender:TObject);
var
  Obj:TD2VisualObject;
begin
  if TD2Button(Sender).TextW='Up' then
  begin
    Obj:=TD2VisualObject(TD2Listbox(TD2Button(Sender).TagObject).Selected.TagObject);
    Obj.TabOrder:=Obj.TabOrder - 1;
  end;
  if TD2Button(Sender).TextW='Down' then
  begin
    Obj:=TD2VisualObject(TD2Listbox(TD2Button(Sender).TagObject).Selected.TagObject);
    Obj.TabOrder:=Obj.TabOrder + 1;
  end;
  doDesignTabOrderRebuildList(TD2Listbox(TD2Button(Sender).TagObject));
end;

procedure TD2CustomScene.doDesignTabOrderRebuildList(ListBox:TD2VisualObject);
var
  Item:TD2ListboxItem;
  List:TD2ListBox;
  Idx,i:integer;
  L:TList;
begin
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  if not FSelected.Parent.IsVisual then Exit;
  List:=TD2ListBox(Listbox);
  if List.Selected <> nil then
    Idx:=TD2VisualObject(List.Selected.TagObject).TabOrder
  else
    Idx:=0;
  { add taborder list }
  List.BeginUpdate;
  List.Clear;
  L:=TList.Create;
  TD2VisualObject(FSelected.Parent).GetTabOrderList(L,false);
  for i:=0 to L.Count - 1 do
  begin
    Item:=TD2ListboxItem.Create(List.Owner);
    Item.Parent:=List;
    Item.TextW:=TD2VisualObject(L[i]).Name;
    if Item.TextW='' then
      Item.TextW:=TD2VisualObject(L[i]).ClassName;
    Item.TagObject:=TD2VisualObject(L[i]);
  end;
  L.Free;
  List.EndUpdate;
  List.ItemIndex:=Idx;
end;

procedure TD2CustomScene.doDesignPopupTabOrder(Sender:TObject);
var
  Form:TForm;
  Scene:TD2Scene;
  Wnd:TD2HudWindow;
  List:TD2Listbox;
  BtnUp,BtnDown:TD2Button;
begin
  if FSelected=nil then Exit;
  if FSelected.Parent=nil then Exit;
  if not FSelected.Parent.IsVisual then Exit;

  Form:=TForm.CreateNew(Application,0);
  Form.BorderStyle:=bsNone;
  Form.Position:=poScreenCenter;
  Form.SetBounds(0,0,300,400);
  Scene:=TD2Scene.Create(Form);
  Scene.Parent:=Form;
  Scene.Align:=alClient;
  Scene.Transparency:=true;
  Wnd:=TD2HudWindow.Create(Form);
  Wnd.ShowSizeGrip:=false;
  Wnd.TextW:='TabOrder Editor';
  Scene.AddObject(Wnd);

  BtnUp:=TD2HudButton.Create(Form);
  BtnUp.Parent:=Wnd;
  BtnUp.SetBounds(Form.Width - 70,50,50,26);
  BtnUp.TextW:='Up';
  BtnUp.OnClick:=doDesignTabOrderBtnClick;
  BtnDown:=TD2HudButton.Create(Form);
  BtnDown.Parent:=Wnd;
  BtnDown.SetBounds(Form.Width - 70,90,50,26);
  BtnDown.TextW:='Down';
  BtnDown.OnClick:=doDesignTabOrderBtnClick;

  List:=TD2HudListbox.Create(Form);
  List.Parent:=Wnd;
  List.Align:=vaClient;
  List.Padding.Rect:=d2Rect(26,45,86,33);
  List.HideSelectionUnfocused:=false;
  List.ItemHeight:=20;

  Form.SetBounds(0,0,300,400);
  BtnUp.TagObject:=List;
  BtnDown.TagObject:=List;
  doDesignTabOrderRebuildList(List);
  List.ItemIndex:=FSelected.TabOrder;
  Form.ShowModal;
  Form.Free;
end;

procedure TD2CustomScene.OpenDesignPopup;
var
  i,j:integer;
  S,T:AnsiString;
  OItem,SItem,Item:TMenuItem;
  CatList:TStringList;
begin
  if not DesignPopupEnabled then Exit;
  if FDesignPopup <> nil then
  begin
    FDesignPopup.Free;
    FDesignPopup:=nil;
  end;
  if FDesignPopup=nil then
  begin
    FDesignPopup:=TPopupMenu.Create(Self);

    if GvarD2ObjectList <> nil then
    begin
      { item }
      if (FSelected <> nil) and (FSelected.ItemClass <> '') then
      begin
        S:=FSelected.ItemClass;
        T:=d2GetToken(S,';');
        while T <> '' do
        begin
          if Pos('TD2',T)=1 then
            Delete(T,1,3);
          Item:=NewItem('Add ' + T,0,false,true,doDesignPopupAddItem,0,'');
          Item.Hint:='TD2' + T;
          FDesignPopup.Items.Add(Item);
          T:=d2GetToken(S,';');
        end;
      end;
      { add }
      Item:=NewItem('Add 2D object',0,false,true,nil,0,'');
      { add categories }
      for i:=0 to GvarD2ObjectList.Count - 1 do
      begin
        if Item.Find(GvarD2ObjectList[i])=nil then
        begin
          SItem:=NewItem(GvarD2ObjectList[i],0,false,true,nil,0,'');
          Item.Add(SItem);
        end;
      end;
      { add controls to list and sort }
      CatList:=TStringList.Create;
      CatList.Sorted:=true;
      for i:=0 to GvarD2ObjectList.Count - 1 do
      begin
        S:=TD2ObjectClass(GvarD2ObjectList.Objects[i]).ClassName;
        if Pos('TD2',S)=1 then
          Delete(S,1,3);
        CatList.Add(S);
      end;
      { add objects }
      for i:=0 to CatList.Count - 1 do
      begin
        OItem:=NewItem(CatList[i],0,false,true,doDesignPopupAdd,0,'');
        OItem.Tag:=$FF;
        for j:=0 to GvarD2ObjectList.Count - 1 do
        begin
          S:=TD2ObjectClass(GvarD2ObjectList.Objects[j]).ClassName;
          if Pos('TD2',S)=1 then
            Delete(S,1,3);
          if S=CatList[i] then
          begin
            SItem:=Item.Find(GvarD2ObjectList[j]);
            Break;
          end;
        end;
        if SItem <> nil then
          SItem.Add(OItem);
      end;
      CatList.Free;
      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Delete object',0,false,true,doDesignPopupDel,0,'');
      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Load From File...',0,false,true,doDesignPopupLoadFromFile,0,'');
      FDesignPopup.Items.Add(Item);
      if FSelected <> nil then
      begin
        { Design Hide }
        Item:=NewItem('Hide in Design-time',0,FSelected.DesignHide,true,doDesignPopupDesignHide,0,'');
        {$IFDEF KS_COMPILER6_UP}
        Item.AutoCheck:=true;
        {$ENDIF}
        FDesignPopup.Items.Add(Item);

        { Edit Style }
        if (FSelected is TD2Control) and (TD2Control(FSelected).GetResourceObject <> nil) then
        begin
          Item:=NewItem('Edit Default Style...',0,false,true,doDesignPopupEditStyle,0,'');
          FDesignPopup.Items.Add(Item);
          Item:=NewItem('Edit Custom Style...',0,false,true,doDesignPopupCreateStyle,0,'');
          FDesignPopup.Items.Add(Item);
        end;
        { TabOrder }
        if (FSelected.IsVisual) and (FSelected.CanFocused) then
        begin
          Item:=NewItem('Tab Order...',0,false,true,doDesignPopupTabOrder,0,'');
          FDesignPopup.Items.Add(Item);
        end;
      end;
      { Reorder }
      Item:=NewItem('Order',0,false,true,nil,0,'');
      SItem:=NewItem('Bring to front',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Send to back',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Edit }
      Item:=NewItem('Edit',0,false,true,nil,0,'');
      SItem:=NewItem('Copy to clipboard',0,false,true,doDesignPopupCopy,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Paste from clipboard',0,false,true,doDesignPopupPaste,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Grid }
      Item:=NewItem('Grid',0,false,true,nil,0,'');
      SItem:=NewItem('Show snap grid',0,FSnapGridShow,true,doDesignPopupGrid,0,'');
      SItem.Tag:=1;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap to grid',0,FSnapToGrid,true,doDesignPopupGrid,0,'');
      SItem.Tag:=2;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap to lines',0,FSnapToLines,true,doDesignPopupGrid,0,'');
      SItem.Tag:=3;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      SItem:=NewItem('Snap disabled',0,not FSnapToLines or not FSnapToLines,true,doDesignPopupGrid,0,'');
      SItem.Tag:=4;
      SItem.RadioItem:=true;
      {$IFDEF KS_DELPHI6_UP}
      SItem.AutoCheck:=true;
      {$ENDIF}
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
    end;
  end;
  GetCursorPos(FPopupPos);
  FDesignPopup.Popup(FPopupPos.X ,FPopupPos.Y);
end;

procedure TD2CustomScene.DefineProperties(Filer:TFiler);
begin
  inherited DefineProperties(Filer);
  Filer.DefineProperty('DesignSnapGridShow',ReadDesignSnapGridShow,WriteDesignSnapGridShow,true);
  Filer.DefineProperty('DesignSnapToGrid',ReadDesignSnapToGrid,WriteDesignSnapToGrid,true);
  Filer.DefineProperty('DesignSnapToLines',ReadDesignSnapToLines,WriteDesignSnapToLines,true);
end;

procedure TD2CustomScene.ReadDesignSnapGridShow(Reader:TReader);
begin
  FSnapGridShow:=Reader.ReadBoolean;
  if not (FDesignTime) then
    FSnapGridShow:=false;
end;

procedure TD2CustomScene.ReadDesignSnapToGrid(Reader:TReader);
begin
  FSnapToGrid:=Reader.ReadBoolean;
end;

procedure TD2CustomScene.ReadDesignSnapToLines(Reader:TReader);
begin
  FSnapToLines:=Reader.ReadBoolean;
end;

procedure TD2CustomScene.WriteDesignSnapGridShow(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapGridShow);
end;

procedure TD2CustomScene.WriteDesignSnapToGrid(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToGrid);
end;

procedure TD2CustomScene.WriteDesignSnapToLines(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToLines);
end;

function TD2CustomScene.GetStyle:TD2Resources;
begin
  Result:=FStyle;
end;

function TD2CustomScene.GetRoot:TD2Object;
begin
  if (FChildren <> nil) and (FChildren.Count > 0) then
    Result:=TD2Object(FChildren[0])
  else
    Result:=nil;
end;

procedure TD2CustomScene.SetFocused(const Value:TD2VisualObject);
begin
  if FFocused <> Value then
  begin
    if FFocused <> nil then
    begin
      TD2VisualObject(FFocused).KillFocus;
      TD2VisualObject(FFocused).RemoveFreeNotify(Self);
    end;
    FFocused:=Value;
    if FFocused <> nil then
    begin
      TD2VisualObject(FFocused).EnterFocus;
      TD2VisualObject(FFocused).AddFreeNotify(Self);
    end;
  end;
end;

procedure TD2CustomScene.DoDesignSelect(AObject:TObject);
begin
  if (AObject <> nil) and (AObject is TD2VisualObject) and (TD2VisualObject(AObject).FScene.GetComponent=Self) then
  begin
    FSelected:=TD2VisualObject(AObject);
    TD2VisualObject(FSelected).DesignSelect;
    AddUpdateRect(d2Rect(0,0,10000,10000));
    Draw;
  end;
end;

procedure TD2CustomScene.SetActiveControl(AControl:TD2Control);
begin
  if AControl <> FActiveControl then
  begin
    FActiveControl:=AControl;
    if (FActiveControl <> nil) and not (csLoading in ComponentState) then
      FActiveControl.SetFocus;
  end;
end;

procedure TD2CustomScene.SetTransparency(const Value:boolean);
begin
  if FTransparency <> Value then
  begin
    FTransparency:=Value;
    AddUpdateRect(d2Rect(0,0,10000,10000));
  end;
end;

procedure TD2CustomScene.SetSelected(const Value:TD2VisualObject);
begin
  if FSelected <> Value then
  begin
    if FSelected <> nil then
      FSelected.Repaint;
    FSelected:=Value;
    if FSelected <> nil then
      FSelected.Repaint;
    if Assigned(FDesignChangeSelection) then
      FDesignChangeSelection(Self);
    AddUpdateRect(d2Rect(0,0,Width,Height));
    Draw;
  end;
end;

function TD2CustomScene.GetActiveControl:TD2Control;
begin
  Result:=FActiveControl;
end;

function TD2CustomScene.GetDisableUpdate:boolean;
begin
  Result:=FDisableUpdate;
end;

function TD2CustomScene.GetDesignTime:boolean;
begin
  Result:=FDesignTime;
end;

function TD2CustomScene.GetCanvas:TD2Canvas;
begin
  Result:=FCanvas;
end;

function TD2CustomScene.GetOwner:TComponent;
begin
  Result:=Owner;
end;

function TD2CustomScene.GetComponent:TComponent;
begin
  Result:=Self;
end;

function TD2CustomScene.GetSelected:TD2VisualObject;
begin
  Result:=FSelected;
end;

function TD2CustomScene.GetDesignPlaceObject:TD2VisualObject;
begin
  Result:=FDesignPlaceObject;
end;

procedure TD2CustomScene.SetDisableUpdate(Value:boolean);
begin
  FDisableUpdate:=Value;
end;

function TD2CustomScene.GetUpdateRectsCount:integer;
begin
  Result:=Length(FUpdateRects);
end;

function TD2CustomScene.GetUpdateRect(const Index:integer):TD2Rect;
begin
  Result:=FUpdateRects[Index];
end;

function TD2CustomScene.GetAnimatedCaret:boolean;
begin
  Result:=FAnimatedCaret;
end;

function TD2CustomScene.LocalToScreen(const Point:TD2Point):TD2Point;
begin
  with ClientToScreen(Classes.Point(trunc(Point.x),trunc(Point.y))) do
    Result:=d2Point(x,y);
end;

function TD2CustomScene.GetCaptured:TD2VisualObject;
begin
  Result:=FCaptured;
end;

procedure TD2CustomScene.SetCaptured(const Value:TD2VisualObject);
begin
  FCaptured:=Value;
  if Assigned(FCaptured) then
    if not Assigned(FOnFlush) then
      MouseCapture:=true;
  if not Assigned(FCaptured) then
    if not Assigned(FOnFlush) then
      MouseCapture:=false;
end;

function TD2CustomScene.GetTransparency:boolean;
begin
  Result:=FTransparency;
end;

function TD2CustomScene.GetFocused:TD2VisualObject;
begin
  Result:=FFocused;
end;

procedure TD2CustomScene.SetDesignRoot(const Value:TD2VisualObject);
begin
  FDesignRoot:=Value;
end;

function TD2CustomScene.GetMousePos:TD2Point;
begin
  Result:=FMousePos;
end;

procedure TD2CustomScene.UpdateResource;
begin
  if Root <> nil then
    Root.UpdateResource;
end;

procedure TD2CustomScene.SetStyle(const Value:TD2Resources);
begin
  if FStyle <> Value then
  begin
    if FStyle <> nil then
      FStyle.RemoveSceneUpdater(Self);
    FStyle:=Value;
    if FStyle <> nil then
      FStyle.AddSceneUpdater(Self);
    UpdateResource;
  end;
end;

function TD2CustomScene.ShowKeyboardForControl(AObject:TD2Object):boolean;
begin
  Result:=false;
end;

function TD2CustomScene.HideKeyboardForControl(AObject:TD2Object):boolean;
begin
  Result:=false;
end;


procedure TD2CustomScene.WMEraseBkgnd(var Msg:TLMEraseBkgnd);
var
  rgnStatus:integer;
  rgn:HRgn;
begin
  if (Msg.DC <> 0) and (Canvas <> nil) then
  begin
    rgn:=CreateRectRgn(0,0,1,1);
   {$IFDEF WINDOWS}
    rgnStatus:=GetUpdateRgn(Handle,rgn,false);
   {$ENDIF}
    rgnStatus :=1;
    if (rgnStatus=1) then
    begin
      Canvas.FlushBuffer(0,0,Msg.DC);
    end;
    DeleteObject(rgn);
  end;
  Msg.Result:=1;
end;

procedure TD2CustomScene.CMMouseLeave(var Message :TLMessage);
begin
  inherited;
  if FHovered <> nil then
  begin
    TD2VisualObject(FHovered).MouseInObject:=false;
    TD2VisualObject(FHovered).MouseLeave;
    TD2VisualObject(FHovered).RemoveFreeNotify(Self);
    FHovered:=nil;
  end;
end;


//========================================================================= 8888
//=========================================================================
//=========================================================================


{$IFDEF UNIX}
procedure TD2CustomScene.WMPaint(var Msg:TLMPaint);
begin
  if FDrawing then Exit;

  inherited ;
  Draw;
  FDrawing:=false;
end;

{$ENDIF}

{$IFDEF WINDOWS}
type
  PRgnRects=^TRgnRects;
  TRgnRects=array [0..0] of TRect;

procedure TD2CustomScene.WMPaint(var Msg:TLMPaint);
var
  i,rgnStatus:integer;
  rgn:HRgn;
  rgnSize:integer;
  rgnData:PRgnData;
  R:windows.TRect;
begin
  if FDrawing then Exit;
  rgn:=CreateRectRgn(0,0,1,1);
  rgnStatus:=GetUpdateRgn(Handle,rgn,false);
  if (rgnStatus=2) or (rgnStatus=3) then
  begin
    rgnSize:=GetRegionData(rgn,$FFFF,nil);
    if rgnSize > 0 then
    begin
      GetMem(rgnData,rgnSize);
      rgnSize:=GetRegionData(rgn,rgnSize,rgnData);
      if rgnSize=rgnSize then
      begin
        SetLength(FUpdateRects,rgnData.rdh.nCount);
        for i:=0 to rgnData.rdh.nCount - 1 do
        begin
          R:=PRgnRects(@rgnData.buffer[0])[i];
          with R do
            FUpdateRects[i]:=d2Rect(left,top,right,bottom);
        end;
      end;
      FreeMem(rgnData,rgnSize);

      FDC:=GetDC(Handle);
      Draw;
      ReleaseDC(Handle,FDC);
      FDC:=0;

      {$IFDEF FPC}
      FDrawing:=true;
      Msg.result:=CallWindowProcW(@PrevWndProc,Handle,Msg.Msg,TMessage(Msg).WParam,TMessage(Msg).LParam);
      FDrawing:=false;
      {$ELSE}
      inherited ;
      {$ENDIF}
    end
    else
    begin
      {$IFDEF FPC}
      FDrawing:=true;
      Msg.result:=CallWindowProcW(@PrevWndProc,Handle,Msg.Msg,TMessage(Msg).WParam,TMessage(Msg).LParam);
      FDrawing:=false;
      {$ELSE}
      inherited ;
      {$ENDIF}
    end;
  end
  else
  begin
    {$IFDEF FPC}
    FDrawing:=true;
    Msg.result:=CallWindowProcW(@PrevWndProc,Handle,Msg.Msg,TMessage(Msg).WParam,TMessage(Msg).LParam);
    FDrawing:=false;
    {$ELSE}
    inherited ;
    {$ENDIF}
  end;
  DeleteObject(rgn);
end;

const
  IID_IDropTargetHelper:TGUID=(
    D1:$4657278b; D2:$411b; D3:$11d2; D4:($83,$9a,$00,$c0,$4f,$d9,$18,$d0));
  SID_IDropTargetHelper='{4657278B-411B-11d2-839A-00C04FD918D0}';
  CLSID_DragDropHelper:TGUID=(
    D1:$4657278a; D2:$411b; D3:$11d2; D4:($83,$9a,$00,$c0,$4f,$d9,$18,$d0));

type
  {_$EXTERNALSYM IDropTargetHelper}
  IDropTargetHelper=interface(IUnknown)
    [SID_IDropTargetHelper]
    function DragEnter(hwndTarget:HWND; const DataObj:IDataObject;
      var pt:TPoint; dwEffect:Longint):HResult; stdcall;
    function DragLeave:HResult; stdcall;
    function DragOver(var pt:TPoint; dwEffect:longInt):HResult; stdcall;
    function Drop(const DataObj:IDataObject; var pt:TPoint;
      dwEffect:longInt):HResult; stdcall;
    function Show(Show:BOOL):HResult; stdcall;
  end;

var
  FDropTargetHelper:IDropTargetHelper;
  FDataObj:IDataObject;

function TD2CustomScene.GetDataObject:TD2DragObject;
var
  formatEtc:TFORMATETC;
  stgMedium:TSTGMEDIUM;
  str:wideString;
  drop:HDrop;
  i,numFiles:integer;
  buffer :array[0..MAX_PATH] of widechar;
begin
  FillChar(Result,SizeOf(Result),0);
  if not Assigned(FDataObj) then Exit;
  // get file name first
  with formatEtc do
  begin
    cfFormat:=CF_HDROP;
    ptd:=nil;
    dwAspect:=DVASPECT_CONTENT;
    lindex:=-1;
    tymed:=TYMED_HGLOBAL;
  end;
  { Get the data }
  str:='';
  if FDataObj.GetData(formatEtc,stgMedium)=S_OK then
  begin
    try
      {Lock the global memory handle to get a pointer to the data}
      drop:=HDrop(GlobalLock(stgMedium.hGlobal));
      { Replace Text }
      numFiles:=DragQueryFile(drop,$FFFFFFFF,nil,0);
      SetLength(Result.Files,numFiles);
      for i:=0 to numFiles - 1 do
      begin
        DragQueryFileW(drop,i,@buffer,sizeof(buffer));
        Result.Files[i]:=buffer;
        if i=0 then
          Result.Data:=Result.Files[0];
      end;
    finally
      {Finished with the pointer}
      GlobalUnlock(stgMedium.hGlobal);
      {Free the memory}
      ReleaseStgMedium(@stgMedium);
    end;
  end
  else
  begin
    // get text
    formatEtc.cfFormat:=CF_UNICODETEXT;
    if FDataObj.GetData(formatEtc,stgMedium)=S_OK then
    begin
      try
        {Lock the global memory handle to get a pointer to the data}
        str:=PWideChar(GlobalLock(stgMedium.hGlobal));
        Result.Data:=str;
      finally
        {Finished with the pointer}
        GlobalUnlock(stgMedium.hGlobal);
        {Free the memory}
        ReleaseStgMedium(@stgMedium);
      end;
    end
  end;
end;

function TD2CustomScene.DragEnter(const dataObj:IDataObject; grfKeyState:DWORD;
      pt:TPoint; var dwEffect:DWORD):HResult;
begin
  try
    if (Root=nil) and not (Root.IsVisual) then
    begin
      dwEffect:=DROPEFFECT_NONE;
      Result:=E_UNEXPECTED;
      Exit;
    end;
    FDataObj:=dataObj;
    Result:=S_OK;
    dwEffect:=DROPEFFECT_NONE;
    if (Succeeded(CoCreateInstance(CLSID_DragDropHelper,nil,CLSCTX_INPROC_SERVER,
      IDropTargetHelper,FDropTargetHelper))) and
      (FDropTargetHelper <> nil) then
    begin
      if (Failed(FDropTargetHelper.DragEnter(Handle,DataObj,pt,dwEffect))) then
        FDropTargetHelper:=nil;
    end;
  except
    dwEffect:=DROPEFFECT_NONE;
    Result:=E_UNEXPECTED;
  end;
end;

function TD2CustomScene.DragOver(grfKeyState:DWORD; pt:TPoint; var dwEffect:DWORD):HResult;
var
  P:TD2Point;
  NewTarget:TD2VisualObject;
begin
  try
    dwEffect:=DROPEFFECT_NONE;
    Result:=S_OK;
    with ScreenToClient(pt) do
      P:=d2Point(X,Y);
    NewTarget:=Root.Visual.FindTarget(P,GetDataObject);
    if (NewTarget <> varD2FTarget) then
    begin
      if varD2FTarget <> nil then  TD2VisualObject(varD2FTarget).DragLeave;

      varD2FTarget:=NewTarget;
      if varD2FTarget <> nil then
      begin
        TD2VisualObject(varD2FTarget).DragEnter(GetDataObject,P);
      end;
    end;
    if NewTarget <> nil then
      dwEffect:=DROPEFFECT_LINK;
    if FDropTargetHelper <> nil then
      FDropTargetHelper.DragOver(pt,dwEffect);
  except
    dwEffect:=DROPEFFECT_NONE;
    Result:=E_UNEXPECTED;
  end;
end;

function TD2CustomScene.DragLeave:HResult;
begin
  if varD2FTarget <> nil then TD2VisualObject(varD2FTarget).DragLeave;

  if (FDropTargetHelper <> nil) then  FDropTargetHelper.DragLeave;
  varD2FTarget:=nil;
  FDropTargetHelper:=nil;
  FDataObj:=nil;
  Result:=S_OK;
end;

function TD2CustomScene.Drop(const dataObj:IDataObject; grfKeyState:DWORD; pt:TPoint;
      var dwEffect:DWORD):HResult;
var
  P:TD2Point;
begin
  try
    if (dataObj=nil) then Exit;
    if varD2FTarget <> nil then
    begin
      with ScreenToClient(pt) do
        P:=d2Point(X,Y);
      TD2VisualObject(varD2FTarget).DragDrop(GetDataObject,P);
    end;
    if (FDropTargetHelper <> nil) then
      FDropTargetHelper.Drop(DataObj,pt,dwEffect)
  finally
    FDataObj:=nil;
    FDropTargetHelper:=nil;
  end;
end;

procedure TD2CustomScene.WMAddUpdateRect(var Msg:TMessage);
 procedure ProcessUpdateMessages;
 var
   Msg:TMsg;
 begin
   while PeekMessage(Msg,Handle,WM_ADDUPDATERECT,WM_ADDUPDATERECT,PM_REMOVE) do
   begin
     if Msg.message=WM_QUIT then
     begin
       { Repost WM_QUIT messages }
       PostQuitMessage(Msg.WParam);
       Break;
     end;
   end;
 end;
begin
  ProcessUpdateMessages;
  Draw;
end;
{$ENDIF}
