{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Window ==========================================

constructor TD2Window.Create(AOwner:TComponent);
begin
  inherited;
  FautoTranslate:=true;
  FShowCloseButton:=true;
  FShowSizeGrip:=true;
  HitTest:=false;
  TextAlign:=d2TextalignNear;
  Width:=200;
  Height:=200;
end;

procedure TD2Window.ApplyStyle;
var
  sizeGrip,closeBtn:TD2Object;
begin
  inherited;
  closeBtn:=FindResource('close');
  if (closeBtn <> nil) and (closeBtn is TD2VisualObject) then
  begin
    TD2VisualObject(closeBtn).visible:=FShowCloseButton;
    if (closeBtn is TD2CloseButton) and (Assigned(FOnCloseClick)) then
    begin
      TD2CloseButton(closeBtn).CloseForm:=false;
      TD2CloseButton(closeBtn).OnClick:=FOnCloseClick;
    end;
  end;
  sizeGrip:=FindResource('sizegrip');
  if (sizeGrip <> nil) and (sizeGrip is TD2VisualObject) then
  begin
    TD2VisualObject(sizeGrip).visible:=FShowSizeGrip;
  end;
end;

destructor TD2Window.Destroy;
begin
  inherited;
end;

procedure TD2Window.SetShowCloseButton(const Value:boolean);
begin
  if FShowCloseButton <> Value then
  begin
    FShowCloseButton:=Value;
    Resource:=FResource;
  end;
end;

procedure TD2Window.SetShowSizeGrip(const Value:boolean);
begin
  if FShowSizeGrip <> Value then
  begin
    FShowSizeGrip:=Value;
    Resource:=FResource;
  end;
end;

//=============== TD2LayerWindow =================================================

constructor TD2LayerWindow.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2LayerWindow.Destroy;
begin
  inherited;
end;

//=============== TD2HudWindow =================================================

constructor TD2HudWindow.Create(AOwner:TComponent);
begin
  inherited;
  FFill:=TD2Brush.Create(d2BrushSolid,$EA2F2F2F);
  FFill.OnChanged:=DoFillChanged;
  FStroke:=TD2Brush.Create(d2BrushSolid,$5B000000);
  FStroke.OnChanged:=DoFillChanged;
  FStrokeThickness:=1;
  FDisableShadowOnOSX:=true;
  FShowCaption:=true;
  Font.Style:=d2FontBold;
end;

destructor TD2HudWindow.Destroy;
begin
  FStroke.Free;
  FFill.Free;
  inherited;
end;

procedure TD2HudWindow.ApplyStyle;
var
  {$IFDEF DARWIN}
  shadow:TD2Object;
  {$endif}
  back:TD2Object;
begin
  inherited;
  {$IFDEF DARWIN}
  shadow:=FindResource('shadow');
  if (shadow <> nil) and (shadow is TD2VisualObject) and (FDisableShadowOnOSX) then
  begin
    TD2VisualObject(shadow).visible:=false;
  end;
  {$ENDIF}
  back:=FindResource('close');
  if (back <> nil) and (back is TD2CloseButton) then
  begin
    if FCloseAlign=d2ButtonAlignLeft then
      TD2CloseButton(back).Align:=vaLeft
    else
      TD2CloseButton(back).Align:=vaRight;
    if TD2VisualObject(back).Visible then
    begin
      TD2VisualObject(back).Visible:=FShowCaption;
      TD2VisualObject(back).DesignHide:=not FShowCaption;
    end;
  end;
  back:=FindResource('back');
  if (back <> nil) and (back is TD2Shape) then
  begin
    TD2Shape(back).Fill.Assign(FFill);
  end;
  back:=FindResource('stroke');
  if (back <> nil) and (back is TD2Shape) then
  begin
    TD2Shape(back).Stroke.Assign(FStroke);
    TD2Shape(back).StrokeThickness:=FStrokeThickness;
    TD2Shape(back).StrokeCap:=FStrokeCap;
    TD2Shape(back).StrokeDash:=FStrokeDash;
    TD2Shape(back).StrokeJoin:=FStrokeJoin;
    TD2VisualObject(back).Margins.Rect:=d2Rect(FStrokeThickness,FStrokeThickness,FStrokeThickness,FStrokeThickness);
  end;
  back:=FindResource('caption');
  if (back <> nil) and (back is TD2VisualObject) then
  begin
    TD2VisualObject(back).Height:=20 + FStrokeThickness;
    TD2VisualObject(back).Padding.Rect:=d2Rect(FStrokeThickness,FStrokeThickness,FStrokeThickness,0);
    TD2VisualObject(back).Visible:=FShowCaption;
    TD2VisualObject(back).DesignHide:=not FShowCaption;
  end;
  back:=FindResource('text');
  if (back <> nil) and (back is TD2VisualObject) then
  begin
    TD2VisualObject(back).Visible:=FShowCaption;
    TD2VisualObject(back).DesignHide:=not FShowCaption;
  end;
end;

procedure TD2HudWindow.SetDisableShadowOnOSX(const Value:boolean);
begin
  if FDisableShadowOnOSX <> Value then
  begin
    FDisableShadowOnOSX:=Value;
    Resource:=FResource;
  end;
end;

procedure TD2HudWindow.SetFill(const Value:TD2Brush);
begin
  FFill.Assign(Value);
end;

procedure TD2HudWindow.DoFillChanged(Sender:TObject);
begin
  if not (csLoading in ComponentState) then
    ApplyStyle;
end;

function TD2HudWindow.isStrokeThicknessStored:boolean;
begin
  Result:=StrokeThickness <> 1;
end;

procedure TD2HudWindow.SetStroke(const Value:TD2Brush);
begin
  if FStroke <> Value then
  begin
    FStroke:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetStrokeCap(const Value:TD2StrokeCap);
begin
  if FStrokeCap <> Value then
  begin
    FStrokeCap:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetStrokeDash(const Value:TD2StrokeDash);
begin
  if FStrokeDash <> Value then
  begin
    FStrokeDash:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetStrokeJoin(const Value:TD2StrokeJoin);
begin
  if FStrokeJoin <> Value then
  begin
    FStrokeJoin:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetStrokeThickness(const Value:single);
begin
  if FStrokeThickness <> Value then
  begin
    FStrokeThickness:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetCloseAlign(const Value:TD2CloseAlign);
begin
  if FCloseAlign <> Value then
  begin
    FCloseAlign:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2HudWindow.SetShowCaption(const Value:boolean);
begin
  if FShowCaption <> Value then
  begin
    FShowCaption:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;


//=============== TD2Frame =============================================

constructor TD2Frame.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2Frame.Destroy;
begin
  if not (Assigned(FScene) and (FScene.GetDesignTime)) then
    if (FSceneObject <> nil) and (FChildren <> nil) and (FChildren.Count > 0) then
    begin
      FSceneObject.AddObject(TD2Object(FChildren[0]));
      FSceneObject.FOpenInFrame:=nil;
      if FSceneObject.FCloneFrame <> nil then
        FreeAndNil(FSceneObject.FCloneFrame);
    end;
  if FBuffer <> nil then
    FBuffer.Free;
  inherited;
end;

procedure TD2Frame.Loaded;
begin
  inherited ;
end;

procedure TD2Frame.GetChildren(Proc:TGetChildProc; Root:TComponent);
begin
end;

procedure TD2Frame.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FSceneObject) then
    SceneObject:=nil;
end;

procedure TD2Frame.Paint;
var
  Obj:TD2Object;
  CloneFrame:TForm;
  SaveR:TD2Rect;
begin
  if FNeedClone then
  begin
    FNeedClone:=false;
    if not (Assigned(FScene) and (FScene.GetDesignTime)) and (FSceneObject <> nil) then
    begin

      if (FSceneObject <> nil) and (FSceneObject.FOpenInFrame <> nil) then
      begin
        Application.CreateForm(TComponentClass(FSceneObject.Owner.ClassType),CloneFrame);
        if CloneFrame <> nil then
        begin
          FSceneObject:=TD2CustomScene(CloneFrame.FindComponent(FSceneObject.Name));
          if FSceneObject <> nil then
          begin
            FSceneObject.FCloneFrame:=CloneFrame;
          end;
        end else
          FSceneObject:=nil;
      end;

      if (FSceneObject <> nil) and (FSceneObject.FOpenInFrame=nil) and (FSceneObject.Root <> nil) then
      begin
        FSceneObject.FOpenInFrame:=Self;
        Obj:=FSceneObject.Root;
        Obj.Parent:=Self;

        if Obj.isVisual then
        begin
          TD2VisualObject(Obj).Align:=vaClient;
          Realign;
        end;

      end;
    end;
  end;

  if (FScene <> nil) and (Assigned(FScene) and (FScene.GetDesignTime)) and (SceneObject <> nil) and (SceneObject.Root <> nil) then
  begin
    Obj:=SceneObject.Root;
    if Obj.isVisual then
    begin
      if FBuffer=nil then
        FBuffer:=TD2Bitmap.Create(Round(Width),Round(Height));

      if (FBuffer.Width <> Trunc(Width)) or (FBuffer.Height <> Trunc(Height)) then
        FBuffer.SetSize(Trunc(Width),Trunc(Height));

      FBuffer.Clear(0);
      SaveR:=TD2VisualObject(Obj).ParentedRect;
      TD2VisualObject(Obj).SetBounds(0,0,Width,Height);
      TD2VisualObject(Obj).PaintTo(FBuffer.Canvas,LocalRect);
      Canvas.DrawBitmap(FBuffer,d2Rect(0,0,FBuffer.Width,FBuffer.Height),
                        d2Rect(0,0,FBuffer.Width - 1,FBuffer.Height - 1),1);

      TD2VisualObject(Obj).SetBounds(SaveR.Left,SaveR.Top,d2RectWidth(SaveR),d2RectHeight(SaveR));
    end;
  end;
end;

procedure TD2Frame.SetSceneObject(const Value:TD2CustomScene);
var
  Obj:TD2Object;
begin
  if FSceneObject <> Value then
  begin
    if Assigned(FScene) and (Value=FScene.GetComponent) then Exit;

    if not (Assigned(FScene) and (FScene.GetDesignTime)) and (FSceneObject <> nil) then
    begin
      if (FSceneObject <> nil) and (FChildren <> nil) and (FChildren.Count > 0) then
      begin
        FSceneObject.AddObject(TD2Object(FChildren[0]));
        FSceneObject.FOpenInFrame:=nil;
        if (FSceneObject.FCloneFrame <> nil) and not (csDestroying in FSceneObject.ComponentState) then
          FreeAndNil(FSceneObject.FCloneFrame);
      end;
    end;

    FSceneObject:=Value;

    if not (Assigned(FScene) and (FScene.GetDesignTime)) and (FSceneObject <> nil) then
    begin

      if (FSceneObject <> nil) and (FSceneObject.FOpenInFrame <> nil) then
      begin
        FNeedClone:=true;
        Exit;
      end;

      if (FSceneObject <> nil) and (FSceneObject.FOpenInFrame=nil) and (FSceneObject.Root <> nil) then
      begin
        FSceneObject.FOpenInFrame:=Self;
        Obj:=FSceneObject.Root;
        Obj.Parent:=Self;
        if Obj.isVisual then
        begin
          TD2VisualObject(Obj).Align:=vaClient;
          Realign;
        end;
      end;
    end;
    if Assigned(FScene) and (FScene.GetDesignTime) then
      Repaint;
  end;
end;
       
             
                                 
