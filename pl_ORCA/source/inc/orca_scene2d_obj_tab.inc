{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

{ TD2TabItem }

constructor TD2TabItem.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Height:=20;
  Width:=80;
  HitTest:=true;
end;

procedure TD2TabItem.ApplyStyle;
begin
  inherited;
  if (Parent <> nil) and (Parent is TD2TabControl) and (TD2TabControl(Parent).ItemIndex=Index) then
    Select(true)
  else
    Select(false)
end;

destructor TD2TabItem.Destroy;
begin
  inherited;
end;

procedure TD2TabItem.DesignSelect;
begin
  inherited;
  if (Parent <> nil) and (Parent is TD2TabControl) then
    TD2TabControl(Parent).ItemIndex:=Index;
end;

procedure TD2TabControl.AddObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2TabItem then
    Realign;
end;

procedure TD2TabItem.Realign;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  if (FLayout <> nil) and (Parent <> nil) and (Parent is TD2TabControl) then
  begin
    FLayout.Position.X:=TD2TabControl(Parent).Margins.left + FLayout.Padding.left;
    FLayout.Position.Y:=Self.Height + TD2TabControl(Parent).Margins.top + FLayout.Padding.bottom;
    FLayout.Width:=TD2TabControl(Parent).Width - TD2TabControl(Parent).Margins.left - TD2TabControl(Parent).Margins.right -
      FLayout.Padding.left - FLayout.Padding.Right;
    FLayout.Height:=TD2TabControl(Parent).Height - Self.Height - TD2TabControl(Parent).Margins.top - TD2TabControl(Parent).Margins.bottom -
      FLayout.Padding.top - FLayout.Padding.bottom;
  end;
  FDisableAlign:=false;
end;

procedure TD2TabItem.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    if (Parent <> nil) and (Parent is TD2TabControl) then
    begin
      TD2TabControl(Parent).ItemIndex:=Index;
    end;
  end;
end;

procedure TD2TabItem.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FLayout) then
    FLayout:=nil;
end;

procedure TD2TabItem.Select(ASelected:boolean);
begin
  FIsSelected:=ASelected;
  StartTriggerAnimation(Self,'IsSelected');
  ApplyTriggerEffect(Self,'IsSelected');
end;

procedure TD2TabItem.SetIndex(const Value:integer);
begin
  if FIndex <> Value then
  begin
    inherited Index:=Value;
    Realign;
  end;
end;

procedure TD2TabItem.SetVisible(const Value:boolean);
var
  S:TD2TabItem;
begin
  if Value <> Visible then
  begin
    S:=TD2TabControl(Parent).TabItem(TD2TabControl(Parent).ItemIndex);
    inherited;
    if not Visible and IsSelected then
    begin
      Select(false);
      TD2TabControl(Parent).FItemIndex:=-1;
      if Index >= TD2TabControl(Parent).TabCount then
        TD2TabControl(Parent).ItemIndex:=TD2TabControl(Parent).TabCount - 1
      else
        TD2TabControl(Parent).ItemIndex:=Index;
    end
    else
    begin
      TD2TabControl(Parent).Realign;
      if S <> nil then
      begin
        TD2TabControl(Parent).FItemIndex:=-1;
        TD2TabControl(Parent).ItemIndex:=S.Index;
      end;
    end;
  end;
end;

//============ TD2Tab ================================================

constructor TD2TabControl.Create(AOwner:TComponent);
begin
  inherited;
  FItemIndex:=-1;
  AutoCapture:=true;
  Width:=200;
  Height:=200;
end;

destructor TD2TabControl.Destroy;
begin
  inherited;
end;

procedure TD2TabControl.FreeStyle;
begin
  inherited;
  FBackground:=nil;
end;

procedure TD2TabControl.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('background');
  if (B <> nil) and (B.IsVisual) then
  begin
    FBackground:=B.Visual;
  end;
  Realign;
end;

procedure TD2TabControl.PaintChildren;
var
  Sel:TD2TabItem;
  SaveOp:single;
begin
  Sel:=TabItem(ItemIndex);
  if (Sel <> nil) and (Sel.Visible) then
  begin
    SaveOp:=Sel.Opacity;
    Sel.Opacity:=0;
    inherited;
    Sel.Opacity:=SaveOp;

    Canvas.SetMatrix(Sel.AbsoluteMatrix);
    Sel.BeforePaint;
    Sel.Paint;
    Sel.PaintChildren;
  end
  else
    inherited;
end;

procedure TD2TabControl.Realign;
var
  Idx,i:integer;
  CurX,CurY:single;
  AutoWidth,MaxHeight:single;
  B:TD2Object;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  { move all non TabItem to end of list }
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if not (TD2Object(FChildren[i]) is TD2TabItem) then
        TD2Object(FChildren[i]).Index:=FChildren.Count - 1;
  { calc max height }
  MaxHeight:=0;
  Idx:=0;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]) is TD2TabItem then
        with TD2TabItem(FChildren[i]) do
        begin
          if not Visible then Continue;
          FIndex:=Idx;
          if Height + Padding.Top + Padding.Bottom > MaxHeight then
            MaxHeight:=Height + Padding.Top + Padding.Bottom;
          Idx:=Idx + 1;
        end;
  if Idx=0 then
    MaxHeight:=0
  else
    if FItemHeight > 0 then
      MaxHeight:=FItemHeight;
  { background }
  if FResourceLink <> nil then
  begin
    B:=FResourceLink;
    if (B <> nil) and (B.IsVisual) then
      with TD2VisualObject(B) do
      begin
        Align:=vaNone;
        Position.X:=Padding.Left;
        Position.Y:=MaxHeight + Padding.Top;
        Width:=Self.Width - Padding.Left - Padding.Top;
        Height:=Self.Height - MaxHeight - Padding.Top - Padding.Bottom;
        BringToFront;
      end;
  end;
  { align }
  CurX:=0;
  CurY:=0;
  { fullsize }
  AutoWidth:=Width;
  if FBackground <> nil then
    AutoWidth:=Width - FBackground.Margins.Left - FBackground.Margins.Right;
  if FFullSize and (Idx > 0) then
    AutoWidth:=AutoWidth / Idx
  else
    AutoWidth:=AutoWidth;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]) is TD2TabItem then
        with TD2TabItem(FChildren[i]) do
        begin
          if not Visible then Continue;
          Align:=vaNone;
          if Layout <> nil then
          begin
            Layout.Align:=vaNone;
            Layout.Visible:=Index=ItemIndex;
            Layout.DesignHide:=not (Index=ItemIndex);
            Layout.ClipChildren:=true;
            if Layout.Visible then
              Layout.BringToFront;
          end;
          if FFullSize then
            SetBounds(CurX + Padding.Left,CurY + Padding.Top,AutoWidth,MaxHeight - Padding.Top - Padding.Bottom)
          else
            SetBounds(CurX + Padding.Left,CurY + Padding.Top,Width,MaxHeight - Padding.Top - Padding.Bottom);
          CurX:=CurX + Padding.Left + Width + Padding.Right;
          Realign;
        end;
  FDisableAlign:=false;
end;

function TD2TabControl.TabCount:integer;
var
  i:integer;
begin
  Result:=0;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]) is TD2TabItem then
      begin
        if TD2TabItem(FChildren[i]).Visible then
        begin
          Inc(Result);
        end;
      end;
end;

function TD2TabControl.TabItem(AIndex:integer):TD2TabItem;
var
  i:integer;
begin
  { calc max height }
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]) is TD2TabItem then
      begin
        if TD2TabItem(FChildren[i]).Visible and (TD2TabItem(FChildren[i]).Index=AIndex) then
        begin
          Result:=TD2TabItem(FChildren[i]);
          Exit;
        end;
      end;
  Result:=nil;
end;

procedure TD2TabControl.KeyDown(var Key:Word; var KeyChar:WideChar;
  Shift:TShiftState);
begin
//  inherited ;
end;

procedure TD2TabControl.KeyUp(var Key:Word; var KeyChar:WideChar;
  Shift:TShiftState);
begin
//  inherited ;
end;

procedure TD2TabControl.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
//var	//Deleted by GoldenFox
  //Item:TD2TabItem;	//Deleted by GoldenFox
begin
  inherited;
  Realign;
{  if Button=mbLeft then
  begin
    Item:=ItemByPoint(X,Y);
    if Item <> nil then
      ItemIndex:=Item.Index;
    FMouseSelecting:=true;
  end;}
end;

procedure TD2TabControl.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
//var	//Deleted by GoldenFox
//  Item:TD2TabItem;	//Deleted by GoldenFox
begin
  inherited;
{  if (ssLeft in Shift) and FMouseSelecting then
  begin
    Item:=ItemByPoint(X,Y);
    if Item <> nil then
      ItemIndex:=Item.Index;
  end;}
end;

procedure TD2TabControl.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
//  FMouseSelecting:=false;
end;

procedure TD2TabControl.MouseWheel(Shift:TShiftState; WheelDelta:integer;
  var Handled:boolean);
begin
  inherited;
end;

procedure TD2TabControl.SetItemIndex(const Value:integer);
begin
  if FItemIndex <> Value then
  begin
    if TabItem(FItemIndex) <> nil then
      TabItem(FItemIndex).Select(false);
    FItemIndex:=Value;
    Realign;
    if TabItem(FItemIndex) <> nil then
      TabItem(FItemIndex).Select(true);
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2TabItem.DesignInsert;
var
  L:TD2Layout;
begin
  inherited;
  if FLayout=nil then
  begin
    L:=TD2Layout.Create(Owner);
    L.Parent:=Parent;
    if GvarD2Designer <> nil then
      L.Name:=GvarD2Designer.UniqueName(Owner,L.ClassName);
    Layout:=L;
  end;
end;

function TD2TabControl.ItemClass:string;
begin
  Result:='TD2TabItem';
end;

procedure TD2TabControl.SetItem(AItem:TD2TabItem);
begin
  ItemIndex:=AItem.Index;
end;

procedure TD2TabControl.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    Realign;
  end;
end;

procedure TD2TabControl.SetFullSize(const Value:boolean);
begin
  if FFullSize <> Value then
  begin
    FFullSize:=Value;
    Realign;
  end;
end;

{ TD2HudTabControl }

function TD2HudTabControl.ItemClass:string;
begin
  Result:='TD2HudTabItem';
end;

                   
