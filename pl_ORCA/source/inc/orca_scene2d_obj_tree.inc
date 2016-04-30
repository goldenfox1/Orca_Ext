{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//============== TD2TreeViewItem =============================

constructor TD2TreeViewItem.Create(AOwner:TComponent);
begin
  inherited;
  Position.Point:=d2Point(5000,5000);
  FAutoTranslate:=true;
  TextAlign:=d2TextAlignNear;
  Height:=19;
  HitTest:=false;
  CanFocused:=false;
end;

procedure TD2TreeViewItem.Realign;
begin
  if (TreeView <> nil) and (TreeView.FUpdating > 0) then Exit;
  inherited;
end;

procedure TD2TreeViewItem.DragEnd;
begin
  inherited;
  DragLeave;
  if (TreeView <> nil) and (TreeView.FDragItem <> nil) then
  begin
    TreeView.FDragItem.RemoveFreeNotify(TreeView);
    TreeView.FDragItem:=nil;
  end;
end;

function TD2TreeViewItem.GetCount:integer;
var
  i:integer;
begin
  Result:=0;
  if ChildrenCount > 0 then
    for i:=0 to ChildrenCount - 1 do
      if Children[i] is TD2TreeViewItem then
      begin
        Inc(Result);
      end;
end;

function TD2TreeViewItem.ItemByPoint(const X,Y:single):TD2TreeViewItem;
var
  i:integer;
  P,P1:TD2Point;
begin
  P:=LocaltoAbsolute(d2Point(X,Y));
  for i:=0 to Count - 1 do
    with ItemByIndex(i) do
    begin
      if not Visible then Continue;
      if pointInObject(P.X,P.Y) then
      begin
        Result:=Self.ItemByIndex(i);
        Exit;
      end
      else
        if (Count > 0) and (IsExpanded) then
        begin
          P1:=AbsoluteToLocal(P);
          Result:=ItemByPoint(P1.X,P1.Y);
          if Result <> nil then
            Exit;
        end;
    end;
  Result:=nil;
end;

function TD2TreeViewItem.ItemByIndex(const Idx:integer):TD2TreeViewItem;
var
  c,i:integer;
begin
  c:=0;
  if ChildrenCount > 0 then
    for i:=0 to ChildrenCount - 1 do
      if Children[i] is TD2TreeViewItem then
      begin
        if c=Idx then
        begin
          Result:=TD2TreeViewItem(Children[i]);
          Exit;
        end;
        Inc(c);
      end;
  Result:=nil;
end;

procedure TD2TreeViewItem.Paint;
var
  R:TD2Rect;
begin
  inherited Paint;
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

function TD2TreeViewItem.ItemClass:string;
begin
  Result:=ClassName;
end;

procedure TD2TreeViewItem.DesignClick;
begin
  inherited;
  IsExpanded:=not IsExpanded;
end;

function TD2TreeViewItem.EnterFocusChildren(AObject:TD2VisualObject):boolean;
begin
  Result:=inherited EnterFocusChildren(AObject);
  if (TreeView <> nil) then
  begin
    TreeView.Selected:=Self;
    Result:=true;
  end;
end;

function TD2TreeViewItem.Level:integer;
var
  P:TD2Object;
begin
  Result:=0;
  P:=Parent;
  while (P <> nil) and not (P is TD2TreeView) and not (P is TD2Content) do
  begin
    Result:=Result + 1;
    P:=P.Parent;
  end;
end;

function TD2TreeViewItem.TreeView:TD2TreeView;
var
  P:TD2Object;
begin
  P:=Parent;
  while (P <> nil) do
  begin
    if P is TD2TreeView then
    begin
      Result:=TD2TreeView(P);
      Exit;
    end;
    P:=P.Parent;
  end;
  Result:=nil;
end;

procedure TD2TreeViewItem.FreeStyle;
begin
  inherited;
  FButton:=nil;
  FCheck:=nil;
end;

procedure TD2TreeViewItem.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('button');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FButton:=TD2CustomButton(B);
    FButton.OnClick:=DoButtonClick;
    FButton.Visible:=Count > 0;
    if FButton.Visible then
    begin
      FButton.ApplyResource;
      FButton.StartTriggerAnimation(Self,'IsExpanded');
    end;
  end;
  B:=FindResource('check');
  if (B <> nil) and (B is TD2CheckBox) then
  begin
    FCheck:=TD2CheckBox(B);
    FCheck.IsChecked:=IsChecked;
    FCheck.OnChange:=DoCheckClick;
    if TreeView <> nil then
      FCheck.Visible:=TreeView.ShowCheckboxes;
  end;
  StartTriggerAnimation(Self,'IsSelected');
end;

procedure TD2TreeViewItem.DoCheckClick(Sender:TObject);
begin
  if FCheck <> nil then
    FIsChecked:=FCheck.IsChecked;
  if TreeView <> nil then
  begin
    TreeView.SetFocus;
    TreeView.Selected:=Self;
    if Assigned(TreeView.OnChangeCheck) then
      TreeView.OnChangeCheck(Self);
  end;
end;

procedure TD2TreeViewItem.UpdateCheck;
var
  i:integer;
begin
  if (TreeView <> nil) and (FCheck <> nil) then
    FCheck.Visible:=TreeView.ShowCheckboxes;
  if ChildrenCount > 0 then
    for i:=0 to ChildrenCount - 1 do
      if Children[i] is TD2TreeViewItem then
        TD2TreeViewItem(Children[i]).UpdateCheck;
end;

procedure TD2TreeViewItem.SetIsChecked(const Value:boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    if FCheck <> nil then
      FCheck.IsChecked:=FIsChecked;
  end;
end;

procedure TD2TreeViewItem.SetIsSelected(const Value:boolean);
begin
  if FIsSelected <> Value then
  begin
    FIsSelected:=Value;
    StartTriggerAnimation(Self,'IsSelected');
    if Treeview <> nil then Treeview.UpdateSelection;
  end;
end;

procedure TD2TreeViewItem.DoButtonClick(Sender:TObject);
begin
  IsExpanded:=not IsExpanded;
end;

procedure TD2TreeViewItem.SetIsExpanded(const Value:boolean);
begin
  if FIsExpanded <> Value then
  begin
    FIsExpanded:=Value;
    if (FButton <> nil) and not (csLoading in ComponentState) then
    begin
      FButton.Visible:=Count > 0;
      if FButton.Visible then
        FButton.StartTriggerAnimation(Self,'IsExpanded');
    end;
    if TreeView <> nil then
      TreeView.Realign;
  end;
end;

function TD2TreeViewItem.GetItem(Index:integer):TD2TreeViewItem;
begin
  Result:=ItemByIndex(Index);
end;

//============== TD2TreeView ==================================================

constructor TD2TreeView.Create(AOwner:TComponent);
begin
  inherited;
  FGlobalList:=TList.Create;
  FGlobalList.Capacity:=100;
  CanFocused:=true;
  AutoCapture:=true;
  HideSelectionUnfocused:=true;
  Width:=100;
  Height:=100;
  FItemHeight:=0;
end;

destructor TD2TreeView.Destroy;
begin
  if FSelections <> nil then
    FreeAndNil(FSelections);
  FreeAndNil(FGlobalList);
  inherited;
end;

procedure TD2TreeView.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FSelection:=TD2VisualObject(T);
    FSelection.Visible:=false;
  end;
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    TD2VisualObject(T).Visible:=false;
  end;
  UpdateSelection;
end;

procedure TD2TreeView.FreeStyle;
begin
  inherited;
  FSelection:=nil;
  if FSelections <> nil then
    FSelections.Clear;
end;

procedure TD2TreeView.UpdateGlobalIndexes;
var
  GlobalIdx:integer;

  procedure AlignItem(AItem:TD2TreeViewItem);
  var
    i:integer;
    //P:TD2Point;
  begin
    AItem.GlobalIndex:=GlobalIdx;
    GlobalIdx:=GlobalIdx + 1;
    FGlobalList.Add(AItem);
    if AItem.Count > 0 then
    begin
      if AItem.IsExpanded then
        for i:=0 to AItem.Count - 1 do
          AlignItem(AItem.ItemByIndex(i));
    end;
  end;
var
  i:integer;
begin
  FGlobalList.Clear;
  GlobalIdx:=0;
  for i:=0 to Count - 1 do
    AlignItem(ItemByIndex(i));
  FGlobalCount:=GlobalIdx;
end;

function CompareTreeItem(item1,item2:TD2Object):integer;
begin
  if (item1 is TD2TreeViewItem) and (item2 is TD2TreeViewItem) then
  begin
    if (TD2TreeViewItem(item1).TreeView <> nil) and Assigned(TD2TreeViewItem(item1).TreeView.OnCompare) then
      Result:=TD2TreeViewItem(item1).TreeView.OnCompare(TD2TreeViewItem(item1),TD2TreeViewItem(item2))
    else
      Result:=WideCompareText(TD2TreeViewItem(item1).TextW,TD2TreeViewItem(item2).TextW);

  end
  else
    Result:=0;
end;

procedure TD2TreeView.SortItems;
begin
  if not FSorted then Exit;
  FContent.Sort(CompareTreeItem);
end;

function TD2TreeView.GetItemRect(Item:TD2TreeViewItem):TD2Rect;
var
  P:TD2Point;
begin
  if Item <> nil then
  begin
    P:=Item.LocalToAbsolute(d2Point(0,0));
    P:=FContent.AbsoluteToLocal(P);
    Result:=d2Rect(0,0,Item.Width,Item.Height);
    d2OffsetRect(Result,P.X,P.Y);
  end
  else
    Result:=d2Rect(0,0,0,0);
end;

procedure TD2TreeView.UpdateSelection;
var
  i:integer;
  P:TD2Point;
  R:TD2Rect;
  //Sel:boolean;
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
  Vis:boolean;
begin
  if FSelection=nil then Exit;
  // calc rects
  Vis:=true;
  //Sel:=false;
  SetLength(SelRects,0);
  for i:=0 to GlobalCount - 1 do
  begin
    if (ItemByGlobalIndex(i).IsSelected) then
    begin
      P:=ItemByGlobalIndex(i).LocalToAbsolute(d2Point(0,0));
      if (FSelection.Parent <> nil) and (FSelection.Parent.IsVisual) then
        P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
      R:=d2Rect(P.X,P.Y,P.X + ItemByGlobalIndex(i).Width,P.Y + ItemByGlobalIndex(i).Height);
      if (Length(SelRects) > 0) and (i > 0) and (ItemByGlobalIndex(i - 1).IsSelected) then
        SelRects[High(SelRects)]:=d2UnionRect(R,SelRects[High(SelRects)])
      else
      begin
        SetLength(SelRects,Length(SelRects) + 1);
        SelRects[High(SelRects)]:=R;
      end;
      //Sel:=true;
    end;
  end;
  // Create selection list
  if FSelections=nil then
    FSelections:=TList.Create;
  // create selections
  if FSelections.Count < Length(SelRects) then
    for i:=FSelections.Count to Length(SelRects) - 1 do
    begin
      Clone:=TD2VisualObject(FSelection.Clone(Self));
      FSelections.Add(Clone);
      Clone.Parent:=FSelection.Parent;
    end;
  // hide if not need
  if Length(SelRects) < FSelections.Count then
    for i:=Length(SelRects) to FSelections.Count - 1 do
    begin
      TD2VisualObject(FSelections[i]).Visible:=false;
    end;
  // Check visible
  if HideSelectionUnfocused and not IsFocused then
    Vis:=false;
  // align selections
  for i:=0 to High(SelRects) do
  begin
    TD2VisualObject(FSelections[i]).Visible:=Vis;
    if Vis then
    begin
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left,Top,Right - Left,Bottom - Top);
    end;
  end;
end;

function TD2TreeView.GetContentBounds:TD2Rect;
const
  StepX=19;
var
  CurY,CurX:single;
  R:TD2Rect;

  procedure HideItem(AItem:TD2TreeViewItem);
  var
    i:integer;
  begin
    AItem.Visible:=false;
    AItem.Opacity:=0;
    if AItem.Count > 0 then
      for i:=0 to AItem.Count - 1 do
        HideItem(AItem.ItemByIndex(i));
  end;

  procedure AlignItem(AItem:TD2TreeViewItem);
  var
    i:integer;
    P:TD2Point;
  begin
    P:=d2Point(CurX,CurY);
    P:=FContent.LocalToAbsolute(P);
    P:=AItem.Parent.Visual.AbsoluteToLocal(P);
    if FItemHeight <> 0 then
      AItem.SetBounds(P.X + AItem.Padding.Left,P.Y + AItem.Padding.Top,
        R.Right - R.Left - AItem.Padding.Left - AItem.Padding.Right - (AItem.Level * StepX),FItemHeight)
    else
      AItem.SetBounds(P.X + AItem.Padding.Left,P.Y + AItem.Padding.Top,
        R.Right - R.Left - AItem.Padding.Left - AItem.Padding.Right - (AItem.Level * StepX),AItem.Height);

    if AItem.FButton <> nil then
      AItem.FButton.Visible:=AItem.Count > 0;

    CurY:=CurY + AItem.Height + AItem.Padding.Top + AItem.Padding.Bottom;

    if AItem.Count > 0 then
    begin
      if AItem.IsExpanded then
      begin
        CurX:=CurX + StepX;
        for i:=0 to AItem.Count - 1 do
        begin
          with AItem.ItemByIndex(i) do
          begin
            Visible:=true;
            Opacity:=1;
          end;
          AlignItem(AItem.ItemByIndex(i));
        end;
        CurX:=CurX - StepX;
      end
      else
      begin
        for i:=0 to AItem.Count - 1 do
          HideItem(AItem.ItemByIndex(i));
      end;
    end;
  end;
var
  i:integer;
  //C:integer;
  //P:TD2Point;
  //Sel:TD2TreeViewItem;
begin
  Result:=LocalRect;
  UpdateGlobalIndexes;
  if FUpdating > 0 then Exit;
  if ContentLayout=nil then Exit;
  R:=ContentLayout.LocalRect;
  { content }
  FCountExpanded:=0;
  if FContent <> nil then
  begin
    { Sort if need }
    SortItems;
    { align }
    CurY:=0;
    CurX:=0;
    for i:=0 to Count - 1 do
      AlignItem(ItemByIndex(i));
    R.Bottom:=R.Top + CurY;
  end;
  if R.Bottom=R.Top then
    R.Bottom:=R.Top + 1;
  Result:=R;
  UpdateSelection;
end;

procedure TD2TreeView.HScrollChange(Sender:TObject);
begin
  inherited;
  UpdateSelection;
end;

procedure TD2TreeView.VScrollChange(Sender:TObject);
begin
  inherited;
  UpdateSelection;
end;

function TD2TreeView.ItemByIndex(const Idx:integer):TD2TreeViewItem;
begin
  if (FContent <> nil) and (FContent.ChildrenCount > 0) and (Idx >= 0) and (Idx < FContent.ChildrenCount) then
    Result:=TD2TreeViewItem(FContent.Children[Idx])
  else
    Result:=nil;
end;

function TD2TreeView.ItemByGlobalIndex(const Idx:integer):TD2TreeViewItem;
begin
  Result:=TD2TreeViewItem(FGlobalList[Idx]);
end;

function TD2TreeView.ItemByPoint(const X,Y:single):TD2TreeViewItem;
var
  i:integer;
  P,P1:TD2Point;
begin
  P:=LocalToAbsolute(d2Point(X,Y));
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i:=0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2TreeViewItem then
      begin
        if not TD2TreeViewItem(FContent.Children[i]).Visible then Continue;
        if not d2IntersectRect(TD2TreeViewItem(FContent.Children[i]).UpdateRect,UpdateRect) then Continue;
        if TD2TreeViewItem(FContent.Children[i]).pointInObject(P.X,P.Y) then
        begin
          Result:=TD2TreeViewItem(FContent.Children[i]);
          Exit;
        end
        else
          if (TD2TreeViewItem(FContent.Children[i]).IsExpanded) and (TD2TreeViewItem(FContent.Children[i]).Count > 0) then
          begin
            P1:=TD2TreeViewItem(FContent.Children[i]).AbsoluteToLocal(P);
            Result:=TD2TreeViewItem(FContent.Children[i]).ItemByPoint(P1.X,P1.Y);
            if Result <> nil then
              Exit;
          end;
      end;
  Result:=nil;
end;

procedure TD2TreeView.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
  if (Count > 0) and (Selected <> nil) then
  begin
    case Key of
      VK_ADD:Selected.IsExpanded:=true;
      VK_SUBTRACT:Selected.IsExpanded:=false;
      VK_HOME:Selected:=ItemByGlobalIndex(0);
      VK_END:Selected:=ItemByGlobalIndex(GlobalCount - 1);
      VK_UP:
        if Selected.GlobalIndex > 0 then
          Selected:=ItemByGlobalIndex(Selected.GlobalIndex - 1);
      VK_DOWN:
        if Selected.GlobalIndex < GlobalCount - 1 then
          Selected:=ItemByGlobalIndex(Selected.GlobalIndex + 1);
    else
      Exit;
    end;
    Key:=0;
  end;
end;

procedure TD2TreeView.KeyUp(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
end;

procedure TD2TreeView.DragOver(const Data:TD2DragObject;
  const Point:TD2Point; var Accept:boolean);
var
  Obj:TD2TreeViewItem;
begin
  inherited;
  with AbsoluteToLocal(Point) do
    Obj:=ItemByPoint(X,Y);
  if (Obj <> FDragItem) then
  begin
    if FDragItem <> nil then
    begin
      FDragItem.DragLeave;
      FDragItem.RemoveFreeNotify(Self);
    end;
    FDragItem:=Obj;
    if FDragItem <> nil then
    begin
      FDragItem.AddFreeNotify(Self);
      FDragItem.DragEnter(Data,Point);
      Accept:=true;
    end
    else
      Accept:=false;
  end
  else
    Accept:=true;

  if FDragItem=Selected then
    Accept:=false;
end;

procedure TD2TreeView.DragDrop(const Data:TD2DragObject;
  const Point:TD2Point);
var
  Obj:TD2TreeViewItem;
  Allow:boolean;
begin
  inherited;
  if FDragItem <> nil then
  begin
    FDragItem.DragLeave;
    FDragItem.RemoveFreeNotify(Self);
    FDragItem:=nil;
  end;
  with AbsoluteToLocal(Point) do
    Obj:=ItemByPoint(X,Y);
  if Obj=nil then
  begin
    // to root
    Allow:=true;
    if Assigned(OnDragChange) then
      OnDragChange(TD2TreeViewItem(Data.Source),nil,Allow);
    if Allow then
    begin
      TD2TreeViewItem(Data.Source).Parent:=Self;
      Realign;
    end;
  end
  else
  begin
    Allow:=true;
    if Assigned(OnDragChange) then
      OnDragChange(TD2TreeViewItem(Data.Source),Obj,Allow);
    if Allow then
    begin
      if not Obj.IsExpanded then
        Obj.IsExpanded:=true;
      TD2TreeViewItem(Data.Source).Parent:=Obj;
      Realign;
    end;
  end;
end;

procedure TD2TreeView.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  Item:TD2TreeViewItem;
begin
  inherited;
  if Button=mbLeft then
  begin
    Item:=ItemByPoint(X,Y);
    if Item <> nil then
    begin
      if Multiselect then
      begin
        if ssCtrl in Shift then
          Item.IsSelected:=not Item.IsSelected
        else
        if ssShift in Shift then
        begin
          SelectRange(Selected,Item);
          Selected:=Item;
        end
        else
        begin
          SelectRange(Item,Item);
          Selected:=Item;
        end;
        FFirstSelect:=Item;
      end
      else
      begin
        if Selected <> Item then
          Selected:=Item
        else
          if AllowDrag then
            Scene.BeginVCLDrag(Selected,MakeScreenshot);
      end;
    end;
    FMouseSelecting:=true;
  end;
end;

procedure TD2TreeView.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
end;

procedure TD2TreeView.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  FFirstSelect:=nil;
  FMouseSelecting:=false;
end;

procedure TD2TreeView.Clear;
var
  i:integer;
begin
  BeginUpdate;
  if FContent <> nil then
    if FContent.ChildrenCount > 0 then
      for i:=FContent.ChildrenCount - 1 downto 0 do
        if FContent.Children[i] is TD2TreeViewItem then
          TD2Object(FContent.Children[i]).Free;
  FScrollDesign.Y:=0;
  FScrollDesign.X:=0;
  FSelected:=nil;
  UpdateGlobalIndexes;
  UpdateSelection;
  EndUpdate;
end;

procedure TD2TreeView.SelectRange(Item1,Item2:TD2TreeViewItem);
var
  i:integer;
begin
  if Item1=nil then Exit;
  if Item2=nil then Exit;
  for i:=0 to Min(Item1.GlobalIndex,Item2.GlobalIndex) - 1 do
    ItemByGlobalIndex(i).IsSelected:=false;
  for i:=Max(Item1.GlobalIndex,Item2.GlobalIndex) + 1 to GlobalCount - 1 do
    ItemByGlobalIndex(i).IsSelected:=false;
  for i:=Min(Item1.GlobalIndex,Item2.GlobalIndex) to Max(Item1.GlobalIndex,Item2.GlobalIndex) do
    ItemByGlobalIndex(i).IsSelected:=true;
end;

procedure TD2TreeView.ClearSelection;
var
  i:integer;
begin
  for i:=0 to GlobalCount - 1 do
    ItemByGlobalIndex(i).IsSelected:=false;
end;

procedure TD2TreeView.SelectAll;
var
  i:integer;
begin
  for i:=0 to GlobalCount - 1 do
    ItemByGlobalIndex(i).IsSelected:=true;
end;

procedure TD2TreeView.EnterFocus;
begin
  inherited;
  if HideSelectionUnfocused and (Selected <> nil) then
    UpdateSelection;
end;

procedure TD2TreeView.KillFocus;
begin
  inherited;
  if HideSelectionUnfocused and (Selected <> nil) then
    UpdateSelection;
end;

procedure TD2TreeView.AddObject(AObject:TD2Object);
begin
  if (FContent <> nil) and ((AObject is TD2TreeViewItem) or (AObject is TD2SelectionItem)) then
  begin
    FContent.AddObject(AObject);
  end
  else
    inherited;
end;

procedure TD2TreeViewItem.AddObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2TreeViewItem then
    if FUpdating=0 then
    begin
      TreeView.UpdateGlobalIndexes;
      TreeView.Realign;
    end;
end;

procedure TD2TreeViewItem.RemoveObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2TreeViewItem then
  begin
    TD2TreeViewItem(AObject).IsSelected:=false;
    if FUpdating=0 then
    begin
      TreeView.UpdateGlobalIndexes;
      TreeView.Realign;
    end;
  end;
end;

procedure TD2TreeView.ContentAddObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2TreeViewItem then
    if FUpdating=0 then
    begin
      UpdateGlobalIndexes;
      Realign;
    end;
end;

procedure TD2TreeView.ContentRemoveObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2TreeViewItem then
  begin
    TD2TreeViewItem(AObject).IsSelected:=false;
    if FUpdating=0 then
    begin
      UpdateGlobalIndexes;
      Realign;
    end;
  end;
end;

procedure TD2TreeView.SetSelected(const Value:TD2TreeViewItem);
var
  i:TD2Object;
  P:TD2Point;
begin
  if FSelected <> Value then
  begin
    if (FSelected <> nil) and not MultiSelect then
      FSelected.IsSelected:=false;

    FSelected:=Value;
    if (FSelected <> nil) and (FContent <> nil) then
    begin
      i:=FSelected.Parent;
      while ((i <> nil) and not (i is TD2TreeView)) do
      begin
        if (i is TD2TreeViewItem) then
          TD2TreeViewItem(i).IsExpanded:=true;
        i:=i.Parent;
      end;
      if (FContent <> nil) and (ContentLayout <> nil) and (VScrollBar <> nil) then
      begin
        P:=ContentLayout.AbsoluteToLocal(FSelected.LocalToAbsolute(d2Point(0,0)));
        if P.Y < 0 then
          VScrollBar.Value:=VScrollBar.Value + P.Y;
        if P.Y + FSelected.Padding.Top + FSelected.Padding.Bottom + FSelected.Height > ContentLayout.Height then
          VScrollBar.Value:=VScrollBar.Value + (P.Y + FSelected.Padding.Top + FSelected.Padding.Bottom + FSelected.Height - ContentLayout.Height);
      end;
      FSelected.IsSelected:=true;
    end;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2TreeView.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FSelected) then
    FSelected:=nil;
  if (Operation=opRemove) and (AComponent=FDragItem) then
    FDragItem:=nil;
end;

function TD2TreeView.ItemClass:string;
begin
  Result:='TD2TreeViewItem';
end;

procedure TD2TreeView.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    Realign;
  end;
end;

procedure TD2TreeView.CollapseAll;
var
  i:integer;
  item:TD2TreeViewItem;
begin
  BeginUpdate;
  for i:=0 to Count - 1 do
  begin
    item:=ItemByIndex(i);
    if item <> nil then
      item.IsExpanded:=false;
  end;
  EndUpdate;
end;

procedure TD2TreeView.ExpandAll;
var
  i:integer;
  item:TD2TreeViewItem;
begin
  BeginUpdate;
  for i:=0 to Count - 1 do
  begin
    item:=ItemByIndex(i);
    if item <> nil then
      item.IsExpanded:=true;
  end;
  EndUpdate;
end;

function TD2TreeView.GetSelection:TD2SelectionItem;
var
  B:TD2Object;
begin
  B:=FindResource('selection');
  if (B <> nil) and (B is TD2SelectionItem) then
    Result:=TD2SelectionItem(B)
  else
    Result:=nil;
end;

procedure TD2TreeView.SetShowCheckboxes(const Value:boolean);
var
  i:integer;
begin
  if FShowCheckboxes <> Value then
  begin
    FShowCheckboxes:=Value;
    for i:=0 to Count - 1 do
      if ItemByIndex(i) <> nil then
        ItemByIndex(i).UpdateCheck;
  end;
end;

function TD2TreeView.GetItem(Index:integer):TD2TreeViewItem;
begin
  Result:=ItemByIndex(Index);
end;

procedure TD2TreeView.SetSorted(const Value:boolean);
begin
  if FSorted <> Value then
  begin
    FSorted:=Value;
    Realign;
  end;
end;

procedure TD2TreeView.SetAllowDrag(const Value:boolean);
begin
  if FAllowDrag <> Value then
  begin
    FAllowDrag:=Value;
    if FAllowDrag then
      DragDisableHighlight:=true;
  end;
end;

procedure TD2TreeView.EndUpdate;
begin
  inherited;
end;

function TD2TreeView.GetCount:integer;
begin
  Result:=0;
  if (FContent <> nil) then
    Result:=FContent.ChildrenCount;
end; 

