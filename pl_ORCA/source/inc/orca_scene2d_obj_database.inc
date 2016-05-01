{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

const
  MaxMapSize=(MaxInt div 2) div SizeOf(Integer);  { 250 million }

{ TD2DBNavigator }

var
  BtnTypeName:array[TD2NavigateBtn] of PChar=('FIRST','PRIOR','NEXT',
    'LAST','INSERT','DELETE','EDIT','POST','CANCEL','REFRESH');
  BtnTypePath:array[TD2NavigateBtn] of string=(
    'M 361.374,349.551 L 325.968,315.176 L 361.374,280.801 Z M 323.202,349.551 L 287.797,315.176 L 323.202,280.801 Z M 286.357,349.551 L 279.277,349.551 L 279.277,280.801 L 286.357,280.801 Z',
    'M 327.076,346.113 L 291.667,311.738 L 327.076,277.363 Z ',
    'M 341.236,311.738 L 305.830,346.113 L 305.830,277.363 Z ',
    'M 361.374,349.551 L 354.294,349.551 L 354.294,280.801 L 361.374,280.801 Z M 352.854,315.176 L 317.448,349.551 L 317.448,280.801 Z M 314.682,315.176 L 279.277,349.551 L 279.277,280.801 Z',
    // plus
    'M 315.303,336.714 L 315.303,315.122 L 293.228,315.122 L 293.228,306.099 L 315.303,306.099 L 315.303,284.668 L 324.706,284.668 L 324.706,306.099 L 346.781,306.099 L 346.781,315.122 L '+
    '324.706,315.122 L 324.706,336.714 Z ',
    // minus
    'M 286.766,375.304 L 286.766,364.321 L 352.763,364.321 L 352.763,375.304 Z ',
    // edit
    'M 350.074,271.455 L 350.074,350.947 L 289.995,350.947 L 289.995,271.455 Z M 347.362,274.087 L 292.704,274.087 L 292.704,348.315 L 347.362,348.315 Z M 300.892,337.681 L 300.892,335.049'+
    ' L 339.121,335.049 L 339.121,337.681 Z M 300.892,327.100 L 300.892,324.468 L 339.121,324.468 L 339.121,327.100 Z M 300.892,316.519 L 300.892,313.887 L 339.121,313.887 L 339.121,316.519 '+
    'Z M 300.892,305.884 L 300.892,303.252 L 339.121,303.252 L 339.121,305.884 Z M 300.892,295.249 L 300.892,292.617 L 339.121,292.617 L 339.121,295.249 Z M 300.892,284.668 L 300.892,282.036 L'+
    ' 339.121,282.036 L 339.121,284.668 Z ',
    // post
    'M 358.467,266.729 L '+
    '360.400,269.414 C 352.512,275.181 '+
    '343.733,284.064 334.069,296.058 L '+
    '334.069,296.058 C 324.407,308.056 '+
    '317.029,319.261 311.940,329.678 L '+
    '311.940,329.678 L 307.844,332.363 '+
    'C 304.454,334.659 302.148,336.358 '+
    '300.929,337.466 L 300.929,337.466 '+
    'C 300.452,335.787 299.402,333.028 '+
    '297.777,329.194 L 297.777,329.194 '+
    'L 296.229,325.703 C '+
    '294.017,320.695 291.959,316.989 '+
    '290.059,314.588 L 290.059,314.588 '+
    'C 288.159,312.191 286.031,310.597 '+
    '283.671,309.805 L 283.671,309.805 '+
    'C 287.656,305.726 291.308,303.685 '+
    '294.625,303.682 L 294.625,303.682 '+
    'C 297.465,303.685 300.620,307.428 '+
    '304.085,314.907 L 304.085,314.907 '+
    'L 305.800,318.667 C '+
    '312.034,308.465 320.037,298.549 '+
    '329.809,288.915 L 329.809,288.915 '+
    'C 339.584,279.283 349.135,271.888 '+
    '358.467,266.729 L 358.467,266.729 '+
    'Z ',
    // cancel
    'M 319.704,321.353 L 318.875,322.480 C 313.121,330.933 308.402,335.160 304.712,335.156 L 304.712,335.156 C 300.472,335.160 296.306,331.813 292.211,325.112 L 292.211,325.112 C 292.765,325.153 293.171,325.169 293.426,325.166 L 293.426,325.166 '+
    'C 298.260,325.169 '+
    '303.645,321.588 309.580,314.424 L 309.580,314.424 L 311.074,312.598 L 309.140,310.557 C 303.719,304.974 301.006,300.231 301.006,296.323 L 301.006,296.323 C 301.006,293.141 303.977,289.381 309.912,285.044 L 309.912,285.044 C 310.761,290.596 '+
    '313.289,296.004 '+
    '317.492,301.265 L 317.492,301.265 L 319.150,303.306 L 320.480,301.641 C 326.640,294.017 332.226,290.204 337.241,290.200 L 337.241,290.200 C 341.152,290.204 344.123,293.087 346.150,298.848 L 346.150,298.848 C 345.559,298.781 345.136,298.744 '+
    '344.878,298.740 '+
    'L 344.878,298.740 C 343.109,298.744 340.618,299.898 337.409,302.208 L 337.409,302.208 C 334.200,304.518 331.490,307.123 329.275,310.020 L 329.275,310.020 L 327.617,312.222 L 329.221,313.726 C 335.160,319.315 341.357,322.108 347.809,322.104 '+
    'L 347.809,322.104 '+
    'C 344.344,328.912 340.729,332.313 336.966,332.310 L 336.966,332.310 C 333.575,332.313 328.667,329.413 322.249,323.608 L 322.249,323.608 Z ',
    // refresh
    'M 354.848,307.012 C 354.848,312.779 353.633,318.224 351.196,323.340 L 351.196,323.340 C '+
    '348.614,328.677 344.999,332.994 340.353,336.284 L 340.353,336.284 L 346.493,340.957 L '+
    '326.744,346.113 L 328.570,327.046 L 334.102,331.289 C 339.819,326.388 342.676,319.567 '+
    '342.676,310.825 L 342.676,310.825 C 342.676,299.620 337.180,290.865 326.190,284.561 L '+
    '326.190,284.561 L 333.159,271.401 C 339.947,274.590 345.298,279.515 349.205,286.172 L '+
    '349.205,286.172 C 352.968,292.550 354.848,299.496 354.848,307.012 L 354.848,307.012 Z M '+
    '312.581,332.954 L 305.609,346.113 C 298.861,342.931 293.530,338.006 289.623,331.343 L '+
    '289.623,331.343 C 285.823,324.971 283.923,318.026 283.923,310.503 L 283.923,310.503 C '+
    '283.923,304.742 285.158,299.297 287.629,294.175 L 287.629,294.175 C 290.214,288.844 '+
    '293.809,284.527 298.418,281.230 L 298.418,281.230 L 292.278,276.504 L 312.027,271.401 L '+
    '310.201,290.469 L 304.669,286.226 C 298.955,291.133 296.095,297.955 296.095,306.689 L '+
    '296.095,306.689 C 296.095,317.902 301.590,326.656 312.581,332.954 L 312.581,332.954 Z '
  );

constructor TD2DBNavigator.Create(AOwner:TComponent);
begin
  inherited Create(AOwner);
  FCorners:=AllCorners;
  FxRadius:=4;
  FyRadius:=4;
  FDataLink:=TD2NavDataLink.Create(Self);
  FVisibleButtons:=[nbFirst,nbPrior,nbNext,nbLast,nbInsert,
    nbDelete,nbEdit,nbPost,nbCancel,nbRefresh];
  FHints:=TStringList.Create;
  TStringList(FHints).OnChange:=HintsChanged;
  InitButtons;
  InitHints;
  Width:=241;
  Height:=25;
  ButtonWidth:=0;
  FocusedButton:=nbFirst;
  FConfirmDelete:=True;
end;

destructor TD2DBNavigator.Destroy;
begin
  FDefHints.Free;
  FDataLink.Free;
  FHints.Free;
  FDataLink:=nil;
  inherited Destroy;
end;

procedure TD2DBNavigator.InitButtons;
var
  I:TD2NavigateBtn;
  Btn:TD2NavButton;
  X:single;
  ResName:string;
begin
  MinBtnSize:=d2Point(20,18);
  X:=0;
  for I:=Low(Buttons) to High(Buttons) do
  begin
    Btn:=TD2NavButton.Create (Self);
    Btn.Index:=I;
    Btn.Visible:=I in FVisibleButtons;
    Btn.Enabled:=True;
    Btn.SetBounds(X,0,MinBtnSize.X,MinBtnSize.Y);
    FmtStr(ResName,'dbn_%s',[BtnTypeName[I]]);
//    Btn.Glyph.LoadFromResourceName(HInstance,ResName);
//    Btn.NumGlyphs:=2;
    Btn.Enabled:=False;
    Btn.Enabled:=True;
    Btn.OnClick:=ClickHandler;
    Btn.OnMouseDown:=BtnMouseDown;
    Btn.Parent:=Self;
    Buttons[I]:=Btn;
    X:=X + MinBtnSize.X;

    Btn.FPath:=TD2Path.Create(Self);
    Btn.FPath.Parent:=Btn;
    Btn.FPath.Width:=18;
    Btn.FPath.Height:=18;
    Btn.FPath.Align:=vaCenter;
    Btn.FPath.HitTest:=false;
    Btn.FPath.Locked:=true;
    Btn.FPath.Stored:=false;
    Btn.FPath.Data.Data:=BtnTypePath[I];
    Btn.FPath.WrapMode:=d2PathFit;
    Btn.FPath.Stroke.Style:=d2BrushNone;
  end;
  Buttons[nbPrior].NavStyle:=Buttons[nbPrior].NavStyle + [nsAllowTimer];
  Buttons[nbNext].NavStyle :=Buttons[nbNext].NavStyle + [nsAllowTimer];
end;

procedure TD2DBNavigator.InitHints;
//var
  //I:integer;
  //J:TD2NavigateBtn;
begin
(*  if not Assigned(FDefHints) then
  begin
    FDefHints:=TStringList.Create;
    for J:=Low(Buttons) to High(Buttons) do
      FDefHints.Add(LoadResString(BtnHintId[J]));
  end;
  for J:=Low(Buttons) to High(Buttons) do
    Buttons[J].Hint:=FDefHints[Ord(J)];
  J:=Low(Buttons);
  for I:=0 to (FHints.Count - 1) do
  begin
    if FHints.Strings[I] <> '' then Buttons[J].Hint:=FHints.Strings[I];
    if J=High(Buttons) then Exit;
    Inc(J);
  end; *)
end;

procedure TD2DBNavigator.HintsChanged(Sender:TObject);
begin
  InitHints;
end;

procedure TD2DBNavigator.SetHints(Value:TStrings);
begin
  if Value.Text=FDefHints.Text then
    FHints.Clear else
    FHints.Assign(Value);
end;

function TD2DBNavigator.GetHints:TStrings;
begin
  if (csDesigning in ComponentState) and not (csWriting in ComponentState) and
     not (csReading in ComponentState) and (FHints.Count=0) then
    Result:=FDefHints else
    Result:=FHints;
end;

procedure TD2DBNavigator.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited Notification(AComponent,Operation);
  if (Operation=opRemove) and (FDataLink <> nil) and
    (AComponent=DataSource) then DataSource:=nil;
end;

procedure TD2DBNavigator.SetVisible(Value:TD2NavButtonSet);
var
  I:TD2NavigateBtn;
  W,H:single;
begin
  W:=Width;
  H:=Height;
  FVisibleButtons:=Value;
  for I:=Low(Buttons) to High(Buttons) do
    Buttons[I].Visible:=I in FVisibleButtons;
  SetSize(W,H);
  if (W <> Width) or (H <> Height) then
    SetBounds(Position.X,Position.Y,W,H);
end;

procedure TD2DBNavigator.CalcMinSize(var W,H:single);
var
  Count:integer;
  I:TD2NavigateBtn;
begin
  if (csLoading in ComponentState) then Exit;
  if Buttons[nbFirst]=nil then Exit;

  Count:=0;
  for I:=Low(Buttons) to High(Buttons) do
    if Buttons[I].Visible then
      Inc(Count);
  if Count=0 then Inc(Count);

  W:=d2MaxFloat(W,Count * MinBtnSize.X);
  H:=d2MaxFloat(H,MinBtnSize.Y);

  if Align=vaNone then
    W:=Trunc(W / Count) * Count;
end;

procedure TD2DBNavigator.SetSize(var W:single; var H:single);
var
  Count:integer;
  I:TD2NavigateBtn;
  Space,Temp,Remain:single;
  X:single;
begin
  if (csLoading in ComponentState) then Exit;
  if Buttons[nbFirst]=nil then Exit;

  CalcMinSize(W,H);

  Count:=0;
  for I:=Low(Buttons) to High(Buttons) do
    if Buttons[I].Visible then
      Inc(Count);
  if Count=0 then Inc(Count);

  ButtonWidth:=trunc(W / Count);
  Temp:=Count * ButtonWidth;
  if Align=vaNone then W:=Temp;

  X:=0;
  Remain:=W - Temp;
  Temp:=Count div 2;
  for I:=Low(Buttons) to High(Buttons) do
  begin
    if Buttons[I].Visible then
    begin
      if X=0 then
        Buttons[I].Corners:=[d2CornerTopLeft] * FCorners + [d2CornerBottomLeft] * FCorners
      else
      if X > Width - (ButtonWidth * 1.5) then
        Buttons[I].Corners:=[d2CornerTopRight] * FCorners + [d2CornerBottomRight] * FCorners
      else
        Buttons[I].Corners:=[];
      Buttons[I].xRadius:=FxRadius;
      Buttons[I].yRadius:=FyRadius;
      Buttons[I].CornerType:=FCornerType;
      Buttons[I].ApplyStyle;

      Space:=0;
      if Remain <> 0 then
      begin
        Temp:=Temp - Remain;
        if Temp < 0 then
        begin
          Temp:=Temp + Count;
          Space:=1;
        end;
      end;
      Buttons[I].SetBounds(X,0,ButtonWidth + Space,Height);
      X:=X + ButtonWidth + Space;
    end
    else
      Buttons[I].SetBounds (Width + 1,0,ButtonWidth,Height);
  end;
end;

procedure TD2DBNavigator.Realign;
var
  W,H:single;
begin
  inherited ;
  W:=Width;
  H:=Height;
  SetSize(W,H);
end;

procedure TD2DBNavigator.ClickHandler(Sender:TObject);
begin
  BtnClick (TD2NavButton (Sender).Index);
end;

procedure TD2DBNavigator.BtnMouseDown(Sender:TObject; Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
var
  OldFocus:TD2NavigateBtn;
begin
  OldFocus:=FocusedButton;
  FocusedButton:=TD2NavButton(Sender).Index;
  if IsFocused then
  begin
    SetFocus;
  end
  else
  if CanFocused and (IsFocused) and (OldFocus <> FocusedButton) then
  begin
    Buttons[OldFocus].Repaint;
    Buttons[FocusedButton].Repaint;
  end;
end;

procedure TD2DBNavigator.BtnClick(Index:TD2NavigateBtn);
begin
  if (DataSource <> nil) and (DataSource.State <> dsInactive) then
  begin
    if not (csDesigning in ComponentState) and Assigned(FBeforeAction) then
      FBeforeAction(Self,Index);
    with DataSource.DataSet do
    begin
      case Index of
        nbPrior:Prior;
        nbNext:Next;
        nbFirst:First;
        nbLast:Last;
        nbInsert:Insert;
        nbEdit:Edit;
        nbCancel:Cancel;
        nbPost:Post;
        nbRefresh:Refresh;
        nbDelete:
          if not FConfirmDelete or (MessageDlg(TranslateText('Delete record?'),mtConfirmation,mbOKCancel,0) <> idCancel) then Delete;
      end;
    end;
  end;
  if not (csDesigning in ComponentState) and Assigned(FOnNavClick) then
    FOnNavClick(Self,Index);
end;

procedure TD2DBNavigator.KeyDown(var Key:Word; var KeyChar:System.WideChar; Shift:TShiftState);
//var
  //NewFocus:TD2NavigateBtn;
  //OldFocus:TD2NavigateBtn;
begin
(*  OldFocus:=FocusedButton;
  case Key of
    VK_RIGHT:
      begin
        if OldFocus < High(Buttons) then
        begin
          NewFocus:=OldFocus;
          repeat
            NewFocus:=Succ(NewFocus);
          until (NewFocus=High(Buttons)) or (Buttons[NewFocus].Visible);
          if Buttons[NewFocus].Visible then
          begin
            FocusedButton:=NewFocus;
            Buttons[OldFocus].Invalidate;
            Buttons[NewFocus].Invalidate;
          end;
        end;
      end;
    VK_LEFT:
      begin
        NewFocus:=FocusedButton;
        repeat
          if NewFocus > Low(Buttons) then
            NewFocus:=Pred(NewFocus);
        until (NewFocus=Low(Buttons)) or (Buttons[NewFocus].Visible);
        if NewFocus <> FocusedButton then
        begin
          FocusedButton:=NewFocus;
          Buttons[OldFocus].Invalidate;
          Buttons[FocusedButton].Invalidate;
        end;
      end;
    VK_SPACE:
      begin
        if Buttons[FocusedButton].Enabled then
          Buttons[FocusedButton].Click;
      end;
  end; *)
end;

procedure TD2DBNavigator.DataChanged;
var
  UpEnable,DnEnable:boolean;
begin
  UpEnable:=Enabled and FDataLink.Active and not FDataLink.DataSet.BOF;
  DnEnable:=Enabled and FDataLink.Active and not FDataLink.DataSet.EOF;
  Buttons[nbFirst].Enabled:=UpEnable;
  Buttons[nbPrior].Enabled:=UpEnable;
  Buttons[nbNext].Enabled:=DnEnable;
  Buttons[nbLast].Enabled:=DnEnable;
  Buttons[nbDelete].Enabled:=Enabled and FDataLink.Active and
    FDataLink.DataSet.CanModify and
    not (FDataLink.DataSet.BOF and FDataLink.DataSet.EOF);
end;

procedure TD2DBNavigator.EditingChanged;
var
  CanModify:boolean;
begin
  CanModify:=Enabled and FDataLink.Active and FDataLink.DataSet.CanModify;
  Buttons[nbInsert].Enabled:=CanModify;
  Buttons[nbEdit].Enabled:=CanModify and not FDataLink.Editing;
  Buttons[nbPost].Enabled:=CanModify and FDataLink.Editing;
  Buttons[nbCancel].Enabled:=CanModify and FDataLink.Editing;
  Buttons[nbRefresh].Enabled:=CanModify;
end;

procedure TD2DBNavigator.ActiveChanged;
var
  I:TD2NavigateBtn;
begin
  if not (Enabled and FDataLink.Active) then
    for I:=Low(Buttons) to High(Buttons) do
      Buttons[I].Enabled:=False
  else
  begin
    DataChanged;
    EditingChanged;
  end;
end;

procedure TD2DBNavigator.SetDataSource(Value:TDataSource);
begin
  FDataLink.DataSource:=Value;
  if not (csLoading in ComponentState) then
    ActiveChanged;
  if Value <> nil then Value.FreeNotification(Self);
end;

function TD2DBNavigator.GetDataSource:TDataSource;
begin
  Result:=FDataLink.DataSource;
end;

procedure TD2DBNavigator.Loaded;
var
  W,H:single;
begin
  inherited Loaded;
  W:=Width;
  H:=Height;
  SetSize(W,H);
  if (W <> Width) or (H <> Height) then
    SetBounds(Position.X,Position.Y,W,H);
  InitHints;
  ActiveChanged;
end;

procedure TD2DBNavigator.SetCornerType(const Value:TD2CornerType);
begin
  if FCornerType <> Value then
  begin
    FCornerType:=Value;
    Realign;
  end;
end;

procedure TD2DBNavigator.SetxRadius(const Value:single);
begin
  if FxRadius <> Value then
  begin
    FxRadius:=Value;
    Realign;
  end;
end;

procedure TD2DBNavigator.SetyRadius(const Value:single);
begin
  if FyRadius <> Value then
  begin
    FyRadius:=Value;
    Realign;
  end;
end;

function TD2DBNavigator.IsCornersStored:boolean;
begin
  Result:=FCorners <> AllCorners;
end;

procedure TD2DBNavigator.SetCorners(const Value:TD2Corners);
begin
  if FCorners <> Value then
  begin
    FCorners:=Value;
    Realign;
  end;
end;

{ TD2NavButton }

constructor TD2NavButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FResource:='CornerButtonStyle';
  Locked:=true;
  Stored:=false;
end;

destructor TD2NavButton.Destroy;
begin
  if FRepeatTimer <> nil then
    FRepeatTimer.Free;
  inherited Destroy;
end;

procedure TD2NavButton.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited MouseDown (Button,Shift,X,Y);
  if nsAllowTimer in FNavStyle then
  begin
    if FRepeatTimer=nil then
      FRepeatTimer:=TTimer.Create(Self);

    FRepeatTimer.OnTimer:=TimerExpired;
    FRepeatTimer.Interval:=cnInitRepeatPause;
    FRepeatTimer.Enabled :=True;
  end;
end;

procedure TD2NavButton.MouseUp(Button:TMouseButton; Shift:TShiftState;
                                  X,Y:single);
begin
  inherited MouseUp (Button,Shift,X,Y);
  if FRepeatTimer <> nil then
    FRepeatTimer.Enabled :=False;
end;

procedure TD2NavButton.TimerExpired(Sender:TObject);
begin
  FRepeatTimer.Interval:=cnRepeatPause;
  if (IsPressed) then
  begin
    try
      Click;
    except
      FRepeatTimer.Enabled:=False;
      raise;
    end;
  end;
end;

procedure TD2NavButton.ApplyStyle;
var
  S:TD2Object;
begin
  inherited;
  { from style }
  S:=FindResource('text');
  if (S <> nil) and (S is TD2Shape) and (FPath <> nil) then
    FPath.Fill.Assign(TD2Shape(S).Fill);
end;

{ TD2NavDataLink }

constructor TD2NavDataLink.Create(ANav:TD2DBNavigator);
begin
  inherited Create;
  FNavigator:=ANav;
  VisualControl:=True;
end;

destructor TD2NavDataLink.Destroy;
begin
  FNavigator:=nil;
  inherited Destroy;
end;

procedure TD2NavDataLink.EditingChanged;
begin
  if FNavigator <> nil then FNavigator.EditingChanged;
end;

procedure TD2NavDataLink.DataSetChanged;
begin
  if FNavigator <> nil then FNavigator.DataChanged;
end;

procedure TD2NavDataLink.ActiveChanged;
begin
  if FNavigator <> nil then FNavigator.ActiveChanged;
end;

{ TD2DBLabel ===================================================================}

constructor TD2DBLabel.Create(AOwner:TComponent);
begin
  inherited;
  TextAlign:=d2TextAlignNear;
  FResource:='labelstyle';
  FDataLink:=TFieldDataLink.Create;
  FDataLink.Control:=Self;
  FDataLink.OnDataChange:=DataChange;
end;

procedure TD2DBLabel.DataChange(Sender:TObject);
begin
  TextW:=GetFieldText;
end;

destructor TD2DBLabel.Destroy;
begin
  FreeAndNil(FDataLink);
  inherited;
end;

procedure TD2DBLabel.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited Notification(AComponent,Operation);
  if (Operation=opRemove) and (FDataLink <> nil) and
    (AComponent=DataSource) then DataSource:=nil;
end;

function TD2DBLabel.GetFieldText:string;
begin
  if FDataLink.Field <> nil then
    Result:=FDataLink.Field.DisplayText
  else
    if csDesigning in ComponentState then
      Result:=Name
    else
      Result:='';
end;

function TD2DBLabel.GetDataField:string;
begin
  Result:=FDataLink.FieldName;
end;

procedure TD2DBLabel.SetDataField(const Value:string);
begin
  FDataLink.FieldName:=Value;
end;

function TD2DBLabel.GetDataSource:TDataSource;
begin
  Result:=FDataLink.DataSource;
end;

procedure TD2DBLabel.SetDataSource(const Value:TDataSource);
begin
  if not (FDataLink.DataSourceFixed and (csLoading in ComponentState)) then
    FDataLink.DataSource:=Value;
  if Value <> nil then Value.FreeNotification(Self);
end;

{ TD2DBImage ===================================================================}

constructor TD2DBImage.Create(AOwner:TComponent);
begin
  inherited;
  FDataLink:=TFieldDataLink.Create;
  FDataLink.Control:=Self;
  FDataLink.OnDataChange:=DataChange;
  FDataLink.OnUpdateData:=UpdateData;
end;

procedure TD2DBImage.DoBitmapChanged(Sender:TObject);
begin
  if (not Assigned(FDataLink.Field) or FDataLink.Field.IsBlob) then
  begin
    FDataLink.OnDataChange:=nil;
    try
      FDataLink.Edit;
      FDataLink.Modified;
      FDataLink.UpdateRecord;
    finally
      FDataLink.OnDataChange:=DataChange;
    end;
    Repaint;
  end;
end;

procedure TD2DBImage.UpdateData(Sender:TObject);
var
  P:TPicture;
begin
  if (not Assigned(FDataLink.Field) or FDataLink.Field.IsBlob) then
  begin
    P:=TPicture.Create;
    P.Assign(Bitmap);
    FDataLink.Field.Assign(P);
    P.Free;
  end;
end;

procedure TD2DBImage.DataChange(Sender:TObject);
begin
  if Assigned(FDataLink.Field) and FDataLink.Field.IsBlob then
  begin
    Bitmap.OnChange:=nil;
    try
      Bitmap.Assign(FDataLink.Field);
    finally
      Bitmap.OnChange:=DoBitmapChanged;
    end;
    Repaint;
  end
  else
    Bitmap.Clear();
end;

destructor TD2DBImage.Destroy;
begin
  FreeAndNil(FDataLink);
  inherited;
end;

procedure TD2DBImage.Paint;
begin
  inherited ;
  if Assigned(FScene) and FScene.GetDesignTime then
  begin
    Canvas.Fill.Style:=d2BrushSolid;
    Canvas.Fill.Color:=vcGray;
    if not Assigned(FDataLink.Field) then
      Canvas.FillText(LocalRect,LocalRect,'[TD2DBImage]',false,AbsoluteOpacity,d2TextAlignCenter)
    else
      Canvas.FillText(LocalRect,LocalRect,'[' + FDataLink.FieldName + ']',false,AbsoluteOpacity,d2TextAlignCenter);
  end;
end;

procedure TD2DBImage.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited Notification(AComponent,Operation);
  if (Operation=opRemove) and (FDataLink <> nil) and
    (AComponent=DataSource) then DataSource:=nil;
end;

function TD2DBImage.GetFieldText:string;
begin
  if FDataLink.Field <> nil then
    Result:=FDataLink.Field.DisplayText
  else
    if csDesigning in ComponentState then
      Result:=Name
    else
      Result:='';
end;

function TD2DBImage.GetDataField:string;
begin
  Result:=FDataLink.FieldName;
end;

procedure TD2DBImage.SetDataField(const Value:string);
begin
  FDataLink.FieldName:=Value;
end;

function TD2DBImage.GetDataSource:TDataSource;
begin
  Result:=FDataLink.DataSource;
end;

procedure TD2DBImage.SetDataSource(const Value:TDataSource);
begin
  if not (FDataLink.DataSourceFixed and (csLoading in ComponentState)) then
    FDataLink.DataSource:=Value;
  if Value <> nil then Value.FreeNotification(Self);
end;

{ TD2DBTextBox ===================================================================}

constructor TD2DBTextBox.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='textboxstyle';
  FDataLink:=TFieldDataLink.Create;
  FDataLink.Control:=Self;
  FDataLink.OnDataChange:=DataChange;
  FDataLink.OnUpdateData:=UpdateData;
{  FDataLink.OnEditingChange:=EditingChange;
  FDataLink.OnActiveChange:=ActiveChange;}
end;

destructor TD2DBTextBox.Destroy;
begin
  FreeAndNil(FDataLink);
  inherited;
end;

procedure TD2DBTextBox.DataChange(Sender:TObject);
begin
  TextW:=GetFieldText;
end;

procedure TD2DBTextBox.UpdateData(Sender:TObject);
begin
  FDataLink.Field.Text:=Text;
end;

procedure TD2DBTextBox.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited Notification(AComponent,Operation);
  if (Operation=opRemove) and (FDataLink <> nil) and
    (AComponent=DataSource) then DataSource:=nil;
end;

function TD2DBTextBox.GetFieldText:string;
begin
  if FDataLink.Field <> nil then
    Result:=UTF8Decode(FDataLink.Field.DisplayText)
  else
    if csDesigning in ComponentState then
      Result:=Name
    else
      Result:='';
end;

function TD2DBTextBox.GetDataField:string;
begin
  Result:=FDataLink.FieldName;
end;

procedure TD2DBTextBox.SetDataField(const Value:string);
begin
  FDataLink.FieldName:=Value;
end;

function TD2DBTextBox.GetDataSource:TDataSource;
begin
  Result:=FDataLink.DataSource;
end;

procedure TD2DBTextBox.SetDataSource(const Value:TDataSource);
begin
  if not (FDataLink.DataSourceFixed and (csLoading in ComponentState)) then
    FDataLink.DataSource:=Value;
  if Value <> nil then Value.FreeNotification(Self);
end;

procedure TD2DBTextBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if (Key=VK_DELETE) or ((Key=VK_INSERT) and (ssShift in Shift)) then
    FDataLink.Edit;
  if (KeyChar >= #32) and (FDataLink.Field <> nil) and
    not FDataLink.Field.IsValidChar(Char(KeyChar)) then
  begin
    KeyChar:=#0;
  end;
  case KeyChar of
    ^H,^V,^X,#32..High(KeyChar):
      if not FDataLink.Edit then
        KeyChar:=#0;
    #27:
      begin
        FDataLink.Reset;
        SelectAll;
        KeyChar:=#0;
      end;
  end;
  inherited;
end;

procedure TD2DBTextBox.Change;
begin
  inherited;
  if FDataLink.Editing then
  begin
    FDataLink.Modified;
    FDataLink.UpdateRecord;
  end;
end;

procedure TD2DBTextBox.EnterFocus;
begin
  inherited;
  FDataLink.Reset;
end;

procedure TD2DBTextBox.KillFocus;
begin
  inherited;
  FDataLink.Reset;
end;

{ TD2DBMemo ===================================================================}

constructor TD2DBMemo.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='Memostyle';
  FDataLink:=TFieldDataLink.Create;
  FDataLink.Control:=Self;
  FDataLink.OnDataChange:=DataChange;
  FDataLink.OnUpdateData:=UpdateData;
{  FDataLink.OnEditingChange:=EditingChange;
  FDataLink.OnActiveChange:=ActiveChange;}
end;

destructor TD2DBMemo.Destroy;
begin
  FreeAndNil(FDataLink);
  inherited;
end;

procedure TD2DBMemo.DataChange(Sender:TObject);
begin
  if Assigned(FDataLink.Field) and FDataLink.Field.IsBlob then
  begin
    TextW:=FDataLink.Field.AsString;
  end
  else
    TextW:=GetFieldText;
end;

procedure TD2DBMemo.UpdateData(Sender:TObject);
begin
  FDataLink.Field.AsString:=UTF8Encode(TextW);
end;

procedure TD2DBMemo.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited Notification(AComponent,Operation);
  if (Operation=opRemove) and (FDataLink <> nil) and
    (AComponent=DataSource) then DataSource:=nil;
end;

function TD2DBMemo.GetFieldText:string;
begin
  if FDataLink.Field <> nil then
    Result:=FDataLink.Field.DisplayText
  else
    if csDesigning in ComponentState then
      Result:=Name
    else
      Result:='';
end;

function TD2DBMemo.GetDataField:string;
begin
  Result:=FDataLink.FieldName;
end;

procedure TD2DBMemo.SetDataField(const Value:string);
begin
  FDataLink.FieldName:=Value;
end;

function TD2DBMemo.GetDataSource:TDataSource;
begin
  Result:=FDataLink.DataSource;
end;

procedure TD2DBMemo.SetDataSource(const Value:TDataSource);
begin
  if not (FDataLink.DataSourceFixed and (csLoading in ComponentState)) then
    FDataLink.DataSource:=Value;
  if Value <> nil then Value.FreeNotification(Self);
end;

procedure TD2DBMemo.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if (Key=VK_DELETE) or ((Key=VK_INSERT) and (ssShift in Shift)) then
    FDataLink.Edit;
  if (KeyChar >= #32) and (FDataLink.Field <> nil) and
    not FDataLink.Field.IsValidChar(Char(KeyChar)) then
  begin
    KeyChar:=#0;
  end;
  case KeyChar of
    ^H,^V,^X,#32..High(KeyChar):
      if not FDataLink.Edit then
        KeyChar:=#0;
    #27:
      begin
        FDataLink.Reset;
        SelectAll;
        KeyChar:=#0;
      end;
  end;
  inherited;
end;

procedure TD2DBMemo.Change;
begin
  inherited;
  if FDataLink.Editing then
  begin
    FDataLink.Modified;
    FDataLink.UpdateRecord;
  end;
end;

procedure TD2DBMemo.EnterFocus;
begin
  inherited;
  FDataLink.Reset;
end;

procedure TD2DBMemo.KillFocus;
begin
  inherited;
  FDataLink.Reset;
end;

//{ TD2GridDataLink }
//
//type
//  EInvalidGridOperation=class(Exception);
//
//procedure RaiseGridError(const S:string);
//begin
//  raise EInvalidGridOperation.Create(S);
//end;
//
//type
//  TIntArraydb=array[0..MaxMapSize] of Integer;
//  PIntArraydb=^TIntArraydb;
//
//constructor TD2GridDataLink.Create(AGrid:TD2DBGrid);
//begin
//  inherited Create;
//  FGrid:=AGrid;
//  VisualControl:=True;
//end;
//
//destructor TD2GridDataLink.Destroy;
//begin
//  inherited Destroy;
//end;
//
//function TD2GridDataLink.GetDefaultFields:boolean;
//var
//  I:integer;
//begin
//  Result:=True;
//  if DataSet <> nil then Result:=DataSet.DefaultFields;
//  if Result and SparseMap then
//  for I:=0 to FFieldCount-1 do
//    if FFieldMap[I] < 0 then
//    begin
//      Result:=False;
//      Exit;
//    end;
//end;
//
//function TD2GridDataLink.GetFields(I:integer):TField;
//begin
//  if (0 <= I) and (I < FFieldCount) and (FFieldMap[I] >= 0) then
//    Result:=DataSet.Fields[FFieldMap[I]]
//  else
//    Result:=nil;
//end;
//
//procedure TD2GridDataLink.ActiveChanged;
//begin
//  if Active and Assigned(DataSource) then
//    if Assigned(DataSource.DataSet) then
//      if DataSource.DataSet.IsUnidirectional then
//        DatabaseError('Operation not allowed on a unidirectional dataset');
//  FGrid.LinkActive(Active);
//  FModified:=False;
//end;
//
//procedure TD2GridDataLink.Modified;
//begin
//  FModified:=True;
//end;
//
//procedure TD2GridDataLink.DataSetChanged;
//begin
//  FGrid.DataSetChanged;
//  FModified:=False;
//end;
//
//procedure TD2GridDataLink.DataSetScrolled(Distance:integer);
//begin
//  FGrid.DataChanged;
//end;
//
//procedure TD2GridDataLink.LayoutChanged;
//begin
//  inherited LayoutChanged;
//end;
//
//procedure TD2GridDataLink.FocusControl(Field:TFieldRef);
//begin
//  if Assigned(Field) and Assigned(Field^) then
//  begin
//    FGrid.SelectedField:=Field^;
//  end;
//end;
//
//procedure TD2GridDataLink.EditingChanged;
//begin
//  FGrid.EditingChanged;
//end;
//
//procedure TD2GridDataLink.RecordChanged(Field:TField);
//begin
//  FGrid.RecordChanged(Field);
//  FModified:=False;
//end;
//
//procedure TD2GridDataLink.UpdateData;
//begin
//  FInUpdateData:=True;
//  try
//    if FModified then FGrid.UpdateData;
//    FModified:=False;
//  finally
//    FInUpdateData:=False;
//  end;
//end;
//
//function TD2GridDataLink.GetMappedIndex(ColIndex:integer):integer;
//begin
//  if (0 <= ColIndex) and (ColIndex < FFieldCount) then
//    Result:=FFieldMap[ColIndex]
//  else
//    Result:=-1;
//end;
//
//procedure TD2GridDataLink.Reset;
//begin
//  if FModified then RecordChanged(nil) else Dataset.Cancel;
//end;
//
//function TD2GridDataLink.IsAggRow(Value:integer):boolean;
//begin
//  Result:=False;
//end;
//
//procedure TD2GridDataLink.BuildAggMap;
//begin
//end;

{ TD2DBColumn }

//Deleted by GoldenFox
//constructor TD2DBColumn.Create(AOwner:TComponent);
//begin
//  inherited;
//end;

destructor TD2DBColumn.Destroy;
begin
  //if Assigned(FField) and (Grid <> nil) then
  //  FField.RemoveFreeNotification(Grid);
  FField:=nil;
  //FieldName:='';
  inherited;
end;


function TD2DBColumn.GetField:TField;
var
  G:TD2CustomDBGrid;
begin    { Returns Nil if FieldName can't be found in dataset }
  G:=TD2CustomDBGrid(Grid);
  if (FField=nil) and (Length(FFieldName) > 0) and Assigned(G) and Assigned(G.DataController.DataSet) then
  with G.DataController.Dataset do
    if Active or (not DefaultFields) then
      SetField(FindField(FieldName));
  Result:=FField;
end;

procedure TD2DBColumn.SetField(Value:TField);
begin
  if FField=Value then Exit;
  //if Assigned(FField) and (Grid <> nil) then
  //  FField.RemoveFreeNotification(Grid);
  if Assigned(Value) and (csDestroying in Value.ComponentState) then
    Value:=nil;    // don't acquire references to fields being destroyed
  FField:=Value;
  if Assigned(Value) then
  begin
    //if Grid <> nil then
    //  FField.FreeNotification(Grid);

    FFieldName:=Value.FieldName;
  end;
end;

procedure TD2DBColumn.SetFieldName(const Value:String);
begin
  if FFieldName <> Value then
  begin
    if Header=FFieldName then Header:=Value;   //Added by GoldenFox
    FField:=nil;                               //Added by GoldenFox
    FFieldName:=Value;
    //if Header='' then       //Deleted by GoldenFox
    //  Header:=FieldName;    //Deleted by GoldenFox
    UpdateColumn;
  end;
end;

function TD2DBColumn.GetData:Variant;
begin
  if Assigned(Field) then
    Result:=Field.Value;
end;

procedure TD2DBColumn.SetData(Value:Variant);
begin
  if Assigned(Field) then
    Field.Value:=Value
end;


{ TD2DBTextColumn }
procedure TD2DBTextColumn.SetData(Value:Variant); {procedure Added by GoldenFox}
begin
  if Assigned(Field) then
    Field.Text:=UTF8Encode(Value);
end;

function TD2DBTextColumn.GetData: Variant;
begin
  if Assigned(Field) then
    Result:= UTF8Decode(Field.DisplayText);
end;

{ TD2DBCheckColumn }

function TD2DBCheckColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2CheckCell.Create(Self);
  TD2CheckCell(Result).OnChange:=DoCheckChanged;
end;

procedure TD2DBCheckColumn.DoCheckChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  with d2StringToPoint(TD2Object(Sender).TagString) do
    TD2DBGrid(Grid).SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
end;

function TD2DBCheckColumn.GetData:Variant;
begin
  if Assigned(Field) then
    Result:=Field.AsBoolean
end;

{ TD2DBPopupColumn }

constructor TD2DBPopupColumn.Create(AOwner:TComponent);
begin
  inherited;
  FItems:=TD2WideStringList.Create;
end;

destructor TD2DBPopupColumn.Destroy;
begin
  FItems.Free;
  inherited;
end;

function TD2DBPopupColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2PopupCell.Create(Self);
  TD2PopupCell(Result).Items.Assign(FItems);
  TD2PopupCell(Result).OnChange:=DoPopupChanged;
end;

procedure TD2DBPopupColumn.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

procedure TD2DBPopupColumn.DoPopupChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  with d2StringToPoint(TD2Object(Sender).TagString) do
    TD2DBGrid(Grid).SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
end;

{ TD2DBImageColumn }

function TD2DBImageColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2ImageCell.Create(Self);
  TD2ImageCell(Result).OnChange:=DoImageChanged;
  TD2ImageCell(Result).EnableOpenDialog:=true;
end;

destructor TD2DBImageColumn.Destroy;
begin
  if FCurrent <> nil then
    FCurrent.Free;
  inherited;
end;

procedure TD2DBImageColumn.DoImageChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if not TD2DBGrid(Grid).CanEditModify then Exit;
  with d2StringToPoint(TD2Object(Sender).TagString) do
    TD2DBGrid(Grid).SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
end;

function TD2DBImageColumn.GetData:Variant;
begin
  if Assigned(Field) and Field.IsBlob and (TBlobField(Field).BlobSize > 0) then
  begin
    if FCurrent=nil then
      FCurrent:=TD2Bitmap.Create(1,1);
    FCurrent.Assign(Field);
    Result:=ObjectToVariant(FCurrent);
  end
  else
    Result:=NULL;
end;

procedure TD2DBImageColumn.SetData(Value:Variant);
var
  P:TPicture;
begin
  if Assigned(Field) and Field.IsBlob and VarIsObject(Value) then
  begin
    P:=TPicture.Create;
    P.Assign(TPersistent(VariantToObject(Value)));
    Field.Assign(P);
    P.Free;
  end
  else
    Field.Value:=NULL
end;

{ TD2DBProgressColumn }

constructor TD2DBProgressColumn.Create(AOwner:TComponent);
begin
  inherited;
  FMax:=100;
end;

function TD2DBProgressColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2ProgressCell.Create(Self);
  TD2ProgressCell(Result).Min:=FMin;
  TD2ProgressCell(Result).Max:=FMax;
end;

{ TD2DBGrid }

//constructor TD2DBGrid.Create(AOwner:TComponent);
//begin
//  inherited;
//  FResource:='gridstyle';
//  FDataLink:=TD2GridDataLink.Create(Self);
//  RowCount:=0;
//end;
//
//destructor TD2DBGrid.Destroy;
//begin
//  //FDataLink.Free
//  //FDataLink:=nil;
//  DataSource:=nil;
//  FreeAndNil(FDataLink);
//  inherited;
//end;
//
//procedure TD2DBGrid.Loaded;
//begin
//  inherited ;
//  DataChanged;
//end;
//
//function TD2DBGrid.ItemClass:string;
//begin
//  Result:='TD2DBTextColumn;TD2DBCheckColumn;TD2DBPopupColumn;TD2DBImageColumn;TD2DBProgressColumn';
//end;
//
//type
//  THack=class(TDataSet);
//
//procedure TD2DBGrid.UpdateRowCount;
//begin
//  if FDataLink.Active then
//  begin
//    FDataLink.BufferCount:=VisibleRows * 2;
//    if FDataLink.BufferCount < FDataLink.DataSet.RecordCount then
//      RowCount:=FDataLink.DataSet.RecordCount - FDataLink.BufferCount + FDataLink.RecordCount
//    else
//      RowCount:=FDataLink.RecordCount;
//  end
//  else
//    RowCount:=0;
//end;
//
//procedure TD2DBGrid.LinkActive(Value:boolean);
//begin
//  UpdateRowCount;
//  UpdateSelection;
//end;
//
//procedure TD2DBGrid.EditingChanged;
//begin
//  LinkActive(FDataLink.Active);
//end;
//
//procedure TD2DBGrid.RecordChanged(Field:TField);
//begin
//  if DataSource.State <> dsInactive then
//    UpdateColumns;
//end;
//
//procedure TD2DBGrid.DataChanged;
//begin
//  if (DataSource <> nil) and (DataSource.State <> dsInactive) then
//  begin
//    FDisableMove:=true;
//    if FDataLink.DataSet.RecNo > 0 then
//      Selected:=(FDataLink.DataSet.RecNo - 1);
//    FDisableMove:=false;
//    UpdateColumns;
//  end;
//end;
//
//procedure TD2DBGrid.DataSetChanged;
//begin
//  if (FDataLink.DataSet <> nil) then
//  begin
//    UpdateRowCount;
//    FDisableMove:=true;
//    if FDataLink.DataSet.RecNo > 0 then
//      Selected:=(FDataLink.DataSet.RecNo - 1);
//    FDisableMove:=false;
//    UpdateColumns;
//  end;
//end;
//
//function TD2DBGrid.GetValue(Col,Row:integer):Variant;
//var
//  C:TD2DBColumn;
//  F,OldActive:integer;
//begin
//  Result:=NULL;
//  if Assigned(DataLink) and DataLink.Active then
//  begin
//    C:=TD2DBColumn(Columns[Col]);
//    if (C <> nil) and (C.GetField <> nil) then
//    begin
//      OldActive:=FDataLink.ActiveRecord;
//      F:=(Selected - OldActive);
//      try
//        if (Row - F >= 0) and (Row - F < FDataLink.BufferCount) then
//        begin
//          FDatalink.ActiveRecord:=Row - F;
//          Result:=C.GetData;
//        end;
// 
//      finally
//        FDatalink.ActiveRecord:=OldActive;
//      end;
//    end;
//  end;
//end;
//
//procedure TD2DBGrid.SetValue(Col,Row:integer; const Value:Variant);
//var
//  C:TD2DBColumn;
//begin
//  if Assigned(DataLink) and DataLink.Active and not FDataLink.ReadOnly then
//  begin
//    C:=TD2DBColumn(Columns[Col]);
//    if (C <> nil) and (C.Field <> nil) and C.Field.CanModify then
//    begin
//      FDataLink.Modified;
//      FEditValue:=Value;
//      FNeedUpdate:=true;
//      FDatalink.UpdateData;
//    end;
//  end;
//end;
//
//function TD2DBGrid.GetDataSource:TDataSource;
//begin
//  Result:=FDataLink.DataSource;
//end;
//
//procedure TD2DBGrid.SetDataSource(const Value:TDataSource);
//begin
//  if not (FDataLink.DataSourceFixed and (csLoading in ComponentState)) then
//  begin
//    FDataLink.DataSource:=Value;
//  end;
//  //if Value <> nil then Value.FreeNotification(Self); //Deleted by GoldenFox
//end;
//
//procedure TD2DBGrid.Notification(AComponent:TComponent;
//  Operation:TOperation);
//begin
//  inherited;
//  if (Operation=opRemove) and (FDataLink <> nil) and
//    (AComponent=DataSource) then DataSource:=nil;
//end;
//
//procedure TD2DBGrid.UpdateData;
//var
//  C:TD2DBColumn;
//begin
//  C:=TD2DBColumn(Columns[ColumnIndex]);
//  if (C <> nil) and FNeedUpdate then
//    TD2DBColumn(Columns[ColumnIndex]).SetData(FEditValue);
//  FEditValue:=NULL;
//  FNeedUpdate:=false;
//end;
//
//procedure TD2DBGrid.KeyDown(var Key:Word; var KeyChar:System.WideChar;
//  Shift:TShiftState);
//
//  procedure DoSelection(Select:boolean; Direction:integer);
//  begin
//    BeginUpdate;
//    try
//      FDatalink.MoveBy(Direction);
//    finally
//      EndUpdate;
//    end;
//  end;
//
//  procedure NextRow(Select:boolean);
//  begin
//    with FDatalink.Dataset do
//    begin
//      if (State=dsInsert) and not Modified and not FDatalink.FModified then
//        if FDataLink.EOF then Exit else Cancel
//      else
//        DoSelection(Select,1);
//      if FDataLink.EOF and CanModify and (not ReadOnly) then
//        Append;
//    end;
//  end;
//
//  procedure PriorRow(Select:boolean);
//  begin
//    with FDatalink.Dataset do
//      if (State=dsInsert) and not Modified and FDataLink.EOF and
//        not FDatalink.FModified then
//        Cancel
//      else
//        DoSelection(Select,-1);
//  end;
//
//begin
//  if (RowCount > 0) and (FDataLink.Active) then
//  begin
//    case Key of
//      VK_UP:PriorRow(True);
//      VK_DOWN:NextRow(True);
//      VK_NEXT:FDataLink.MoveBy(VisibleRows);
//      VK_PRIOR:FDataLink.MoveBy(-VisibleRows);
//      VK_DELETE:
//        if (not ReadOnly) and not FDataLink.DataSet.IsEmpty
//          and FDataLink.DataSet.CanModify
//        then
//          FDataLink.DataSet.Delete;
//      VK_HOME:
//        if ssCtrl in Shift then
//          FDataLink.DataSet.First
//        else
//          ColumnIndex:=0;
//      VK_END:
//        if ssCtrl in Shift then
//          FDataLink.DataSet.Last
//        else
//          ColumnIndex:=ColumnCount - 1;
//      VK_INSERT:
//        if FDataLink.DataSet.CanModify and (not ReadOnly) then
//        begin
//          FDataLink.DataSet.Insert;
//        end;
//      VK_ESCAPE:
//        begin
//          FDatalink.Reset;
//          SetFocus;
//        end;
//    else
//      inherited ;
//      Exit;
//    end;
//    Key:=0;
//  end;
//  inherited ;
//end;
//
//procedure TD2DBGrid.SetSelected(const Value:integer);
//var
//  S:integer;
//begin
//  S:=Selected;
//  if Selected <> Value then
//  begin
//    inherited ;
//    if not FDisableMove then
//    begin
//      if FDataLink.Active then
//        FDataLink.MoveBy(Selected - S);
//    end;
//    UpdateSelection;
//  end;
//end;
//
//function TD2DBGrid.GetSelectedField:TField;
//begin
//  Result:=TD2DBColumn(Columns[ColumnIndex]).Field;
//end;
//
//procedure TD2DBGrid.SetSelectedField(const Value:TField);
//var
//  i:integer;
//begin
//  for i:=0 to ColumnCount - 1 do
//  begin
//    if TD2DBColumn(Columns[i]).Field=Value then
//    begin
//      ColumnIndex:=i;
//      Break;
//    end;
//  end;
//end;
//
//function TD2DBGrid.CanEditAcceptKey(Key:System.WideChar):boolean;
//begin
//  with TD2DBColumn(Columns[ColumnIndex]) do
//    Result:=FDatalink.Active and Assigned(Field) and Field.IsValidChar(Char(Key));
//end;
//
//function TD2DBGrid.CanEditModify:boolean;
//begin
//  Result:=False;
//  if not ReadOnly and FDatalink.Active and not FDatalink.Readonly then
//  with TD2DBColumn(Columns[ColumnIndex]) do
//    if (not ReadOnly) and Assigned(Field) and Field.CanModify then
//    begin
//      FDatalink.Edit;
//      Result:=FDatalink.Editing;
//      if Result then FDatalink.Modified;
//    end;
//end;
//
//procedure TD2DBGrid.Reset;
//begin
//  inherited;
//  if FDatalink.Active then
//    FDatalink.Reset;
//end;
//
//procedure TD2DBGrid.VScrollChange(Sender:TObject);
//begin
//  inherited;
//  //if FDataLink.Active then
//  //begin
//  //  if TopRow < Selected - FDataLink.ActiveRecord then
//  //    Selected:=TopRow;
//  //  if TopRow + VisibleRows - 1 > Selected - FDataLink.ActiveRecord + FDataLink.BufferCount then
//  //  begin
//  //    Selected:=TopRow + VisibleRows - 1;
//  //    Selected:=Selected;
//  //  end;
//  //end;
//end;
//
//function TD2DBGrid.GetContentBounds:TD2Rect;
//begin
//  Result:=inherited GetContentBounds;
//  UpdateRowCount;
//end;

{ TD2CustomDBGrid }

function TD2CustomDBGrid.GetDataSource:TDataSource;
begin
  Result:=FDataController.DataSource;
end;

procedure TD2CustomDBGrid.SetDataSource(const Value:TDataSource);
begin
  if not (FDataController.DataSourceFixed and (csLoading in ComponentState))
    then FDataController.DataSource:=Value;
end;

function TD2CustomDBGrid.GetSelectedField: TField;
begin
  Result:=TD2DBColumn(Columns[ColumnIndex]).Field;
end;

procedure TD2CustomDBGrid.SetSelectedField(const Value: TField);
var
  i:integer;
begin
  for i:=0 to ColumnCount - 1 do
  begin
    if TD2DBColumn(Columns[i]).Field=Value then
    begin
      ColumnIndex:=i;
      Break;
    end;
  end;
end;

procedure TD2CustomDBGrid.SetDataController(const AValue: TD2GridDataController);
begin
  FDataController.Assign(AValue);
end;

procedure TD2CustomDBGrid.UpdateRowCount;
begin
  if FDataController.Active then
  begin
    if FMultiSelect
    then begin
           FDataController.BufferCount:=FDataController.DataSet.RecordCount;
           RowCount:=FDataController.DataSet.RecordCount;
         end
    else begin
           FDataController.BufferCount:=VisibleRows * 2;
           if FDataController.BufferCount < FDataController.DataSet.RecordCount
             then RowCount:=FDataController.DataSet.RecordCount - FDataController.BufferCount + FDataController.RecordCount
             else RowCount:=FDataController.RecordCount;
         end;
  end
  else RowCount:=0;
end;

procedure TD2CustomDBGrid.OnRecordChanged(Field: TField);
begin
  if FDataController.DataSource.State <> dsInactive then
    UpdateColumns;
end;

procedure TD2CustomDBGrid.OnDataSetChanged(aDataSet: TDataSet);
begin
  if (FDataController.DataSet <> nil) then
  begin
    UpdateRowCount;
    FDisableMove:=true;
    if FDataController.DataSet.RecNo > 0 then
      Selected:=(FDataController.DataSet.RecNo - 1);
    FDisableMove:=false;
    UpdateColumns;
  end;
end;

procedure TD2CustomDBGrid.OnDataSetOpen(aDataSet: TDataSet);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnDataSetClose(aDataSet: TDataSet);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnEditingChanged(aDataSet: TDataSet);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnInvalidDataSet(aDataSet: TDataSet);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnInvalidDataSource(aDataSet: TDataset);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnLayoutChanged(aDataSet: TDataSet);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnNewDataSet(aDataSet: TDataset);
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBGrid.OnDataSetScrolled(aDataSet: TDataSet;
  Distance: Integer);
begin
  if (FDataController.DataSource <> nil) and (FDataController.DataSource.State <> dsInactive) then
  begin
    FDisableMove:=true;
    if FDataController.DataSet.RecNo > 0 then
      Selected:=(FDataController.DataSet.RecNo - 1);
    FDisableMove:=false;
    UpdateColumns;
  end;
end;

function TD2CustomDBGrid.GetValue(Col, Row: integer): Variant;
var
  C:TD2DBColumn;
  F,OldActive:integer;
begin
  Result:=NULL;
  if Assigned(FDataController) and FDataController.Active then
  begin
    C:=TD2DBColumn(Columns[Col]);
    if (C <> nil) and (C.GetField <> nil) then
    begin
      OldActive:=FDataController.ActiveRecord;
      F:=(Selected - OldActive);
      try
        if (Row - F >= 0) and (Row - F < FDataController.BufferCount) then
        begin
          FDataController.ActiveRecord:=Row - F;
          Result:=C.GetData;
        end;

      finally
        FDataController.ActiveRecord:=OldActive;
      end;
    end;
  end;
end;

procedure TD2CustomDBGrid.SetValue(Col, Row: integer; const Value: Variant);
var C: TD2DBColumn;
begin
  if Assigned(FDataController) and FDataController.Active and not FDataController.ReadOnly then
  begin
    C:=TD2DBColumn(Columns[Col]);
    if (C <> nil) and (C.Field <> nil) and C.Field.CanModify then
    begin
      FDataController.Modified:=True;
      FEditValue:=Value;
      FNeedUpdate:=true;
      FDataController.UpdateData;
    end;
  end;
end;

function TD2CustomDBGrid.CanEditAcceptKey(Key: System.WideChar): Boolean;
begin
  with TD2DBColumn(Columns[ColumnIndex]) do
      Result:=FDataController.Active and Assigned(Field) and Field.IsValidChar(Char(Key));
end;

function TD2CustomDBGrid.CanEditModify: Boolean;
begin
  Result:=False;
  if not ReadOnly and FDataController.Active and not FDataController.Readonly then
  with TD2DBColumn(Columns[ColumnIndex]) do
    if (not ReadOnly) and Assigned(Field) and Field.CanModify then
    begin
      FDataController.Edit;
      Result:=FDataController.Editing;
      if Result then FDataController.Modified:=True;
    end;
end;

procedure TD2CustomDBGrid.KeyDown(var Key: Word; var KeyChar: System.WideChar;
  Shift: TShiftState);

  procedure DoSelection(Select:boolean; Direction:integer);
  begin
    BeginUpdate;
    try
      FDataController.MoveBy(Direction);
    finally
      EndUpdate;
    end;
  end;

  procedure NextRow(Select:boolean);
  begin
    with FDataController.Dataset do
    begin
      if (State=dsInsert) and not Modified and not FDataController.Modified then
        if FDataController.EOF then Exit else Cancel
      else
        DoSelection(Select,1);
      //if FDataController.EOF and CanModify and (not ReadOnly) then
      //  Append;
    end;
  end;

  procedure PriorRow(Select:boolean);
  begin
    with FDataController.Dataset do
      if (State=dsInsert) and not Modified and FDataController.EOF and
        not FDataController.Modified then
        Cancel
      else
        DoSelection(Select,-1);
  end;

begin
  if (RowCount > 0) and (FDataController.Active) then
  begin
    case Key of
      VK_UP:PriorRow(True);
      VK_DOWN:NextRow(True);
      VK_NEXT:FDataController.MoveBy(VisibleRows);
      VK_PRIOR:FDataController.MoveBy(-VisibleRows);
      VK_DELETE:
        if (not ReadOnly) and not FDataController.DataSet.IsEmpty
          and FDataController.DataSet.CanModify
        then
          FDataController.DataSet.Delete;
      VK_HOME:
        if ssCtrl in Shift then
          FDataController.DataSet.First
        else
          ColumnIndex:=0;
      VK_END:
        if ssCtrl in Shift then
          FDataController.DataSet.Last
        else
          ColumnIndex:=ColumnCount - 1;
      VK_INSERT:
        if FDataController.DataSet.CanModify and (not ReadOnly) then
        begin
          FDataController.DataSet.Insert;
        end;
      VK_ESCAPE:
        begin
          if FDataController.Modified then
            FDataController.RecordChanged(nil) else FDataController.Dataset.Cancel;
          SetFocus;
        end;
    else
      inherited ;
      Exit;
    end;
    Key:=0;
  end;
  inherited;
end;

procedure TD2CustomDBGrid.Reset;
begin
  inherited;
  if FDataController.Active and FDataController.Modified
    then FDataController.RecordChanged(nil) else FDataController.Dataset.Cancel;
end;

procedure TD2CustomDBGrid.OnUpdateData(aDataSet: TDataSet);
var C: TD2DBColumn;
begin
  C:=TD2DBColumn(Columns[ColumnIndex]);
  if (C <> nil) and FNeedUpdate then
    TD2DBColumn(Columns[ColumnIndex]).SetData(FEditValue);
  FEditValue:=NULL;
  FNeedUpdate:=false;
end;

procedure TD2CustomDBGrid.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation=opRemove) and (FDataController <> nil) and
    (AComponent=FDataController.DataSource) then FDataController.DataSource:=nil;
end;

procedure TD2CustomDBGrid.Loaded;
begin
  inherited;
  OnDataSetScrolled(DataController.DataSet, 0);
end;

procedure TD2CustomDBGrid.LinkActive(Value: Boolean);
begin
  UpdateRowCount;
  UpdateSelection;
end;

function TD2CustomDBGrid.GetContentBounds: TD2Rect;
begin
  Result:=inherited GetContentBounds;
  UpdateRowCount;
end;

procedure TD2CustomDBGrid.SetSelected(const Value: integer);
var
  S:integer;
begin
  S:=Selected;
  if Selected <> Value then
  begin
    inherited ;
    if not FDisableMove then
    begin
      if FDataController.Active then
        FDataController.MoveBy(Selected - S);
    end;
    UpdateSelection;
  end;
end;

constructor TD2CustomDBGrid.Create(AOwner: TComponent);
begin
  inherited;
  FResource:='gridstyle';
  FDataController := TD2GridDataController.Create;//(Self);
  FDataController.OnRecordChanged:=OnRecordChanged;
  FDataController.OnDatasetChanged:=OnDataSetChanged;
  FDataController.OnDataSetOpen:=OnDataSetOpen;
  FDataController.OnDataSetClose:=OnDataSetClose;
  FDataController.OnNewDataSet:=OnNewDataSet;
  FDataController.OnInvalidDataSet:=OnInvalidDataset;
  FDataController.OnInvalidDataSource:=OnInvalidDataSource;
  FDataController.OnDataSetScrolled:=OnDataSetScrolled;
  FDataController.OnLayoutChanged:=OnLayoutChanged;
  FDataController.OnEditingChanged:=OnEditingChanged;
  FDataController.OnUpdateData:=OnUpdateData;
  FDataController.VisualControl:= True;

  //FSelectedRows := TBookmarkList.Create(Self);
  RowCount:=0;
  FDisableMove:=false;
end;

destructor TD2CustomDBGrid.Destroy;
begin
  //FSelectedRows.Free;

  FDataController.OnDataSetChanged:=nil;
  FDataController.OnRecordChanged:=nil;
  FDataController.Free;
  inherited Destroy;
end;

function TD2CustomDBGrid.ItemClass: string;
begin
  Result:='TD2DBTextColumn;TD2DBCheckColumn;TD2DBPopupColumn;TD2DBImageColumn;TD2DBProgressColumn';
end;
