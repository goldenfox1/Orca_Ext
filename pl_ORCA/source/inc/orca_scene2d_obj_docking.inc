{%mainunit ../orca_scene2d.pas}

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

 { TD2DockingTab }

procedure TD2DockingTab.ApplyStyle;
  var O: TD2Object;
begin
  inherited;

  if (FPanel <> nil) and
     (FPanel.IsMouseOverChildren or IsMouseOver)
    then ShowPanel(true)
    else if FTimer <> nil
           then FTimer.Enabled:=true;

    O := FindResource('text');
    if (O <> nil) and (O is TD2Text) then
      begin
        FTextControl := TD2Text(O);
        if FAutoWidth then DoAutoWidth(FHeight);
      end;

end;

procedure TD2DockingTab.DesignSelect;
begin
  inherited;
  if (FPanel <> nil) and (FPanel is TD2DockingPanel) then
    with TD2DockingPanel(FPanel) do
      case Align of
          daLeft, daRight: FOldSize.X:=Width;
          daTop, daBottom: FOldSize.Y:=Height;
      end;
  if FIsSelected
    then begin                   //скрыть панель
           Select(false);
           ShowPanel(false);
         end
    else Select(true);           //показать панель
end;

procedure TD2DockingTab.MouseDown(Button: TMouseButton; Shift: TShiftState; X,
  Y: single);
begin
  inherited;
  if FPressed then
    begin
      FMouseDownPos:=d2Point(X,Y);
      FOldIndex:=Index;
      BringToFront;
    end;
end;

procedure TD2DockingTab.MouseMove(Shift: TShiftState; X, Y, Dx, Dy: single);
var
  C: TD2Content;
  idx,st,t: integer;
begin
  inherited;
  if FPressed and
     (Parent<>nil) and
     (Parent is TD2DockingPlace) and
     (TD2DockingPlace(Parent).FContent<>nil) and
     (TD2DockingPlace(Parent).FContent is TD2Content) then
    begin
      C:=TD2DockingPlace(Parent).FContent;
      idx:=FOldIndex;
      //BringToFront;
      case Align of
          daLeft: begin
                    Position.Y:=Position.Y-(X-FMouseDownPos.X);
                    if Position.Y<C.Position.Y+Width then Position.Y:=C.Position.Y+Width;
                    if Position.Y>C.Position.Y+C.Height then Position.Y:=C.Position.Y+C.Height;
                    with TD2DockingPlace(Parent) do
                    begin
                      if FIndexMaxLeft<1 then exit;
                      if X-FMouseDownPos.X<0 then st:=1 else st:=-1;
                      t:=0;
                      while (idx>=0) and (idx<=FIndexMaxLeft) do
                      begin
                        if (idx>0) and (st=-1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx-1]) do
                            if Self.Position.Y<FLastPosition.Y-Width/2+Self.Width then
                              begin
                                FLastPosition.Y:=FLastPosition.Y+Self.Width;
                                AnimateFloat('Position.Y',FLastPosition.Y,0.2);
                                t:=-1;
                              end;
                        if (idx<FIndexMaxLeft) and (st=1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx]) do
                            if Self.Position.Y>FLastPosition.Y-Width/2 then
                              begin
                                FLastPosition.Y:=FLastPosition.Y-Self.Width;
                                AnimateFloat('Position.Y',FLastPosition.Y,0.2);
                                t:=1;
                              end;
                        idx:=idx+st;
                      end;
                      FOldIndex:=FOldIndex+t;
                    end;
                  end;
           daTop: begin
                    Position.X:=Position.X+(X-FMouseDownPos.X);
                    if Position.X<C.Position.X then Position.X:=C.Position.X;
                    if Position.X>C.Position.X+C.Width-Width then Position.X:=C.Position.X+C.Width-Width;
                    with TD2DockingPlace(Parent) do
                    begin
                      if FIndexMaxTop-FIndexMaxLeft<2 then exit;
                      if X-FMouseDownPos.X>0 then st:=1 else st:=-1;
                      t:=0;
                      while (FIndexMaxLeft<idx) and (idx<=FIndexMaxTop) do
                      begin
                        if (FIndexMaxLeft+1<idx) and (st=-1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx-1]) do
                            if Self.Position.X<FLastPosition.X+Width/2 then
                              begin
                                FLastPosition.X:=FLastPosition.X+Self.Width;
                                AnimateFloat('Position.X',FLastPosition.X,0.2);
                                t:=-1;
                              end;
                        if (idx<FIndexMaxTop) and (st=1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx]) do
                            if Self.Position.X>FLastPosition.X+Width/2-Self.Width then
                              begin
                                FLastPosition.X:=FLastPosition.X-Self.Width;
                                AnimateFloat('Position.X',FLastPosition.X,0.2);
                                t:=1;
                              end;
                        idx:=idx+st;
                      end;
                      FOldIndex:=FOldIndex+t;
                    end;
                  end;
         daRight: begin
                    Position.Y:=Position.Y+(X-FMouseDownPos.X);
                    if Position.Y<C.Position.Y then Position.Y:=C.Position.Y;
                    if Position.Y+Width>C.Position.Y+C.Height then Position.Y:=C.Position.Y+C.Height-Width;
                    with TD2DockingPlace(Parent) do
                    begin
                      if FIndexMaxRight-FIndexMaxTop<2 then exit;
                      if X-FMouseDownPos.X>0 then st:=1 else st:=-1;
                      t:=0;
                      while (FIndexMaxTop<idx) and (idx<=FIndexMaxRight) do
                      begin
                        if (FIndexMaxTop+1<idx) and (st=-1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx-1]) do
                            if Self.Position.Y<FLastPosition.Y+Width/2 then
                              begin
                                FLastPosition.Y:=FLastPosition.Y+Self.Width;
                                AnimateFloat('Position.Y',FLastPosition.Y,0.2);
                                t:=-1;
                              end;
                        if (idx<FIndexMaxRight) and (st=1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx]) do
                            if Self.Position.Y>FLastPosition.Y+Width/2-Self.Width then
                              begin
                                FLastPosition.Y:=FLastPosition.Y-Self.Width;
                                AnimateFloat('Position.Y',FLastPosition.Y,0.2);
                                t:=1;
                              end;
                        idx:=idx+st;
                      end;
                      FOldIndex:=FOldIndex+t;
                    end;
                  end;
        daBottom: begin
                    Position.X:=Position.X-(X-FMouseDownPos.X);
                    if Position.X-Width<C.Position.X then Position.X:=C.Position.X+Width;
                    if Position.X>C.Position.X+C.Width then Position.X:=C.Position.X+C.Width;
                    with TD2DockingPlace(Parent) do
                    begin
                      if FIndexMaxBottom-FIndexMaxRight<2 then exit;
                      if X-FMouseDownPos.X<0 then st:=1 else st:=-1;
                      t:=0;
                      while (FIndexMaxRight<idx) and (idx<=FIndexMaxBottom) do
                      begin
                        if (FIndexMaxRight+1<idx) and (st=-1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx-1]) do
                          begin
                            if Self.Position.X<FLastPosition.X-Width/2+Self.Width then
                              begin
                                FLastPosition.X:=FLastPosition.X+Self.Width;
                                AnimateFloat('Position.X',FLastPosition.X,0.2);
                                t:=-1;
                              end;
                          end;
                        if (idx<FIndexMaxBottom) and (st=1) then
                          with TD2DockingTab(TD2DockingPlace(Self.Parent).FChildren[idx]) do
                            if Self.Position.X>FLastPosition.X-Width/2 then
                              begin
                                FLastPosition.X:=FLastPosition.X-Self.Width;
                                AnimateFloat('Position.X',FLastPosition.X,0.2);
                                t:=1;
                              end;
                        idx:=idx+st;
                      end;
                      FOldIndex:=FOldIndex+t;
                    end;
                  end;
      end;
    end;
end;

procedure TD2DockingTab.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: single);
begin
  inherited;
  Index:=FOldIndex;
  if (Parent<>nil) and (Parent is TD2DockingPlace) then Parent.Visual.Realign;
end;

procedure TD2DockingTab.MouseEnter;
begin
  inherited;
  If not IsVisible then exit;
  Select(true);
end;

procedure TD2DockingTab.MouseLeave;
begin
  inherited;
  if FTimer<>nil then FTimer.Enabled:=true;
end;

procedure TD2DockingTab.Select(ASelected: boolean);
begin
  FIsSelected := ASelected;
  StartTriggerAnimation(Self, 'IsSelected');
  ApplyTriggerEffect(Self, 'IsSelected');
  if FIsSelected
    then begin
           if (Parent <> nil) and
              (Parent is TD2DockingPlace)
             then TD2DockingPlace(Parent).HideAllPanels(Self);
           ShowPanel(true);
         end
    else if FTimer<>nil then FTimer.Enabled:=true; //если рабочий режим то запустить таймер скрытия
end;

procedure TD2DockingTab.DoTimer(Sender: TObject);
begin
  if FTimer<>nil then FTimer.Enabled:=false; //остановить таймер
  If IsVisible then
  begin
    Select(false);
    ShowPanel(false);     //скрыть панель
  end;
end;


procedure TD2DockingTab.ShowPanel(const Value: boolean);
var s: Single;
begin
  if (FTimer<>nil) then FTimer.Enabled:=false; //остановить таймер
  FIsShowPanel:=Value;
  if (FPanel <> nil) and
     (FPanel.Parent <> nil) and
     (FPanel is TD2DockingPanel) then
  with TD2DockingPanel(FPanel) do
  begin
    if  FShadow <> nil
      then s:=FShadow.Distance*3
      else s:=3;
    if FIsShowPanel
      then begin
             BringToFront;
             case Align of  //показать панель
                 daLeft: AnimateFloat('Position.X',0,0.2);
                  daTop: AnimateFloat('Position.Y',0,0.2);
                daRight: AnimateFloat('Position.X',Parent.Visual.Width-FOldSize.X-Padding.Right,0.2);
               daBottom: AnimateFloat('Position.Y',Parent.Visual.Height-FOldSize.Y-Padding.Bottom,0.2);
             end;
           end
      else begin
             case Align of  //скрыть панель
                 daLeft: AnimateFloat('Position.X',-Width-Padding.Left-Padding.Right-s,0.2);
                  daTop: AnimateFloat('Position.Y',-Height-Padding.Top-Padding.Bottom-s,0.2);
                daRight: AnimateFloat('Position.X',Parent.Visual.Width+s,0.2);
               daBottom: AnimateFloat('Position.Y',Parent.Visual.Height+s,0.2);
             end;
           end;
  end;
end;

procedure TD2DockingTab.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) then
  begin
    if AComponent = FPanel then FPanel := nil;
    if AComponent = FTimer then FTimer := nil;
  end;
end;

procedure TD2DockingTab.SetText(const Value: WideString);
begin
  inherited;
  if FDisableSetText then exit;
  FDisableSetText:=true;
  if FAutoWidth then DoAutoWidth(FHeight);
  if (FPanel<>nil) and (FPanel is TD2DockingPanel)
    then TD2DockingPanel(FPanel).Text:=Value;
  FDisableSetText:=false;
end;

function TD2DockingTab.GetAlign: TD2DockingAlign;
begin
  if (FPanel <> nil) and
     (FPanel is TD2DockingPanel)
    then Result := TD2DockingPanel(FPanel).Align
    else Result := daNone;
end;

procedure TD2DockingTab.SetAlign(const Value: TD2DockingAlign);
begin
  if (FPanel <> nil) and
     (FPanel is TD2DockingPanel)
    then TD2DockingPanel(FPanel).Align:=Value;
end;

procedure TD2DockingTab.SetAutoWidth(const Value: boolean);
begin
  if FAutoWidth <> Value then
  begin
    FAutoWidth := Value;
    if FAutoWidth then DoAutoWidth(FHeight);
  end;
end;

constructor TD2DockingTab.Create(AOwner: TComponent);
begin
  inherited;
  FAutoWidth := true;
  FDisableSetText:= false;
  FPanel := nil;
  FTextControl := nil;
  FTimer := TD2Timer.Create(Self);
  FTimer.Interval:=500;
  FTimer.OnTimer:=DoTimer;
  Height := 20;
  Width := 80;
  RotateCenter.SetPoint(d2Point(0,0));
  FAutoTranslate := true;
  AutoCapture := true;
  HitTest := true;
end;

destructor TD2DockingTab.Destroy;
begin
  FTimer.Enabled:=false; //остановить таймер
  FTimer.Free;
  FTimer:=nil;
  inherited;
end;

procedure TD2DockingTab.DoAutoWidth(AMinWidth: Single);
var
  H,V: TD2TextAlign;
  S: TD2Align;
begin
  if (FTextControl<>nil) and FAutoWidth then
    begin
      if FText<>''
        then begin
               FTextControl.WordWrap := false;
               V:=FTextControl.VertTextAlign;
               H:=FTextControl.HorzTextAlign;
               S := FTextControl.Align;
               FTextControl.VertTextAlign := d2TextAlignNear;
               FTextControl.HorzTextAlign := d2TextAlignNear;
               FTextControl.Align := vaNone;
               FTextControl.AutoSize := true;
               Width := FTextControl.Width + AMinWidth;
               FTextControl.AutoSize := false;
               FTextControl.Align := S;
               FTextControl.VertTextAlign := V;
               FTextControl.HorzTextAlign := H;
             end
        else Width := AMinWidth;
      if (Parent<>nil) and (Parent is TD2DockingPlace)
        then Parent.Visual.Realign;
    end;
end;

procedure TD2DockingTab.Realign;
var s: Single;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign := true;
  if (FPanel<> nil) and
     (FPanel.Parent <> nil) and
     (FPanel is TD2DockingPanel) and
     (not TD2DockingPanel(FPanel).IsFixed) then
    with TD2DockingPanel(FPanel) do
    begin
      if  FShadow <> nil
        then s:=FShadow.Distance*3
        else s:=3;
      if IsShowPanel
        then case Align of    //панель видна
                 daLeft: SetBounds(Padding.Left,
                                   Padding.Top,
                                   FOldSize.X,
                                   Parent.Visual.Height-Padding.Top-Padding.Bottom);
                  daTop: SetBounds(Padding.Left,
                                   Padding.Top,
                                   Parent.Visual.Width-Padding.Left-Padding.Right,
                                   FOldSize.Y);
                daRight: SetBounds(Parent.Visual.Width-FOldSize.X-Padding.Right,
                                   Padding.Top,
                                   FOldSize.X,
                                   Parent.Visual.Height-Padding.Top-Padding.Bottom);
               daBottom: SetBounds(Padding.Left,
                                   Parent.Visual.Height-FOldSize.Y-Padding.Bottom,
                                   Parent.Visual.Width-Padding.Left-Padding.Right,
                                   FOldSize.Y);
             end
        else case Align of   //панель скрыта
                 daLeft: SetBounds(-Width-Padding.Left-Padding.Right-s,
                                   Padding.Top,
                                   FOldSize.X,
                                   Parent.Visual.Height-Padding.Top-Padding.Bottom);
                  daTop: SetBounds(Padding.Left,
                                   -Height-Padding.Top-Padding.Bottom-s,
                                   Parent.Visual.Width-Padding.Left-Padding.Right,
                                   FOldSize.Y);
                daRight: SetBounds(Parent.Visual.Width+s,
                                   Padding.Top,
                                   FOldSize.X,
                                   Parent.Visual.Height-Padding.Top-Padding.Bottom);
               daBottom: SetBounds(Padding.Left,
                                   Parent.Visual.Height+s,
                                   Parent.Visual.Width-Padding.Left-Padding.Right,
                                   FOldSize.Y);
             end;
    end;
  FDisableAlign := false;
end;

{ TD2DockingPanel }

procedure TD2DockingPanel.SetShowHeader(const Value: boolean);
begin
  if FShowHeader <> Value then
  begin
    FShowHeader := Value;
    if FHeader <> nil then
      FHeader.Visible := FShowHeader;
    if FContent<>nil then
    begin
      if FShowHeader
        then FContent.Padding.Top := 23
        else FContent.Padding.Top := 2;
        Realign;
    end;
  end;
end;

procedure TD2DockingPanel.SetTabAutoWidth(const Value: boolean);
begin
  if FTabAutoWidth <> Value then
  begin
    FTabAutoWidth := Value;
    if (FTab<>nil) and (FTab is TD2DockingTab)
        then TD2DockingTab(FTab).AutoWidth:=Value;
  end;
end;

procedure TD2DockingPanel.ApplyAlign;
begin
  if FTSplitter <> nil  then FTSplitter.Visible:=false;
  if FTLSplitter <> nil then FTLSplitter.Visible:=false;
  if FTRSplitter <> nil then FTRSplitter.Visible:=false;
  if FBSplitter <> nil  then FBSplitter.Visible:=false;
  if FBLSplitter <> nil then FBLSplitter.Visible:=false;
  if FBRSplitter <> nil then FBRSplitter.Visible:=false;
  if FLSplitter <> nil  then FLSplitter.Visible:=false;
  if FRSplitter <> nil  then FRSplitter.Visible:=false;

  case FAlign of
      daLeft: begin
                if FShadow <> nil then FShadow.Enabled := not FIsFixed;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=FShowFixedCheckBox;
                if IsFixed then inherited Align:=vaLeft;
                Width:=FOldSize.X;
                if FRSplitter <> nil then FRSplitter.Visible:=true;
              end;
       daTop: begin
                if FShadow <> nil then FShadow.Enabled := not FIsFixed;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=FShowFixedCheckBox;
                if IsFixed then inherited Align:=vaTop;
                Height:=FOldSize.Y;
                if FBSplitter <> nil then FBSplitter.Visible:=true;
              end;
     daRight: begin
                if FShadow <> nil then FShadow.Enabled := not FIsFixed;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=FShowFixedCheckBox;
                if IsFixed then inherited Align:=vaRight;
                Width:=FOldSize.X;
                if FLSplitter <> nil then FLSplitter.Visible:=true;
              end;
    daBottom: begin
                if FShadow <> nil then FShadow.Enabled := not FIsFixed;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=FShowFixedCheckBox;
                if IsFixed then inherited Align:=vaBottom;
                Height:=FOldSize.Y;
                if FTSplitter <> nil then FTSplitter.Visible:=true;
              end;
    daClient: begin
                IsFixed:=true;
                if FShadow <> nil then FShadow.Enabled:= false;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=false;
                inherited Align:=vaClient;
              end;
      daNone: begin
                IsFixed:=true;
                if FShadow <> nil then FShadow.Enabled:= true;
                if FFixedCheckBox <> nil then FFixedCheckBox.Visible:=false;
                inherited Align:=vaNone;
                Width:=FOldSize.X;
                Height:=FOldSize.Y;
                if FTSplitter  <> nil then FTSplitter.Visible:=true;
                if FTLSplitter <> nil then FTLSplitter.Visible:=true;
                if FTRSplitter <> nil then FTRSplitter.Visible:=true;
                if FBSplitter  <> nil then FBSplitter.Visible:=true;
                if FBLSplitter <> nil then FBLSplitter.Visible:=true;
                if FBRSplitter <> nil then FBRSplitter.Visible:=true;
                if FLSplitter  <> nil then FLSplitter.Visible:=true;
                if FRSplitter  <> nil then FRSplitter.Visible:=true;
              end;
  end;
  {
  if FTab <> nil
    then FTab.Parent.Visual.Realign
    else begin
           if (Parent<>nil)
             then begin
                    if (Parent.Parent<>nil) and
                       (Parent.Parent is TD2DockingPlace)
                      then Parent.Parent.Visual.Realign
                      else Parent.Visual.Realign;
                  end;
         end;
         }
  if (Parent<>nil) then
    begin
      if (Parent.Parent<>nil) and
         (Parent.Parent is TD2DockingPlace)
        then Parent.Parent.Visual.Realign
        else if Parent.IsVisual then Parent.Visual.Realign;
    end;
end;

procedure TD2DockingPanel.SetShowFixedCheck(const Value: boolean);
begin
  if FShowFixedCheckBox <> Value then
  begin
    FShowFixedCheckBox := Value;
    if FFixedCheckBox <> nil then
      FFixedCheckBox.Visible := FShowFixedCheckBox;
  end;
end;

procedure TD2DockingPanel.SetIsFixed(const Value: boolean);
begin
  if FDisableFixed then exit;
  if FIsFixed <> Value then
  begin
    FIsFixed := Value;
    if FShadow <> nil then
      if FAlign = daNone
        then FShadow.Enabled := true
        else FShadow.Enabled := not FIsFixed;
    if FFixedCheckBox<>nil then
      begin
        FDisableFixed:=true;
        FFixedCheckBox.IsChecked:=FIsFixed;
        FDisableFixed:=false;
      end;
    if not (csLoading in ComponentState)
      then begin
             if (FIsFixed) or (FAlign in [daNone, daClient])
               then TabDelete
               else TabInsert;
           end
      else if not FIsFixed then inherited Align:=vaNone;
  end;
end;

procedure TD2DockingPanel.SetMinHeight(const Value: Single);
begin
  if FMinHeight <> Value then
  begin
     FMinHeight:=Value;
     if Height < FMinHeight then Height:=FMinHeight;
  end;
end;

procedure TD2DockingPanel.SetMinWidth(const Value: Single);
begin
  if FMinWidth <> Value then
  begin
     FMinWidth:=Value;
     if Width < FMinWidth then Width:=FMinWidth;
  end;
end;

procedure TD2DockingPanel.DoFixedChange(Sender: TObject);
begin
  if (FFixedCheckBox <> nil) then
  begin
    IsFixed := FFixedCheckBox.IsChecked;
    if Assigned(FOnFixedChange)
      then FOnFixedChange(Self);
  end;
end;

procedure TD2DockingPanel.SetAlign(const Value: TD2DockingAlign);
begin
  if (FAlign = Value) or
     (not (Value in FAllowDock)) or
     ((Parent <> nil) and
      (Parent.Parent <> nil) and
      (Parent.Parent is TD2DockingPlace) and
      (not (Value in TD2DockingPlace(Parent.Parent).FAllowDockChildren))
     )
    then exit;
  if FAlign=daNone then                //если раньше был daNone
  begin
    //FOldPosition:=Position.GetPoint;    //то сохраним позицию панели
    FOldSize:=d2Point(Width,Height);    //и сохраним размеры панели
  end;
  FAlign:=Value;                                      //меняем значение размещения
  //if FAlign=daNone then Position.SetPoint(FOldPosition);
  ApplyAlign;
  if Assigned(FOnAlignChange) then FOnAlignChange(Self);
end;

procedure TD2DockingPanel.SetAllowDock(Value: TD2DockingAllowAligns);
var FA: TD2DockingAllowAligns;
begin
  if Value = []
    then begin
           if FAllowDock = [daNone]
             then FAllowDock :=[daClient]
             else FAllowDock :=[daNone];
         end
    else FAllowDock := Value;
    { Align }
  if (Parent<>nil) and
     (Parent.Parent<>nil) and
     (Parent.Parent is TD2DockingPlace)
    then FA:=TD2DockingPlace(Parent.Parent).FAllowDockChildren
    else FA:=FAllowDock;
  if (not (Align in FAllowDock)) or (not (Align in FA)) then
  begin
    if (daLeft in FAllowDock) and (daLeft in FA)
      then Align:=daLeft
      else if (daTop in FAllowDock) and (daTop in FA)
             then Align:=daTop
             else if (daRight in FAllowDock) and (daRight in FA)
                    then Align:=daRight
                    else if (daBottom in FAllowDock) and (daBottom in FA)
                           then Align:=daBottom
                           else if (daClient in FAllowDock) and (daClient in FA)
                                  then Align:=daClient
                                  else begin
                                         if not (daNone in FAllowDock)
                                           then  FAllowDock:=FAllowDock+[daNone];
                                         Align:=daNone;
                                       end;
  end;
end;

procedure TD2DockingPanel.SetDragMode(const Value: TD2DragMode);
begin
  FDragMode:=Value;
  FIsDockable:=(FDragMode=d2DragAutomatic);
end;

procedure TD2DockingPanel.SetIsDockable(const Value: boolean);
begin
  FIsDockable := Value;
  if FIsDockable
    then FDragMode:=d2DragAutomatic
    else FDragMode:=d2DragManual;
end;

procedure TD2DockingPanel.SetShowCloseButton(const Value: boolean);
begin
  if FShowCloseButton <> Value then
  begin
    FShowCloseButton := Value;
    if FCloseButton <> nil then
      FCloseButton.Visible := FShowCloseButton;
  end;
end;

procedure TD2DockingPanel.DoCloseClick(Sender: TObject);
var P: TD2VisualObject;
begin
  if (Parent<>nil) and (Parent.IsVisual)
    then P:=Parent.Visual
    else P:=nil;
  if (FCloseButton <> nil) and (Assigned(FOnCloseClick))
    then FOnCloseClick(Self);
  if FFreeOnClose
    then begin
           if FTab<>nil then
             begin
               FTab.Free;
               FTab:=nil;
             end;
           Free;
         end
    else Visible:=false;
  if (P<>nil) then P.Realign;
end;

procedure TD2DockingPanel.ApplyStyle;
  procedure SetSplitter (var S:TD2SplitLayout);
  begin
    S.CanFocused := false;
    S.AutoCapture := true;
    S.OnMouseDown := DoSplitterMouseDown;
    S.OnMouseUp:=DoSplitterMouseUp;
    S.OnMouseMove:=DoSplitterMouseMove;
  end;
var
  O: TD2Object;
begin
  inherited;
  O := FindResource('tsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FTSplitter := TD2SplitLayout(O);
    SetSplitter(FTSplitter);
  end;
  O := FindResource('tlsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FTLSplitter := TD2SplitLayout(O);
    SetSplitter(FTLSplitter);
  end;
  O := FindResource('trsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FTRSplitter := TD2SplitLayout(O);
    SetSplitter(FTRSplitter);
  end;
  O := FindResource('bsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FBSplitter := TD2SplitLayout(O);
    SetSplitter(FBSplitter);
  end;
  O := FindResource('blsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FBLSplitter := TD2SplitLayout(O);
    SetSplitter(FBLSplitter);
  end;
  O := FindResource('brsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FBRSplitter := TD2SplitLayout(O);
    SetSplitter(FBRSplitter);
  end;
  O := FindResource('lsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FLSplitter:=TD2SplitLayout(O);
    SetSplitter(FLSplitter);
  end;
  O := FindResource('rsplitter');
  if (O <> nil) and (O is TD2SplitLayout) then
  begin
    FRSplitter := TD2SplitLayout(O);
    SetSplitter(FRSplitter);
  end;
  O := FindResource('header');
  if (O <> nil) and (O is TD2Rectangle) then
  begin
    FHeader := TD2Rectangle(O);
    FHeader.Visible := FShowHeader;
    FHeader.CanFocused := false;
    FHeader.HitTest:=true;
    FHeader.DragMode:=d2DragAutomatic;
    FHeader.OnDblClick:=DoHeaderDblClick;
    FHeader.OnDragEnd:=DoHeaderDragEnd;
  end;
  O := FindResource('checkbox');
  if (O <> nil) and (O is TD2CheckBox) then
  begin
    FFixedCheckBox := TD2CheckBox(O);
    if (FAlign in [daNone, daClient])
      then FFixedCheckBox.Visible := false
      else FFixedCheckBox.Visible := FShowFixedCheckBox;
    FFixedCheckBox.IsChecked := FIsFixed;
    FFixedCheckBox.OnChange := DoFixedChange;
    FFixedCheckBox.CanFocused := false;
  end;
  O := FindResource('closebutton');
  if (O <> nil) and (O is TD2CloseButton) then
  begin
    FCloseButton := TD2CloseButton(O);
    FCloseButton.Visible := FShowCloseButton;
    FCloseButton.OnClick := DoCloseClick;
    FCloseButton.CanFocused := false;
  end;
  O := FindResource('shadow');
  if (O <> nil) and (O is TD2ShadowEffect)
    then FShadow := TD2ShadowEffect(O);
  ApplyAlign;
end;

procedure TD2DockingPanel.FreeStyle;
begin
  inherited;
  FTLSplitter := nil;
  FTRSplitter := nil;
  FBSplitter := nil;
  FBLSplitter := nil;
  FBRSplitter := nil;
  FLSplitter := nil;
  FRSplitter := nil;
  FHeader := nil;
  FFixedCheckBox := nil;
  FCloseButton := nil;
  FShadow := nil;
end;

procedure TD2DockingPanel.DesignClick;
begin
  inherited;
    case FAlign of
      daLeft, daRight: FOldSize.X:=Width;
      daTop, daBottom: FOldSize.Y:=Height;
    end;
  if not (FAlign in [daNone, daClient]) then IsFixed := not FIsFixed;
end;

procedure TD2DockingPanel.DesignInsert;
begin
  inherited;
  if not (FAlign in [daNone, daClient]) then TabInsert;
  Realign;
end;

procedure TD2DockingPanel.DesignSelect;
begin
  inherited;
  case FAlign of
    daLeft, daRight: FOldSize.X:=Width;
    daTop, daBottom: FOldSize.Y:=Height;
  end;
end;

procedure TD2DockingPanel.DoHeaderDblClick(Sender: TObject);
begin
  if FAlign <> daClient
    then begin
           FOldAlign:=FAlign;
           FOldFixed:=FIsFixed;
           if FAlign = daNone then FOldPos:=Position.GetPoint;
           Align:=daClient;
           FIsExpanded:=true;
         end
    else if FIsExpanded then
           begin
             FIsExpanded:=false;
             Align:=FOldAlign;
             IsFixed:=FOldFixed;
             if FAlign = daNone then Position.SetPoint(FOldPos);
           end;
end;

procedure TD2DockingPanel.TabInsert;
begin
  if (FTab <> nil) or
     (FAlign in [daNone, daClient]) or     //если панель без привязки или клиент
     (not ((Parent <> nil) and                    //или нет родителя
           (Parent is TD2Content) and             //или родитель не TD2Content
           (Parent.Parent <> nil) and             //или у родителя нет родителя
           (Parent.Parent is TD2DockingPlace)))   //или родитель родителя не TD2DockingPlace
     then exit;                                //то выйти
  inherited Align:=vaNone;
  if FTab=nil then
  begin
    FTab:= TD2DockingTab.Create(Owner);
    if GvarD2Designer <> nil
      then FTab.Name := GvarD2Designer.UniqueName(Owner, FTab.ClassName);
  end;
  TD2DockingTab(FTab).FPanel:=Self;
  TD2DockingTab(FTab).AutoWidth:=FTabAutoWidth;
  TD2DockingTab(FTab).Text:=Text;
  TD2DockingTab(FTab).FIsShowPanel:=true;
  if TD2DockingPlace(Parent.Parent).FTabHeight>0
    then TD2DockingTab(FTab).Height:=TD2DockingPlace(Parent.Parent).FTabHeight;
  TD2DockingTab(FTab).Parent:=Parent.Parent;
  TD2DockingTab(FTab).Select(true);
end;

procedure TD2DockingPanel.TabDelete;
begin
  if FTab <> nil then
  begin
    FTab.Free;
    FTab := nil;
  end;
  case FAlign of
    daBottom: inherited Align:=vaBottom;
      daLeft: inherited Align:=vaLeft;
     daRight: inherited Align:=vaRight;
       daTop: inherited Align:=vaTop;
  end;
   if (Parent <> nil) and (Parent.IsVisual) then
   begin
     if (Parent is TD2Content) and
        (Parent.Parent <> nil) and
        (Parent.Parent is TD2DockingPlace)
       then Parent.Parent.Visual.Realign
       else Parent.Visual.Realign;
   end;
end;

procedure TD2DockingPanel.DoSplitterMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: single);
var s: string;
begin
  if (Button = mbLeft) and (Sender is TD2SplitLayout) then
  begin
    FMouseDownPos:=d2Point(X,Y);
    FMouseDownSide := dmsNone;
    if Align=daNone then BringToFront;
    s:=LowerCase(TD2SplitLayout(Sender).ResourceName);
    if s='tsplitter'  then begin FMouseDownSide := dmsTop; exit; end;
    if s='tlsplitter' then begin FMouseDownSide := dmsTopLeft; exit; end;
    if s='trsplitter' then begin FMouseDownSide := dmsTopRight; exit; end;
    if s='bsplitter'  then begin FMouseDownSide := dmsBottom; exit; end;
    if s='blsplitter' then begin FMouseDownSide := dmsBottomLeft; exit; end;
    if s='brsplitter' then begin FMouseDownSide := dmsBottomRight; exit; end;
    if s='lsplitter'  then begin FMouseDownSide := dmsLeft; exit; end;
    if s='rsplitter'  then begin FMouseDownSide := dmsRight; exit; end;
  end;

end;

procedure TD2DockingPanel.DoSplitterMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: single);
begin
  if (Sender is TD2SplitLayout) and (Button = mbLeft)
    then FMouseDownSide := dmsNone;
  Realign;
end;

procedure TD2DockingPanel.DoSplitterMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y, Dx, Dy: single);
  procedure Top;
  var h: Single;
  begin
    h:=Y-FMouseDownPos.Y;
    if (Height = FMinHeight) and (h > 0) then exit;
    Height:=Height-h;
    if Height < FMinHeight
      then begin
             if not (IsFixed and (FAlign=daBottom)) then Position.Y:=Position.Y+h-FMinHeight+Height;
             Height:=FMinHeight;
           end
      else if not (IsFixed and (FAlign=daBottom)) then Position.Y:=Position.Y+h;
  end;

  procedure Left;
  var w: Single;
  begin
    w:=X-FMouseDownPos.X;
    if (Width = FMinWidth) and (w > 0) then exit;
    Width:=Width-w;

    if Width < FMinWidth
      then begin
             if not (IsFixed and (FAlign=daRight)) then Position.X:=Position.X+w-FMinWidth+Width;
             Width:=FMinWidth;
           end
      else if not (IsFixed and (FAlign=daRight)) then Position.X:=Position.X+w;
  end;
  procedure Bottom;
  begin
    Height:=Height+(Y-FMouseDownPos.Y);
    if Height < FMinHeight then Height:=FMinHeight;
  end;
  procedure Right;
  begin
    Width:=Width+(X-FMouseDownPos.X);
    if Width < FMinWidth then Width:=FMinWidth;
  end;

begin
  if FMouseDownSide <> dmsNone then
    case FMouseDownSide of
              dmsTop: Top;
           dmsBottom: Bottom;
             dmsLeft: Left;
            dmsRight: Right;
          dmsTopLeft: begin
                        Top;
                        Left;
                      end;
         dmsTopRight: begin
                        Top;
                        Right;
                      end;
       dmsBottomLeft: begin
                        Bottom;
                        Left;
                      end;
      dmsBottomRight: begin
                        Bottom;
                        Right;
                      end;
    end;
end;

procedure TD2DockingPanel.SetMouseOverChildren(Sender: TObject; Value: Boolean);
begin
  inherited;
  if Value
    then begin
           if (FTab<>nil) and (TD2DockingTab(FTab).FTimer <> nil)
             then TD2DockingTab(FTab).FTimer.Enabled:=false;
         end
    else begin
           if (FTab<>nil) and (TD2DockingTab(FTab).FTimer <> nil)
             then begin
                    TD2DockingTab(FTab).FTimer.Enabled:=false;
                    TD2DockingTab(FTab).FTimer.Enabled:=true;
                  end;
         end;
end;

procedure TD2DockingPanel.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if Operation = opRemove then
  begin
    if AComponent = FTab then FTab := nil;
  end;
end;

procedure TD2DockingPanel.SetHeight(const Value: single);
begin
  inherited;
  case FAlign of
    daClient, daLeft, daRight: exit;
      daNone, daTop, daBottom: FOldSize.Y:=Height;
  end;
end;

procedure TD2DockingPanel.SetText(const Value: WideString);
begin
  inherited;
  if FDisableSetText then exit;
  FDisableSetText:=true;
  if (FTab<>nil) and (FTab is TD2DockingTab)
    then TD2DockingTab(FTab).Text:=Value;
  FDisableSetText:=false;
end;

procedure TD2DockingPanel.SetWidth(const Value: single);
begin
  inherited;
  case FAlign of
    daClient, daTop, daBottom: exit;
    daNone, daLeft, daRight: FOldSize.X:=Width;
  end;
end;

procedure TD2DockingPanel.SetVisible(const Value: boolean);
var
  s: boolean;
begin
  if FTab<>nil then FTab.Visible:=Value;
  if (not Value) and (FShadow <> nil)
    then begin
           s:=FShadow.Enabled;
           FShadow.Enabled:=false;
           inherited;
           FShadow.Enabled:=s;
         end
    else inherited;
  if (Parent<>nil) and Parent.IsVisual then Parent.Visual.Realign;
end;

constructor TD2DockingPanel.Create(AOwner: TComponent);
begin
  inherited;
  FAutoTranslate := true;
  FDisableFixed:=false;
  Width := 130;
  Height := 130;
  FMinHeight:=42;
  FMinWidth:=42;
  FAlign  :=  daNone;
  FIsExpanded:=false;
  FIsFixed := true;
  FOldSize:=d2Point(Width,Height);
  FOldAlign := daNone;
  FOldPos:=Position.GetPoint;
  FOldFixed:=true;
  FAllowDock := [daNone, daClient, daBottom, daLeft, daRight, daTop];
  FTab := nil;
  FTabAutoWidth:=true;
  FIsDockable:=true;
  FIsNotDraged:=true;
  FDragTarget:=nil;
  FMouseDownSide := dmsNone;
  FFreeOnClose:=true;
  FShowHeader := true;
  FShowFixedCheckBox := true;
  FShowCloseButton := true;
  FContent := TD2Content.Create(Self);
  //FContent := TD2ScrollContent.Create(Self);
  FContent.Parent := Self;
  FContent.ClipChildren := true;
  FContent.HitTest := false;
  FContent.Locked := true;
  FContent.Stored := false;
  if FShowHeader
    then FContent.Padding.Top := 23
    else FContent.Padding.Top := 2;
  FContent.Padding.Bottom := 2;
  FContent.Padding.Left := 2;
  FContent.Padding.Right := 2;
  FContent.Width := Width;
  FContent.Height := Height;
end;

destructor TD2DockingPanel.Destroy;
begin
  inherited;
end;

procedure TD2DockingPanel.Realign;
begin
  inherited;
  if csLoading in ComponentState then Exit;
  if FDisableAlign then Exit;
  FDisableAlign := true;
  if (FContent <> nil) then
  begin
    FContent.Position.X := FContent.Padding.Left;
    FContent.Position.Y := FContent.Padding.Top;
    FContent.Width := Width - FContent.Padding.Left - FContent.Padding.Right;
    FContent.Height := Height - FContent.Padding.Top - FContent.Padding.Bottom;
  end;
  FDisableAlign := false;
end;

procedure TD2DockingPanel.AddObject(AObject: TD2Object);
begin
  if (FContent <> nil) and
     (AObject <> FContent) and
     (AObject <> FResourceLink)and
     (not (AObject is TD2Effect)) and
     (not (AObject is TD2Animation))
    then begin
           FContent.AddObject(AObject);
         end
    else inherited;
end;

procedure TD2DockingPanel.DragMove(Sender: TD2VisualObject; Point: TD2Point);
begin
  if Sender<>FDragTarget then exit;
  Position.X:=Point.X-FDragDistance.X;
  Position.Y:=Point.Y-FDragDistance.Y;
  FDragMousePos:=Point;
end;

procedure TD2DockingPanel.DoHeaderDragEnd(Sender: TObject);
begin
  if (FDragTarget<>nil) and (FDragTarget is TD2DockingPlace)
    then TD2DockingPlace(FDragTarget).FDragPanel:=nil;
  FDragTarget:=nil;
  FIsNotDraged:=true;
end;

{ TD2DockingPlace }

procedure TD2DockingPlace.DoContentDragLeave(Sender: TObject);
var Par: TD2Object;
begin
  if FDragPanel<>nil then
    with FDragPanel do
    begin
      if Align <> daNone then
        begin
          Align:=daNone;
          Par:=Parent;
          while (Par.Parent<>nil) and (Par.Parent.IsVisual) do Par:=Par.Parent;
          Parent:=Par;
          FDragDistance.X:=Width/2;
          FDragDistance.Y:=11;
        end;
      DragMove(Self, FDragMousepos);
      Self.FDragPanel:=nil;
    end;
end;

procedure TD2DockingPlace.DoContentDragEnter(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point);
begin
  if FDragPanel<>nil then
    with FDragPanel do
    begin
      FDragTarget:=Self;
    end;
end;


procedure TD2DockingPlace.DoContentDragOver(Sender: TObject;
  const Data: TD2DragObject; Shift: TShiftState; const Point: TD2Point; var Accept: Boolean);
var
  Par: TD2Object;
begin
  Accept:=false;
  if  (FDragPanel = nil) or
      ((FDragPanel <> nil) and
       (FDragPanel.FHeader <> Data.Source))
    then begin
           if (Data.Source is TD2VisualObject) and
              (TD2VisualObject(Data.Source).ResourceName='header') and
              (TD2VisualObject(Data.Source).Parent <> nil) and
              (TD2VisualObject(Data.Source).Parent.Parent <> nil) and
              (TD2VisualObject(Data.Source).Parent.Parent.Parent <> nil) and
              (TD2VisualObject(Data.Source).Parent.Parent.Parent is TD2DockingPanel)
             then begin
                    FDragPanel:=TD2DockingPanel(TD2VisualObject(Data.Source).Parent.Parent.Parent);
                    if FDragPanel.FDragTarget=nil
                      then with FDragPanel do
                           begin
                             FDragTarget:=Self;
                             if FIsNotDraged then
                               begin
                                 FIsNotDraged:=false;
                                 FMouseDownPos:=Point;
                                 if Align=daNone then
                                   begin
                                     FDragDistance.X:=Point.X-Position.X;
                                     FDragDistance.Y:=Point.Y-Position.Y;
                                   end;
                               end;
                             Accept:=true;
                             Exit;
                           end
                      else exit;
                  end
              else exit;
         end;
  Accept:=true;

  if FDragPanel.FDragTarget<>Self then exit;
  FDragPanel.SetMouseOverChildren(FDragPanel,true);
  FDragPanel.BringToFront;
  if (FDragPanel.FMouseDownPos.X=Point.X) and
     (FDragPanel.FMouseDownPos.Y=Point.Y) then exit;
  with FDragPanel do
    if (Self.FContent <> nil) and
       (Self.FContent.FIsDragOver) then
      begin
        IsFixed:=true;
        if (daClient in Self.AllowDockChildren) and
           (daClient in AllowDock) and
            d2PtInRect(Point,FDragRectClient)
          then begin
                 if Parent<>Self then Parent:=Self;
                 Align:=daClient;
                 FIsExpanded:=false;
                 exit;
               end;
        if (daLeft in Self.AllowDockChildren) and
           (daLeft in AllowDock) and
            d2PtInRect(Point,FDragRectLeft)
          then begin
                 if Parent<>Self then Parent:=Self;
                 Align:=daLeft;
                 exit;
               end;
        if (daTop in Self.AllowDockChildren) and
           (daTop in AllowDock) and
            d2PtInRect(Point,FDragRectTop)
          then begin
                 if Parent<>Self then Parent:=Self;
                 Align:=daTop;
                 exit;
               end;
        if (daRight in Self.AllowDockChildren) and
           (daRight in AllowDock) and
            d2PtInRect(Point,FDragRectRight)
          then begin
                 if Parent<>Self then Parent:=Self;
                 Align:=daRight;
                 exit;
               end;
        if  (daBottom in Self.AllowDockChildren) and
            (daBottom in AllowDock) and
             d2PtInRect(Point,FDragRectBottom)
           then begin
                  if Parent<>Self then Parent:=Self;
                  Align:=daBottom;
                  exit;
                 end;
        if Align<>daNone
          then begin
                 Align:=daNone;
                 Par:=Parent;
                 while (Par.Parent<>nil) and (Par.Parent.IsVisual) do Par:=Par.Parent;
                 Parent:=Par;
                 FDragDistance.X:=Width/2;
                 FDragDistance.Y:=11;
               end
          else DragMove(Self,Point);
      end
    else DragMove(Self,Point);
end;

procedure TD2DockingPlace.SetDragRects;
var
  R: TD2Rect;
  w,h: Single;
begin
  if FContent=nil then exit;

  R:=GetLocalRect;
  R.Left:=R.Left+Margins.Left;
  R.Top:=R.Top+Margins.Top;
  R.Right:=R.Right-Margins.Right;
  R.Bottom:=R.Bottom-Margins.Bottom;
  R.TopLeft:=LocalToAbsolute(R.TopLeft);
  R.BottomRight:=LocalToAbsolute(R.BottomRight);

  h:=d2RectHeight(R)/3;
  if (h < 5) and (h*3 > 15) then h:=5;
  w:=d2RectWidth(R)/3;
  if (w < 5) and (w*3 > 15) then w:=5;
  FDragRectLeft:=d2Rect(R.Left, R.Top+h, R.Left+w, R.Bottom-h);
  FDragRectTop:=d2Rect(R.Left, R.Top, R.Right, R.Top+h);
  FDragRectRight:=d2Rect(R.Right-w, R.Top+h, R.Right, R.Bottom-h);
  FDragRectBottom:=d2Rect(R.Left, R.Bottom-h, R.Right, R.Bottom);
  d2InflateRect(R,-w,-h);
  FDragRectClient:=R;

  if d2IsRectEmpty(FDragRectLeft) then FDragRectLeft:=d2Rect(0,0,0,0);
  if d2IsRectEmpty(FDragRectTop) then FDragRectTop:=d2Rect(0,0,0,0);
  if d2IsRectEmpty(FDragRectRight) then FDragRectRight:=d2Rect(0,0,0,0);
  if d2IsRectEmpty(FDragRectBottom) then FDragRectBottom:=d2Rect(0,0,0,0);
  if d2IsRectEmpty(FDragRectClient) then FDragRectClient:=d2Rect(0,0,0,0);

end;

procedure TD2DockingPlace.SetAllowDockChildren(Value: TD2DockingAllowAligns);
begin
  if FAllowDockChildren <> Value then
  begin
    FAllowDockChildren := Value;
    Realign;
  end;
end;

procedure TD2DockingPlace.SetTabHeight(const Value: single);
begin
  if FTabHeight <> Value then
  begin
    FTabHeight := Value;
    Realign;
  end;
end;

procedure TD2DockingPlace.ApplyStyle;
var
  B: TD2Object;
begin
  inherited;
  B := FindResource('background');
  if (B <> nil) and (B.IsVisual) then
  begin
    FBackground := B.Visual;
  end;
  Realign;
end;

procedure TD2DockingPlace.FreeStyle;
begin
  inherited;
  FBackground := nil;
end;

constructor TD2DockingPlace.Create(AOwner: TComponent);
begin
  inherited;
  //FItemIndex := -1;
  AutoCapture := true;
  Width := 200;
  Height := 200;
  FAllowDockChildren:=[daNone, daClient, daBottom, daLeft, daRight, daTop];
  FContent := TD2Content.Create(Self);
  FContent.Parent := Self;
  FContent.ClipChildren := true;
  FContent.HitTest := true;
  FContent.Locked := true;
  FContent.Stored := false;
  FContent.Position.X:=0;
  FContent.Position.Y:=0;
  FContent.Width := Width;
  FContent.Height := Height;
  FContent.OnDragOver:=DoContentDragOver;
  FContent.OnDragEnter:=DoContentDragEnter;
  FContent.OnDragLeave:=DoContentDragLeave;
  FDragRectLeft:=d2Rect(0,0,0,0);
  FDragRectTop:=d2Rect(0,0,0,0);
  FDragRectRight:=d2Rect(0,0,0,0);
  FDragRectBottom:=d2Rect(0,0,0,0);
  FDragRectClient:=d2Rect(0,0,0,0);
  FDragPanel:=nil;
end;

destructor TD2DockingPlace.Destroy;
begin
  inherited;
end;

procedure TD2DockingPlace.HideAllPanels(ANotHide: TD2DockingTab);
var i: integer;
begin
  if FChildren = nil then exit;
  for i := 0 to FChildren.Count - 1 do
    if (TD2Object(FChildren[i]) is TD2DockingTab) and
       (TD2DockingTab(FChildren[i]) <> ANotHide) and
       (TD2DockingTab(FChildren[i]).Visible)
      then TD2DockingTab(FChildren[i]).DoTimer(Nil);
end;

function TD2DockingPlace.ItemClass: string;
begin
  Result := 'TD2DockingPanel';
end;

procedure TD2DockingPlace.Realign;
var
  i: integer;
  CurXL, CurXT, CurXR, CurXB, CurYL, CurYT, CurYR, CurYB: single;
  MaxLeft, MaxTop, MaxRight, MaxBottom, H: single;
  B: TD2Object;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign := true;

  // определяем максимальную высоту и сортируем закладки,
  MaxLeft := 0;
  MaxTop := 0;
  MaxRight := 0;
  MaxBottom := 0;
  FIndexMaxLeft:=-1;
  FIndexMaxTop:=-1;
  FIndexMaxRight:=-1;
  FIndexMaxBottom:=-1;
  if FChildren <> nil then
    for i := 0 to FChildren.Count - 1 do
      if TD2Object(FChildren[i]) is TD2DockingTab then
        with TD2DockingTab(FChildren[i]) do
        begin
          if not Visible then Continue;
          H:=Height + Padding.Top + Padding.Bottom;
          case Align of
              daLeft: begin
                        if H > MaxLeft then MaxLeft := H;
                        FIndexMaxLeft:=FIndexMaxLeft+1;
                        TD2DockingTab(Self.FChildren[i]).Index:=FIndexMaxLeft;
                        FIndexMaxTop:=FIndexMaxTop+1;
                        FIndexMaxRight:=FIndexMaxRight+1;
                        FIndexMaxBottom:=FIndexMaxBottom+1;
                      end;
               daTop: begin
                        if H > MaxTop then MaxTop := H;
                        FIndexMaxTop:=FIndexMaxTop+1;
                        TD2DockingTab(Self.FChildren[i]).Index:=FIndexMaxTop;
                        FIndexMaxRight:=FIndexMaxRight+1;
                        FIndexMaxBottom:=FIndexMaxBottom+1;
                      end;
             daRight: begin
                        if H > MaxRight then MaxRight := H;
                        FIndexMaxRight:=FIndexMaxRight+1;
                        TD2DockingTab(Self.FChildren[i]).Index:=FIndexMaxRight;
                        FIndexMaxBottom:=FIndexMaxBottom+1;
                      end;
            daBottom: begin
                        if H > MaxBottom then MaxBottom := H;
                        FIndexMaxBottom:=FIndexMaxBottom+1;
                        TD2DockingTab(Self.FChildren[i]).Index:=FIndexMaxBottom;
                      end;
          end;
        end;

  // размер рабочей области
  if FResourceLink <> nil then
  begin
    B := FResourceLink;
    if (B <> nil) and (B.IsVisual) then
      with TD2VisualObject(B) do
      begin
        Align := vaNone;
        Position.X := MaxLeft + Padding.Left;
        Position.Y := MaxTop + Padding.Top;
        Width := Self.Width - MaxLeft - MaxRight - Padding.Left - Padding.Top;
        Height := Self.Height - MaxTop - MaxBottom - Padding.Top - Padding.Bottom;
        FContent.Position:=Position;
        FContent.Width := Width;
        FContent.Height := Height;
        SetDragRects;
      end;
  end;

  //размещаем свободные панели выше всех,
  //изменяем привязку при запрете на размещение
  if (FContent<> nil) and
     (FContent.FChildren <> nil) then
    begin
      for i := 0 to FContent.FChildren.Count - 1 do
        if TD2Object(FContent.FChildren[i]) is TD2DockingPanel then
          with TD2DockingPanel(FContent.FChildren[i]) do
          begin
            if not Visible then Continue;
            if not (FAlign in Self.FAllowDockChildren)
              then SetAllowDock(FAllowDock);
            if FAlign=daNone then Index:=Self.FContent.FChildren.Count - 1;
          end;
    end;

  // размещаем закладки
  CurXL := 0;
  CurYL := MaxTop;
  CurXT := MaxLeft;
  CurYT := 0;
  CurXR := Width;
  CurYR := MaxTop;
  CurXB := MaxLeft;
  CurYB := Height;
  if FChildren <> nil then
    for i := 0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2DockingTab then
        with TD2DockingTab(FChildren[i]) do
        begin
          if not Visible then Continue;
          case Align of
              daLeft: begin
                        if Resource<>'dockingtableftstyle'
                          then Resource:='dockingtableftstyle';
                        RotateAngle:=270;
                        if (FTextControl<>nil) then FTextControl.RotateAngle:=0;
                        SetBounds(CurXL + Padding.Top, CurYL + Padding.Left + Width + Padding.Right, Width, MaxLeft - Padding.Top - Padding.Bottom);
                        CurYL := CurYL + Padding.Left + Width + Padding.Right;
                      end;
               daTop: begin
                        if Resource<>'' then Resource:='';
                        RotateAngle:=0;
                        if (FTextControl<>nil) then FTextControl.RotateAngle:=0;
                        SetBounds(CurXT + Padding.Left, CurYT + Padding.Top, Width, MaxTop - Padding.Top - Padding.Bottom);
                        CurXT := CurXT + Padding.Left + Width + Padding.Right;
                      end;
             daRight: begin
                        if Resource<>'' then Resource:='';
                        RotateAngle:=90;
                        if (FTextControl<>nil) then FTextControl.RotateAngle:=0;
                        SetBounds(CurXR - Padding.Top, CurYR + Padding.Left + Padding.Right, Width, MaxRight - Padding.Top - Padding.Bottom);
                        CurYR := CurYR + Padding.Left + Width + Padding.Right;
                      end;
            daBottom: begin
                        if Resource <> 'dockingtabbottomstyle'
                          then Resource:='dockingtabbottomstyle';
                        RotateAngle:=180;
                        if (FTextControl<>nil) then FTextControl.RotateAngle:=180;
                        SetBounds(CurXB + Padding.Left + Width + Padding.Right, CurYB - Padding.Top, Width, MaxBottom - Padding.Top - Padding.Bottom);
                        CurXB := CurXB + Padding.Left + Width + Padding.Right;
                      end;
          end;
          Realign;
          FLastPosition:=FPosition.GetPoint;
        end;
    end;
  FDisableAlign := false;
end;

procedure TD2DockingPlace.AddObject(AObject: TD2Object);
begin
  if (FContent <> nil) and
     (AObject <> FContent) and
     (AObject <> FResourceLink) and
     (not (AObject is TD2Effect)) and
     (not (AObject is TD2Animation)) and
     (not (AObject is TD2DockingTab))
    then FContent.AddObject(AObject)
    else inherited;
  if AObject is TD2DockingTab then Realign;
end;





