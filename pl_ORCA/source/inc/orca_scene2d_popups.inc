{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2PopupForm ===========================================

constructor TD2PopupForm.Create(AOwner:TComponent);
begin
  inherited CreateNew(AOwner,0);
  Tag:=$FFFF;
  BorderStyle:=bsNone;
  FormStyle:=fsStayOnTop;
  Position:=poDesigned;
end;

destructor TD2PopupForm.Destroy;
begin
  if FPopup <> nil then
    FPopup.ClosePopup;
  inherited;
end;

procedure TD2PopupForm.CreateParams(var Params:TCreateParams);
begin
  inherited;
  with Params do
  begin
  end;
end;

procedure TD2PopupForm.WMDeactivate(var Msg:TLMActivate);
var
  C:TWinControl;
begin
  inherited ;
  if (not Msg.Active=0) and (FPopup <> nil) and (not FPopup.StaysOpen) then
  begin
    C:=FindControl(Msg.ActiveWindow);
    if (C <> nil) and (C is TD2PopupForm) and (not TD2PopupForm(C).FPopup.StaysOpen) then
    begin
      if (varD2PopupList.IndexOf(TD2PopupForm(C).FPopup) >= 0) and (varD2PopupList.IndexOf(FPopup) > varD2PopupList.IndexOf(TD2PopupForm(C).FPopup)) then
        FPopup.ClosePopup;
      Exit;
    end;
    CloseAllPopups;
  end;
end;

procedure TD2PopupForm.DoClose(var Action:TCloseAction);
begin
  inherited;
  Action:=caFree;
  if (FPopup <> nil) then
  begin
    FNoFree:=true;
    FPopup.ClosePopup;
  end;
end;

//=============== TD2Popup ==================================

constructor TD2Popup.Create(AOwner:TComponent);
begin
  inherited;
  FPlacementRectangle:=TD2Bounds.Create(d2Rect(0,0,0,0));
  Visible:=false;
  CanFocused:=true;
end;

destructor TD2Popup.Destroy;
begin
  ClosePopup;
  FPlacementRectangle.Free;
  inherited;
end;

procedure TD2Popup.Paint;
var
  R:TD2Rect;
begin
  inherited;
  if Assigned(Scene) and Scene.GetDesignTime then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
  end;
end;

procedure TD2Popup.ApplyPlacement;
var
  Target:TD2VisualObject;
  AbsolutePos,LocalPos:TD2Point;
  Pos:TPoint;
begin
  Target:=PlacementTarget;

  if (Target=nil) and (Parent <> nil) then
    Target:=Parent.Visual;
  if Target=nil then
  begin
    case Placement of
      d2PlacementBottom:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.bottom + VerticalOffset);
        end;
      d2PlacementTop:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.top - Height - VerticalOffset);
        end;
      d2PlacementLeft:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left - Width - HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementRight:
        begin
          LocalPos:=d2Point(FPlacementRectangle.right + HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementBottomCenter:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.bottom + VerticalOffset);
        end;
      d2PlacementTopCenter:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.top - Height - VerticalOffset);
        end;
      d2PlacementLeftCenter:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left - Width - HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementRightCenter:
        begin
          LocalPos:=d2Point(FPlacementRectangle.right + HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementCenter:
        begin
          LocalPos:=d2Point(FPlacementRectangle.left + (FPlacementRectangle.right - FPlacementRectangle.left - Width) / 2,
            FPlacementRectangle.top + (FPlacementRectangle.bottom - FPlacementRectangle.top - Height) / 2);
          LocalPos.X:=LocalPos.X + HorizontalOffset;
          LocalPos.Y:=LocalPos.Y + VerticalOffset;
        end;
      d2PlacementAbsolute:
        begin

          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Round(FPlacementRectangle.Left);
            FPopupForm.Top:=Round(FPlacementRectangle.Top);
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
{            FPopupForm.Left:=Round(FPlacementRectangle.Left);
            FPopupForm.Top:=Round(FPlacementRectangle.Top);}
          end;

          Exit;
        end;
      d2PlacementMouse:
        begin
          if IsOpen then Exit;

          GetCursorPos(Pos);
          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Pos.X;
            FPopupForm.Top:=Pos.Y;
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
          end;

          Exit;
        end;
      d2PlacementMouseCenter:
        begin
          if IsOpen then Exit;

          GetCursorPos(Pos);
          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Pos.X - round(Width / 2);
            FPopupForm.Top:=Pos.Y;
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
          end;

          Exit;
        end;
    end;
    Pos:=Point(Trunc(LocalPos.X),Trunc(LocalPos.Y));
  end
  else
  begin
    case Placement of
      d2PlacementBottom:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(HorizontalOffset,Target.Height + VerticalOffset)
          else
            LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.bottom + VerticalOffset);
        end;
      d2PlacementTop:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(HorizontalOffset,-Height - HorizontalOffset)
          else
            LocalPos:=d2Point(FPlacementRectangle.left + HorizontalOffset,FPlacementRectangle.top - Height - VerticalOffset);
        end;
      d2PlacementLeft:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(-Width - HorizontalOffset,VerticalOffset)
          else
            LocalPos:=d2Point(FPlacementRectangle.left - Width - HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementRight:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(Target.Width + HorizontalOffset,VerticalOffset)
          else
            LocalPos:=d2Point(FPlacementRectangle.right + HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementBottomCenter:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(HorizontalOffset - (Width / 2) + (Target.Width / 2),Target.Height + VerticalOffset)
          else
            LocalPos:=d2Point((FPlacementRectangle.left + FPlacementRectangle.right) / 2 - (Width / 2) + HorizontalOffset,
              FPlacementRectangle.bottom + VerticalOffset);
        end;
      d2PlacementTopCenter:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(HorizontalOffset - (Width / 2) + (Target.Width / 2),-Height - HorizontalOffset)
          else
            LocalPos:=d2Point((FPlacementRectangle.left + FPlacementRectangle.right) / 2 - (Width / 2) + HorizontalOffset,
              FPlacementRectangle.top - Height - VerticalOffset);
        end;
      d2PlacementLeftCenter:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(-Width - HorizontalOffset,VerticalOffset - (Height / 2) + (Target.Height / 2))
          else
            LocalPos:=d2Point(FPlacementRectangle.left - Width - HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementRightCenter:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point(Target.Width + HorizontalOffset,VerticalOffset - (Height / 2) + (Target.Height / 2))
          else
            LocalPos:=d2Point(FPlacementRectangle.right + HorizontalOffset,FPlacementRectangle.top + VerticalOffset);
        end;
      d2PlacementCenter:
        begin
          if FPlacementRectangle.empty then
            LocalPos:=d2Point((Target.Width - Width) / 2,(Target.Height - Height) / 2)
          else
            LocalPos:=d2Point(FPlacementRectangle.left + (FPlacementRectangle.right - FPlacementRectangle.left - Width) / 2,
              FPlacementRectangle.top + (FPlacementRectangle.bottom - FPlacementRectangle.top - Height) / 2);
          LocalPos.X:=LocalPos.X + HorizontalOffset;
          LocalPos.Y:=LocalPos.Y + VerticalOffset;
        end;
      d2PlacementAbsolute:
        begin

          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Round(FPlacementRectangle.Left);
            FPopupForm.Top:=Round(FPlacementRectangle.Top);
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
          end;

          Exit;
        end;
      d2PlacementMouse:
        begin
          if IsOpen then Exit;

          GetCursorPos(Pos);
          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Pos.X;
            FPopupForm.Top:=Pos.Y;
            if FPopupForm.Monitor <> nil then
            begin
              if FPopupForm.Left + FPopupForm.Width > FPopupForm.Monitor.Left + FPopupForm.Monitor.Width then
                FPopupForm.Left:=FPopupForm.Monitor.Left + FPopupForm.Monitor.Width - FPopupForm.Width;
              if FPopupForm.Top + FPopupForm.Height > FPopupForm.Monitor.Top + FPopupForm.Monitor.Height then
                FPopupForm.Top:=FPopupForm.Monitor.Top + FPopupForm.Monitor.Height - FPopupForm.Height;
            end;
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
          end;

          Exit;
        end;
      d2PlacementMouseCenter:
        begin
          if IsOpen then Exit;

          GetCursorPos(Pos);
          if FPlacementScene=d2PlacementSceneNew then
          begin
            FPopupForm.Left:=Pos.X - round(Width / 2);
            FPopupForm.Top:=Pos.Y;
            if FPopupForm.Monitor <> nil then
            begin
              if FPopupForm.Left + FPopupForm.Width > FPopupForm.Monitor.Left + FPopupForm.Monitor.Width then
                FPopupForm.Left:=FPopupForm.Monitor.Left + FPopupForm.Monitor.Width - FPopupForm.Width;
              if FPopupForm.Top + FPopupForm.Height > FPopupForm.Monitor.Top + FPopupForm.Monitor.Height then
                FPopupForm.Top:=FPopupForm.Monitor.Top + FPopupForm.Monitor.Height - FPopupForm.Height;
            end;
          end;
          if FPlacementScene=d2PlacementSceneTarget then
          begin
          end;

          Exit;
        end;
    end;
    AbsolutePos:=Target.LocalToAbsolute(LocalPos);
    Pos:=Point(Trunc(AbsolutePos.X),Trunc(AbsolutePos.Y));
    if FPlacementScene=d2PlacementSceneNew then
    begin
      if (Target.Scene <> nil) then
        Pos:=Point(round(Target.Scene.LocalToScreen(d2Point(Pos.X,Pos.Y)).x),round(Target.Scene.LocalToScreen(d2Point(Pos.X,Pos.Y)).y))
      else
        if (Scene <> nil) then
          Pos:=Point(round(Scene.LocalToScreen(d2Point(Pos.X,Pos.Y)).x),round(Scene.LocalToScreen(d2Point(Pos.X,Pos.Y)).y));
    end;
  end;

  if FPlacementScene=d2PlacementSceneNew then
  begin

    FPopupForm.Left:=Pos.X;
    FPopupForm.Top:=Pos.Y;
    if FPopupForm.Monitor <> nil then
    begin
      if FPopupForm.Left + FPopupForm.Width > FPopupForm.Monitor.Left + FPopupForm.Monitor.Width then
        FPopupForm.Left:=FPopupForm.Monitor.Left + FPopupForm.Monitor.Width - FPopupForm.Width;
      if FPopupForm.Top + FPopupForm.Height > FPopupForm.Monitor.Top + FPopupForm.Monitor.Height then
        FPopupForm.Top:=FPopupForm.Monitor.Top + FPopupForm.Monitor.Height - FPopupForm.Height;
    end;

  end;
  if FPlacementScene=d2PlacementSceneTarget then
  begin
    if (Parent <> nil) and (Parent.IsVisual) then
      AbsolutePos:=Parent.Visual.AbsoluteToLocal(AbsolutePos);
    Position.Point:=AbsolutePos;
  end;
end;

procedure TD2Popup.Popup;
begin
  if FAnimating then
  begin
    FIsOpen:=false;
    Exit;
  end;
  if FPopupForm <> nil then
  begin
    ClosePopup;
    Exit;
  end;

  FSaveScale:=Scale.Point;
  FSaveParent:=Parent;
  case PlacementScene of
    d2PlacementSceneNew:
      begin

        FPopupForm:=TD2PopupForm.Create(Application);
        if Owner is TCustomForm then
          TD2PopupForm(FPopupForm).FOwnerForm:=TCustomForm(Owner);
        if (Scene <> nil) and (Scene.GetComponent <> nil) and (Scene.GetComponent.Owner is TCustomForm) then
          TD2PopupForm(FPopupForm).FOwnerForm:=TCustomForm(Scene.GetComponent.Owner);
        FPopupForm.Width:=Round(Width * AbsoluteMatrix.m11);
        FPopupForm.Height:=Round(Height * AbsoluteMatrix.m22);
        Scale.X:=AbsoluteMatrix.m11;
        Scale.Y:=AbsoluteMatrix.m22;
        ApplyPlacement;
        TD2PopupForm(FPopupForm).FPopup:=Self;
        TD2PopupForm(FPopupForm).FNoFree:=false;
        FPopupScene:=TD2Scene.Create(FPopupForm);
        with FPopupScene do
        begin
          Transparency:=true;
          Parent:=FPopupForm;
          Align:=alClient;
          Style:=Self.FStyle;
          if (FStyle=nil) and Assigned(FScene) then
            Style:=FScene.GetStyle;
        end;
        FPopupScene.Width:=Round(Width);
        FPopupScene.Height:=Round(Height);
        { show }
        Visible:=true;
        { add self}
        FPopupScene.AddObject(Self);
        SetNewScene(FPopupScene);
        FPopupScene.ActiveControl:=Self;
        { apply resoruces }
        FNeedResource:=true;
        ApplyResource;
        { show }
        FPopupForm.Show;
        FPopupScene.SetFocus;
        SetFocus;

      end;
    d2PlacementSceneTarget:
      begin
        FPopupLayout:=TD2VisualObject.Create(Self);
        if (FPlacementTarget <> nil) and (FPlacementTarget.Scene <> nil) then
          FPopupLayout.Parent:=TD2VisualObject(FPlacementTarget.Scene.GetRoot)
        else
          FPopupLayout.Parent:=Scene.GetRoot;
        FPopupLayout.Align:=vaContents;
        FPopupLayout.OnClick:=DoPopupLayoutClick;
        FPopupLayout.HitTest:=not FStaysOpen;

        if FScene <> nil then  FSaveFocused:=FScene.GetFocused;
        FSaveParent:=Parent;
        Parent:=FPopupLayout;
        SetNewScene(FPopupLayout.Scene);
        SetFocus;
        ApplyPlacement;

        Visible:=true;

        FNeedResource:=true;
        ApplyResource;

        BringToFront;
        CanFocused:=true;
        SetFocus;
      end;
  end;
  if varD2PopupList=nil then  varD2PopupList:=TList.Create;
  if not StaysOpen then  varD2PopupList.Add(Self);

  FIsOpen:=true;
  ApplyTriggerEffect(Self,'IsOpen');
  StartTriggerAnimation(Self,'IsOpen');


  if (FPlacementScene=d2PlacementSceneNew) and  FDragWithParent and FStaysOpen then
  begin
    FDragTimer:=TD2Timer.Create(Self);
    FDragTimer.Interval:=10;
    FDragTimer.OnTimer:=DoTimer;
    FDragTimer.Enabled:=true;
  end;

end;

procedure TD2Popup.SetModalResult(const Value:TModalResult);
begin
  FModalResult:=Value;

  if FPopupForm <> nil then
    FPopupForm.ModalResult:=Value;

end;

function TD2Popup.PopupModal:TModalResult;
begin

  if FAnimating then
  begin
    FIsOpen:=false;
    Exit;
  end;
  if FPopupForm <> nil then
  begin
    if FModal then
    begin
      FModalResult:=mrCancel;
      FModal:=false;
      Exit;
    end;
    ClosePopup;
    Exit;
  end;
  Popup;
  FModalResult:=0;
  FModal:=true;
  repeat
    Application.HandleMessage;
    if Application.Terminated then
    begin
      FModalResult:=mrCancel;
      if FPopupForm <> nil then
        FPopupForm.ModalResult:=mrCancel;
    end;
    if FPopupForm <> nil then
    begin
      ApplyPlacement;
      FModalResult:=FPopupForm.ModalResult;
    end;
  until not ((FModalResult=0) and FModal);
  FModal:=true;
  Result:=FModalResult;
  ClosePopup;

end;

procedure TD2Popup.DialogKey(var Key:Word; Shift:TShiftState);
begin
  inherited DialogKey(Key,Shift);
  if (Key=VK_ESCAPE) and (IsOpen) and not (StaysOpen) then
  begin
    IsOpen:=false;
    Key:=0;
  end;
end;

procedure TD2Popup.ClosePopup;
//var	//Deleted by GoldenFox
  //Idx,i:integer;	//Deleted by GoldenFox
begin
  if FAnimating then Exit;
  if (PlacementScene=d2PlacementSceneNew) and (FPopupScene=nil) then Exit;
  if FModal and (FModalResult=0) then
  begin
    ModalResult:=mrCancel;
    Exit;
  end;

  if (FDragTimer <> nil) then
    FreeAndNil(FDragTimer);
  FAnimating:=true;
  FIsOpen:=false;
  if not (csDestroying in ComponentState) then
  begin
    ApplyTriggerEffect(Self,'IsOpen');
    StartTriggerAnimationWait(Self,'IsOpen');
  end;
  Visible:=false;
  if FPlacementScene=d2PlacementSceneNew then
  begin
    FPopupScene.RemoveObject(Self);
    SetNewScene(nil);

    if not TD2PopupForm(FPopupForm).FNoFree then
    begin
      FPopupScene:=nil; // Free called by PopupForm.Release
      TD2PopupForm(FPopupForm).FPopup:=nil;
      FPopupForm.Release;
      FPopupForm:=nil;
    end
    else
      FPopupForm:=nil;

    FPopupScene:=nil;
    Scale.Point:=FSaveScale;
    Parent:=FSaveParent;
  end;
  if FPlacementScene=d2PlacementSceneTarget then
  begin
    SetNewScene(nil);
    Parent:=FSaveParent;
    if FSaveFocused <> nil then
      FSaveFocused.SetFocus;
    FSaveFocused:=nil;
    FPopupLayout.Free;
    FPopupLayout:=nil;
  end;
  FAnimating:=false;
  FSaveParent:=nil;

  if Assigned(FOnClosePopup) then FOnClosePopup(Self);
  if (varD2PopupList <> nil) then varD2PopupList.Remove(Self);
end;

procedure TD2Popup.KillFocus;
begin
  inherited;
end;

procedure TD2Popup.Notification(AComponent:TComponent;
  Operation:TOperation);
begin                                
  inherited;
  if (AComponent=FPlacementTarget) and (Operation=opRemove) then
    FPlacementTarget:=nil;
  if (AComponent=FSaveParent) and (Operation=opRemove) then
    FSaveParent:=nil;
end;

procedure TD2Popup.SetIsOpen(const Value:boolean);
begin
  if FIsOpen <> Value then
  begin
    if Assigned(Scene) and Scene.GetDesignTime then
    begin
      FIsOpen:=false;
      Exit;
    end;
    FIsOpen:=Value;
    if FIsOpen then
      Popup
    else
      ClosePopup;
  end;
end;

procedure TD2Popup.DoFormClose(Sender:TObject; var Action:TCloseAction);
begin
  //..nothing...
end;

procedure TD2Popup.DoPopupLayoutClick(Sender:TObject);
begin
  if (FIsOpen) and (not StaysOpen) then
    ClosePopup;
end;

procedure TD2Popup.Dotimer(Sender:TObject);
begin
  ApplyPlacement;
end;

procedure TD2Popup.SetPlacementRectangle(const Value:TD2Bounds);
begin
end;

//=============== TD2PopupItem =======================================

constructor TD2PopupItem.Create(AOwner:TComponent);
begin
  inherited;
  FPopup:=TD2Popup.Create(Self);
  FPopup.PlacementTarget:=Self;
//  FPopup.PlacementScene:=d2PlacementSceneTarget;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.OnClosePopup:=DoClosePopup;
end;

destructor TD2PopupItem.Destroy;
begin
  inherited;
end;

procedure TD2PopupItem.AddObject(AObject:TD2Object);
begin
  if (FPopup <> nil) and (AObject <> FPopup) and (AObject <> FResourceLink) and not (AObject is TD2Effect) and not (AObject is TD2Animation) then
  begin
    FPopup.AddObject(AObject);
  end
  else
    inherited;
end;

procedure TD2PopupItem.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  j:integer;
begin
  inherited;
  if (FPopup <> nil) then
    if (FPopup.ChildrenCount > 0) then
    begin
      for j:=0 to FPopup.ChildrenCount - 1 do
        if FPopup.Children[j].Stored then
          Proc(TComponent(FPopup.Children[j]));
    end;
end;

procedure TD2PopupItem.Realign;
begin
  inherited ;
  ApplyPlacement;
end;

procedure TD2PopupItem.ApplyPlacement;
var
  SavePlacement:TD2PlacementScene;
begin
  FPopup.Placement:=Placement;
  FPopup.VerticalOffset:=VerticalOffset;
  FPopup.HorizontalOffset:=HorizontalOffset;
  if (FScene <> nil) and (FScene.GetDesignTime) and not (FPopup.DesignHide) then
  begin
    SavePlacement:=FPopup.PlacementScene;
    FPopup.PlacementScene:=d2PlacementSceneTarget;
    FPopup.ApplyPlacement;
    FPopup.PlacementScene:=SavePlacement;
  end;
end;

procedure TD2PopupItem.DesignClick;
begin
  inherited ;
  FPopup.DesignHide:=not FPopup.DesignHide;
  ApplyPlacement;
  FPopup.Locked:=FPopup.DesignHide;
end;

procedure TD2PopupItem.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineProperty('PopupSize',ReadRect,WriteRect,true);
end;

procedure TD2PopupItem.ReadRect(Reader:TReader);
var
  R:TD2Rect;
begin
  R:=d2StringToRect(Reader.ReadString);
  FPopup.SetBounds(FPopup.Position.X,FPopup.Position.Y,d2RectWidth(R),d2RectHeight(R));
end;

procedure TD2PopupItem.WriteRect(Writer:TWriter);
begin
  Writer.WriteString(d2RectToString(FPopup.LocalRect));
end;

procedure TD2PopupItem.DoClosePopup(Sender:TObject);
var
  List:TList;
  i:integer;
begin
  // close all children
  List:=TList.Create;
  AddControlsToList(List);
  for i:=0 to List.Count - 1 do
    if (TObject(List[i]) is TD2PopupItem) then
      TD2PopupItem(List[i]).MouseLeave;
  List.Free;
end;

procedure TD2PopupItem.MouseEnter;
begin
  inherited ;
  if (FPopup <> nil) and not (FPopup.IsOpen) then
  begin
    ApplyPlacement;
    FPopup.IsOpen:=true;
  end;
end;

procedure TD2PopupItem.MouseLeave;
var
  P:TPoint;
begin
  inherited ;
  if (FPopup <> nil) and (FPopup.IsOpen) then
  begin

    if FPopup.FPopupScene <> nil then
    begin
      GetCursorPos(P);
      with FPopup.FPopupScene.ScreenToClient(P) do
      begin
        if FPopup.PointInObject(x,y) then Exit;
      end;
    end
    else
    begin
      GetCursorPos(P);
      with TD2Scene(FScene.GetComponent).ScreenToClient(P) do
      begin
        if FPopup.PointInObject(x,y) then Exit;
        if PointInObject(x,y) then Exit;
      end;
    end;
    FPopup.IsOpen:=false;

  end;
end;

procedure TD2PopupItem.SetPlacement(Value:TD2Placement);
begin
  if FPlacement <> Value then
  begin
    FPlacement:=Value;
    ApplyPlacement;
  end;
end;

procedure TD2PopupItem.SetHorizontalOffset(Value:single);
begin
  if FHorizontalOffset <> Value then
  begin
    FHorizontalOffset:=Value;
    ApplyPlacement;
  end;
end;

procedure TD2PopupItem.SetVerticalOffset(Value:single);
begin
  if FVerticalOffset <> Value then
  begin
    FVerticalOffset:=Value;
    ApplyPlacement;
  end;
end;

//=============== TD2MessagePopup =================================

constructor TD2MessagePopup.Create(AOwner:TComponent);
begin
  inherited;
  FDragWithParent:=true;
  FStaysOpen:=true;
  FDisableScene:=true;
end;

destructor TD2MessagePopup.Destroy;
begin
  inherited;
end;

function TD2MessagePopup.PopupModal:TModalResult;
var
  SaveEnabled:boolean;
begin
  if (Scene <> nil) and (Scene.GetRoot <> nil) and FDisableScene then
  begin
    SaveEnabled:=Scene.GetRoot.Visual.Enabled;
    Scene.GetRoot.Visual.Enabled:=false;
  end;
  Result:=inherited PopupModal;
  if (Scene <> nil) and (Scene.GetRoot <> nil) and FDisableScene then
  begin
    Scene.GetRoot.Visual.Enabled:=SaveEnabled;
  end;
end;

procedure TD2MessagePopup.ApplyPlacement;
begin
  if PlacementTarget=nil then
  begin

    if (Owner <> nil) and (Owner is TCustomForm) then
    begin
      with TCustomForm(Owner).ClientToScreen(Point(0,0)) do
        PlacementRectangle.Rect:=d2Rect(X,Y,X + Width,Y);
      HorizontalOffset:=(TCustomForm(Owner).Width - Width) / 2;
    end;

  end
  else
  begin
    HorizontalOffset:=(PlacementTarget.Width - Width) / 2;
  end;
  inherited ;
end;

function MessagePopup(const ACaption,AMessage:{Wide}String; AType:TD2MessageType;	//Edit by GoldenFox
  Buttons:TD2MessageButtons; const AOwner:Id2Scene; const Target:TD2VisualObject=nil;
  const ADisableScene:boolean=true;
  const ABitmap:TD2Bitmap=nil;
  const AStyle:TD2Resources=nil):integer;
var
  B,DefaultButton,CancelButton:TD2MessageButton;
  PopupCtrl:TD2MessagePopup;
  C,M:TD2Label;
  L:TD2Layout;
  I:TD2VisualObject;
  Btn:TD2Button;
  SaveEnabled:boolean;
begin
  if (AOwner <> nil) and (AOwner.GetRoot <> nil) and ADisableScene then
  begin
    SaveEnabled:=AOwner.GetRoot.Visual.Enabled;
    AOwner.GetRoot.Visual.Enabled:=false;
  end;
  try
    if (AOwner <> nil) and (AOwner.GetComponent <> nil) then
      PopupCtrl:=TD2MessagePopup.Create(AOwner.GetComponent.Owner)
    else
      PopupCtrl:=TD2MessagePopup.Create(AOwner.GetComponent);
    PopupCtrl.StaysOpen:=true;
    PopupCtrl.DragWithParent:=true;
    PopupCtrl.PlacementScene:=d2PlacementSceneNew;
    PopupCtrl.Width:=500;
    PopupCtrl.Height:=150;
    PopupCtrl.Style:=AStyle;
    PopupCtrl.PlacementTarget:=Target;
    if Target <> nil then
    begin
      PopupCtrl.Scale.Point:=d2Point(Target.AbsoluteMatrix.m11,Target.AbsoluteMatrix.m22);
    end;
    if (AStyle=nil) and (AOwner <> nil) and (AOwner.GetStyle <> nil) then
      PopupCtrl.Style:=AOwner.GetStyle;

    PopupCtrl.ApplyResource;

    if ABitmap <> nil then
    begin
      I:=TD2Image.Create(PopupCtrl);
      with TD2Image(I) do
      begin
        Parent:=PopupCtrl;
        SetBounds(30,10,64,64);
        Bitmap.Assign(ABitmap);
      end;
    end
    else
    begin
      I:=TD2Control.Create(PopupCtrl);
      with I do
      begin
        Parent:=PopupCtrl;
        SetBounds(30,10,64,64);
      end;
      case AType of
        d2MessageWarning:TD2Control(I).Resource:='iconwarning';
        d2MessageError:TD2Control(I).Resource:='iconerror';
        d2MessageInformation:TD2Control(I).Resource:='iconinformation';
        d2MessageConfirmation:TD2Control(I).Resource:='iconconfirmation';
      end;
    end;

    if d2ButtonOk in Buttons then
      DefaultButton:=d2ButtonOk
    else
      if d2ButtonYes in Buttons then
        DefaultButton:=d2ButtonYes
      else
        DefaultButton:=d2ButtonRetry;
    if d2ButtonCancel in Buttons then
      CancelButton:=d2ButtonCancel
    else
      if d2ButtonNo in Buttons then
        CancelButton:=d2ButtonNo
      else
        CancelButton:=d2ButtonOk;

    C:=TD2Label.Create(PopupCtrl);
    with C do
    begin
      Parent:=PopupCtrl;
      Align:=vaTop;
      Height:=30;
      Padding.Rect:=d2Rect(30,10,30,0);
      if I <> nil then
        Padding.Left:=Padding.Rect.Left + I.width + 10;
      //TextW:=ACaption;	//Deleted by GoldenFox
      Text:=ACaption;	//Added by GoldenFox
      TextAlign:=d2TextAlignNear;
      Font.Size:=Font.Size * 1.4;
    end;
    M:=TD2Label.Create(PopupCtrl);
    with M do
    begin
      Parent:=PopupCtrl;
      Align:=vaClient;
      Height:=50;
      Padding.Rect:=d2Rect(30,0,30,0);
      if I <> nil then
        Padding.Left:=Padding.Rect.Left + I.width + 10;
      //TextW:=AMessage;	//Deleted by GoldenFox
      Text:=AMessage;	//Added by GoldenFox
      TextAlign:=d2TextAlignNear;
    end;
    L:=TD2Layout.Create(PopupCtrl);
    with L do
    begin
      Parent:=PopupCtrl;
      Align:=vaBottom;
      Height:=20;
      Padding.Rect:=d2Rect(30,0,30,30);
    end;
    for B:=Low(TD2MessageButton) to High(TD2MessageButton) do
    begin
      if not (B in Buttons) then Continue;
      Btn:=TD2Button.Create(PopupCtrl);
      with Btn do
      begin
        Parent:=L;
        Align:=vaRight;
        Position.X:=1000;
        Padding.Rect:=d2Rect(7,0,0,0);
        TextW:=MessageButtonNames[B];
        ModalResult:=ModalResults[B];
        if B=DefaultButton then Default:=True;
        if B=CancelButton then Cancel:=True;
      end;
    end;

    Result:=PopupCtrl.PopupModal;
    PopupCtrl.Free;
  finally
    if (AOwner <> nil) and (AOwner.GetRoot <> nil) and ADisableScene  then
      AOwner.GetRoot.Visual.Enabled:=SaveEnabled;
  end;
end; 


//=============== TD2PopupBox ==============================================

constructor TD2PopupBox.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=true;
  Height:=21;
  FItems:=TD2WideStringList.Create;;
  TD2WideStringList(FItems).OnChange:=DoItemsChanged;
  FPopup:=TPopupMenu.Create(nil);
  FItemIndex:=-1;
  FText:='';
end;

destructor TD2PopupBox.Destroy;
begin
  FreeAndNil(FPopup);
  FreeAndNil(FItems);
  inherited;
end;

function TD2PopupBox.GetData:Variant;
begin
  Result:=TextW;
end;

procedure TD2PopupBox.SetData(const Value:Variant);
var
  S:WideString;
begin
  if VarIsNull(Value) then
    ItemIndex:=-1
  else
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    if VarIsNumeric(Value) then
      ItemIndex:=Value
    else
      if VarIsStr(Value) then
      begin
        S:=VarToWideStr(Value);
        if FItems.IndexOf(S) < 0 then
          TextW:=S
        else
          ItemIndex:=FItems.IndexOf(S);
      end;
end;

procedure TD2PopupBox.ApplyStyle;
begin
  inherited;
end;

procedure TD2PopupBox.Click;
begin
  inherited;
  DoPopup;
end;

procedure TD2PopupBox.DoPopup;
var
  Item:TMenuItem;
  VP:TD2Point;
  i:integer;
begin
  FPopup.Items.Clear;
  for i:=0 to FItems.Count - 1 do
  begin
    Item:=NewItem(UTF8Encode(FItems[i]),0,i=FItemIndex,true,DoItemClick,0,'');
    FPopup.Items.Add(Item);
  end;
  if Scene <> nil then
  begin
    VP:=LocalToAbsolute(d2Point(0,Trunc((Height / 2) - ((FItems.Count * 20) div 2))));
    VP:=Scene.LocalToScreen(VP);
    FPopup.Popup(round(VP.X),round(VP.Y));
  end;
end;

procedure TD2PopupBox.DoItemClick(Sender:TObject);
begin
  ItemIndex:=TMenuItem(Sender).MenuIndex;
end;

procedure TD2PopupBox.DoItemsChanged(Sender:TObject);
begin
  Repaint;
end;

procedure TD2PopupBox.SetItemIndex(const Value:integer);
begin
  if FItemIndex <> Value then
  begin
    FItemIndex:=Value;
    if (FItemIndex >= 0) and (Items.Count > 0) then
      TextW:=Items[FItemIndex]
    else
    begin
      TextW:='';
      FItemIndex:=-1;
    end;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2PopupBox.SetText(const Value:String); // 7777
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);

  if FItems.Count > 0 then
  begin
    FItemIndex:=Items.IndexOf(ss);
    if FItemIndex >= 0 then
      inherited SetText(ss)
    else
      inherited SetText('')
  end
  else
  begin
    FItemIndex:=-1;
    inherited SetText('')
  end;
end;

procedure TD2PopupBox.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;
           
                 
