{%mainunit ../orca_scene3d.pas}

{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//================= TD3Scene ===============================================

const
  d3SceneCount:integer=0;

constructor TD3Scene.Create(AOwner:TComponent);
begin
  inherited;
  ShowHint:=true;
  FDesignShowHint:=true;
  FDesignAllowPanAndRotate:=true;
  FUsingDesignCamera:=true;
  OnDragOver:=DoDragOver;
  OnDragDrop:=DoDragDrop;
  ControlStyle:=ControlStyle + [csCaptureMouse,csOpaque,csDoubleClicks];
  FSnapGridShow:=true;
  FSnapToLines:=true;
  FAlignRoot:=true;
  FSnapGridSize:=0.25;
  FLighting:=true;
  FFill:=$FF303030;
  FAmbient:=$FF202020;

  //Align:=alClient;
  Width :=200;
  Height:=200;

  FDesignTime:=(csDesigning in ComponentState);

  if GvarD3Designer <> nil then
    GvarD3Designer.AddScene(Self);

  FDesignCameraZ:=TD3Dummy.Create(Self);
  with FDesignCameraZ do
  begin
    Tag:=$FFFe;
    Locked:=true;
    Stored:=false;
  end;
  AddObject(FDesignCameraZ);

  FDesignCameraX:=TD3Dummy.Create(Self);
  with FDesignCameraX do
  begin
    Tag:=$FFFe;
    Parent:=FDesignCameraZ;
    Locked:=true;
    Stored:=false;
    RotateAngle.X:=20;
  end;

  FDesignCamera:=TD3Camera.Create(Self);
  with FDesignCamera do
  begin
    Tag:=$FFFe;
    Parent:=FDesignCameraX;
    Locked:=true;
    Stored:=false;
    Position.Point:=d3Point(0,-20,0);
  end;

  FDesignGrid:=TD3Grid.Create(Self);
  with TD3Grid(FDesignGrid) do
  begin
    Width:=50;
    Height:=50;
    Frequency:=1;
    Marks:=4;
    Tag:=$FFFe;
    Locked:=true;
    Stored:=false;
    Visible:=FDesigntime;
  end;
  AddObject(FDesignGrid);

  d3SceneCount:=d3SceneCount + 1;
end;

procedure TD3Scene.CreateEmbedded(const AWidth,AHeight:integer; AOnFlush:TNotifyEvent);
begin
  Width:=AWidth;
  Height:=AHeight;
  if Canvas=nil then
  begin
    FCanvas:=GvarD3DefaultCanvasClass.Create(Handle,Width,Height,true,Quality);
    FCanvas.FScene:=Self;
  end
  else
  begin
    Canvas.FLockable:=true;
    Canvas.ResizeBuffer(AWidth,AHeight);
  end;
  FOnFlush:=AOnFlush;
  Draw;
end;

procedure TD3Scene.EmbeddedMouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:integer);
begin
  MouseDown(Button,Shift,X,Y);
end;

procedure TD3Scene.EmbeddedMouseMove(Shift:TShiftState; X,Y:integer);
begin
  MouseMove(Shift,X,Y);
end;

procedure TD3Scene.EmbeddedMouseUp(Button:TMouseButton; Shift:TShiftState; X,Y:integer);
begin
  MouseUp(Button,Shift,X,Y);
end;

function TD3Scene.EmbeddedMouseWheel(Shift:TShiftState; WheelDelta:integer):boolean;
var
  MousePos:TPoint;
begin
  Result:=DoMouseWheel(Shift,WheelDelta,MousePos);
end;

procedure TD3Scene.EmbeddedKeyUp(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
begin
  NewKeyUp(Key,Char,Shift);
end;

procedure TD3Scene.EmbeddedKeyDown(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
begin
  NewKeyDown(Key,Char,Shift);
end;

procedure TD3Scene.Loaded;
begin
  inherited;
  FLoadCursor:=Cursor;
  if FSnapToLines then
    FSnapToGrid:=false;
end;

procedure TD3Scene.CreateParams(var Params:TCreateParams);
begin
  inherited;
  Params.WindowClass.style:=Params.WindowClass.style or CS_HREDRAW or CS_VREDRAW;
end;

procedure TD3Scene.CreateHandle;
{$IFDEF UNIX}
 {$IFNDEF DARWIN}
var
  FGTKWidget:PGtkwidget;
 {$ENDIF}
{$ENDIF}
begin
  inherited;
  {$IFDEF WINDOWS}
  SetWindowLongW(Handle,GWL_WNDPROC,GetWindowLong(Handle,GWL_WNDPROC));
  PrevWndProc:=Windows.WNDPROC(SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)));
  {$ENDIF}
  if not (DesignTime) and FInvisibleAtRuntime then
  else
  begin
    FCanvas:=GvarD3DefaultCanvasClass.Create(Handle,Width,Height,Transparency,Quality);
    FCanvas.FScene:=Self;
  end;
  if FTransparency and not (DesignTime) then
  begin
    if Parent is TCustomForm then
    begin
      {$IFDEF WINDOWS}
      SetWindowLong(Parent.Handle,GWL_EXSTYLE,GetWindowLong(Parent.Handle,GWL_EXSTYLE) or WS_EX_LAYERED);
      UpdateLayer;
      {$ENDIF}
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

      WndEventHandlerUPP:=NewEventHandlerUPP(EventHandlerProcPtr(Pointer(@CtrlEventHandler)));
      EventKinds[0].eventClass:=kEventClassControl;
      EventKinds[0].eventKind:=kEventControlDraw;
      InstallEventHandler(GetControlEventTarget(TCarbonWidget(Handle).Widget),WndEventHandlerUPP,1,@EventKinds[0],Self,nil);
      {$ENDIF}
    end
  end;

{$IFDEF UNIX}

 {$IFDEF DARWIN}
  SetControlDragTrackingEnabled(TCarbonWidget(Handle).Widget,true);
  if Parent is TCustomForm then
    SetAutomaticControlDragTrackingEnabledForWindow(TCarbonWindow(TCustomForm(Parent).Handle).Window,true);
  WndEventHandlerUPP:=NewEventHandlerUPP(EventHandlerProcPtr(Pointer(@CtrlEventHandler)));
  EventKinds[0].eventClass:=kEventClassControl;
  EventKinds[0].eventKind:=kEventControlDragEnter;
  EventKinds[1].eventClass:=kEventClassControl;
  EventKinds[1].eventKind:=kEventControlDragReceive;
  EventKinds[2].eventClass:=kEventClassControl;
  EventKinds[2].eventKind:=kEventControlDragWithin;
  EventKinds[3].eventClass:=kEventClassControl;
  EventKinds[3].eventKind:=kEventControlDragLeave;
  InstallEventHandler(GetControlEventTarget(TCarbonWidget(Handle).Widget),WndEventHandlerUPP,4,@EventKinds[0],Self,nil);
 {$ELSE}
  FGTKWidget:=GetFixedWidget(pgtkwidget(Handle));
  gtk_widget_realize(FGTKWidget);
  gtk_widget_set_double_buffered(FGTKWidget,false);
 {$ENDIF}

{$ENDIF}

  if not HandleAllocated then Exit;
end;

procedure TD3Scene.CreateWnd;
begin
  inherited ;
  {$IFDEF WINDOWS}
  RegisterDragDrop(Handle,Self);
  {$ENDIF}
end;

procedure TD3Scene.DestroyWnd;
begin
  if d3SceneCount=0 then
  begin
    if VarD3AniThread <> nil then VarD3AniThread.Free;
    VarD3AniThread:=nil;
  end;

  if (FCanvas <> nil) then  FreeAndNil(FCanvas);

  {$IFDEF WINDOWS}
  if HandleAllocated then  RevokeDragDrop(Handle);

  if PtrInt(SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@WndCallback)))=PtrInt(@WndCallback) then
    SetWindowLongPtrW(Self.Handle,GWL_WNDPROC,PtrInt(@PrevWndProc));
  {$ENDIF}
  inherited ;
end;


destructor TD3Scene.Destroy;
begin
  if GvarD3Designer <> nil then
    GvarD3Designer.RemoveScene(Self);
  d3SceneCount:=d3SceneCount - 1;
  if Realtime then
    Application.OnIdle:=nil;
  DeleteChildren;
  if (FCanvas <> nil) then
    FreeAndNil(FCanvas);
  if FPhysics <> nil then
    FreeAndNil(FPhysics);
  if FBlendedList <> nil then
    FreeAndNil(FBlendedList);
  if FChildren <> nil then
    FreeAndNil(FChildren);
  FreeAndNil(FCanvas);
  inherited;
end;

procedure TD3Scene.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  i:integer;
begin
  inherited;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).Stored then
      begin
        Proc(FChildren[i]);
      end;
end;


procedure TD3Scene.AddUpdateRect(const R:TD2Rect);
begin
  if FDisableUpdate then Exit;
  if csLoading in ComponentState then Exit;
  if not HandleAllocated then Exit;
  {$IFDEF WINDOWS}
  if Transparency and (Parent is TCustomForm) then
  begin
    PostMessage(Handle,WM_ADDUPDATERECT,0,0); // only for layered
  end
  else
  begin
    Windows.InvalidateRect(Handle,nil,false);
  end;
  {$ENDIF}
  {$IFDEF UNIX}
    {$IFDEF DARWIN}
      HIViewSetNeedsDisplay(HiViewRef(TCarbonWidget(Handle).Widget),true);
    {$ELSE}
      gdk_window_invalidate_rect(PGtkWidget(Handle)^.Window,nil,true);
    {$ENDIF}
  {$ENDIF}
end;

procedure TD3Scene.Draw;
var
  i,j:integer;
  R:TD2Rect;
  Rgn,NRgn:Cardinal;
  M,CameraMatrix:TD3Matrix;
  V:TD3Vector;
  P:TPoint;
  Back:TBitmap;
  Dist:single;
begin
  if FDrawing then Exit;
  if FDisableUpdate then Exit;
  if Canvas=nil then Exit;
  FDrawing:=true;
  try
    Canvas.Reset;
    Canvas.Lighting:=FLighting;
    Canvas.Ambient:=FAmbient;
    if FBlendedList=nil then
      FBlendedList:=TList.Create;
    { set matrix and camera }
    if Camera <> nil then
    begin
      Canvas.SetCamera(Camera);
    end;
    if DesignTime or FUsingDesignCamera then
    begin
      Canvas.SetCamera(FDesignCamera);
      FDesignGrid.Visible:=DesignTime;
    end;
    { set lights }
    if FChildren <> nil then
      for i:=0 to FChildren.Count - 1 do
      begin
        if not (TObject(FChildren[i]) is TD3VisualObject) then Continue;
        if not TD3VisualObject(FChildren[i]).Visible then Continue;
        Canvas.SetMatrix(TD3VisualObject(FChildren[i]).AbsoluteMatrix);
        TD3VisualObject(FChildren[i]).PreRender;
      end;
    { draw back }
    if Canvas.BeginScene then
    begin
      Canvas.SetRenderState(rsGouraud);
      Canvas.SetRenderState(rs3DScene);
      Canvas.SetRenderState(rsZWriteOn);
      Canvas.SetRenderState(rsLightOn);
      Canvas.SetRenderState(rsTexLinear);
      Canvas.SetRenderState(rsFrontFace);
      if not Transparency then
        Canvas.Clear(FFill)
      else
      begin
        Canvas.Clear(FFill);
      end;
      { reset }
      if FChildren <> nil then
        for i:=0 to FChildren.Count - 1 do
        begin
          if not (TObject(FChildren[i]) is TD3VisualObject) then Continue;
          if not TD3VisualObject(FChildren[i]).Visible then Continue;
          Canvas.SetMatrix(TD3VisualObject(FChildren[i]).AbsoluteMatrix);
          TD3VisualObject(FChildren[i]).BeforePaint;
          TD3VisualObject(FChildren[i]).Paint;
          TD3VisualObject(FChildren[i]).PaintChildren;
        end;
      {$IFDEF MOUSERAY}
      { helpvector }
      Canvas.SetMatrix(IdentityMatrix);
      Canvas.SetRenderState(rsZWriteOff);
      Canvas.Material.Diffuse:='#FFFF0000';
      V:=d3VectorScale(d3VectorSubtract(FV2,FV1),10);
      Canvas.DrawLine(d3VectorSubtract(FV1,V),d3VectorAdd(FV2,V),1);
      {$ENDIF}
      { design }
      if (FSelected <> nil) and (FSelected <> Root) and (Canvas.FCurrentCamera <> nil) then
      begin
        dist:=d3VectorLength(d3VectorSubtract(Canvas.FCurrentCamera.AbsolutePosition,FSelected.AbsolutePosition)) / (Canvas.GetProjectionMatrix.m11 * Canvas.GetProjectionMatrix.m22);
        if FSelected.Projection=d3ProjectionCamera then
        begin
          dist:=dist * 0.14;
          RotateGripDist:=1.8;
          GripSize:=0.18 * (Dist);
          Canvas.SetRenderState(rs3DScene)
        end
        else
        begin
          RotateGripDist:=10;
          GripSize:=3;
          Canvas.SetRenderState(rs2DScene);
        end;
        Canvas.SetMatrix(FSelected.AbsoluteMatrix);
        Canvas.SetRenderState(rsZWriteOn);
        Canvas.Material.Lighting:=false;
        Canvas.Material.Bitmap:='';
        Canvas.Material.NativeDiffuse:=$FF5B91DE;
        { box }
        Canvas.DrawCube(d3Vector(0,0,0),d3Vector(FSelected.Width,FSelected.Height,FSelected.Depth),1);
        if FSelected.FCanResize then
        begin
          { righttop }
          V:=d3VectorAdd(d3Vector(0,0,0),d3Vector(FSelected.Width / 2,-FSelected.Height / 2,FSelected.Depth / 2));
          Canvas.Material.NativeDiffuse:=$FF5B91DE;
          if FRightTop then
            Canvas.Material.NativeDiffuse:=$FFFF5010;
          Canvas.FillCube(V,d3Vector(GripSize * 3,GripSize * 1,GripSize * 3),1);
          Canvas.Material.NativeDiffuse:=$AA000000;
          Canvas.DrawCube(V,d3Vector(GripSize * 3,GripSize * 1,GripSize * 3),1);
          { leftfar }
          V:=d3VectorAdd(d3Vector(0,0,0),d3Vector(-FSelected.Width / 2,FSelected.Height / 2,FSelected.Depth / 2));
          Canvas.Material.NativeDiffuse:=$FF5B91DE;
          if FLeftFar then
            Canvas.Material.NativeDiffuse:=$FFFF5010;
          Canvas.FillCube(V,d3Vector(GripSize * 3,GripSize * 3,GripSize * 1),1);
          Canvas.Material.NativeDiffuse:=$AA000000;
          Canvas.DrawCube(V,d3Vector(GripSize * 3,GripSize * 3,GripSize * 1),1);
        end;
        { rotation }
        if FSelected.FCanRotate then
        begin
          { z rotate }
          V:=d3VectorAdd(d3Vector(0,0,0),d3Vector(0,-FSelected.Height / 2 - RotateGripDist,0));
          Canvas.Material.NativeDiffuse:=$FF5BDE91;
          if FRotateZ then
            Canvas.Material.NativeDiffuse:=$FFFF5010;
          Canvas.DrawLine(d3Vector(0,-FSelected.Height / 2,0),d3Vector(0,-FSelected.Height / 2 - RotateGripDist,0),1);
          Canvas.FillCube(V,d3Vector(GripSize * 5,GripSize * 1.5,GripSize * 1.5),1);
          Canvas.Material.NativeDiffuse:=$AA000000;
          Canvas.DrawCube(V,d3Vector(GripSize * 5,GripSize * 1.5,GripSize * 1.5),1);
          if FSelected.Projection=d3ProjectionCamera then
          begin
            { y rotate }
            V:=d3VectorAdd(d3Vector(0,0,0),d3Vector(FSelected.Width / 2 + RotateGripDist,0,0));
            Canvas.Material.NativeDiffuse:=$FF5BDE91;
            if FRotateY then
              Canvas.Material.NativeDiffuse:=$FFFF5010;
            Canvas.DrawLine(d3Vector(FSelected.Width / 2,0,0),d3Vector(FSelected.Width / 2 + RotateGripDist,0,0),1);
            Canvas.FillCube(V,d3Vector(GripSize * 1.5,GripSize * 1.5,GripSize * 5),1);
            Canvas.Material.NativeDiffuse:=$AA000000;
            Canvas.DrawCube(V,d3Vector(GripSize * 1.5,GripSize * 1.5,GripSize * 5),1);
            { x rotate }
            V:=d3VectorAdd(d3Vector(0,0,0),d3Vector(0,0,FSelected.Depth / 2 + RotateGripDist));
            Canvas.Material.NativeDiffuse:=$FF5BDE91;
            if FRotateX then
              Canvas.Material.NativeDiffuse:=$FFFF5010;
            Canvas.DrawLine(d3Vector(0,0,FSelected.Depth / 2),d3Vector(0,0,FSelected.Depth / 2 + RotateGripDist),1);
            Canvas.FillCube(V,d3Vector(GripSize * 1.5,GripSize * 5,GripSize * 1.5),1);
            Canvas.Material.NativeDiffuse:=$AA000000;
            Canvas.DrawCube(V,d3Vector(GripSize * 1.5,GripSize * 5,GripSize * 1.5),1);
          end;
        end;
      end;
      { design modes }
      if (DesignTime) and FDesignShowHint then
      begin
        Canvas.SetRenderState(rs2DScene);
        Canvas.SetMatrix(IdentityMatrix);
        Canvas.Material.Diffuse:='#8091DE5B';
        Canvas.Material.Lighting:=false;
        Canvas.Font.Size:=9;
        Canvas.Font.Style:=d3FontRegular;
        Canvas.DrawText(0,0,'Alt+mouse - rotate scene | Shift+mouse - move scene',1);
        Canvas.DrawText(0,10,'Mouse - move object in (XoY) | Ctrl + Mouse - move object in (XoZ)',1);
        Canvas.DrawText(0,20,'Shift+mouse - move layer object in (XoY) | Hold Shift to use d3 Menu in ObjectLayers',1);
      end;
    end;
    { buffer }
    Canvas.FlushBuffer;
    if Assigned(FOnFlush) then
      FOnFlush(Self)
    else
    begin
      { Transparancy }
      {$ifdef WINDOWS}
      if FTransparency and (Parent is TCustomForm) then
        UpdateLayer;
      {$endif}
      {$ifdef darwin}
      if FTransparency and (Parent is TCustomForm) then
        UpdateLayer;
      {$endif}
    end;
  finally
    FDrawing:=false;
  end;
end;

{ Drag and Drop }

type
  THackControl=class(TCustomControl);

var
  VCLDragSource:TControl;

procedure TD3Scene.BeginVCLDrag(Source:TObject);
begin
  FCaptured:=nil;
  MouseCapture:=false;
  VCLDragSource:=TCustomControl.Create(Application);
  VCLDragSource.Parent:=Self;
  VCLDragSource.Width:=0;
  VCLDragSource.Height:=0;
  VCLDragSource.Tag:=PtrInt(Source);
  VCLDragSource.BeginDrag(true,-1);
  THackControl(VCLDragSource).OnEndDrag:=EndDragEvent;
end;

type
  TD2HackVisualObject=class(TD2VisualObject);
  TD2HackObject=class(TD2Object);
  TD3HackScene=class(TD3Scene);
  TD2HackControl=class(TD2Control);
  TxxHackCanvas=class(TD2Canvas);

procedure TD3Scene.EndDragEvent(Sender,Target:TObject; X,Y:integer);
begin
  if TObject(Pointer(VCLDragSource.Tag)) is TD3VisualObject then
    TD3VisualObject(Pointer(VCLDragSource.Tag)).DragEnd;
  if TObject(Pointer(VCLDragSource.Tag)) is TD2VisualObject then
    TD2HackVisualObject(Pointer(VCLDragSource.Tag)).DragEnd;
  VCLDragSource.Tag:=0;
end;

procedure TD3Scene.DoDragOver(Sender,Source:TObject; X,Y:integer; State:TDragState;
  var Accept:boolean);
var
  P:TD3Point;
  NewTarget:TD3VisualObject;
  Data:TD3DragObject;
  Distance:single;
begin
  Accept:=false;
  if Root=nil then Exit;

  FillChar(Data,SizeOf(Data),0);
  if (VCLDragSource <> nil) and (VCLDragSource.Tag <> 0) then
    Data.Source:=TD3Object(VCLDragSource.Tag)
  else
    Data.Source:=Source;

  P:=d3Point(X,Y,0);
  Distance:=$FFFF;
  NewTarget:=Root.Visual.FindTarget(P,d3ProjectionScreen,Data,Distance);
  if NewTarget=nil then
    NewTarget:=Root.Visual.FindTarget(P,d3ProjectionCamera,Data,Distance);

  if (VCLDragSource <> nil) and (NewTarget=TD3Object(VCLDragSource.Tag)) then
  begin
    Accept:=false;
    Exit;
  end;

  if VarD3SceneTarget <> nil then
  begin
    VarD3SceneTarget.DragOver(Data,VarD3SceneTarget.AbsoluteToLocal(P),Accept);
  end;
  if (NewTarget <> VarD3SceneTarget) then
  begin
    if VarD3SceneTarget <> nil then
      VarD3SceneTarget.DragLeave;
    VarD3SceneTarget:=NewTarget;
    if VarD3SceneTarget <> nil then
    begin
      VarD3SceneTarget.DragEnter(Data,VarD3SceneTarget.AbsoluteToLocal(P));
    end;
  end;
  if VarD3SceneTarget=nil then
    Accept:=false;
end;

procedure TD3Scene.DoDragDrop(Sender,Source:TObject;
  X,Y:integer);
var
  Data:TD3DragObject;
begin
  if VarD3SceneTarget <> nil then
  begin
    FillChar(Data,SizeOf(Data),0);
    if (VCLDragSource <> nil) and (VCLDragSource.Tag <> 0) then
      Data.Source:=TD3Object(VCLDragSource.Tag)
    else
      Data.Source:=Source;
    VarD3SceneTarget.DragDrop(Data,VarD3SceneTarget.AbsoluteToLocal(d3Point(X,Y,0)));
  end;
  VarD3SceneTarget:=nil;
end;

procedure TD3Scene.Paint;
begin
  if not FDisableDraw then Draw;
end;

function TD3Scene.ObjectByPoint(X,Y:single):TD3VisualObject;
var
  i:integer;
  Obj,NewObj:TD3Object;
  Distance:single;
begin
  Result:=nil;
  for i:=Count - 1 downto 0 do
  begin
    Obj:=Children[i];
    if not (Obj is TD3VisualObject) then Exit;
    if Obj.Tag=$FFFe then Continue;
    if not TD3VisualObject(Obj).Visible and (not DesignTime) then Continue;
    if (TD3VisualObject(Obj).Locked) and (DesignTime) then Continue;

    Distance:=$FFFF;
    // first see screen projection
    NewObj:=TD3VisualObject(Obj).ObjectByPoint(X,Y,d3ProjectionScreen,Distance);
    if (NewObj <> nil) then
    begin
      Result:=TD3VisualObject(NewObj);
    end
    else
    begin
      Distance:=$FFFF;
      NewObj:=TD3VisualObject(Obj).ObjectByPoint(X,Y,d3ProjectionCamera,Distance);
      if (NewObj <> nil) and (NewObj <> Root) then
      begin
        Result:=TD3VisualObject(NewObj);
      end
      else
        if TD3VisualObject(Root).CheckHitTest(TD3VisualObject(Root).HitTest) then
        begin
          if (not DesignTime and not AllowDrag) then
            Result:=TD3VisualObject(Root)
          else
            Result:=nil;
        end
        else
          Result:=nil;
    end;
  end;
end;

procedure TD3Scene.DeleteChildren;
var
  Child:TD3Object;
begin
  if Assigned(FChildren) then
    while FChildren.Count > 0 do
    begin
      Child:=TD3Object(FChildren[0]);
      FChildren.Delete(0);
      Child.FParent:=nil;
      Child.FScene:=nil;
      Child.Free;
    end;
end;

procedure TD3Scene.AddObject(AObject:TD3Object);
begin
  if AObject.Parent <> nil then
    AObject.Parent:=nil;
  if FChildren=nil then
    FChildren:=TList.Create;
  FChildren.Add(AObject);
  AObject.FScene:=Self;
  if AObject.IsVisual and not (csDestroying in ComponentState) then
    TD3VisualObject(AObject).Repaint;
  if (DesignTime) and (AObject is TD3Camera) and (AObject.Tag <> $FFFe) then
    Camera:=TD3Camera(AObject);
end;

procedure TD3Scene.RemoveObject(AObject:TD3Object);
begin
  if FChildren <> nil then
  begin
    FChildren.Remove(AObject);
    Invalidate;
  end;
end;

function TD3Scene.GetCount:integer;
begin
  if FChildren <> nil then
    Result:=FChildren.Count
  else
    Result:=0;
end;

function TD3Scene.GetChildrenObject(Index:integer):TD3Object;
begin
  if FChildren <> nil then
    Result:=TD3Object(FChildren[Index])
  else
    Result:=nil;
end;

procedure TD3Scene.SetChildren(Index:integer; const Value:TD3Object);
begin
end;

procedure TD3Scene.BeginDrag;
var
  P:TPoint;
begin
  FDragging:=true;
  GetCursorPos(P);
  P:=TWinControl(Owner).ScreenToClient(P);
//  FDownPos:=d3Point(P.X,P.Y);
  MouseCapture;
end;

procedure TD3Scene.BeginResize;
var
  P:TPoint;
begin
  FResizing:=true;
  GetCursorPos(P);
  P:=TWinControl(Owner).ScreenToClient(P);
//  FDownPos:=d3Point(P.X,P.Y);
  FResizeSize:=Point(TWinControl(Owner).Width,TWinControl(Owner).Height);
  MouseCapture;
end;

function TD3Scene.SnapToGridValue(Value:single):single;
begin
  if (DesignSnapToGrid) and (DesignSnapGridSize <> 0) then
    Result:=Trunc(Value / DesignSnapGridSize) * DesignSnapGridSize
  else
    Result:=Value;
end;

procedure TD3Scene.SnapToGridLines(AllowChangePosition:boolean);
var
  i:integer;
begin
Exit;
  if (DesignSnapToLines) and (FSelected.Parent <> nil) then
    for i:=0 to FSelected.Parent.FChildren.Count - 1 do
    begin
      if TD3Object(FSelected.Parent.FChildren[i])=FSelected then Continue;
      if not TD3Object(FSelected.Parent.FChildren[i]).isVisual then Continue;
      with TD3VisualObject(FSelected.Parent.FChildren[i]) do
      begin
        if (Abs(Position.X - FSelected.Position.X) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.X:=Position.X;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs(Position.Y - FSelected.Position.Y) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.Y:=Position.Y;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs(Position.X - (FSelected.Position.X + FSelected.Width)) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.X:=Position.X - FSelected.Width;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs(Position.Y - (FSelected.Position.Y + FSelected.Height)) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.Y:=Position.Y - FSelected.Height;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs((Position.X + Width) - FSelected.Position.X) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.X:=Position.X + Width;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs((Position.Y + Height) - FSelected.Position.Y) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.Y:=Position.Y + Height;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs((Position.X + Width) - (FSelected.Position.X + FSelected.Width)) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.X:=Position.X + Width - FSelected.Width;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
        if (Abs((Position.Y + Height) - (FSelected.Position.Y + FSelected.Height)) < (2)) then
        begin
          if AllowChangePosition then
            FSelected.Position.Y:=Position.Y + Height - FSelected.Height;
          SetLength(FDesignGridLines,Length(FDesignGridLines) + 1);
          FDesignGridLines[High(FDesignGridLines)]:=TD3VisualObject(FSelected.Parent.FChildren[i]);
          Continue;
        end;
      end;
    end;
end;

function TD3Scene.SnapPointToGridLines(const APoint:TD3Point):TD3Point;
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
    Result:=TD3VisualObject(FSelected.Parent).AbsoluteToLocal(Result);
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
    Result:=TD3VisualObject(FSelected.Parent).LocalToAbsolute(Result);
    Result:=FSelected.AbsolutetoLocal(Result);
  end;
end;

procedure TD3Scene.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:integer);
var
  P:TD3Point;
  R:TD2Rect;
  Obj:TD3VisualObject;
  InsertObject:TD3Object;
  V,V1,BoxCenter:TD3Vector;
  i:integer;
begin
  inherited;
  if Canvas=nil then Exit;
  if not (DesignTime) and not Assigned(FOnFlush) then
    SetFocus;
  { translate coord }
  FUnsnapMousePos:=d3Point(x,y,0);
  FMousePos:=d3Point(SnapToGridValue(x),SnapToGridValue(y),0);
  FDownPos:=FMousePos;
  SetLength(FDesignGridLines,0);
  { design }
  if (DesignTime) then
  begin
    { Create root }
    if (Root=nil) then
    begin
      Obj:=TD3VisualObject.Create(Owner);
      if GvarD3Designer <> nil then
        Obj.Name:=GvarD3Designer.UniqueName(Owner,'Root');
      AddObject(Obj);
      Exit;
    end;
    { Resize }
    if (FSelected <> nil) and (FSelected <> Root) then
    begin
      Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
      V:=FSelected.AbsoluteToLocalVector(V);
      V1:=FSelected.AbsoluteToLocalVector(V1);
      if FSelected.FCanResize then
      begin
        { rightTop }
        BoxCenter:=d3VectorAdd(d3Vector(0,0,0),d3Vector(FSelected.Width / 2,-FSelected.Height / 2,FSelected.Depth / 2));
        if RayCastIntersectsSphere(V,V1,BoxCenter,GripSize * 2) then
        begin
          if GvarD3Designer <> nil then
            GvarD3Designer.SelectObject(Owner,FSelected,[]);
          FRightTop:=true;
          FMoving:=false;
          Exit;
        end;
        { leftFar }
        BoxCenter:=d3VectorAdd(d3Vector(0,0,0),d3Vector(-FSelected.Width / 2,FSelected.Height / 2,FSelected.Depth / 2));
        if RayCastIntersectsSphere(V,V1,BoxCenter,GripSize * 2) then
        begin
          if GvarD3Designer <> nil then
            GvarD3Designer.SelectObject(Owner,FSelected,[]);
          FLeftFar:=true;
          FMoving:=false;
          Exit;
        end;
      end;
      if FSelected.FCanRotate then
      begin
        if FSelected.Projection=d3ProjectionCamera then
        begin
          { rotatez }
          BoxCenter:=d3VectorAdd(d3Vector(0,0,0),d3Vector(0,0,FSelected.Depth / 2 + RotateGripDist));
          if RayCastIntersectsSphere(V,V1,BoxCenter,GripSize * 2) then
          begin
            if GvarD3Designer <> nil then
              GvarD3Designer.SelectObject(Owner,FSelected,[]);
            FRotateX:=true;
            FMoving:=false;
            Exit;
          end;
          { rotatey }
          BoxCenter:=d3VectorAdd(d3Vector(0,0,0),d3Vector(FSelected.Width / 2 + RotateGripDist,0,0));
          if RayCastIntersectsSphere(V,V1,BoxCenter,GripSize * 2) then
          begin
            if GvarD3Designer <> nil then
              GvarD3Designer.SelectObject(Owner,FSelected,[]);
            FRotateY:=true;
            FMoving:=false;
            Exit;
          end;
        end;
        { rotatex }
        BoxCenter:=d3VectorAdd(d3Vector(0,0,0),d3Vector(0,-FSelected.Height / 2 - RotateGripDist,0));
        if RayCastIntersectsSphere(V,V1,BoxCenter,GripSize * 2) then
        begin
          if GvarD3Designer <> nil then
            GvarD3Designer.SelectObject(Owner,FSelected,[]);
          FRotateZ:=true;
          FMoving:=false;
          Exit;
        end;
      end;
    end;
    { Change Selected }
    Obj:=ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y);
    if (Obj <> nil) and (Obj=FSelected) and (ssDouble in Shift) then
    begin
      Obj.DesignClick;
    end;
    { Design interact }
    if (Obj=FSelected) and (FSelected <> nil) and (FSelected.DesignInteract) and ([ssShift] * Shift=[]) then
    begin
      Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
      V:=FSelected.AbsoluteToLocalVector(V);
      V1:=FSelected.AbsoluteToLocalVector(V1);
      FSelected.MouseDown(Button,Shift,FUnsnapMousePos.X,FUnsnapMousePos.Y,V,V1);
      Exit;
    end;
    { Popup }
    if Button=mbRight then
      OpenDesignPopup;
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
        if GvarD3Designer <> nil then
          GvarD3Designer.SelectObject(Owner,Selected,FSelection);
        Exit;
      end;
      SetLength(FSelection,0);
      if (Obj <> FSelected) then
      begin
        if FSelected <> nil then
          FSelected.Repaint;
        FSelected:=Obj;
        FSelected.DesignSelect;
        // Select in IDE
        if GvarD3Designer <> nil then
          GvarD3Designer.SelectObject(Owner,Obj,[]);
        FSelected.Repaint;
      end
      else
       if (FSelected <> Root) then
         FMoving:=true;
    end
    else
      if Root <> nil then
      begin
        if FSelected <> nil then
          FSelected.Repaint;
        FSelected:=TD3VisualObject(Root);
        FSelected.DesignSelect;
        // Select in IDE
        if GvarD3Designer <> nil then
          GvarD3Designer.SelectObject(Owner,FSelected,[]);
        FSelected.Repaint;
      end;
    Exit;
  end;
  { event }
  if not (DesignTime) then
  begin
    Obj:=TD3VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
    if (Obj <> nil) then
    begin
      P:=Obj.AbsoluteToLocal(d3Point(FUnsnapMousePos.X,FUnsnapMousePos.Y,0));
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,Obj.Projection,V,V1);
      V:=Obj.AbsoluteToLocalVector(V);
      V1:=Obj.AbsoluteToLocalVector(V1);
      Obj.MouseDown(Button,Shift,P.X,P.Y,V,V1);
    end
    else
      if AllowDrag and (Button=mbLeft) then
        BeginDrag;
  end;
end;

procedure TD3Scene.MouseMove(Shift:TShiftState; X,Y:integer);
var
  R:TD2Rect;
  P:TD3Point;
  V,V1,V2:TD3Vector;
  Obj:TD3VisualObject;
  SaveWidth:single;
  NewCursor:TCursor;
begin
  inherited;
  if Canvas=nil then Exit;
  NewCursor:=FLoadCursor;
  { drag }
  if FDragging then
  begin
    with TWinControl(Owner).ScreenToClient(ClientToScreen(Point(X,Y))) do
    begin
      TWinControl(Owner).Left:=TWinControl(Owner).Left + Trunc(X - FDownPos.X);
      TWinControl(Owner).Top:=TWinControl(Owner).Top + Trunc(Y - FDownPos.Y);
    end;
    Exit;
  end;
{  if Shift=[] then
  begin
    Obj:=TD3VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    if (Obj <> nil) and ((Obj.QueryInterface(Id2SizeGrip,SG)=0) and Assigned(SG)) then
      SetCursor(Screen.Cursors[crSizeNWSE]);
  end;}
  if FResizing then
  begin
    with TWinControl(Owner).ScreenToClient(ClientToScreen(Point(X,Y))) do
    begin
      FResizeSize.X:=FResizeSize.X + Trunc(X - FUnsnapMousePos.X);
      FResizeSize.Y:=FResizeSize.Y + Trunc(Y - FUnsnapMousePos.Y);
      TWinControl(Owner).Width:=FResizeSize.X;
      TWinControl(Owner).Height:=FResizeSize.Y;
//      SetCursor(Screen.Cursors[crSizeNWSE]);
    end;
    FUnsnapMousePos:=d3Point(x,y,0);
    Exit;
  end;
  { translate coord }
  FMousePos:=d3Point(SnapToGridValue(x),SnapToGridValue(y),0);
  FUnsnapMousePos:=d3Point(x,y,0);
  { design }
  if (DesignTime) then
  begin
    { root pan and rotate }
    if FDesignAllowPanAndRotate and (ssLeft in Shift) and (ssAlt in Shift) then
    begin
      { rotate Z }
      FDesignCameraZ.RotateAngle.Z:=FDesignCameraZ.RotateAngle.Z + ((FMousePos.X - FDownPos.X) * 0.3);
      { rotate X }
      FDesignCameraX.RotateAngle.X:=FDesignCameraX.RotateAngle.X + ((FMousePos.Y - FDownPos.Y) * 0.3);
      FDownPos:=FMousePos;
      Exit;
    end;
    { Manipulation }
    if FDesignAllowPanAndRotate and ((FSelected=nil) or (FSelected=Root)) and (ssLeft in Shift) and (ssShift in Shift) then
    begin
      Canvas.Pick(FDownPos.X,FDownPos.Y,d3ProjectionCamera,V,V1);
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,d3ProjectionCamera,V,V2);
      if ssCtrl in Shift then
      begin
        { x0z }
        RayCastPlaneIntersect(V,V1,FDesignCameraZ.AbsolutePosition,FDesignCameraZ.AbsoluteDirection,V1);
        RayCastPlaneIntersect(V,V2,FDesignCameraZ.AbsolutePosition,FDesignCameraZ.AbsoluteDirection,V2);
      end
      else
      begin
        { x0y }
        RayCastPlaneIntersect(V,V1,FDesignCameraZ.AbsolutePosition,FDesignCameraZ.AbsoluteUp,V1);
        RayCastPlaneIntersect(V,V2,FDesignCameraZ.AbsolutePosition,FDesignCameraZ.AbsoluteUp,V2);
      end;
      FV1:=V1;
      FV2:=V2;
      if (FDesignCameraZ.Parent <> nil) and (FDesignCameraZ.Parent.IsVisual) then
      begin
        V1:=TD3VisualObject(FDesignCameraZ.Parent).AbsoluteToLocalVector(V1);
        V2:=TD3VisualObject(FDesignCameraZ.Parent).AbsoluteToLocalVector(V2);
      end;
      V:=d3VectorSubtract(V1,V2);
      FDesignCameraZ.Position.Vector:=d3VectorAdd(FDesignCameraZ.Position.Vector,V);
      FDownPos:=FMousePos;
      Exit;
    end;
    { resize and move }
    if (FSelected <> Root) and (FSelected <> nil) then
    begin
      if (ssLeft in Shift) and (FMoving) then
      begin
        Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
        Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
        if ssCtrl in Shift then
        begin
          { x0z }
          RayCastPlaneIntersect(V,V1,FSelected.AbsolutePosition,d3Vector(0,1,0),V1);
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,d3Vector(0,1,0),V2);
        end
        else
        begin
          { x0y }
          RayCastPlaneIntersect(V,V1,FSelected.AbsolutePosition,d3Vector(0,0,1),V1);
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,d3Vector(0,0,1),V2);
        end;
        FV1:=V1;
        FV2:=V2;
        if (FSelected.Parent <> nil) and (FSelected.Parent.IsVisual) then
        begin
          V1:=TD3VisualObject(FSelected.Parent).AbsoluteToLocalVector(V1);
          V2:=TD3VisualObject(FSelected.Parent).AbsoluteToLocalVector(V2);
        end;
        V:=d3VectorSubtract(V2,V1);
        with d3VectorAdd(FSelected.Position.Vector,V) do
        begin
          if FSelected.Projection=d3ProjectionScreen then
            FSelected.Position.Vector:=d3Vector(round(X),round(Y),round(Z))
          else
            FSelected.Position.Vector:=d3Vector(X,Y,Z);
        end;
        { lines grid }
        SnapToGridLines(true);
        { check place }
        FSelected.FLocked:=true;
        Obj:=TD3VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
        FSelected.FLocked:=false;
      end;
      if FSelected.FCanResize then
      begin
        if (ssLeft in Shift) and (FSelected <> nil) and (FRightTop) then
        begin
          Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
          Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
          { x0z }
          RayCastPlaneIntersect(V,V1,FSelected.AbsolutePosition,FSelected.AbsoluteDirection,V1);
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,FSelected.AbsoluteDirection,V2);
          V1:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V1);
          V2:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V2);
          V:=d3VectorSubtract(V2,V1);
          FSelected.Width:=FSelected.Width + V.X;
          FSelected.Depth:=FSelected.Depth + V.Z;
          FSelected.Repaint;
        end;
        if (ssLeft in Shift) and (FSelected <> nil) and (FLeftFar) then
        begin
          Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
          Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
          { x0y }
          RayCastPlaneIntersect(V,V1,FSelected.AbsolutePosition,FSelected.AbsoluteUp,V1);
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,FSelected.AbsoluteUp,V2);
          V1:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V1);
          V2:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V2);
          V:=d3VectorSubtract(V2,V1);
          FSelected.Width:=FSelected.Width - V.X;
          FSelected.Height:=FSelected.Height + V.Y;
          FSelected.Repaint;
        end;
      end;
      if FSelected.FCanRotate then
      begin
        if (ssLeft in Shift) and (FSelected <> nil) and (FRotateX) then
        begin
          Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
          { x0y }
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,FSelected.AbsoluteRight,V2);
          V1:=d3Vector(0,0,1);
          V2:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V2);
          V:=d3VectorCrossProduct(V1,V2);
          if V.X < 0 then
            FSelected.RotateAngle.X:=FSelected.RotateAngle.X + RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)))
          else
            FSelected.RotateAngle.X:=FSelected.RotateAngle.X - RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)));
          FSelected.Repaint;
        end;
        if (ssLeft in Shift) and (FSelected <> nil) and (FRotateY) then
        begin
          Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
          { x0y }
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,FSelected.AbsoluteDirection,V2);
          V1:=d3Vector(1,0,0);
          V2:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V2);
          V:=d3VectorCrossProduct(V1,V2);
          if V.Y > 0 then
            FSelected.RotateAngle.Y:=FSelected.RotateAngle.Y + RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)))
          else
            FSelected.RotateAngle.Y:=FSelected.RotateAngle.Y - RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)));
          FSelected.Repaint;
        end;
        if (ssLeft in Shift) and (FSelected <> nil) and (FRotateZ) then
        begin
          Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FSelected.Projection,V,V2);
          { x0y }
          RayCastPlaneIntersect(V,V2,FSelected.AbsolutePosition,FSelected.AbsoluteUp,V2);
          V1:=d3Vector(0,-1,0);
          V2:=TD3VisualObject(FSelected).AbsoluteToLocalVector(V2);
          V:=d3VectorCrossProduct(V1,V2);
          if V.Z < 0 then
            FSelected.RotateAngle.Z:=FSelected.RotateAngle.Z + RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)))
          else
            FSelected.RotateAngle.Z:=FSelected.RotateAngle.Z - RadToDeg(ArcCos(d3VectorAngleCosine(V1,V2)));
          FSelected.Repaint;
        end;
      end;
      FDownPos:=FMousePos;
    end;
    { Design interact }
    if (FSelected <> nil) and (FSelected.DesignInteract) then
    begin
      Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
      V:=FSelected.AbsoluteToLocalVector(V);
      V1:=FSelected.AbsoluteToLocalVector(V1);
      FSelected.MouseMove(Shift,FUnsnapMousePos.X,FUnsnapMousePos.Y,0,0,V,V1);
      Exit;
    end;
    Exit;
  end;
  { event }
  if not (DesignTime) then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d3Point(FUnsnapMousePos.X,FUnsnapMousePos.Y,0));
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FCaptured.Projection,V,V1);
      V:=FCaptured.AbsoluteToLocalVector(V);
      V1:=FCaptured.AbsoluteToLocalVector(V1);
      FCaptured.MouseMove(Shift,P.X,P.Y,0,0,V,V1);
      Exit;
    end;
    Obj:=TD3VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
    if (Obj <> nil) then
    begin
{      if (Obj.QueryInterface(Id2SizeGrip,SG)=0) and Assigned(SG) then
        NewCursor:=crSizeNWSE
      else}
        NewCursor:=Obj.Cursor;

      if (Obj <> FHovered) then
      begin
        if FHovered <> nil then
        begin
          FHovered.MouseInObject:=false;
          FHovered.MouseLeave;
        end;
        FHovered:=Obj;
        FHovered.MouseInObject:=true;
        FHovered.MouseEnter;
      end;

      P:=Obj.AbsoluteToLocal(d3Point(FUnsnapMousePos.X,FUnsnapMousePos.Y,0));
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,Obj.Projection,V,V1);
      V:=Obj.AbsoluteToLocalVector(V);
      V1:=Obj.AbsoluteToLocalVector(V1);
      Obj.MouseMove(Shift,P.X,P.Y,0,0,V,V1);
    end
    else
      if FHovered <> nil then
      begin
        FHovered.MouseInObject:=false;
        FHovered.MouseLeave;
        FHovered:=nil;
      end;
  end;
  FDownPos:=FMousePos;
  Cursor:=NewCursor;
end;

procedure TD3Scene.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:integer);
var
  P:TD3Point;
  Obj:TD3VisualObject;
  V,V1:TD3Vector;
begin
  inherited;
  if Canvas=nil then Exit;
  { design }
  SetLength(FDesignGridLines,0);
  if (FSelected <> nil) and (FMoving or FRightTop or FLeftFar or FRotateY or FRotateX or FRotateZ) then
  begin
    if (FSelected.Parent <> nil) and (TD3Object(FSelected.Parent).IsVisual) then
      TD3VisualObject(FSelected.Parent).Realign;
    if (GvarD3Designer <> nil) then
      GvarD3Designer.Modified(Owner);
    if not Assigned(FOnFlush) then
      ReleaseCapture;
  end;
  FMoving:=false;
  FRightTop:=false;
  FLeftFar:=false;
  FRotateY:=false;
  FRotateZ:=false;
  FRotateX:=false;
  { drag }
  if FDragging then
  begin
    FDragging:=false;
    ReleaseCapture;
  end;
  if FResizing then
  begin
    FResizing:=false;
    ReleaseCapture;
  end;
  { event }
  if not (DesignTime) then
  begin
    if (FCaptured <> nil) then
    begin
      P:=FCaptured.AbsoluteToLocal(d3Point(FUnsnapMousePos.X,FUnsnapMousePos.Y,0));
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,FCaptured.Projection,V,V1);
      V:=FCaptured.AbsoluteToLocalVector(V);
      V1:=FCaptured.AbsoluteToLocalVector(V1);
      FCaptured.MouseUp(Button,Shift,P.X,P.Y,V,V1);
      Exit;
    end;
    Obj:=TD3VisualObject(ObjectByPoint(FUnsnapMousePos.X,FUnsnapMousePos.Y));
    if (Obj <> nil) then
    begin
      P:=Obj.AbsoluteToLocal(d3Point(FUnsnapMousePos.X,FUnsnapMousePos.Y,0));
      Canvas.Pick(FUnsnapMousePos.X,FUnsnapMousePos.Y,Obj.Projection,V,V1);
      V:=Obj.AbsoluteToLocalVector(V);
      V1:=Obj.AbsoluteToLocalVector(V1);
      Obj.MouseUp(Button,Shift,P.X,P.Y,V,V1);
    end;
  end
  else
  begin
    { Design interact }
    if (FSelected <> nil) and (FSelected.DesignInteract) then
    begin
      Canvas.Pick(FDownPos.X,FDownPos.Y,FSelected.Projection,V,V1);
      V:=FSelected.AbsoluteToLocalVector(V);
      V1:=FSelected.AbsoluteToLocalVector(V1);
      FSelected.MouseUp(Button,Shift,FUnsnapMousePos.X,FUnsnapMousePos.Y,V,V1);
      Exit;
    end;
  end;
end;

function TD3Scene.DoMouseWheel(Shift:TShiftState; WheelDelta:integer;
   MousePos:TPoint):boolean;
var
  Obj:TD3VisualObject;
begin
  Result:=inherited DoMouseWheel(Shift,WheelDelta,MousePos);
  {$IFDEF DARWIN}
//  WheelDelta:=WheelDelta * 40;
  {$ENDIF}
  { event }
  if (FDesignCamera <> nil) and (FDesignTime) then
  begin
    FDesignCamera.Position.Vector:=d3VectorAdd(FDesignCamera.Position.Vector,d3VectorScale(d3Vector(0,1,0),WheelDelta / 120));
  end;
  if not (DesignTime) then
  begin
    if (FCaptured <> nil) then
    begin
      FCaptured.MouseWheel(Shift,WheelDelta,Result);
      Exit;
    end;
    Obj:=TD3VisualObject(ObjectByPoint(FMousePos.X,FMousePos.Y));
    while (Obj <> nil) do
    begin
      Obj.MouseWheel(Shift,WheelDelta,Result);
      if Result then Break;
      if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
        Obj:=TD3VisualObject(Obj.Parent)
      else
        Obj:=nil;
    end;
  end;
end;

procedure TD3Scene.CMDesignHitTest(var Msg: TLMMouse );
begin
  inherited ;
  Msg.Result:=1;
end;

procedure TD3Scene.CMHintShow(var Message: TLMessage );
var
  Obj:TD3VisualObject;
begin
  inherited ;
  with TCMHintShow(Message).HintInfo^ do
  begin
    Obj:=ObjectByPoint(CursorPos.X,CursorPos.Y);
    if (Obj <> nil) and (Obj.ShowHint) then
    begin
      if not Obj.DoHintShow(Message) then
      begin
        HintStr:=Obj.Hint;
        CursorRect:=types.Rect(CursorPos.X - 10,CursorPos.Y - 10,CursorPos.X + 10,CursorPos.Y + 10);
      end;
    end
    else
    begin
      HintStr:='';
    end;
  end;
end;


procedure TD3Scene.KeyUp(var Key:Word; Shift:TShiftState);
var
  Char:system.WideChar;
begin
  inherited;
  FShift:=[];
  if (DesignTime) then
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

procedure TD3Scene.KeyDown(var Key:Word; Shift:TShiftState);
var
  List:TList;
  i,CurId3:integer;
  Found:boolean;
  K:Word;
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
        NewKeyDown(K,Ch,[ssCtrl]);
        NewKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      86:// ctrl+v
      begin
        Ch:='v';
        NewKeyDown(K,Ch,[ssCtrl]);
        NewKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      88:// ctrl+x
      begin
        Ch:='x';
        NewKeyDown(K,Ch,[ssCtrl]);
        NewKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
      99:// ctrl+z
      begin
        Ch:='z';
        NewKeyDown(K,Ch,[ssCtrl]);
        NewKeyUp(K,Ch,[ssCtrl]);
        Exit;
      end;
    end;
  end;
  { change focus }
  if (Key=VK_TAB) and (Root <> nil) then
  begin
    Key:=0;
    List:=TList.Create;
    TD3VisualObject(Root).GetTabOrderList(List);
    if ssShift in Shift then
    begin
      if (FFocused <> nil) then
      begin
        if FFocused is TD3CustomObjectLayer then
        begin
          CurId3:=List.IndexOf(TD3CustomObjectLayer(FFocused).Focused) - 1
        end
        else
          CurId3:=List.IndexOf(FFocused) - 1
      end
      else
        CurId3:=0;
      Found:=false;
      { first search in last part of list }
      if CurId3 > 0 then
        for i:=CurId3 - 1 downto 0 do
        begin
          if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
          begin
            if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) and (TD3VisualObject(List[i]) <> FFocused) then
            begin
              TD3VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
          if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
          begin
            if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
            begin
              TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
        end;
      { second search in last part of list }
      if not Found then
        if (List.Count > 2) and (CurId3 < List.Count) then
          for i:=List.Count - 1 downto CurId3 do
          begin
            if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
            begin
              if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) and (TD3VisualObject(List[i]) <> FFocused) then
              begin
                TD3VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
            if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
            begin
              if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
              begin
                TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
                TD2VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
          end;
    end
    else
    begin
      if (FFocused <> nil) then
      begin
        if FFocused is TD3CustomObjectLayer then
        begin
          CurId3:=List.IndexOf(TD3CustomObjectLayer(FFocused).Focused) + 1
        end
        else
          CurId3:=List.IndexOf(FFocused) + 1
      end
      else
        CurId3:=0;
      Found:=false;
      { first search in last part of list }
      if (List.Count > 2) and (CurId3 < List.Count) then
        for i:=CurId3 to List.Count - 1 do
        begin
          if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
          begin
            if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) then
            begin
              TD3VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
          if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
          begin
            if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
            begin
              TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
        end;
      { second search in last part of list }
      if not Found then
        if CurId3 > 0 then
          for i:=0 to CurId3 - 1 do
          begin
            if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
            begin
              if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) then
              begin
                TD3VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
            if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
            begin
              if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
              begin
                TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
                TD2VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
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

procedure TD3Scene.UTF8KeyPress(var UTF8Key:TUTF8Char);
var
  S:System.WideChar;
  K:word;
  C,Ch:System.WideChar;
begin
  K:=0;
  C:=UTF8ToUTF16(UTF8Key)[1];
  case C of
    #3:// ctrl+c
      begin
        Ch:='c';
        NewKeyDown(K,Ch,FShift);
        NewKeyUp(K,Ch,FShift);
      end;
    #$16:// ctrl+v
      begin
        Ch:='v';
        NewKeyDown(K,Ch,FShift);
        NewKeyUp(K,Ch,FShift);
      end;
    #$18:// ctrl+x
      begin
        Ch:='x';
        NewKeyDown(K,Ch,FShift);
        NewKeyUp(K,Ch,FShift);
      end;
    #$1A:// ctrl+z
      begin
        Ch:='z';
        NewKeyDown(K,Ch,FShift);
        NewKeyUp(K,Ch,FShift);
      end;
    else
      NewKeyDown(K,C,FShift);
      NewKeyUp(K,C,FShift);
      UTF8Key:='';
  end;
end;

procedure TD3Scene.NewKeyDown(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
var
  List:TList;
  i,CurId3:integer;
  Found:boolean;
begin
  inherited;
  { change focus }
  if (Key=VK_TAB) and (Root <> nil) then
  begin
    Key:=0;
    List:=TList.Create;
    TD3VisualObject(Root).GetTabOrderList(List);
    if ssShift in Shift then
    begin
      if (FFocused <> nil) then
      begin
        if FFocused is TD3CustomObjectLayer then
        begin
          CurId3:=List.IndexOf(TD3CustomObjectLayer(FFocused).Focused) - 1
        end
        else
          CurId3:=List.IndexOf(FFocused) - 1
      end
      else
        CurId3:=0;
      Found:=false;
      { first search in last part of list }
      if CurId3 > 0 then
        for i:=CurId3 - 1 downto 0 do
        begin
          if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
          begin
            if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) then
            begin
              TD3VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
          if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
          begin
            if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
            begin
              TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
        end;
      { second search in last part of list }
      if not Found then
        if (List.Count > 2) and (CurId3 < List.Count) then
          for i:=List.Count - 1 downto CurId3 do
          begin
            if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
            begin
              if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) then
              begin
                TD3VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
            if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
            begin
              if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
              begin
                TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
                TD2VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
          end;
    end
    else
    begin
      if (FFocused <> nil) then
      begin
        if FFocused is TD3CustomObjectLayer then
        begin
          CurId3:=List.IndexOf(TD3CustomObjectLayer(FFocused).Focused) + 1
        end
        else
          CurId3:=List.IndexOf(FFocused) + 1
      end
      else
        CurId3:=0;
      Found:=false;
      { first search in last part of list }
      if (List.Count > 2) and (CurId3 < List.Count) then
        for i:=CurId3 to List.Count - 1 do
        begin
          if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
          begin
            if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) and (TD3VisualObject(List[i]) <> FFocused) then
            begin
              TD3VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
          if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
          begin
            if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
            begin
              TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
              TD2VisualObject(List[i]).SetFocus;
              Found:=true;
              Break;
            end;
          end;
        end;
      { second search in last part of list }
      if not Found then
        if CurId3 > 0 then
          for i:=0 to CurId3 - 1 do
          begin
            if (TObject(List[i]) is TD3Object) and TD3Object(List[i]).isVisual then
            begin
              if TD3Object(List[i]).isVisual and TD3VisualObject(List[i]).Visible and (TD3VisualObject(List[i]).CanFocused) and (TD3VisualObject(List[i]) <> FFocused) then
              begin
                TD3VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
            if (TObject(List[i]) is TD2Object) and TD2Object(List[i]).isVisual then
            begin
              if TD2Object(List[i]).isVisual and TD2VisualObject(List[i]).CheckParentVisible and TD2VisualObject(List[i]).CanFocused then
              begin
                TD3CustomObjectLayer(TD2VisualObject(List[i]).Scene.GetComponent).SetFocus;
                TD2VisualObject(List[i]).SetFocus;
                Found:=true;
                Break;
              end;
            end;
          end;
    end;
    List.Free;
    Exit;
  end;
  { focused handler }
  if FFocused <> nil then
  begin
    FFocused.KeyDown(Key,Char,Shift);
  end;
end;

procedure TD3Scene.NewKeyUp(var Key:Word; var Char:System.WideChar; Shift:TShiftState);
begin
  inherited;
  if (DesignTime) then
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
    FFocused.KeyUp(Key,Char,Shift);
  end;
end;

procedure TD3Scene.SetBounds(ALeft,ATop,AWidth,AHeight:integer);
begin
  inherited;
  if (Canvas <> nil) and (HandleAllocated) then
  begin
    Canvas.FLockable:=Transparency;
    Canvas.ResizeBuffer(AWidth,AHeight);
    if (Root <> nil) and (Root.IsVisual) then
      Root.Visual.Realign;
  end;
end;

procedure TD3Scene.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FCaptured) then
    FCaptured:=nil;
  if (Operation=opRemove) and (AComponent=FSelected) then
    FSelected:=nil;
  if (Operation=opRemove) and (AComponent=FHovered) then
    FHovered:=nil;
  if (Operation=opRemove) and (AComponent=FFocused) then
    FFocused:=nil;
  if (Operation=opRemove) and (AComponent=FCamera) then
    FCamera:=nil;
end;

procedure TD3Scene.SetSnapGridShow(const Value:boolean);
begin
  FSnapGridShow:=Value;
  if FSnapGridShow then
    FDesignGrid.Opacity:=1
  else
    FDesignGrid.Opacity:=0;
end;

procedure TD3Scene.SetAllowPanAndRotate(const Value:boolean);
begin
  FDesignAllowPanAndRotate:=Value;
end;

procedure TD3Scene.SetDesignHintShow(const Value:boolean);
begin
  FDesignShowHint:=Value;
  Invalidate;
end;

procedure TD3Scene.SetSnapGridSize(const Value:single);
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

procedure TD3Scene.InsertObject(const ClassName:string); // 7777
var
  P:TPoint;
  Obj:TD3Object;
  InsertPos:TD3Point;
begin
  if GetClass(ClassName) <> nil then
  begin
    if GetClass(ClassName).InheritsFrom(TD3Object) then
    begin
      if FSelected <> nil then
      begin
        try
          Obj:=TD3ObjectClass(GetClass(ClassName)).Create(Owner);

          if GvarD3Designer <> nil then
           begin
            Obj.Name:=GvarD3Designer.UniqueName(Owner,Obj.ClassName);
            GvarD3Designer.AddObject(Obj);
           end;

          FSelected.AddObject(Obj);

          if GvarD3Designer <> nil then
          begin
            GvarD3Designer.SelectObject(Owner,Obj,[]);
            GvarD3Designer.Modified(Owner);
          end;

          if Obj.IsVisual then
          begin
            if GetPropInfo(Obj.ClassInfo,'Text',[tkString,tkLString,tkWString]) <> nil then
              SetStrProp(Obj,'Text',Copy(Obj.ClassName,4,Length(Obj.ClassName)));
            if (Owner is TWinControl) and (FPopupPos.X > 0) then
            begin
              P:=TWinControl(Owner).ScreenToClient(FPopupPos);
              if TD3VisualObject(Obj).Projection=d3ProjectionScreen then
              begin
                InsertPos:=FSelected.AbsoluteToLocal(d3Point(P.X,P.Y,0));
                TD3VisualObject(Obj).Position.X:=InsertPos.X + TD3VisualObject(Obj).Width / 2;
                TD3VisualObject(Obj).Position.Y:=InsertPos.Y + TD3VisualObject(Obj).Height / 2;
              end;
            end;
            FPopupPos:=Point(-1,-1);
            FSelected:=TD3VisualObject(Obj);
            FSelected.DesignSelect;
          end;
        except
        end;
      end
      else
        if (FChildren=nil) or (FChildren.Count=0) then
        begin
          // insert root object
          try
            Obj:=TD3ObjectClass(GetClass(ClassName)).Create(Owner);
            if GvarD3Designer <> nil then
              Obj.Name:=GvarD3Designer.UniqueName(Owner,Obj.ClassName);
            AddObject(Obj);
            if GvarD3Designer <> nil then
            begin
              GvarD3Designer.SelectObject(Owner,Obj,[]);
              GvarD3Designer.Modified(Owner);
            end;
            if Obj.IsVisual then
            begin
              if (Owner is TWinControl) and (FPopupPos.X > 0) then
              begin
                P:=TWinControl(Owner).ScreenToClient(FPopupPos);
                TD3VisualObject(Obj).Position.X:=P.X;
                TD3VisualObject(Obj).Position.Y:=P.Y;
              end;
              FPopupPos:=Point(-1,-1);
              FSelected:=TD3VisualObject(Obj);
              FSelected.DesignSelect;
            end;
          except
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

procedure TD3Scene.Resize;
begin
  inherited;
end;
  {
procedure TD3Scene.popupDrawItem(Sender:TObject; ACanvas:TCanvas; ARect:TRect; Selected:boolean);
var
  B:TD3Bitmap;
  C:TD3Canvas;
  S:TStream;
  R:TD2Rect;
  RName:string;
  W,H:integer;
begin
  W:=ARect.Right - ARect.Left;
  H:=ARect.Bottom - ARect.Top;

  C:=DefaultCanvasClass.Create(W,H);
  C.Clear($FF000000 or ColorToRGB(clMenu));

  if TMenuItem(Sender).Tag=$FF then
  begin
    RName:='PNG_TD3' + StripHotkey(TMenuItem(Sender).Caption);
    if Windows.FindResource(HInstance,PChar(RName),RT_RCDATA) <> 0 then
    begin
      S:=TResourceStream.Create(HInstance,RName,RT_RCDATA);
      B:=TD3Bitmap.CreateFromStream(S);
      S.Free;
    end
    else
    begin
      S:=TResourceStream.Create(HInstance,'PNG_Default',RT_RCDATA);
      B:=TD3Bitmap.CreateFromStream(S);
      S.Free;
    end;
    R:=d3Rect(2,2,H - 2,H - 2);
    C.DrawBitmap(B,d3Rect(0,0,B.Width,B.Height),R,1);
    B.Free;
    R:=d3Rect(H + 2,2,W - 2,H - 2);
    C.Fill.Color:='#FF000000';
    C.FillText(R,R,'  ' + StripHotkey(TMenuItem(Sender).Caption),false,1,d2TextAlignNear);
  end
  else
  begin
    R:=d3Rect(12,2,W - 2,H - 2);
    C.Fill.Color:='#FF000000';
    C.FillText(R,R,StripHotkey(TMenuItem(Sender).Caption),false,1,d2TextAlignNear);
  end;

  R:=d3Rect(2,2,W - 2,H - 2);

  if Selected then
  begin
    d2InflateRect(R,1,1);
    C.Fill.Color:='#50808080';
    C.FillRoundRect(R,5,5,1);
    C.Stroke.Color:='#FF404040';
    C.StrokeThickness:=1;
    C.Stroke.Style:=d2BrushSolid;
    C.DrawRoundRect(R,5,5,1);
  end;

  C.FlushBuffer(ARect.Left,ARect.Top,ACanvas.Handle);

  C.Free;
end; }

procedure TD3Scene.popupMeasureItem(Sender:TObject; ACanvas:TCanvas;
  var Width,Height:integer);
begin
  Width:=136;
  if TMenuItem(Sender).Tag=$FF then
    Height:=36
  else
    Height:=21;
end;

procedure TD3Scene.doDesignPopupDesignHide(Sender:TObject);
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    FSelected.FDesignHide:=not FSelected.FDesignHide;
    if FSelected.FDesignHide and (FSelected.Parent.IsVisual) then
    begin
      FSelected:=TD3VisualObject(FSelected.Parent);
    end;
    AddUpdateRect(d2Rect(0,0,Width,Height));
  end;
end;

procedure TD3Scene.doDesignPopupDel(Sender:TObject);
var
  Obj:TD3VisualObject;
begin
  if (FSelected <> nil) and (FSelected <> Root) then
  begin
    Obj:=FSelected;
    if (Obj.Parent <> nil) and (Obj.Parent.IsVisual) then
    begin
      FSelected:=TD3VisualObject(Obj.Parent);
      FSelected.DesignSelect;
    end
    else
    begin
      FSelected:=TD3VisualObject(Root);
      if FSelected <> nil then
        FSelected.DesignSelect;
    end;
    Obj.Free;
  end;
end;

procedure TD3Scene.doDesignPopupShowGrid(Sender:TObject);
begin
  if FDesignGrid <> nil then
  begin
    if TMenuItem(Sender).Checked then
      FDesignGrid.Opacity:=1
    else
      FDesignGrid.Opacity:=0;
  end;
end;

procedure TD3Scene.doDesignPopupShowHint(Sender:TObject);
begin
  DesignShowHint:=TMenuItem(Sender).Checked;
end;

procedure TD3Scene.doDesignPopupLoadFromFile(Sender:TObject);
var
  S:TStream;
  Result:TD3Object;
  Open:TOpenDialog;
begin
  Open:=TOpenDialog.Create(nil);
  Open.Filter:='d3Scene Files|*.d3scene';
  if Open.Execute then
  begin
    S:=TFileStream.Create(Open.FileName,fmOpenRead);
    Result:=CreateObject3DFromStream(Owner,S);
    if FSelected <> nil then
      Result.Parent:=FSelected
    else
      Result.Parent:=Root;
    if GvarD3Designer <> nil then
      Result.Name:=GvarD3Designer.UniqueName(Owner,Result.ClassName);
    S.Free;
  end;
  Open.Free;
end;


function StripHotkey(S:string):string;
begin
  Result:=S;
end;


procedure TD3Scene.doDesignPopupReorder(Sender:TObject);
begin
  if FSelected=nil then Exit;

  if StripHotkey(TMenuItem(Sender).Caption)='Bring to front' then
    FSelected.BringToFront;
  if StripHotkey(TMenuItem(Sender).Caption)='Send to back' then
    FSelected.SendToBack;
end;

procedure TD3Scene.doDesignPopupCopy(Sender:TObject);
var
  S:TStringStream;
begin
  if FSelected <> nil then
  begin
    S:=TStringStream.Create('');
    FSelected.SaveToStream(S);
    Clipboard.AsText:=S.DataString;
    S.Free;
  end;
end;

procedure TD3Scene.doDesignPasteFromClip(Sender:TObject);
var
  Str:TStringStream;
  Obj:TD3Object;
begin
  if Clipboard.AsText <> '' then
  begin
    Str:=TStringStream.Create(Clipboard.AsText);
    try
      if FSelected <> nil then
      begin
        try
          Obj:=CreateObject3DFromStream(Owner,Str);
          if Obj=nil then
          begin
            ShowMessage('No object in clipboard');
            Exit;
          end;
          if GvarD3Designer <> nil then
            Obj.Name:=GvarD3Designer.UniqueName(Owner,Obj.ClassName);
          FSelected.AddObject(Obj);
          if GvarD3Designer <> nil then
          begin
            GvarD3Designer.SelectObject(Owner,Obj,[]);
            GvarD3Designer.Modified(Owner);
          end;
          if Obj.IsVisual then
          begin
            if GetPropInfo(Obj.ClassInfo,'Text',[tkString,tkLString,tkWString]) <> nil then
              SetStrProp(Obj,'Text',Copy(Obj.ClassName,4,Length(Obj.ClassName)));
            FSelected:=TD3VisualObject(Obj);
            FSelected.DesignSelect;
          end;
        except
          ShowMessage('Error paste object from clipboard');
        end;
      end
    finally
      Str.Free;
    end;
  end;
end;

procedure TD3Scene.doDesignPopupAdd(Sender:TObject);
var
  S:string;
begin
  S:=StripHotkey(TMenuItem(Sender).Caption);
  if (S <> '') then
    S:='TD3' + S;
  InsertObject(S);
end;

procedure TD3Scene.OpenDesignPopup;
var
  i:integer;
  S:string;
  OItem,SItem,Item:TMenuItem;
begin
  if FDesignPopup=nil then
  begin
    FDesignPopup:=TPopupMenu.Create(Self);
    if GvarD3ObjectList <> nil then
    begin
      { add }
      Item:=NewItem('Add 3D Object',0,false,true,nil,0,'');
      { add categories }
      for i:=0 to GvarD3ObjectList.Count - 1 do
      begin
        if Item.Find(GvarD3ObjectList[i])=nil then
        begin
          SItem:=NewItem(GvarD3ObjectList[i],0,false,true,nil,0,'');
          Item.Add(SItem);
        end;
      end;
      { add objects }
      for i:=0 to GvarD3ObjectList.Count - 1 do
      begin
        S:=TD3ObjectClass(GvarD3ObjectList.Objects[i]).ClassName;
        if Pos('TD3',S)=1 then
          Delete(S,1,3);
        OItem:=NewItem(S,0,false,true,doDesignPopupAdd,0,'');
        OItem.Tag:=$FF;
{        if i mod 8=0 then
          OItem.Break:=mbBreak;}
        SItem:=Item.Find(GvarD3ObjectList[i]);
        if SItem <> nil then
          SItem.Add(OItem);
      end;
      FDesignPopup.Items.Add(Item);
      { Design Hide }
      if FSelected <> nil then
      begin
        Item:=NewItem('Hide in Design-time',0,FSelected.FDesignHide,true,doDesignPopupDesignHide,0,'');
        Item.AutoCheck:=true;
        FDesignPopup.Items.Add(Item);
      end;
      { Reorder }
      Item:=NewItem('Order',0,false,true,nil,0,'');
      SItem:=NewItem('Bring to front',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Send to back',0,false,true,doDesignPopupReorder,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Delete object',0,false,true,doDesignPopupDel,0,'');
      FDesignPopup.Items.Add(Item);
      { add from clip }
      { Edit }
      Item:=NewItem('Edit',0,false,true,nil,0,'');
      SItem:=NewItem('Copy to clipboard',0,false,true,doDesignPopupCopy,0,'');
      Item.Add(SItem);
      SItem:=NewItem('Paste from clipboard',0,false,true,doDesignPasteFromClip,0,'');
      Item.Add(SItem);
      FDesignPopup.Items.Add(Item);
      { Delete }
      Item:=NewItem('Load From File...',0,false,true,doDesignPopupLoadFromFile,0,'');
      FDesignPopup.Items.Add(Item);
      { Grid }
      Item:=NewItem('Show grid',0,FSnapGridShow,true,doDesignPopupShowGrid,0,'');
      Item.AutoCheck:=true;
      FDesignPopup.Items.Add(Item);
      { Hint }
      Item:=NewItem('Show hint',0,FDesignShowHint,true,doDesignPopupShowHint,0,'');
      Item.AutoCheck:=true;
      FDesignPopup.Items.Add(Item);
    end;
  end;
  GetCursorPos(FPopupPos);
  FDesignPopup.Popup(FPopupPos.X ,FPopupPos.Y);
end;

procedure TD3Scene.DefineProperties(Filer:TFiler);
begin
  inherited DefineProperties(Filer);
  Filer.DefineProperty('DesignCameraPos',ReadDesignCameraPos,WriteDesignCameraPos,true);
  Filer.DefineProperty('DesignCameraZAngle',ReadDesignCameraZAngle,WriteDesignCameraZAngle,true);
  Filer.DefineProperty('DesignCameraXAngle',ReadDesignCameraXAngle,WriteDesignCameraXAngle,true);
  Filer.DefineProperty('DesignSnapGridShow',ReadDesignSnapGridShow,WriteDesignSnapGridShow,true);
  Filer.DefineProperty('DesignSnapToGrid',ReadDesignSnapToGrid,WriteDesignSnapToGrid,true);
  Filer.DefineProperty('DesignSnapToLines',ReadDesignSnapToLines,WriteDesignSnapToLines,true);
  Filer.DefineProperty('DesignShowHint',ReadDesignShowHint,WriteDesignShowHint,true);
end;

procedure TD3Scene.ReadDesignCameraPos(Reader:TReader);
begin
  FDesignCamera.Position.Y:=Reader.ReadFloat;
end;

procedure TD3Scene.WriteDesignCameraPos(Writer:TWriter);
begin
  Writer.WriteFloat(FDesignCamera.Position.Y);
end;

procedure TD3Scene.ReadDesignCameraZAngle(Reader:TReader);
begin
  FDesignCameraZ.RotateAngle.Z:=Reader.ReadFloat;
end;

procedure TD3Scene.WriteDesignCameraZAngle(Writer:TWriter);
begin
  Writer.WriteFloat(FDesignCameraZ.RotateAngle.Z);
end;

procedure TD3Scene.ReadDesignCameraXAngle(Reader:TReader);
begin
  FDesignCameraX.RotateAngle.X:=Reader.ReadFloat;
end;

procedure TD3Scene.WriteDesignCameraXAngle(Writer:TWriter);
begin
  Writer.WriteFloat(FDesignCameraX.RotateAngle.X);
end;

procedure TD3Scene.ReadDesignSnapGridShow(Reader:TReader);
var
  B:boolean;
begin
  B:=Reader.ReadBoolean;
  if not (DesignTime) then
    B:=false;
  DesignGridShow:=B;
end;

procedure TD3Scene.ReadDesignShowHint(Reader:TReader);
begin
  FDesignShowHint:=Reader.ReadBoolean;
end;

procedure TD3Scene.ReadDesignSnapToGrid(Reader:TReader);
begin
  FSnapToGrid:=Reader.ReadBoolean;
end;

procedure TD3Scene.ReadDesignSnapToLines(Reader:TReader);
begin
  FSnapToLines:=Reader.ReadBoolean;
end;

procedure TD3Scene.WriteDesignSnapGridShow(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapGridShow);
end;

procedure TD3Scene.WriteDesignShowHint(Writer:TWriter);
begin
  Writer.WriteBoolean(FDesignShowHint);
end;

procedure TD3Scene.WriteDesignSnapToGrid(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToGrid);
end;

procedure TD3Scene.WriteDesignSnapToLines(Writer:TWriter);
begin
  Writer.WriteBoolean(FSnapToLines);
end;

function TD3Scene.GetRoot:TD3Object;
var
  i:integer;
begin
  Result:=nil;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual and not TD3VisualObject(FChildren[i]).Locked then
        Result:=TD3Object(FChildren[i])
  end;
end;

procedure TD3Scene.DoDesignSelect(AObject:TObject);
begin
  if (AObject <> nil) and (AObject is TD3VisualObject) and (TD3VisualObject(AObject).FScene=Self) then
  begin
    FSelected:=TD3VisualObject(AObject);
    FSelected.DesignSelect;
    FSelected.Repaint;
  end;
end;

procedure TD3Scene.SetFocused(const Value:TD3VisualObject);
begin
  if FFocused <> Value then
  begin
    if FFocused <> nil then
      FFocused.KillFocus;
    FFocused:=Value;
    if FFocused <> nil then
      FFocused.EnterFocus;
  end;
end;

function TD3Scene.GetFill:string;
begin
  Result:=d3ColorToStr(FFill);
end;

procedure TD3Scene.SetFill(const Value:string);
begin
  if FFill <> d3StrToColor(Value) then
  begin
    FFill:=d3StrToColor(Value);
    Invalidate;
  end;
end;

function TD3Scene.GetAmbient:string;
begin
  Result:=d3ColorToStr(FAmbient);
end;

procedure TD3Scene.SetAmbient(const Value:string);
begin
  if FAmbient <> d3StrToColor(Value) then
  begin
    FAmbient:=d3StrToColor(Value);
    Invalidate;
  end;
end;

procedure TD3Scene.SetQuality(const Value:TD3Quality);
begin
  if FQuality <> Value then
  begin
    FQuality:=Value;
    if Canvas <> nil then
      Canvas.SetQuality(FQuality);
  end;
end;

procedure TD3Scene.BeginUpdate;
begin
  FDisableUpdate:=true;
end;

procedure TD3Scene.EndUpdate;
begin
  FDisableUpdate:=false;
end;

procedure TD3Scene.SetRealTime(const Value:boolean);
begin
  if FRealTime <> Value then
  begin
    FRealTime:=Value;
    if not (DesignTime) then
    begin
      Application.OnIdle:=DoIdle;
    end;
  end;
end;

procedure TD3Scene.DoIdle(Sender:TObject; var Done:boolean);
begin
  Done:=true;
  if not FRealTime then Exit;
  ProcessTick;
  Done:=false;
end;

procedure TD3Scene.ProcessTick;
var
  i:integer;
  NewTime:single;
begin
  if FTime=0 then
    FTime:=GetTickCount / 1000;
  NewTime:=GetTickCount / 1000;
  FDeltaTime:=NewTime - FTime;
  if FDeltaTime <= 0 then FDeltaTime:=0.001;
  FTime:=NewTime;
  FRenderTime:=FRenderTime + FDeltaTime;
  FRenderCount:=FRenderCount + 1;
  if FRenderTime > 1.0 then
  begin
    FFps:=FRenderCount / FRenderTime;
    FRenderTime:=0.0;
    FRenderCount:=0;
  end;
  for i:=0 to FChildren.Count - 1 do
    TD3Object(FChildren[i]).ProcessTick(FTime,FDeltaTime);
  if FPhysics <> nil then
  begin
    if FPhysics.FSpaceWorld <> 0 then
      FPhysics.UpdateWorld(FPhysics.FSpaceWorld,deltaTime);
    if FPhysics.FScreenWorld <> 0 then
      FPhysics.UpdateWorld(FPhysics.FScreenWorld,deltaTime);
  end;
  Draw;
  Sleep(FRealTimeSleep);
end;


procedure TD3Scene.CreatePhysics;
begin
  if FPhysics=nil then
    FPhysics:=GvarD3DefaultPhysicsClass.Create(Self);
end;

procedure TD3Scene.SetActiveControl(AControl:TD3VisualObject);
begin
  if AControl <> FActiveControl then
  begin
    FActiveControl:=AControl;
    if (FActiveControl <> nil) and not (csLoading in ComponentState) then
      FActiveControl.SetFocus;
  end;
end;

procedure TD3Scene.SetSelected(Value:TD3VisualObject);
begin
  if FSelected <> nil then
    FSelected.Repaint;
  FSelected:=Value;
  if FSelected <> nil then
  begin
    FSelected.DesignSelect;
    // Select in IDE
    if GvarD3Designer <> nil then
      GvarD3Designer.SelectObject(Owner,Value,[]);
    FSelected.Repaint;
  end;
end;

//================================================================ 8888
//================================================================
//================================================================


{$IFDEF WINDOWS}

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

function TD3Scene.GetDataObject:TD3DragObject;
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

function TD3Scene.DragEnter(const dataObj:IDataObject; grfKeyState:DWORD;  pt:TPoint; var dwEffect:DWORD):HResult;
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

function TD3Scene.DragOver(grfKeyState:DWORD; pt:TPoint; var dwEffect:DWORD):HResult;
var
  P:TD3Point;
  NewTarget:TD3VisualObject;
  Distance:single;
begin
  try
    dwEffect:=DROPEFFECT_NONE;
    Result:=S_OK;
    with ScreenToClient(pt) do
      P:=d3Point(X,Y,0);
    Distance:=$FFFF;
    NewTarget:=Root.Visual.FindTarget(P,d3ProjectionScreen,GetDataObject,Distance);
    if NewTarget=nil then
      NewTarget:=Root.Visual.FindTarget(P,d3ProjectionCamera,GetDataObject,Distance);
    if (NewTarget <> VarD3SceneTarget) then
    begin
      if VarD3SceneTarget <> nil then
        VarD3SceneTarget.DragLeave;
      VarD3SceneTarget:=NewTarget;
      if VarD3SceneTarget <> nil then
      begin
        VarD3SceneTarget.DragEnter(GetDataObject,VarD3SceneTarget.AbsoluteToLocal(P));
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

function TD3Scene.DragLeave:HResult;
begin
  if VarD3SceneTarget <> nil then
    VarD3SceneTarget.DragLeave;
  if (FDropTargetHelper <> nil) then
    FDropTargetHelper.DragLeave;
  VarD3SceneTarget:=nil;
  FDropTargetHelper:=nil;
  FDataObj:=nil;
  Result:=S_OK;
end;

function TD3Scene.Drop(const dataObj:IDataObject; grfKeyState:DWORD; pt:TPoint;
      var dwEffect:DWORD):HResult;
var
  P:TD3Point;
begin
  try
    if (dataObj=nil) then Exit;
    if VarD3SceneTarget <> nil then
    begin
      with ScreenToClient(pt) do
        P:=d3Point(X,Y,0);
      VarD3SceneTarget.DragDrop(GetDataObject,VarD3SceneTarget.AbsoluteToLocal(P));
    end;
    if (FDropTargetHelper <> nil) then
      FDropTargetHelper.Drop(DataObj,pt,dwEffect)
  finally
    FDataObj:=nil;
    FDropTargetHelper:=nil;
  end;
end;

procedure TD3Scene.UpdateLayer;
var
  Blend:TBLENDFUNCTION;
  Origin,Size,BitmapOrigin:Windows.TPoint;
  dragAlpha:integer;
  DC,OldRgn,Rgn:Cardinal;
begin
  if Handle=0 then Exit;
  if Canvas=nil then Exit;
  if (DesignTime) then Exit;
  if not (Owner is TWinControl) then Exit;
  if TWinControl(Owner).Handle=0 then Exit;
  if Canvas.FBufferDC=0 then Exit;

  Origin:=Point(TWinControl(Owner).Left + Left,TWinControl(Owner).Top + Top);
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

  UpdateLayeredWindow(TWinControl(Owner).Handle,0,@Origin,@Size,Canvas.FBufferDC,@BitmapOrigin,$00000000,@Blend,ULW_ALPHA);
end;

procedure TD3Scene.CMMouseLeave(var Message:TMessage);
begin
  inherited ;
  if FHovered <> nil then
  begin
    FHovered.MouseInObject:=false;
    FHovered.MouseLeave;
    FHovered:=nil;
  end;
end;

procedure TD3Scene.WMAddUpdateRect(var Msg:TMessage);
 procedure ProcessUpdateMessages;
 var
   Msg:TMsg;
 begin
   while PeekMessage(Msg,TWinControl(Owner).Handle,WM_ADDUPDATERECT,WM_ADDUPDATERECT,PM_REMOVE) do
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
  { Empty message queue }
  ProcessUpdateMessages;
  { Draw }
  if csDestroying in ComponentState then Exit;
  Draw;
end;

procedure TD3Scene.WMEraseBkgnd(var Msg:TWMEraseBkgnd);
begin
  Msg.Result:=1;
end;

procedure TD3Scene.WMPaint(var Msg:TWMPaint);
begin
  inherited ;
end;
{$ENDIF}

{$IFDEF UNIX}

{$ifdef darwin}
procedure TD3Scene.UpdateLayer;
var
  CGR:CGRect;
  ImgRef:CGImageRef;
begin
  if Handle=0 then Exit;
  if Canvas=nil then Exit;
  if (DesignTime) then Exit;
  if not (Owner is TWinControl) then Exit;
  if TWinControl(Owner).Handle=0 then Exit;
  if Canvas.FBufferBits=nil then Exit;

  if (inherited Canvas.Handle <> 0) then
  begin
    CGR.origin.x:= 0;
    CGR.origin.y:= 0;
    CGR.size.width:=Canvas.FWidth;
    CGR.size.height:=Canvas.FHeight;
    ImgRef:=CGBitmapContextCreateImage(Canvas.FBufferHandle);
    CGContextDrawImage(CGContextRef(inherited Canvas.Handle),CGR,ImgRef);
    CFRelease(ImgRef);
  end;
end;
{$ELSE}
procedure TD3Scene.UpdateLayer;
begin
  if Handle=0 then Exit;
  if Canvas=nil then Exit;
  if (DesignTime) then Exit;
  if not (Owner is TWinControl) then Exit;
  if TWinControl(Owner).Handle=0 then Exit;
  if Canvas.FBufferBits=nil then Exit;

end;
{$endif}

procedure TD3Scene.Invalidate;
begin
  if csCustomPaint in FControlState then exit;
  inherited Invalidate;
end;

procedure TD3Scene.CMMouseLeave(var Message :TLMessage);
begin
  inherited ;
  if FHovered <> nil then
  begin
    FHovered.MouseInObject:=false;
    FHovered.MouseLeave;
    FHovered:=nil;
  end;
end;

procedure TD3Scene.WMEraseBkgnd(var Msg:TLMEraseBkgnd);
begin
  Msg.Result:=1;
end;

procedure TD3Scene.WMPaint(var Msg:TLMPaint);
begin
  inherited ;
end;

{$endif}
