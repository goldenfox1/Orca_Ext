{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Control ==========================================

constructor TD2Control.Create(AOwner:TComponent);
begin
  inherited;
  FHelpType:=htContext;
  FNeedResource:=true;
end;

destructor TD2Control.Destroy;
var
  i:integer;
begin
  if FActionLink <> nil then
  begin
    FActionLink.Free;
    FActionLink:=nil;
  end;

  if FBindingObjects <> nil then
  begin
    for i:=FBindingObjects.Count - 1 downto 0 do
      RemoveBindingObject(TD2Control(FBindingObjects[i]));
    FreeAndNil(FBindingObjects);
  end;
  inherited;
end;

procedure TD2Control.Loaded;
begin
  inherited ;
  if Assigned(FScene) and (FScene.GetActiveControl=Self) then
    SetFocus;
end;

procedure TD2Control.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (FBindingSource=AComponent) then
    BindingSource:=nil;
  if (Operation=opRemove) and (FBindingObjects <> nil) and (FBindingObjects.IndexOf(AComponent) >= 0) then
    FBindingObjects.Remove(AComponent);
  if (Operation=opRemove) and (AComponent=Action) then
    Action:=nil;
end;

procedure TD2Control.SetData(const Value:Variant);
begin
end;

procedure TD2Control.AddBindingObject(AObject:TD2Control);
begin
  if FBindingObjects=nil then
    FBindingObjects:=TList.Create;
  if FBindingObjects.IndexOf(AObject) < 0 then
  begin
    AObject.AddFreeNotify(Self);
    FBindingObjects.Add(AObject);

    ToBindingObjects;
  end;
end;

procedure TD2Control.RemoveBindingObject(AObject:TD2Control);
begin
  if FBindingObjects=nil then Exit;
  if FBindingObjects.IndexOf(AObject) >= 0 then
  begin
    AObject.RemoveFreeNotify(Self);
    FBindingObjects.Remove(AObject);
  end;
end;

procedure TD2Control.SetBindingSource(const Value:TD2Control);
begin
  if FBindingSource <> Value then
  begin
    if FBindingSource <> nil then
      FBindingSource.RemoveBindingObject(Self);
    FBindingSource:=Value;
    if FBindingSource <> nil then
      FBindingSource.AddBindingObject(Self);
  end;
end;

procedure TD2Control.ToBindingObjects;
var
  i:integer;
begin
  if FBindingObjects=nil then Exit;
  for i:=0 to FBindingObjects.Count - 1 do
    TD2Object(FBindingObjects[i]).Data:=Data;
end;

function TD2Control.FindResource(const AResource:string):TD2Object;
begin
  Result:=nil;
  if FResourceLink <> nil then
    Result:=FResourceLink.FindResource(AResource);
  if Result=nil then
    Result:=inherited FindResource(AResource);
end;

function TD2Control.GetResourceObject:TD2VisualObject;
var
  Obj:TD2Object;
  ResourceObject:TD2VisualObject;
  S:TStream;
  StyleName:string;
begin
  ResourceObject:=nil;
  if (FResource <> '') and (FResource=FResourceName) then
  begin
    Result:=ResourceObject;
    Exit; // Stack Overflow
  end;
  if (FResource <> '') then
  begin
    { style }
    Obj:=nil;
    if Assigned(FScene) and (FScene.GetStyle <> nil) and (FScene.GetStyle.FRoot <> nil) then
      Obj:=TD2VisualObject(FScene.GetStyle.FRoot.FindResource(FResource));
    if Obj=nil then
      if GvarD2DefaultStyles <> nil then
        Obj:=TD2VisualObject(GvarD2DefaultStyles.FindResource(FResource));
    if Obj=nil then
      Obj:=orca_scene2d.FindResource(FResource);
    if (Obj <> nil) and (Obj.isVisual) then
    begin
      ResourceObject:=TD2VisualObject(Obj.Clone(Self));
      ResourceObject.ResourceName:='';
    end;
  end;
  if ResourceObject=nil then
  begin
    { default }
    if GvarD2DefaultStyles=nil then
    begin
      { load default styles }

      if System.FindResource(HInstance,PChar('DEFAULTORCASTYLE'),RT_RCDATA) <> 0 then

      begin
        S:=TResourceStream.Create(HInstance,'DEFAULTORCASTYLE',RT_RCDATA);
        GvarD2DefaultStyles:=CreateObjectFromBinStream(nil,S);
        S.Free;
      end;
    end;
    if Assigned(FScene) and (FScene.GetStyle <> nil) and (FScene.GetStyle.FRoot <> nil) then
    begin
      if FResource <> '' then
      begin
        StyleName:=FResource;
        ResourceObject:=TD2VisualObject(FScene.GetStyle.FRoot.FindResource(StyleName));
        if ResourceObject <> nil then
          ResourceObject:=TD2VisualObject(ResourceObject.Clone(Self));
      end;
      if ResourceObject=nil then
      begin
        StyleName:=ClassName + 'style';
        Delete(StyleName,1,3);
        ResourceObject:=TD2VisualObject(FScene.GetStyle.FRoot.FindResource(StyleName));
        if ResourceObject <> nil then
          ResourceObject:=TD2VisualObject(ResourceObject.Clone(Self))
      end;
    end;
    if (ResourceObject=nil) and (GvarD2DefaultStyles <> nil) then
    begin
      if FResource <> '' then
      begin
        StyleName:=FResource;
        ResourceObject:=TD2VisualObject(GvarD2DefaultStyles.FindResource(StyleName));
        if ResourceObject <> nil then
          ResourceObject:=TD2VisualObject(ResourceObject.Clone(Self));
      end;
      if ResourceObject=nil then
      begin
        StyleName:=ClassName + 'style';
        Delete(StyleName,1,3);
        ResourceObject:=TD2VisualObject(GvarD2DefaultStyles.FindResource(StyleName));
        if ResourceObject <> nil then
          ResourceObject:=TD2VisualObject(ResourceObject.Clone(Self))
        else
        begin
          // try parent Class
          StyleName:=ClassParent.ClassName + 'style';
          Delete(StyleName,1,3);
          ResourceObject:=TD2VisualObject(GvarD2DefaultStyles.FindResource(StyleName));
          if ResourceObject <> nil then
            ResourceObject:=TD2VisualObject(ResourceObject.Clone(Self));
        end;
      end;
    end;
  end;
  Result:=ResourceObject;
end;

procedure TD2Control.ApplyResource;
var
  ResourceObject:TD2VisualObject;
  SaveUpdate:boolean;
begin
  if FNeedResource then
  begin
    inherited ;
    FNeedResource:=false;
    ResourceObject:=GetResourceObject;
    if ResourceObject <> nil then
    begin
      if csLoading in ResourceObject.ComponentState then
        CallLoaded(ResourceObject);
      if (FScene <> nil) and (csLoading in ComponentState) then
      begin
        SaveUpdate:=FScene.GetDisableUpdate;
        FScene.SetDisableUpdate(true);
      end;
      if FResourceLink <> nil then
      begin
        FreeStyle;
        FResourceLink.Free;
        FResourceLink:=nil;
      end;
      ResourceObject.FAlign:=vaContents;
      ResourceObject.DesignHide:=false;
      FResourceLink:=ResourceObject;
      AddObject(ResourceObject);
      { bring to front }
      FChildren.Remove(ResourceObject);
      FChildren.Insert(0,ResourceObject);
      { }
      ResourceObject.Stored:=false;
      ResourceObject.Lock;
      ApplyStyle;
      if (FScene <> nil) and (csLoading in ComponentState) then
      begin
        FScene.SetDisableUpdate(SaveUpdate);
      end;
      FUpdateEffects:=true;
      if Assigned(FOnApplyResource) then
        FOnApplyResource(Self);
    end;
  end;
end;

procedure TD2Control.UpdateResource;
begin
  if csLoading in ComponentState then Exit;
  if csDestroying in ComponentState then Exit;
  inherited ;
  FNeedResource:=true;
  ApplyResource;
end;

procedure TD2Control.ApplyStyle;
var
  NewT:WideString;
begin
  if FIsFocused and CanFocused and not FDisableFocusEffect and not GlobalDisableFocusEffect then
  begin
    FRecalcUpdateRect:=true;
    Repaint;
    StartTriggerAnimation(Self,'IsFocused');
    ApplyTriggerEffect(Self,'IsFocused');
  end;
  { translate }
  if FAutoTranslate and ShowHint and (Hint <> '') then
  begin
    NewT:=Translate(Hint); // need for collection texts
    if (FScene <> nil) and not (FScene.GetDesignTime) then
      Hint:=NewT;
  end;
end;

procedure TD2Control.FreeStyle;
begin
end;

procedure TD2Control.EnterFocus;
begin
  ApplyResource;
  inherited ;
end;

procedure TD2Control.BeforePaint;
begin
  inherited;
  ApplyResource;
end;

procedure TD2Control.Paint;
begin
  inherited ;
end;

procedure TD2Control.SetResource(const Value:string);
begin
  FResource:=Value;
  FNeedResource:=true;
  if not (csLoading in ComponentState) and (FUpdating=0) then
  begin
    ApplyResource;
  end;
end;

procedure TD2Control.InitiateAction;
begin
  if FActionLink <> nil then FActionLink.Update;
end;

procedure TD2Control.ActionChange(Sender:TObject; CheckDefaults:boolean);
begin
  if Sender is TCustomAction then
    with TCustomAction(Sender) do
    begin
      if Self is TD2TextControl then
        if not CheckDefaults or (TD2TextControl(Self).TextW='') or (TD2TextControl(Self).TextW=Self.Name) then
          TD2TextControl(Self).TextW:=Caption;
      if not CheckDefaults or (Self.Enabled=True) then
        Self.Enabled:=Enabled;
      if not CheckDefaults or (Self.Hint='') then
        Self.Hint:=Hint;
      if not CheckDefaults or (Self.Visible=True) then
        Self.Visible:=Visible;
      if not CheckDefaults or not Assigned(Self.OnClick) then
        Self.OnClick:=OnExecute;
    end;
end;

procedure TD2Control.DoActionChange(Sender:TObject);
begin
  if Sender=Action then ActionChange(Sender,False);
end;

function TD2Control.GetAction:TBasicAction;
begin
  if FActionLink <> nil then
    Result:=FActionLink.Action
  else
    Result:=nil;
end;

procedure TD2Control.SetAction(Value:TBasicAction);
begin
  if Value=nil then
  begin
    FActionLink.Free;
    FActionLink:=nil;
  end
  else
  begin
    if FActionLink=nil then
      FActionLink:=TD2ControlActionLink.Create(Self);
    FActionLink.Action:=Value;
    FActionLink.OnChange:=DoActionChange;
    ActionChange(Value,csLoading in Value.ComponentState);
    Value.FreeNotification(Self);
  end;
end;

function TD2Control.IsHelpContextStored:boolean;
begin
  Result:=(FActionLink=nil) or not FActionLink.IsHelpContextLinked;
end;

procedure TD2Control.SetHelpContext(const Value:THelpContext);
begin
  if not (csLoading in ComponentState) then FHelpType:=htContext;
  FHelpContext:=Value;
end;

procedure TD2Control.SetHelpKeyword(const Value:String);
begin
  if not (csLoading in ComponentState) then FHelpType:=htKeyword;
  FHelpKeyword:=Value;
end;

//=============== TD2Background =========================================

constructor TD2Background.Create(AOwner:TComponent);
begin
  inherited;
  FFill:=TD2Brush.Create(d2BrushNone,$FFE0E0E0);
  FFill.OnChanged:=FillChanged;
end;

destructor TD2Background.Destroy;
begin
  FFill.Free;
  inherited;
end;

procedure TD2Background.FillChanged(Sender:TObject);
begin
  Repaint;
end;

procedure TD2Background.BeforePaint;                  // *** DB ***
begin
   inherited BeforePaint;
   if Assigned(FOnBeforePaint) then
   begin
      // Only if we're the top background
      if Self.Scene.GetRoot=Self then
         FOnBeforePaint(Self,Self.Canvas,
                      d2Rect(0,0,FWidth,FHeight) );
   end;
end;

procedure TD2Background.Paint;
begin
  if FFill.Style <> d2BrushNone then
  begin
    Canvas.Fill.Assign(FFill);
    Canvas.FillRect(LocalRect,0,0,AllCorners,AbsoluteOpacity);
  end
end;

procedure TD2Background.PaintChildren;
begin
  if FFill.Style <> d2BrushNone then
  begin
    if (FResourceLink <> nil) then
      FResourceLink.Visual.FVisible:=false;
    inherited;
    if FResourceLink <> nil then
      FResourceLink.Visual.FVisible:=true;
  end
  else
  if (Parent=nil) and (FScene <> nil) and (FScene.GetRoot=Self) and (FScene.GetTransparency) then
  begin
    if FResourceLink <> nil then
      FResourceLink.Visual.FVisible:=false;
    inherited;
    if FResourceLink <> nil then
      FResourceLink.Visual.FVisible:=true;
  end
  else
    inherited;
end;

procedure TD2Background.SetFill(const Value:TD2Brush);
begin
  FFill.Assign(Value);
end;

//=============== TD2Content =========================================

//constructor TD2Content.Create(AOwner:TComponent);
//begin
//  inherited;
//end;
//
//destructor TD2Content.Destroy;
//begin
//  inherited;
//end;

//function TD2Content.GetParentComponent:TComponent;
//begin
//  Result:=inherited GetParentComponent;
//end;

procedure TD2Content.Realign;
begin
  if (Parent <> nil) and not (csLoading in Parent.ComponentState) then
    inherited;
  if not FParentAligning and not (csLoading in ComponentState) then
  begin
    FParentAligning:=true;
    if (Parent <> nil) and (Parent.IsVisual) and not (csLoading in Parent.ComponentState) then
      TD2VisualObject(Parent).Realign;
    FParentAligning:=false;
  end;
end;

//procedure TD2Content.Paint;
//begin
//  inherited;
//end;     


//=============== TD2DropTarget ====================================

constructor TD2DropTarget.Create(AOwner:TComponent);
begin
  inherited;
  DragDisableHighlight:=true;
  Width:=120;
  Height:=120;
end;

procedure TD2DropTarget.DragDrop(const Data:TD2DragObject;
  const Point:TD2Point);
begin
  inherited;
  if Assigned(FOnDrop) then
    FOnDrop(Self,Data,Point);
end;

procedure TD2DropTarget.DragOver(const Data:TD2DragObject; Shift: TShiftState;
  const Point:TD2Point; var Accept:boolean);
begin
  inherited;
  Accept:=true;
end;      


//=============== TD2SizeGrip ====================================

constructor TD2SizeGrip.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2SizeGrip.Destroy;
begin
  inherited;
end;

//=============== TD2GroupBox =======================================

constructor TD2GroupBox.Create(AOwner:TComponent);
begin
  inherited;
  FautoTranslate:=true;
  CanFocused:=false;
  Width:=120;
  Height:=100;
end;

destructor TD2GroupBox.Destroy;
begin
  inherited;
end;

procedure TD2GroupBox.ApplyStyle;
begin
  inherited;
end;

//=============== TD2Splitter ===========================================

constructor TD2Splitter.Create(AOwner:TComponent);
begin
  inherited;
  FMinSize:=20;
  FShowGrip:=true;
  AutoCapture:=true;
  Width:=5;
  Align:=vaLeft;
  Cursor:=crHSplit;
end;

destructor TD2Splitter.Destroy;
begin
  inherited;
end;

procedure TD2Splitter.ApplyStyle;
var
  grip:TD2Object;
begin
  inherited;
  grip:=FindResource('grip');
  if (grip <> nil) and (grip is TD2VisualObject) then
  begin
    TD2VisualObject(grip).visible:=FShowGrip;
  end;
end;

procedure TD2Splitter.Paint;
var
  R:TD2Rect;
begin
  inherited;
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
    Canvas.StrokeDash:=d2DashSolid;
  end;
end;

procedure TD2Splitter.SetAlign(const Value:TD2Align);
begin
  inherited;
  if Align in [vaTop,vaBottom] then
    Cursor:=crVSplit
  else
    Cursor:=crHSplit;
end;

function TD2Splitter.FindObject:TD2VisualObject;
var
  P:TD2Point;
  I:integer;
  R:TD2Rect;
begin
  Result:=nil;
  P:=Position.Point;
  case Align of
    vaLeft,vaMostLeft:P.X:=P.X - 1;
    vaRight,vaMostRight:P.X:=P.X + Width + 1;
    vaTop:P.Y:=P.Y - 1;
    vaBottom:P.Y:=P.Y + Height + 1;
  else
    Exit;
  end;
  if Parent <> nil then
    for I:=0 to Parent.ChildrenCount - 1 do
    begin
      if not Parent.Children[I].IsVisual then Continue;
      if TD2VisualObject(Parent.Children[I]).Locked then Continue;
      if (Align in [vaLeft,vaMostLeft,vaRight,vaMostRight]) and not ((TD2VisualObject(Parent.Children[I]).Align in [vaLeft,vaMostLeft,vaRight,vaMostRight])) then Continue;
      if (Align in [vaTop,vaBottom,vaMostTop,vaMostBottom]) and not ((TD2VisualObject(Parent.Children[I]).Align in [vaTop,vaBottom,vaMostTop,vaMostBottom])) then Continue;

      Result:=Parent.Children[I].Visual;
      if Result.Visible then
      begin
        R:=Result.LocalRect;
        d2OffsetRect(R,Result.Position.X,Result.Position.Y);
        if (R.Right - R.Left)=0 then
          if Align in [vaTop,vaLeft,vaMostLeft] then
            R.Left:=R.Left - 1
          else
            R.Right:=R.Right + 1;
        if (R.Bottom - R.Top)=0 then
          if Align in [vaTop,vaLeft,vaMostLeft] then
            R.Top:=R.Top - 1
          else
            R.Bottom:=R.Bottom + 1;
        if d2PtInRect(P,R) then Exit;
      end;
    end;
  Result:=nil;
end;

procedure TD2Splitter.UpdateSize(X,Y:single);
begin
  CalcSplitSize(X,Y,FNewSize,FSplit);
end;

procedure TD2Splitter.CalcSplitSize(X,Y:single; var NewSize,Split:single);
var
  S:single;
begin
  if Align in [vaLeft,vaRight,vaMostLeft,vaMostRight] then
    Split:=X - FDownPos.X
  else
    Split:=Y - FDownPos.Y;
  S:=0;
  case Align of
    vaLeft,vaMostLeft:S:=FControl.Width + Split;
    vaRight,vaMostRight:S:=FControl.Width - Split;
    vaTop:S:=FControl.Height + Split;
    vaBottom:S:=FControl.Height - Split;
  end;
  NewSize:=S;
  if S < FMinSize then
    NewSize:=FMinSize
  else
  if S > FMaxSize then
    NewSize:=FMaxSize;
  if S <> NewSize then
  begin
    if Align in [vaRight,vaMostRight,vaBottom] then
      S:=S - NewSize
    else
      S:=NewSize - S;
    Split:=Split + S;
  end;
end;

function TD2Splitter.DoCanResize(var NewSize:single):boolean;
begin
  Result:=true;
  if (NewSize <= FMinSize) {and FAutoSnap }then
    NewSize:=FMinSize;
end;

procedure TD2Splitter.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  i:integer;
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressed:=true;
    FDownPos:=d2Point(X,Y);
    FControl:=FindObject;
    if Assigned(FControl) then
    begin
      if Align in [vaLeft,vaMostLeft,vaRight,vaMostRight] then
      begin
        FMaxSize:=Parent.Visual.Width - FMinSize - Parent.Visual.Margins.left - Parent.Visual.Margins.right;
        for I:=0 to Parent.ChildrenCount - 1 do
        begin
          if not Parent.Children[I].IsVisual then Continue;
          with Parent.Children[I].Visual do
            if (Align in [vaLeft,vaRight,vaMostLeft,vaMostRight]) then
              FMaxSize:=FMaxSize - Width - Padding.Left - Padding.Right;
        end;
        FMaxSize:=FMaxSize + FControl.Width;
      end
      else
      begin
        FMaxSize:=Parent.Visual.Height - FMinSize - Parent.Visual.Margins.top - Parent.Visual.Margins.bottom;
        for I:=0 to Parent.ChildrenCount - 1 do
        begin
          if not Parent.Children[I].IsVisual then Continue;
          with Parent.Children[I].Visual do
            if Align in [vaTop,vaBottom] then
              FMaxSize:=FMaxSize - Height - Padding.top - Padding.bottom;
        end;
        FMaxSize:=FMaxSize + FControl.Height;
      end;
      UpdateSize(X,Y);
    end;
  end;
end;

procedure TD2Splitter.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  NewSize,Split:single;
begin
  inherited;
  if FPressed and Assigned(FControl) then
  begin
    CalcSplitSize(X,Y,NewSize,Split);
    if DoCanResize(NewSize) then
    begin
      FNewSize:=NewSize;
      FSplit:=Split;
      UpdateControlSize;
    end;
  end;
end;

procedure TD2Splitter.UpdateControlSize;
begin
  if FNewSize <> FOldSize then
  begin
    case Align of
      vaLeft,vaMostLeft:FControl.Width:=FNewSize;
      vaTop:FControl.Height:=FNewSize;
      vaRight,vaMostRight:
        begin
          FControl.Position.X:=FControl.Position.X + (FControl.Width - FNewSize);
          FControl.Width:=FNewSize;
        end;
      vaBottom:
        begin
          FControl.Position.Y:=FControl.Position.Y + (FControl.Height - FNewSize);
          FControl.Height:=FNewSize;
        end;
    end;

    FOldSize:=FNewSize;
  end;
end;

procedure TD2Splitter.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  FPressed:=false;
  FControl:=nil;
end;

procedure TD2Splitter.SetShowGrip(const Value:boolean);
begin
  if FShowGrip <> Value then
  begin
    FShowGrip:=Value;
    ApplyStyle;
  end;
end;

//=============== TD2Thumb ========================================

constructor TD2Thumb.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  AutoCapture:=true;
end;

destructor TD2Thumb.Destroy;
begin
  inherited;
end;

procedure TD2Thumb.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if (Button=mbLeft) and Enabled then
  begin
    FPressed:=true;
    FDownOffset:=d2Point(X,Y);

    if FTrack <> nil then
      FTrack.SetFocus;
  end;
end;

procedure TD2Thumb.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  P:TD2Point;
begin
  inherited;
  if FPressed and (FTrack <> nil) and (Enabled) and (Parent <> nil) and (Parent.IsVisual) then
  begin
    if FTrack.Orientation=d2Horizontal then
    begin
      P:=FTrack.AbsoluteToLocal(LocalToAbsolute(d2Point(X,0)));
      if FTrack.ViewportSize=0 then
        FTrack.Value:=FTrack.Min + (((P.X) / FTrack.Width) * (FTrack.FMax - FTrack.FMin))
      else
        FTrack.Value:=FTrack.Min + (((P.X - FDownOffset.X) / FTrack.Width) * (FTrack.FMax - FTrack.FMin))
    end
    else
    begin
      P:=FTrack.AbsoluteToLocal(LocalToAbsolute(d2Point(0,Y)));
      if FTrack.ViewportSize=0 then
        FTrack.Value:=FTrack.Min + (((P.Y) / FTrack.Height) * (FTrack.FMax - FTrack.FMin))
      else
        FTrack.Value:=FTrack.Min + (((P.Y - FDownOffset.Y) / FTrack.Height) * (FTrack.FMax - FTrack.FMin))
    end;
  end;
end;

procedure TD2Thumb.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  V:single;
begin
  inherited;
  if FPressed then
  begin
    if not FTrack.Tracking and Assigned(FTrack.FOnChange) then
    begin
      FTrack.FTracking:=true;
      V:=FTrack.FValue;
      FTrack.FValue:=$FFFF;
      FTrack.Value:=V;
      FTrack.FTracking:=false;
    end;
    FPressed:=false;
  end;
end;                                


//=============== TD2Expander =======================================================

constructor TD2Expander.Create(AOwner:TComponent);
begin
  inherited;
  Width:=130;
  Height:=130;

  FAutoTranslate:=true;

  FIsExpanded:=true;
  FIsChecked:=true;

  FContent:=TD2Content.Create(Self);
  FContent.Parent:=Self;
  FContent.ClipChildren:=false;
  FContent.HitTest:=false;
  FContent.Locked:=true;
  FContent.Stored:=false;
  FContent.Padding.Top:=25;
  FContent.Width:=Width;
  FContent.Height:=Height;
end;

procedure TD2Expander.AddObject(AObject:TD2Object);
begin
  if (FContent <> nil) and (AObject <> FContent) and (AObject <> FResourceLink) then
  begin
    FContent.AddObject(AObject);
  end
  else
    inherited;
end;

procedure TD2Expander.ApplyStyle;
var
  O:TD2Object;
begin
  inherited;
  O:=FindResource('checkbox');
  if (O <> nil) and (O is TD2CheckBox) then
  begin
    FCheck:=TD2CheckBox(O);
    FCheck.Visible:=FShowCheck;
    FCheck.IsChecked:=FIsChecked;
    FCheck.OnChange:=DoCheckChange;
  end;
  O:=FindResource('button');
  if (O <> nil) and (O is TD2CustomButton) then
  begin
    FButton:=TD2CustomButton(O);
    FButton.OnClick:=DoButtonClick;
    FButton.ApplyResource;
    FButton.StartTriggerAnimation(Self,'IsExpanded');
    FButton.CanFocused:=false;
  end;
  StartTriggerAnimation(Self,'IsExpanded');
end;

procedure TD2Expander.FreeStyle;
begin
  inherited;
  FCheck:=nil;
  FButton:=nil;
end;

destructor TD2Expander.Destroy;
begin
  inherited;
end;

procedure TD2Expander.DoButtonClick(Sender:TObject);
begin
  IsExpanded:=not FIsExpanded;
end;

procedure TD2Expander.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('ContentSize',ReadContentSize,WriteContentSize,true);
end;

procedure TD2Expander.ReadContentSize(Reader:TReader);
begin
  if FContent <> nil then
    FContent.Height:=d2StrToFloat(Reader.ReadString);
end;

procedure TD2Expander.WriteContentSize(Writer:TWriter);
begin
  if FContent <> nil then
    Writer.WriteString(d2FloatToStr(FContent.Height));
end;

procedure TD2Expander.Realign;
begin
  inherited;
  if csLoading in ComponentState then Exit;
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  { content }
  if (FContent <> nil) and (IsExpanded) then
  begin
    FContent.Position.X:=0;
    FContent.Position.Y:=FContent.Padding.Top;
    FContent.Width:=Width;
    FContent.Height:=Height - FContent.Padding.Top;
  end;
  FDisableAlign:=false;
end;

procedure TD2Expander.SetIsExpanded(const Value:boolean);
begin
  if FIsExpanded <> Value then
  begin
    if FResourceLink=nil then
      ApplyResource;
    FIsExpanded:=Value;
    if FIsExpanded then
    begin
      FContent.Visible:=FIsExpanded;
      if FButton <> nil then
        Height:=FButton.Height + FContent.Height;
      Repaint;
    end
    else
    begin
      Repaint;
      FContent.Visible:=FIsExpanded;
      if FButton <> nil then
        Height:=FButton.Height;
    end;
    StartTriggerAnimation(Self,'IsExpanded');
    if FButton <> nil then
      FButton.StartTriggerAnimation(Self,'IsExpanded');
  end;
end;

procedure TD2Expander.DoCheckChange(Sender:TObject);
begin
  if (FCheck <> nil) then
  begin
    FIsChecked:=FCheck.IsChecked;
    FContent.Enabled:=FIsChecked;
    if Assigned(OnCheckChange) then
      OnCheckChange(Self);
  end;
end;

procedure TD2Expander.SetIsChecked(const Value:boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    FContent.Enabled:=FIsChecked;
    if FCheck <> nil then
      FCheck.IsChecked:=FIsChecked;
  end;
end;

procedure TD2Expander.SetShowCheck(const Value:boolean);
begin
  if FShowCheck <> Value then
  begin
    FShowCheck:=Value;
    if FCheck <> nil then
      FCheck.Visible:=FShowCheck;
  end;
end;

procedure TD2Expander.DesignClick;
begin
  inherited;
  IsExpanded:=not IsExpanded;
end;


//============  TD2GradientEdit ===========================================

constructor TD2GradientEdit.Create(AOwner:TComponent);
begin
  inherited;
  FGradient:=TD2Gradient.Create;
  FGradient.OnChanged:=DoChanged;
  Width:=200;
  Height:=20;
  AutoCapture:=true;
end;

destructor TD2GradientEdit.Destroy;
begin
  if FBitmap <> nil then
    FreeAndNil(FBitmap);
  FGradient.Free;
  inherited;
end;

function TD2GradientEdit.GetPointRect(const Point:integer):TD2Rect;
begin
  if (Point >= 0) and (Point < FGradient.Points.Count) then
  with FGradient do
  begin
    Result:=d2Rect(0 + cnColorPickSize + (Points[Point].Offset * (Width - ((0 + cnColorPickSize) * 2))),Height - 0 - cnColorPickSize,
      0 + cnColorPickSize + (Points[Point].Offset * (Width - ((0 + cnColorPickSize) * 2))),Height - 0);
    d2InflateRect(Result,cnColorPickSize / 2,0);
  end
  else
    Result:=d2Rect(0,0,0,0);
end;

procedure TD2GradientEdit.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
var
  NewOffset:single;
  NewColor:TD2Color;
  i:integer;
begin
  inherited;
  FMoving:=false;
  if Button=mbLeft then
  begin
    { select point }
    for i:=0 to FGradient.Points.Count - 1 do
      if d2PtInRect(d2Point(X,Y),GetPointRect(i)) then
      begin
        CurrentPoint:=i;
        if Assigned(OnSelectPoint) then
          OnSelectPoint(Self);
        FMoving:=true;
        Repaint;
        Exit;
      end;
    { add new point }
    if (Y > 0) and (Y < Height - 0 - cnColorPickSize) then
    begin
      NewOffset:=((X - 0 - cnColorPickSize) / (Width - ((0 + cnColorPickSize)* 2)));
      if NewOffset < 0 then NewOffset:=0;
      if NewOffset > 1 then NewOffset:=1;
      NewColor:=FGradient.InterpolateColor(NewOffset);
      for i:=1 to FGradient.Points.Count - 1 do
        if NewOffset < FGradient.Points[i].Offset then
          with TD2GradientPoint(FGradient.Points.Add) do
          begin
            Index:=i;
            CurrentPoint:=Index;
            IntColor:=NewColor;
            Offset:=NewOffset;
            Repaint;
            DoChanged(Self);
            Break;
          end;
    end;
  end;
end;

procedure TD2GradientEdit.MouseMove(Shift:TShiftState; X,Y,Dx,
  Dy:single);
begin
  inherited;
  if ssLeft in Shift then
  begin
    if FMoving then
    begin
      FCurrentPointInvisible:=((Y < -10) or (Y > Height + 10)) and (FGradient.Points.Count > 1) and
        (CurrentPoint <> 0) and (CurrentPoint <> FGradient.Points.Count - 1);
      { move }
      FGradient.Points[CurrentPoint].Offset:=((X - 0 - cnColorPickSize) / (Width - ((0 + cnColorPickSize) * 2)));
      if FGradient.Points[CurrentPoint].Offset < 0 then
        FGradient.Points[CurrentPoint].Offset:=0;
      if FGradient.Points[CurrentPoint].Offset > 1 then
        FGradient.Points[CurrentPoint].Offset:=1;
      { move right }
      if CurrentPoint < FGradient.Points.Count - 1 then
        if FGradient.Points[CurrentPoint].Offset > FGradient.Points[CurrentPoint + 1].Offset then
        begin
          FGradient.Points[CurrentPoint].Index:=FGradient.Points[CurrentPoint].Index + 1;
          CurrentPoint:=CurrentPoint + 1;
        end;
      { move left }
      if CurrentPoint > 0 then
        if FGradient.Points[CurrentPoint].Offset < FGradient.Points[CurrentPoint - 1].Offset then
        begin
          FGradient.Points[CurrentPoint].Index:=FGradient.Points[CurrentPoint].Index - 1;
          CurrentPoint:=CurrentPoint - 1;
        end;
      Repaint;
      DoChanged(Self);
    end;
  end;
end;

procedure TD2GradientEdit.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited;
  FCurrentPointInvisible:=false;
  if FMoving then
  begin
    { delete }
    if (Y > Height + 10) and (FGradient.Points.Count > 1) then
    begin
      FGradient.Points.Delete(CurrentPoint);
      CurrentPoint:=CurrentPoint - 1;
      if CurrentPoint < 0 then CurrentPoint:=0;
      Repaint;
      DoChanged(Self);
      FMoving:=false;
      Exit;
    end;
  end;
  FMoving:=false;
end;

procedure TD2GradientEdit.Paint;
var
  i,j:integer;
  R:TD2Rect;
  SaveIndex:integer;
begin
//  Canvas.DrawRect(d2Rect(0,0,Width,Height));
  if FBitmap=nil then
  begin
    FBitmap:=TD2Bitmap.Create(Trunc(Width - (0 * 2)),Trunc(Height - (0 * 2) - cnColorPickSize));
  end;
  if FBitmap <> nil then
  begin
    for j:=0 to FBitmap.Height - 1 do
    begin
      for i:=0 to FBitmap.Width - 1 do
      begin
        FBitmap.Scanline[j][i]:=d2CorrectColor(FGradient.InterpolateColor(i / FBitmap.Width));
        {$ifdef FPC_BIG_ENDIAN}
        ReverseBytes(@FBitmap.Scanline[j][i],4);
        {$endif}
      end;
    end;
  end;

  { draw back }
  R:=d2Rect(0 + cnColorPickSize,0,Width - 0 - cnColorPickSize,Height - 0 - cnColorPickSize);
  SaveIndex:=Canvas.SaveCanvas;
  Canvas.IntersectClipRect(R);
  Canvas.Stroke.Style:=d2BrushNone;
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.FillRect(R,0,0,AllCorners,AbsoluteOpacity);
  Canvas.Fill.SolidColor:=$FFD3D3D3;
  for i:=0 to Trunc(Width / 10) + 1 do
    for j:=0 to Trunc(Height / 10) + 1 do
    begin
      if Odd(i + j) then
      begin
        Canvas.FillRect(d2Rect(i * 10,j * 10,(i + 1) * 10,(j + 1) * 10),0,0,AllCorners,AbsoluteOpacity);
      end;
    end;
  Canvas.RestoreCanvas(SaveIndex);
  { draw gradient }
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.StrokeThickness:=0;
  if FBitmap <> nil then
  begin
    Canvas.DrawBitmap(FBitmap,d2Rect(0,0,FBitmap.Width,FBitmap.Height),
      d2Rect(0 + cnColorPickSize,0,Width - 0 - cnColorPickSize,Height - 0 - cnColorPickSize),AbsoluteOpacity);
  end;
  { points }
  for i:=0 to FGradient.Points.Count - 1 do
  begin
    if FCurrentPointInvisible and (i=CurrentPoint) then Continue;
    R:=GetPointRect(i);
    d2InflateRect(R,-1,-1);
    Canvas.Stroke.SolidColor:=$FF757575;
    Canvas.Fill.SolidColor:=FGradient.Points[i].IntColor;
    Canvas.FillEllipse(R,AbsoluteOpacity);
    Canvas.DrawEllipse(R,AbsoluteOpacity);
    { color }
    if CurrentPoint=i then
    begin
      d2InflateRect(R,1,1);
      Canvas.Stroke.SolidColor:=$FF000000;
      Canvas.Stroke.SolidColor:=$FFFFFFFF;
      Canvas.DrawEllipse(R,AbsoluteOpacity);
    end;
  end;
end;

procedure TD2GradientEdit.SetGradient(const Value:TD2Gradient);
begin
  FGradient.Assign(Value);
end;

procedure TD2GradientEdit.DoChanged(Sender:TObject);
begin
  if Assigned(FOnChange) then
    FOnChange(Self);
  UpdateGradient;
end;

procedure TD2GradientEdit.SetCurrentPoint(const Value:integer);
begin
  if FCurrentPoint <> Value then
  begin
    FCurrentPoint:=Value;
    if Assigned(OnSelectPoint) then
      OnSelectPoint(Self);
    if (FColorPicker <> nil) and (CurrentPoint >= 0) then
      FColorPicker.Color:=Gradient.Points[CurrentPoint].IntColor;
  end;
end;

procedure TD2GradientEdit.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FColorPicker) then
    ColorPicker:=nil;
end;

procedure TD2GradientEdit.SetColorPicker(const Value:TD2ColorPicker);
begin
  FColorPicker:=Value;
  if (FColorPicker <> nil) and (CurrentPoint >= 0) then
    FColorPicker.Color:=Gradient.Points[CurrentPoint].IntColor;
end;

procedure TD2GradientEdit.UpdateGradient;
begin
  if (FColorPicker <> nil) and (CurrentPoint >= 0) then
    FColorPicker.Color:=Gradient.Points[CurrentPoint].IntColor;
end;        


//================= TD2ColorQuad ==============================

constructor TD2ColorQuad.Create(AOwner:TComponent);
begin
  inherited;
  FAlpha:=1; 
  AutoCapture:=true;
end;

destructor TD2ColorQuad.Destroy;
begin
  if (FColorBitmap <> nil) then
    FColorBitmap.Free;
  inherited;
end;

function TD2ColorQuad.GetAbsoluteRect:TD2Rect;
begin
  Result:=inherited GetAbsoluteRect;
  d2InflateRect(Result,cnColorPickSize + 1,cnColorPickSize + 1);
end;

function TD2ColorQuad.pointInObject(X,Y:single):boolean;
var
  P:TD2Point;
begin
  Result:=false;
  P:=AbsoluteToLocal(d2Point(X,Y));
  if (P.X > -cnColorPickSize / 2) and (P.X < Width + cnColorPickSize / 2) and
     (P.Y > -cnColorPickSize / 2) and (P.Y < Height + cnColorPickSize / 2) then
  begin
    Result:=true;
  end;
end;

procedure TD2ColorQuad.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  if FPressed then
  begin
    if Height <> 0 then
      Lum:=1 - ((Y) / (Height));
    if Width <> 0 then
      Sat:=((X) / (Width));
  end;
end;

procedure TD2ColorQuad.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  if FPressed then
    MouseMove([ssLeft],X,Y,0,0);
  inherited;
end;

procedure TD2ColorQuad.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FColorBox) then
    ColorBox:=nil;
end;

procedure TD2ColorQuad.Paint;
var
  i,j:integer;
  R:TD2Rect;
begin
  if FColorBitmap=nil then
  begin
    FColorBitmap:=TD2Bitmap.Create(Trunc(Width),Trunc(Height));
    if FColorBitmap <> nil then
    begin
      for i:=0 to FColorBitmap.Width - 1 do
      begin
        for j:=0 to FColorBitmap.Height - 1 do
        begin
          FColorBitmap.Scanline[j][i]:=d2CorrectColor(d2HSLtoRGB(FHue,i / FColorBitmap.Width,(1 - (j / FColorBitmap.Height))));
          {$ifdef FPC_BIG_ENDIAN}
          ReverseBytes(@FColorBitmap.Scanline[j][i],4);
          {$endif}
        end;
      end;
    end;
  end;
  if FColorBitmap <> nil then
    Canvas.DrawBitmap(FColorBitmap,d2Rect(0,0,FColorBitmap.Width,FColorBitmap.Height),
      d2Rect(0,0,Width,Height),AbsoluteOpacity);
  { current }
  R:=d2Rect(FSat * (Width),(1 - FLum) * (Height),
    FSat * (Width),(1 - FLum) * (Height));
  d2InflateRect(R,cnColorPickSize / 2,cnColorPickSize / 2);
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.StrokeThickness:=1;
  Canvas.Stroke.SolidColor:=$FF000000;
  Canvas.DrawEllipse(R,AbsoluteOpacity);
  d2InflateRect(R,-1,-1);
  Canvas.Stroke.SolidColor:=$FFFFFFFF;
  Canvas.DrawEllipse(R,AbsoluteOpacity);
  d2InflateRect(R,-1,-1);
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=d2HSLtoRGB(Hue,sat,Lum);
  Canvas.FillEllipse(R,AbsoluteOpacity);
end;

procedure TD2ColorQuad.SetAlpha(const Value:single);
begin
  if FAlpha <> Value then
  begin
    FAlpha:=Value;
    if FAlpha < 0 then FAlpha:=0;
    if FAlpha > 1 then FAlpha:=1;
    if FColorBox <> nil then
      FColorBox.Color:=d2HSLtoRGB(Hue,Sat,Lum) and $FFFFFF or (Round(Alpha * $FF) shl 24);
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2ColorQuad.SetHue(const Value:single);
begin
  if FHue <> Value then
  begin
    FHue:=Value;
    if FHue < 0 then FHue:=0;
    if FHue > 1 then FHue:=1;
    if FColorBitmap <> nil then
      FreeAndNil(FColorBitmap);
    if FColorBox <> nil then
      FColorBox.Color:=d2HSLtoRGB(Hue,Sat,Lum) and $FFFFFF or (Round(Alpha * $FF) shl 24);
    if Assigned(FOnChange) then
      FOnChange(Self);
    Repaint;
  end;
end;

procedure TD2ColorQuad.SetLum(const Value:single);
begin
  if FLum <> Value then
  begin
    FLum:=Value;
    if FLum < 0 then FLum:=0;
    if FLum > 1 then FLum:=1;
    if FColorBox <> nil then
      FColorBox.Color:=d2HSLtoRGB(Hue,Sat,Lum) and $FFFFFF or (Round(Alpha * $FF) shl 24);
    if Assigned(FOnChange) then
      FOnChange(Self);
    Repaint;
  end;
end;

procedure TD2ColorQuad.SetSat(const Value:single);
begin
  if FSat <> Value then
  begin
    FSat:=Value;
    if FSat < 0 then FSat:=0;
    if FSat > 1 then FSat:=1;
    if FColorBox <> nil then
      FColorBox.Color:=d2HSLtoRGB(Hue,Sat,Lum) and $FFFFFF or (Round(Alpha * $FF) shl 24);
    if Assigned(FOnChange) then
      FOnChange(Self);
    Repaint;
  end;
end;

procedure TD2ColorQuad.SetColorBox(const Value:TD2ColorBox);
begin
  if FColorBox <> Value then
  begin
    FColorBox:=Value;
    if FColorBox <> nil then
      FColorBox.Color:=d2HSLtoRGB(Hue,Sat,Lum) and $FFFFFF or (Round(Alpha * $FF) shl 24);
  end;
end;

//============  TD2ColorPicker =================================================

constructor TD2ColorPicker.Create(AOwner:TComponent);
begin
  inherited;
  AutoCapture:=true;
end;

destructor TD2ColorPicker.Destroy;
begin
  if (FHueBitmap <> nil) then
    FreeAndNil(FHueBitmap);
  inherited;
end;

procedure TD2ColorPicker.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FColorQuad) then
    ColorQuad:=nil;
end;

function TD2ColorPicker.GetAbsoluteRect:TD2Rect;
begin
  Result:=inherited GetAbsoluteRect;
  d2InflateRect(Result,0,cnColorPickSize / 2);
end;

function TD2ColorPicker.pointInObject(X,Y:single):boolean;
var
  P:TD2Point;
begin
  Result:=false;
  P:=AbsoluteToLocal(d2Point(X,Y));
  if (P.X > 0) and (P.X < Width) and
     (P.Y > -cnColorPickSize / 2) and (P.Y < Height + cnColorPickSize / 2) then
  begin
    Result:=true;
  end;
end;

procedure TD2ColorPicker.MouseMove(Shift:TShiftState; X,Y,Dx,
  Dy:single);
begin
  inherited;
  if FPressed then
  begin
    if Height <> 0 then
      Hue:=((Y) / (Height));
  end;
end;

procedure TD2ColorPicker.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  if FPressed then
    MouseMove([ssLeft],X,Y,0,0);
  inherited;
end;

procedure TD2ColorPicker.Paint;
var
  i,j:integer;
  R:TD2Rect;
begin
  if FHueBitmap=nil then
  begin
    FHueBitmap:=TD2Bitmap.Create(Trunc(Width),Trunc(Height));
    if FHueBitmap <> nil then
    begin
      for j:=0 to FHueBitmap.Height - 1 do
      begin
        for i:=0 to FHueBitmap.Width - 1 do
        begin
          FHueBitmap.Scanline[j][i]:=d2CorrectColor(d2HSLtoRGB(j / FHueBitmap.Height,0.9,0.5));
          {$ifdef FPC_BIG_ENDIAN}
          ReverseBytes(@FHueBitmap.Scanline[j][i],4);
          {$endif}
        end;
      end;
    end;
  end;

  if FHueBitmap <> nil then
    Canvas.DrawBitmap(FHueBitmap,d2Rect(0,0,FHueBitmap.Width,FHueBitmap.Height),
      d2Rect(0,0,Width,Height),AbsoluteOpacity);

  { hue pos }
  R:=d2Rect(Width / 2,FHue * (Height),
    Width / 2,FHue * (Height));
  d2InflateRect(R,cnColorPickSize / 2,cnColorPickSize / 2);
//  d2OffsetRect(R,01,StrokeThickness);
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.StrokeThickness:=1;
  Canvas.Stroke.SolidColor:=$FF000000;
  Canvas.DrawEllipse(R,AbsoluteOpacity);
  d2InflateRect(R,-1,-1);
  Canvas.Stroke.SolidColor:=$FFFFFFFF;
  Canvas.DrawEllipse(R,AbsoluteOpacity);
  d2InflateRect(R,-1,-1);
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=d2HSLtoRGB(Hue,0.9,0.5);
  Canvas.FillEllipse(R,AbsoluteOpacity);
end;

function TD2ColorPicker.GetColor:TD2Color;
begin
  Result:=d2HSLToRGB(Hue,1,0.5)
end;

procedure TD2ColorPicker.SetColor(const Value:TD2Color);
var
  H,S,L:single;
  //SaveChange:TNotifyEvent;
begin
  d2RGBtoHSL(Value,H,S,L);
  Hue:=H;
  if FColorQuad <> nil then
  begin
    FColorQuad.Alpha:=TD2ColorRec(Value).A / $FF;
    FColorQuad.Hue:=H;
    FColorQuad.Sat:=S;
    FColorQuad.Lum:=L;
  end;
end;

procedure TD2ColorPicker.SetHue(const Value:single);
begin
  if FHue <> Value then
  begin
    FHue:=Value;
    if FHue < 0 then FHue:=0;
    if FHue > 1 then FHue:=1;
    if FColorQuad <> nil then
      FColorQuad.Hue:=FHue;
    Repaint;
  end;
end;
            
                              
