{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2ListBoxItem ========================================================

constructor TD2ListBoxItem.Create(AOwner:TComponent);
begin
  inherited;
  Position.Point:=d2Point(5000,5000);
  TextAlign:=d2TextAlignNear;
  FAutoTranslate:=true;
  FText:='';
  Height:=19;
  Width:=19;
  HitTest:=false;
end;

destructor TD2ListBoxItem.Destroy;
begin
  inherited;
end;

procedure TD2ListBoxItem.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('check');
  if (B <> nil) and (B is TD2CheckBox) then
  begin
    FCheck:=TD2CheckBox(B);
    FCheck.IsChecked:=IsChecked;
    FCheck.OnChange:=DoCheckClick;
    if ListBox <> nil then
      FCheck.Visible:=ListBox.ShowCheckboxes
    else
      FCheck.Visible:=false;
  end;
  StartTriggerAnimation(Self,'IsSelected');
  // Show the trigger effect on the listbox item:                       *** DB ***
  ApplyTriggerEffect(Self,'IsSelected');                              // *** DB ***
end;

procedure TD2ListBoxItem.FreeStyle;
begin
  inherited;
  FCheck:=nil;
end;

procedure TD2ListBoxItem.DoCheckClick(Sender:TObject);
begin
  if FCheck <> nil then
    FIsChecked:=FCheck.IsChecked;
  if ListBox <> nil then
  begin
    ListBox.SetFocus;
    ListBox.ItemIndex:=Index;
    if Assigned(ListBox.OnChangeCheck) then
      ListBox.OnChangeCheck(Self);
  end;
end;

function TD2ListBoxItem.ComboBox:TD2ComboBox;
var
  P:TD2Object;
begin
  P:=Parent;
  while (P <> nil) do
  begin
    if P is TD2ComboBox then
    begin
      Result:=TD2ComboBox(P);
      Exit;
    end;
    P:=P.Parent;
  end;
  Result:=nil;
end;

function TD2ListBoxItem.ListBox:TD2ListBox;
var
  P:TD2Object;
begin
  P:=Parent;
  while (P <> nil) do
  begin
    if P is TD2ListBox then
    begin
      Result:=TD2ListBox(P);
      Exit;
    end;
    if P is TD2ComboBox then
    begin
      Result:=TD2ComboBox(P).FListBox;
      Exit;
    end;
    P:=P.Parent;
  end;
  Result:=nil;
end;

procedure TD2ListBoxItem.DesignSelect;
begin
  inherited;
  if (ComboBox <> nil) then
  begin
    if ComboBox.FPopup.DesignHide then
      ComboBox.DesignClick;
  end;
end;

procedure TD2ListBoxItem.DesignInsert;
begin
  inherited;
  if (ComboBox <> nil) then
  begin
    if ComboBox.FPopup.DesignHide then
      ComboBox.DesignClick;
  end;
end;

procedure TD2ListBoxItem.Paint;
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

function TD2ListBoxItem.GetParentComponent:TComponent;
begin
  if (ComboBox <> nil) then
    Result:=ComboBox
  else
  if (ListBox <> nil) then
    Result:=ListBox
  else
    Result:=inherited GetParentComponent;
end;

function TD2ListBoxItem.EnterFocusChildren(AObject:TD2VisualObject):boolean;
begin
  Result:=inherited EnterFocusChildren(AObject);
  if (ListBox <> nil) then
  begin
    if ListBox.MultiSelect then
      ListBox.ClearSelection;
    ListBox.ItemIndex:=Index;
    Result:=true;
  end;
end;

procedure TD2ListBoxItem.UpdateCheck;
var
  i:integer;
begin
  if (ListBox <> nil) and (FCheck <> nil) then
    FCheck.Visible:=ListBox.ShowCheckboxes;
  if ChildrenCount > 0 then
    for i:=0 to ChildrenCount - 1 do
      if Children[i] is TD2ListboxItem then
        TD2ListboxItem(Children[i]).UpdateCheck;
end;

procedure TD2ListBoxItem.SetIsChecked(const Value:boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    if FCheck <> nil then
      FCheck.IsChecked:=FIsChecked;
  end;
end;

procedure TD2ListBoxItem.SetIsSelected(const Value:boolean);
begin
  if FIsSelected <> Value then
  begin
    FIsSelected:=Value;
    StartTriggerAnimation(Self,'IsSelected');
    // Show the trigger effect on the listbox item:                     *** DB ***
    ApplyTriggerEffect(Self,'IsSelected');                            // *** DB ***
    if FIsSelected and (ListBox <> nil) and not (Listbox.MultiSelect) then
      Listbox.ItemIndex:=Index
    else
    if not FIsSelected and (ListBox <> nil) and not (Listbox.MultiSelect) and (Listbox.ItemIndex=Index) then
      Listbox.ItemIndex:=-1
    else
      if Listbox <> nil then Listbox.UpdateSelection;
  end;
end;

procedure TD2ListBoxItem.DragEnd;
begin
  inherited;
  DragLeave;
  if (ListBox <> nil) then
    ListBox.FDragItem:=nil;
end;

//=============== TD2ListBox ==========================================================

constructor TD2ListBox.Create(AOwner:TComponent);
begin
  inherited;
  FOddFill:=TD2Brush.Create(d2BrushSolid,$20000000);
  FColumns:=1;
  FHideSelectionUnfocused:=true;
  FContent.DisableDefaultAlign:=true;
  FItemIndex:=-1;
  CanFocused:=true;
  AutoCapture:=true;
  Width:=100;
  Height:=100;
end;

destructor TD2ListBox.Destroy;
begin
  if FSelections <> nil then
    FSelections.Free;
  FOddFill.Free;
  inherited;
end;

function TD2ListBox.ItemClass:string;
begin
  Result:='TD2ListBoxItem';
end;

procedure TD2ListBox.HScrollChange(Sender:TObject);
begin
  inherited;
  UpdateSelection;
end;

procedure TD2ListBox.VScrollChange(Sender:TObject);
begin
  inherited;
  UpdateSelection;
end;

function CompareListItem(item1,item2:TD2Object):integer;
begin
  if (item1 is TD2ListBoxItem) and (item2 is TD2ListBoxItem) then
  begin
    if (TD2ListBoxItem(item1).ListBox <> nil) and Assigned(TD2ListBoxItem(item1).ListBox.OnCompare) then
      Result:=TD2ListBoxItem(item1).ListBox.OnCompare(TD2ListBoxItem(item1),TD2ListBoxItem(item2))
    else
      {$IFDEF KS_COMPILER5}
      Result:=CompareText(TD2ListBoxItem(item1).TextW,TD2ListBoxItem(item2).TextW);
      {$ELSE}
      Result:=WideCompareText(TD2ListBoxItem(item1).TextW,TD2ListBoxItem(item2).TextW);
      {$ENDIF}
  end
  else
    Result:=0;
end;

procedure TD2ListBox.SortItems;
begin
  if not FSorted then Exit;
  FContent.Sort(CompareListItem);
end;

procedure TD2ListBox.DoContentPaint(Sender:TObject; const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  i:integer;
  Item:TD2ListBoxItem;
  P:TD2Point;
  R:TD2Rect;
begin
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    if FAlternatingRowBackground then
    begin
      Canvas.Fill.Assign(FOddFill);
//    The following may leave the last column unpainted // *** DB ***
//    for i:=0 to Count div Columns do                // *** DB ***
      for i:=0 to (Count - 1) div Columns do          // *** DB ***
//                         Making things simpler below  // *** DB ***
      begin
        if Odd(i) then
        begin
//        if i * Columns > Count - 1 then               // *** DB ***
//          Item:=ItemByIndex(Count - 1)              // *** DB ***
//        else                                          // *** DB ***
            Item:=ItemByIndex(i * Columns);
          P:=Item.LocalToAbsolute(d2Point(0,0));
          P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
          R:=d2Rect(P.X,P.Y,P.X + ContentLayout.Width,P.Y + Item.Height);
          if not d2IntersectRect(R,ARect) then Continue;
          Canvas.FillRect(R,0,0,[],AbsoluteOpacity);
        end;
      end;
    end;
  end;
end;

procedure TD2ListBox.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('content');
  if (T <> nil) and (T.IsVisual) then
  begin
    TD2VisualObject(T).OnBeforePaint:=DoContentPaint;
  end;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FSelection:=TD2VisualObject(T);
    FSelection.Visible:=false;
    UpdateSelection;
  end;
  T:=FindResource('AlternatingRowBackground');
  if (T <> nil) and (T is TD2BrushObject) then
  begin
    FOddFill.Assign(TD2BrushObject(T).Brush);
  end;
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    TD2VisualObject(T).Visible:=false;
  end;
end;

procedure TD2ListBox.FreeStyle;
begin
  inherited;
  FSelection:=nil;
  if FSelections <> nil then
    FSelections.Clear;
end;

procedure TD2ListBox.UpdateSelection;
var
  i:integer;
  P:TD2Point;
  R:TD2Rect;
  //Sel:boolean;
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
  Vis:boolean;
  Item:TD2ListboxItem;
begin
  if FSelection=nil then Exit;
  // calc rects
  Vis:=true;
  //Sel:=false;
  SetLength(SelRects,0);
  for i:=0 to Count - 1 do
  begin
    Item:=Items[i];
    if (Item.IsSelected) and d2IntersectRect(Item.UpdateRect,UpdateRect) then
    begin
      P:=Item.LocalToAbsolute(d2Point(0,0));
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
        P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
      R:=d2Rect(P.X,P.Y,P.X + Item.Width,P.Y + Item.Height);
      if (Length(SelRects) > 0) and (i > 0) and (Items[i - 1].IsSelected) then
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
      Clone.ResourceName:='';
      FSelections.Add(Clone);
      Clone.Parent:=FSelection.Parent;
      Clone.Stored:=false;
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

procedure TD2ListBox.ContentAddObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2ListBoxItem then
    if FUpdating=0 then
      Realign;
end;

procedure TD2ListBox.ContentBeforeRemoveObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2ListBoxItem then
  begin
    if FItemIndex > TD2ListBoxItem(AObject).Index then
    begin
      Dec(FItemIndex);
      UpdateSelection;
    end;
    TD2ListBoxItem(AObject).IsSelected:=false;
  end;
end;

procedure TD2ListBox.ContentRemoveObject(AObject:TD2Object);
begin
  inherited;
  if AObject is TD2ListBoxItem then
  begin
    if FUpdating=0 then
      Realign;
  end;
end;

function TD2ListBox.GetContentBounds:TD2Rect;
var
  R:TD2Rect;
  i,j,Idx:integer;
  RowHeight,ColWidth,CurY:single;
begin
  Result:=LocalRect;
  if FUpdating > 0 then Exit;
  if ContentLayout=nil then Exit;
  R:=ContentLayout.LocalRect;
  { FContent }
  if FContent <> nil then
  begin
    { Sort if need }
    SortItems;
    { Set Selection }
    if not MultiSelect and (Selected <> nil) then
      Selected.IsSelected:=true;
    { Align }
    case FListStyle of
      d2ListVertical:
        begin
          { correct items size }
          if FItemWidth <> 0 then
          begin
            FColumns:=trunc((R.Right - R.Left) / FItemWidth);
            if FColumns < 1 then FColumns:=1;
            if FContent.ChildrenCount > 0 then
              for i:=0 to (FContent.ChildrenCount - 1) do
                with TD2ListBoxItem(FContent.Children[i]) do
                begin
                  if FItemHeight <> 0 then
                    SetBounds(Position.X,Position.Y,FItemWidth,FItemHeight)
                  else
                    SetBounds(Position.X,Position.Y,FItemWidth,Height);
                end;
          end;
          if (FItemWidth=0) and (FItemHeight <> 0) then
          begin
            if FContent.ChildrenCount > 0 then
              for i:=0 to (FContent.ChildrenCount - 1) do
                if FContent.Children[i] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[i]) do
                  begin
                    SetBounds(Position.X,Position.Y,Width,FItemHeight)
                  end;
          end;
          { calc items size }
          CurY:=0;
          if FContent.ChildrenCount > 0 then
            for i:=0 to (FContent.ChildrenCount - 1) div FColumns do
            begin
              RowHeight:=0;
              for j:=0 to FColumns - 1 do
              begin
                if (i * FColumns) + j > FContent.ChildrenCount - 1 then Continue;
                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    if Height + Padding.Top + Padding.Bottom > RowHeight then
                      RowHeight:=Height + Padding.Top + Padding.Bottom;
                  end;
              end;
              // set correct height
              for j:=0 to FColumns - 1 do
              begin
                if (i * FColumns) + j > FContent.ChildrenCount - 1 then Continue;
                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    Height:=RowHeight - Padding.Top - Padding.Bottom;
                  end;
              end;
              CurY:=CurY + RowHeight;
            end;
          FContent.Height:=CurY;
          { align }
          CurY:=0;
          Idx:=0;
          if FContent.ChildrenCount > 0 then
            for i:=0 to (FContent.ChildrenCount - 1) div FColumns do
            begin
              RowHeight:=0;
              for j:=0 to FColumns - 1 do
              begin
                if (i * FColumns) + j > FContent.ChildrenCount - 1 then Continue;

                if FItemWidth <> 0 then
                  ColWidth:=FItemWidth
                else
                  ColWidth:=(R.Right - R.Left) / FColumns;

                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    SetBounds(Padding.Left + (j * ColWidth),CurY + Padding.Top,
                      ColWidth - Padding.Left - Padding.Right,Height);

                    if Height + Padding.Top + Padding.Bottom > RowHeight then
                      RowHeight:=Height + Padding.Top + Padding.Bottom;
                    Inc(Idx);
                  end;
              end;
              CurY:=CurY + RowHeight;
            end;
          if CurY > 0 then
            R.Bottom:=R.Top + CurY;
          if FItemWidth <> 0 then
            R.Right:=R.Left + (FItemWidth * FColumns);
      end;
      d2ListHorizontal:
        begin
          { correct items size }
          if FItemHeight <> 0 then
          begin
//          Remove the Padding.Top and Padding.Bottom,                                 *** DB ***
//          ... this has been taken into account with the ContentLayout.LocalRect       *** DB ***
//          FColumns:=trunc((R.Bottom - R.Top - Padding.Top - Padding.Bottom) / FItemHeight);
//          FColumns:=trunc((R.Bottom - R.Top) / FItemHeight);
            if FColumns < 1 then FColumns:=1;
            if FContent.ChildrenCount > 0 then
              for i:=0 to (FContent.ChildrenCount - 1) do
                with TD2ListBoxItem(FContent.Children[i]) do
                begin
                  if FItemWidth <> 0 then
                    SetBounds(Position.X,Position.Y,FItemWidth,FItemHeight)
                  else
                    SetBounds(Position.X,Position.Y,Width,FItemHeight);
                end;
          end;
          if (FItemHeight=0) and (FItemWidth <> 0) then
          begin
            if FContent.ChildrenCount > 0 then
              for i:=0 to (FContent.ChildrenCount - 1) do
                with TD2ListBoxItem(FContent.Children[i]) do
                begin
                  SetBounds(Position.X,Position.Y,FItemWidth,Height)
                end;
          end;
          { calc items size }
          CurY:=0;
          if FContent.ChildrenCount > 0 then
            for i:=0 to (FContent.ChildrenCount - 1) div FColumns do
            begin
              ColWidth:=0;
              if FItemHeight <> 0 then
                RowHeight:=FItemHeight
              else
                RowHeight:=(R.Bottom - R.Top) / FColumns;
              for j:=0 to FColumns - 1 do
                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    if ColWidth < Width + Padding.Left + Padding.Right then
                      ColWidth:=Width + Padding.Left + Padding.Right;
                  end;
              // calc width
              for j:=0 to FColumns - 1 do
                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    Width:=ColWidth - (Padding.Left + Padding.Right);
                  end;

              CurY:=CurY + ColWidth;
            end;
          { selection }
          if FItemIndex > Count - 1 then
            FItemIndex:=Count - 1;
          { align }
          CurY:=0;
          Idx:=0;
          if FContent.ChildrenCount > 0 then
            for i:=0 to (FContent.ChildrenCount - 1) div FColumns do
            begin
              ColWidth:=0;
              if FItemHeight <> 0 then
                RowHeight:=FItemHeight
              else
                RowHeight:=(R.Bottom - R.Top) / FColumns;
              for j:=0 to FColumns - 1 do
                if FContent.Children[(i * FColumns) + j] is TD2ListBoxItem then
                  with TD2ListBoxItem(FContent.Children[(i * FColumns) + j]) do
                  begin
                    if VScrollBar <> nil then
                      SetBounds(CurY + Padding.Left - VScrollBar.Value,Padding.Top + (j * RowHeight),
                        Width,RowHeight - Padding.Top - Padding.Bottom)
                    else
                      SetBounds(CurY + Padding.Left,Padding.Top + (j * RowHeight),
                        Width,RowHeight - Padding.Top - Padding.Bottom);
                    if ColWidth < Width + Padding.Left + Padding.Right then
                      ColWidth:=Width + Padding.Left + Padding.Right;
                    Inc(Idx);
                  end;
              CurY:=CurY + ColWidth;
            end;
          if CurY > 0 then
            R.Right:=R.Left + CurY;
          if FItemHeight <> 0 then
            R.Bottom:=R.Top + (FItemHeight * FColumns);
        end;
    end;
  end;
  UpdateSelection;
  Result:=R;
end;

function TD2ListBox.GetCount:integer;
begin
  Result:=0;
  if (FContent <> nil) then
    Result:=FContent.ChildrenCount;
end;

function TD2ListBox.ItemByIndex(const Idx:integer):TD2ListBoxItem;
begin
  if (FContent <> nil) and (FContent.ChildrenCount > 0) and (Idx >= 0) and (Idx < FContent.ChildrenCount) then
    Result:=TD2ListBoxItem(FContent.Children[Idx])
  else
    Result:=nil;
end;

function TD2ListBox.ItemByPoint(const X,Y:single):TD2ListBoxItem;
var
  i:integer;
  P:TD2Point;
begin
  P:=LocaltoAbsolute(d2Point(X,Y));
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i:=0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2ListBoxItem then
        if TD2ListBoxItem(FContent.Children[i]).pointInObject(P.X,P.Y) then
        begin
          Result:=TD2ListBoxItem(FContent.Children[i]);
          Exit;
        end;
  Result:=nil;
end;

procedure TD2ListBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
var
  i:integer;
begin
  inherited ;
  if Count > 0 then
  begin
    if KeyChar <> #0 then
    begin
      for i:=0 to Count - 1 do
        if (Items[i].TextW <> '') and (WideLowerCase(Items[i].TextW[1])=WideLowerCase(KeyChar)) then
        begin
          ItemIndex:=i;
          Break;
        end;
      KeyChar:=#0;
    end;
    case Key of
      VK_HOME:ItemIndex:=0;
      VK_END:ItemIndex:=Count - FColumns;
      VK_UP:If ItemIndex > 0 then
        begin
          ItemIndex:=ItemIndex - FColumns;
          if ItemIndex < 0 then ItemIndex:=0;
        end;
      VK_DOWN:
        begin
          If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + FColumns;
          if ItemIndex > Count - 1 then ItemIndex:=Count - 1;
        end;
      VK_LEFT:If ItemIndex > 0 then ItemIndex:=ItemIndex - 1;
      VK_RIGHT:If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + 1;
    else
      Exit;
    end;
    Key:=0;
  end;
end;

procedure TD2ListBox.KeyUp(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
end;

procedure TD2ListBox.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  Item:TD2ListBoxItem;
begin
  inherited;
  if (Button=mbLeft) and not MouseTracking then
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
          ItemIndex:=Item.Index;
        end
        else
        begin
          SelectRange(Item,Item);
          ItemIndex:=Item.Index;
        end;
        FFirstSelect:=Item;
      end
      else
      begin
        if ItemIndex <> Item.Index then
          ItemIndex:=Item.Index
        else
          if AllowDrag then
            Scene.BeginVCLDrag(Selected,MakeScreenshot);
      end;
      if Assigned(Item.OnClick) then
        Item.OnClick(Item)
    end;
    FMouseSelecting:=true;
  end;
end;

procedure TD2ListBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  Item:TD2ListBoxItem;
begin
  inherited;
  if (ssLeft in Shift) and FMouseSelecting then
  begin
    Item:=ItemByPoint(X,Y);
    if Item <> nil then
    begin
      if Selected=Item then Exit;
      if MultiSelect then
      begin
        if ssCtrl in Shift then
          Item.IsSelected:=not Item.IsSelected
        else
          SelectRange(FFirstSelect,Item);
        ItemIndex:=Item.Index;
      end
      else
        ItemIndex:=Item.Index;
    end;
  end;
end;

procedure TD2ListBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  Item:TD2ListBoxItem;
begin
  inherited;
  FFirstSelect:=nil;
  if MouseTracking and (FLastDelta.X=0) and (FLastDelta.Y=0) then
  begin
    Item:=ItemByPoint(X,Y);
    if Item <> nil then
      ItemIndex:=Item.Index;
  end;
  FMouseSelecting:=false;
end;

function TD2ListBox.GetSelected:TD2ListBoxItem;
begin
  Result:=ItemByIndex(FItemIndex);
end;

procedure TD2ListBox.SetItemIndex(const Value:integer);
var
  Item:TD2ListBoxItem;
begin
  if FItemIndex <> Value then
  begin
    Item:=ItemByIndex(ItemIndex);
    if (Item <> nil) and (not MultiSelect) then
      Item.IsSelected:=false;

    FItemIndex:=Value;
    if (ItemByIndex(FItemIndex) <> nil) and (FContent <> nil) and (VScrollBar <> nil) and (ContentLayout <> nil) then
    begin
      Item:=ItemByIndex(FItemIndex);
      if FContent.Position.Y + Item.Position.Y + Item.Padding.Top + Item.Padding.Bottom + Item.Height > ContentLayout.Position.Y + ContentLayout.Height then
       VScrollBar.Value:=VScrollBar.Value + (FContent.Position.Y + Item.Position.Y + Item.Padding.Top + Item.Padding.Bottom + Item.Height - ContentLayout.Position.Y - ContentLayout.Height);
      if FContent.Position.Y + Item.Position.Y < ContentLayout.Position.Y then
        VScrollBar.Value:=VScrollBar.Value + FContent.Position.Y + Item.Position.Y - ContentLayout.Position.Y;
    end;
    if (ItemByIndex(FItemIndex) <> nil) and (FContent <> nil) and (HScrollBar <> nil) and (ContentLayout <> nil) then
    begin
      Item:=ItemByIndex(FItemIndex);
      if FContent.Position.X + Item.Position.X + Item.Padding.Left + Item.Padding.Right + Item.Width > ContentLayout.Position.X + ContentLayout.Width then
        HScrollBar.Value:=HScrollBar.Value + (FContent.Position.X + Item.Position.X + Item.Padding.Left + Item.Padding.Right + Item.Width - ContentLayout.Position.X - ContentLayout.Width);
      if FContent.Position.X + Item.Position.X < 0 then
        HScrollBar.Value:=HScrollBar.Value + FContent.Position.X + Item.Position.X - ContentLayout.Position.X;
    end;
    if (ItemIndex >= 0) then
    begin
      Item:=ItemByIndex(ItemIndex);
      if Item <> nil then
        Item.IsSelected:=true;
    end;
    if (FUpdating=0) then
    begin
      if Assigned(FBindingObjects) then
        ToBindingObjects;
      if Assigned(FOnChange) then
        FOnChange(ItemByIndex(FItemIndex));
    end;
    UpdateSelection;
  end;
end;

procedure TD2ListBox.Clear;
var
  i:integer;
begin
  BeginUpdate;
  if FContent <> nil then
    if FContent.ChildrenCount > 0 then
      for i:=FContent.ChildrenCount - 1 downto 0 do
        if FContent.Children[i] is TD2ListboxItem then
          TD2Object(FContent.Children[i]).Free;
  FScrollDesign.Y:=0;
  FScrollDesign.X:=0;
  EndUpdate;
end;

procedure TD2ListBox.SelectRange(Item1,Item2:TD2ListBoxItem);
var
  i:integer;
begin
  if Item1=nil then Exit;
  if Item2=nil then Exit;
  for i:=0 to Min(Item1.Index,Item2.Index) - 1 do
    ItemByIndex(i).IsSelected:=false;
  for i:=Max(Item1.Index,Item2.Index) + 1 to Count - 1 do
    ItemByIndex(i).IsSelected:=false;
  for i:=Min(Item1.Index,Item2.Index) to Max(Item1.Index,Item2.Index) do
    ItemByIndex(i).IsSelected:=true;
end;

procedure TD2ListBox.ClearSelection;
var
  i:integer;
begin
  for i:=0 to Count - 1 do
    ItemByIndex(i).IsSelected:=false;
end;

procedure TD2ListBox.SelectAll;
var
  i:integer;
begin
  for i:=0 to Count - 1 do
    ItemByIndex(i).IsSelected:=true;
end;

function TD2ListBox.GetData:Variant;
begin
  if Selected <> nil then
    Result:=Selected.TextW
  else
    Result:='';
end;

procedure TD2ListBox.SetData(const Value:Variant);
begin
  if Selected <> nil then
    Selected.TextW:=Value;
end;

procedure TD2ListBox.EnterFocus;
begin
  inherited ;
  if HideSelectionUnfocused and (Selected <> nil) then
    UpdateSelection;
end;

procedure TD2ListBox.KillFocus;
begin
  inherited;
  if HideSelectionUnfocused and (Selected <> nil) then
    UpdateSelection;
end;

procedure TD2ListBox.AddObject(AObject:TD2Object);
begin
  if (FContent <> nil) and ((AObject is TD2ListBoxItem) or (AObject is TD2SelectionItem)) then
  begin
    FContent.AddObject(AObject);
  end
  else
    inherited;
end;

procedure TD2ListBox.SetColumns(const Value:integer);
begin
  if FColumns <> Value then
  begin
    FColumns:=Value;
    if FColumns < 1 then
      FColumns:=1;
    Realign;
  end;
end;

procedure TD2ListBox.SetAlternatingRowBackground(const Value:boolean);
begin
  if FAlternatingRowBackground <> Value then
  begin
    FAlternatingRowBackground:=Value;
    Repaint;
  end;
end;

procedure TD2ListBox.SetMultiSelect(const Value:boolean);
begin
  if FMultiSelect <> Value then
  begin
    FMultiSelect:=Value;
    if not FMultiSelect then
      ClearSelection;
  end;
end;

procedure TD2ListBox.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    Realign;
  end;
end;

procedure TD2ListBox.SetItemWidth(const Value:single);
begin
  if FItemWidth <> Value then
  begin
    FItemWidth:=Value;
    Realign;
  end;
end;

procedure TD2ListBox.SetListStyle(const Value:TD2ListStyle);
begin
  if FListStyle <> Value then
  begin
    FListStyle:=Value;
    Realign;
  end;
end;

procedure TD2ListBox.SetShowCheckboxes(const Value:boolean);
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

function TD2ListBox.GetItem(Index:integer):TD2ListBoxItem;
begin
  Result:=ItemByIndex(Index);
end;

procedure TD2ListBox.SetSorted(const Value:boolean);
begin
  if FSorted <> Value then
  begin
    FSorted:=Value;
    Realign;
  end;
end;

procedure TD2ListBox.SetAllowDrag(const Value:boolean);
begin
  if FAllowDrag <> Value then
  begin
    FAllowDrag:=Value;
    if FAllowDrag then
      DragDisableHighlight:=true;
  end;
end;



//=============== TD2StringListBox ===================================================

procedure TD2StringListBox.ApplyStyle;
begin
  inherited;
end;

constructor TD2StringListBox.Create(AOwner:TComponent);
begin
  inherited;
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  FAutoTranslate:=true;
  FItems:=TD2WideStringList.Create;
  TD2WideStringList(FItems).OnChange:=DoItemsChanged;
  FItemHeight:=19;
  FNeedResource:=true;
  FResource:='listboxstyle';
  FItemStyle:='listboxitemstyle';
end;

destructor TD2StringListBox.Destroy;
begin
  FreeAndNil(FItems);
  FreeAndNil(FFont);
  inherited;
end;

procedure TD2StringListBox.EndUpdate;
begin
  FUpdating:=FUpdating + 1;
  inherited EndUpdate;
  FUpdating:=FUpdating - 1;
  if FUpdating=0 then
    RebuildList;
end;

procedure TD2StringListBox.DoItemsChanged(Sender:TObject);
begin
  RebuildList;
end;

function TD2StringListBox.GetData:Variant;
begin
  if (ItemIndex >= 0) and (ItemIndex < FItems.Count) then
    Result:=FItems[ItemIndex]
  else
    Result:='';
end;

procedure TD2StringListBox.SetData(const Value:Variant);
begin
  if (ItemIndex >= 0) and (ItemIndex < FItems.Count) then
    FItems[ItemIndex]:=Value;
end;

procedure TD2StringListBox.RebuildList;
var
  i,SaveIndex:integer;
  SaveScrollDesign:TD2Point;
  Item:TD2ListBoxItem;
begin
  if FUpdating > 0 then Exit;
  if csDestroying in ComponentState then Exit;
  if Items=nil then Exit;

  BeginUpdate;
  SaveScrollDesign:=FScrollDesign;
  SaveIndex:=ItemIndex;
  Clear;
  for i:=0 to FItems.Count - 1 do
  begin
    Item:=TD2ListBoxItem.Create(Self);
    Item.AutoTranslate:=FAutoTranslate;
    Item.Parent:=Self;
    Item.Height:=FItemHeight;
    Item.Stored:=false;
    Item.Locked:=true;
    Item.TextW:=FItems[i];
    Item.Font:=FFont;
    Item.FResource:=FItemStyle;
    Item.TextAlign:=TextAlign;
  end;
  FItemIndex:=SaveIndex;
  FScrollDesign:=SaveScrollDesign;
  inherited EndUpdate;
end;

procedure TD2StringListBox.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

procedure TD2StringListBox.SetTextAlign(const Value:TD2TextAlign);
begin
  if FTextAlign <> Value then
  begin
    FTextAlign:=Value;
    if not (csLoading in ComponentState) then
      RebuildList;
  end;
end;

procedure TD2StringListBox.SetFont(const Value:TD2Font);
begin
  FFont.Assign(Value);
end;

procedure TD2StringListBox.FontChanged(Sender:TObject);
var
  i:integer;
begin
  for i:=0 to Count - 1 do
    inherited Items[i].Font.Assign(FFont);
end;

procedure TD2StringListBox.SortItems;
begin
//TD2WideStringList(FItems).Sort;                                  // *** DB ***
  if FSorted then TD2WideStringList(FItems).Sort;                  // *** DB ***
end;



//=============== TD2HorzListBox =========================================

constructor TD2HorzListBox.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='listboxstyle';
  FNeedResource:=true;
  FListStyle:=d2ListHorizontal;
end;

//=============== TD2HudStringListBox ===================================

constructor TD2HudStringListBox.Create(AOwner:TComponent);
begin
  inherited;
  FNeedResource:=true;
  FResource:='hudlistboxstyle';
  FItemStyle:='hudlistboxitemstyle';
end;

//=============== TD2HudListBox ========================================

constructor TD2HudListBox.Create(AOwner:TComponent);
begin
  inherited;
end;

//=============== TD2HudHorzListBox ====================================

constructor TD2HudHorzListBox.Create(AOwner:TComponent);
begin
  inherited;
  FNeedResource:=true;
  FResource:='hudlistboxstyle';
end;



//=============== TD2ImageListBoxItem =====================================

procedure TD2ImageListBoxItem.ApplyStyle;
begin
  inherited;
  if (ListBox is TD2ImageListBox) then
  begin
    if (TextBorder <> nil) then
      TextBorder.Visible:=TD2ImageListBox(ListBox).ShowFileName;
    if (Text <> nil) then
      Text.TextW:=ExtractFileName(TagString);
  end;
  if (ListBox is TD2HorzImageListBox) then
  begin
    if (TextBorder <> nil) then
      TextBorder.Visible:=TD2HorzImageListBox(ListBox).ShowFileName;
    if (Text <> nil) then
      Text.TextW:=ExtractFileName(TagString);
  end;
end;

function TD2ImageListBoxItem.Text:TD2Text;
begin
  Result:=TD2Text(FindResource('text'));
end;

function TD2ImageListBoxItem.TextBorder:TD2VisualObject;
begin
  Result:=TD2VisualObject(FindResource('textborder'));
end;

//=============== TD2ImageListBox ========================================

constructor TD2ImageListBox.Create(AOwner:TComponent);
begin
  inherited;
  FUseThumbnails:=true;
  FItemHeight:=Trunc(Width);
  if Self is TD2HudImageListBox then
    FResource:='HudListBoxStyle'
  else
    FResource:='listboxstyle';
end;

procedure TD2ImageListBox.AddBitmap(const AFile:string;
  const ABitmap:TD2Bitmap);
var
  R:TD2Rect;
  Item:TD2ListBoxItem;
  Thumb:TD2Image;
begin
  // Create ListboxItem
  Item:=TD2ImageListBoxItem.Create(Self);
  Item.Parent:=Self;
  Item.Height:=FItemHeight;
  Item.Stored:=false;
  Thumb:=TD2Image.Create(Item);
  Thumb.Parent:=Item;
  if ShowFileName then
    Thumb.Padding.Rect:=d2Rect(4,4,4,20)
  else
    Thumb.Padding.Rect:=d2Rect(4,4,4,4);
  Thumb.Align:=vaClient;
  Thumb.Stored:=false;
  Thumb.Locked:=true;
  Thumb.HitTest:=false;
  // create thumb
  if FUseThumbnails then
  begin
    R:=d2Rect(0,0,ABitmap.Width,ABitmap.Height);
    d2FitRect(R,d2Rect(0,0,Height,Height));
    Thumb.Bitmap.SetSize(round(d2RectWidth(R)),round(d2RectHeight(R)));
    if Thumb.Bitmap.Canvas.BeginScene then
    begin
      Thumb.Bitmap.Canvas.DrawBitmap(ABitmap,d2Rect(0,0,ABitmap.Width,ABitmap.Height),d2Rect(0,0,Thumb.Bitmap.Width,Thumb.Bitmap.Height),1);
      Thumb.Bitmap.Canvas.EndScene;
    end;
  end
  else
    Thumb.Bitmap.Assign(ABitmap);
  // Use Tag property as image loading state 1 - unload 0 - loaded
  Item.Tag:=0;
  // Set TagString property to image FileName
  Item.TagString:=AFile;
  // Set the Item.TagObject as Thumb for later access
  Item.TagObject:=Thumb;
  Realign;
  if Count=1 then
    ItemIndex:=0;
end;

procedure TD2ImageListBox.AddFile(const AFile:string);
var
  {Dir,}Ext:string;
  //SR:TSearchRec;
  Item:TD2ListBoxItem;
  Thumb:TD2Image;
begin
  Ext:=LowerCase(ExtractFileExt(AFile));
  if Pos(Ext,GvarD2DefaultFilterClass.GetFileTypes) > 0 then
  begin
    // Create ListboxItem
    Item:=TD2ImageListBoxItem.Create(Self);
    Item.Parent:=Self;
    Item.Height:=FItemHeight;
    Item.Stored:=false;
    Item.OnApplyResource:=DoApplyResource;
    Thumb:=TD2Image.Create(Item);
    Thumb.Parent:=Item;
    if ShowFileName then
      Thumb.Padding.Rect:=d2Rect(4,4,4,20)
    else
      Thumb.Padding.Rect:=d2Rect(4,4,4,4);
    Thumb.Align:=vaClient;
    Thumb.Stored:=false;
    Thumb.Locked:=true;
    Thumb.HitTest:=false;
    // Use Tag property as image loading state 1 - unload 0 - loaded
    Item.Tag:=1;
    // Set TagString property to image FileName
    Item.TagString:=AFile;
    // Set the Item.TagObject as Thumb for later access
    Item.TagObject:=Thumb;
  end;
  if Count=1 then
    ItemIndex:=0;
end;

procedure TD2ImageListBox.AddFolder(const Folder:string);
var
  Dir:string;
  SR:TSearchRec;
begin
  { add folder }
  { add SelectDialog }
  Dir:=Folder;
  {$IFDEF UNIX}
  if (Length(Dir) > 0) and (Dir[Length(Dir)] <> '/') then Dir:=Dir + '/';
  {$ELSE}
  if (Length(Dir) > 0) and (Dir[Length(Dir)] <> '\') then Dir:=Dir + '\';
  {$ENDIF}
  BeginUpdate;
  if FindFirst(Dir + '*.*',$FFFF,SR)=0 then
  begin
    repeat
      if SR.Name='.' then Continue;
      if SR.Name='..' then Continue;
      if SR.Attr and faDirectory=faDirectory then Continue;
      AddFile(Dir + SR.Name);
    until FindNext(SR) <> 0;
    FindClose(SR);
  end;
  EndUpdate;
  if Count > 0 then
    ItemIndex:=0;
end;

procedure TD2ImageListBox.DoApplyResource(Sender:TObject);
begin
  if (TD2ImageListBoxItem(Sender).Tag=1) and (TD2ImageListBoxItem(Sender).TagString <> '') then
  begin
    // Create ImageThread
    with TD2ImageThread.Create(TD2Image(TD2ImageListBoxItem(Sender).TagObject),TD2ImageListBoxItem(Sender).TagString,FUseThumbnails) do
    begin
      // Clear Tag state
      TD2ImageListBoxItem(Sender).Tag:=0;
      // Resume thread
      Resume;
    end;
  end;
end;

function TD2ImageListBox.GetImage(Index:integer):TD2Image;
var
  Item:TD2ListBoxItem;
begin
  Item:=ItemByIndex(Index);
  if (Item <> nil) and (Item.TagObject <> nil) and (Item.TagObject is TD2Image) then
    Result:=TD2Image(Item.TagObject)
  else
    Result:=nil;
end;

function TD2ImageListBox.GetSelectedFileName:string;
begin
  if (Selected <> nil) then
    Result:=Selected.TagString
  else
    Result:='';
end;

function TD2ImageListBox.GetSelectedImage:TD2Image;
begin
  if (Selected <> nil) and (Selected.TagObject <> nil) and (Selected.TagObject is TD2Image) then
    Result:=TD2Image(Selected.TagObject)
  else
    Result:=nil;
end;

procedure TD2ImageListBox.SetShowFileName(const Value:boolean);
begin
  if FShowFileName <> Value then
  begin
    FShowFileName:=Value;
  end;
end;

procedure TD2ImageListBox.SetItemHeight(const Value:single);
var
  i:integer;
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    FDisableAlign:=true;
    try
      for i:=0 to Count - 1 do
        ItemByIndex(i).Height:=ItemHeight;
    finally
      FDisableAlign:=false;
      Realign;
    end;
  end;
end;

procedure TD2ImageListBox.Clear;
var
  i:integer;
begin
  for i:=0 to Count - 1 do
    ItemByIndex(i).Tag:=0;
  inherited;
end;

procedure TD2ImageListBox.BeginAutoDrag;
begin
  if SelectedImage <> nil then
    FScene.BeginVCLDrag(SelectedImage,SelectedImage.Bitmap);
end;

//=============== TD2HorzImageListBox ============================================

constructor TD2HorzImageListBox.Create(AOwner:TComponent);
begin
  inherited;
  FItemHeight:=0;
  FItemWidth:=Trunc(Height + 10);
  FListStyle:=d2ListHorizontal;
  if Self is TD2HudHorzImageListBox then
    FResource:='HudListBoxStyle'
  else
    FResource:='ListBoxStyle';
end;

procedure TD2ListBox.DragDrop(const Data:TD2DragObject;
  const Point:TD2Point);
var
  Obj:TD2ListBoxItem;
  Allow:boolean;
begin
  inherited;
  if FDragItem <> nil then
  begin
    FDragItem.DragLeave;
    FDragItem:=nil;
  end;
  with AbsoluteToLocal(Point) do
    Obj:=ItemByPoint(X,Y);
  if Obj <> nil then
  begin
    Allow:=true;
    if Assigned(OnDragChange) then
      OnDragChange(TD2ListBoxItem(Data.Source),Obj,Allow);
    if Allow then
    begin
      Exchange(TD2ListBoxItem(Data.Source),Obj);
    end;
  end;
end;

procedure TD2ListBox.DragOver(const Data:TD2DragObject; Shift: TShiftState;
  const Point:TD2Point; var Accept:boolean);
var
  Obj:TD2ListBoxItem;
begin
  inherited;
  with AbsoluteToLocal(Point) do
    Obj:=ItemByPoint(X,Y);
  if (Obj <> FDragItem) then
  begin
    if FDragItem <> nil then
      FDragItem.DragLeave;
    FDragItem:=Obj;
    if FDragItem <> nil then
    begin
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

procedure TD2ListBox.Exchange(Item1,Item2:TD2ListBoxItem);
begin
  if Item1.Index=FItemIndex then
    FItemIndex:=Item2.Index
  else
    if Item2.Index=FItemIndex then
      FItemIndex:=Item1.Index;
  FContent.Exchange(Item1,Item2);
end;
          
      
          
                   
