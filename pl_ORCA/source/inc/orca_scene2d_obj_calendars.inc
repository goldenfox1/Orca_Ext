{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Calendar ===========================================================

constructor TD2Calendar.Create(AOwner:TComponent);
var
  i:integer;
  L:TD2VisualObject;
  AYear,AMonth,ADay:Word;
begin
  inherited;
  FDateTime:=Now;
  DecodeDate(FDateTime,AYear,AMonth,ADay);
  FFirstDayOfWeek:=d2LocaleDefault;
  Width:=180;
  Height:=160;
  L:=TD2Layout.Create(Self);
  with L do
  begin
    Parent:=Self;
    Locked:=true;
    Stored:=false;
    Height:=19;
    Align:=vaMostTop;
    Padding.Bottom:=2;
    FPrev:=TD2Button.Create(Self);
    with FPrev do
    begin
      Parent:=L;
      Width:=19;
      Locked:=true;
      Stored:=false;
      Align:=vaLeft;
      Padding.Right:=2;
      Resource:='transparentcirclebuttonstyle';
      OnClick:=DoPrevClick;
      RepeatClick:=true;
      with TD2ScrollArrowLeft.Create(Self) do
      begin
        Parent:=FPrev;
        Width:=5;
        Height:=5;
        Stroke.Style:=d2BrushNone;
        Align:=vaCenter;
        HitTest:=false;
        Stored:=false;
        Locked:=true;
      end;
    end;
    FToday:=TD2Button.Create(Self);
    with FToday do
    begin
      Parent:=L;
      Width:=19;
      Locked:=true;
      Stored:=false;
      Align:=vaLeft;
      Position.X:=30;
      Padding.Right:=2;
      Resource:='transparentcirclebuttonstyle';
      OnClick:=DoTodayClick;
      RepeatClick:=true;
      with TD2Ellipse.Create(Self) do
      begin
        Parent:=FToday;
        Width:=5;
        Height:=5;
        Stroke.Style:=d2BrushNone;
        Align:=vaCenter;
        HitTest:=false;
        Stored:=false;
        Locked:=true;
      end;
    end;
    FNext:=TD2Button.Create(Self);
    with FNext do
    begin
      Parent:=L;
      Width:=19;
      Locked:=true;
      Stored:=false;
      Position.X:=50;
      Align:=vaLeft;
      Padding.Right:=2;
      Resource:='transparentcirclebuttonstyle';
      RepeatClick:=true;
      OnClick:=DoNextClick;
      with TD2ScrollArrowRight.Create(Self) do
      begin
        Parent:=FNext;
        Width:=5;
        Height:=5;
        Stroke.Style:=d2BrushNone;
        Align:=vaCenter;
        HitTest:=false;
        Stored:=false;
        Locked:=true;
      end;
    end;
    FMonths:=TD2PopupBox.Create(Self);
    with FMonths do
    begin
      Parent:=L;
      Align:=vaClient;
      Locked:=true;
      Stored:=false;
      DisableFocusEffect:=true;
      Padding.Left:=5;
      Padding.Right:=5;
      Resource:='labelstyle';
      for i:=1 to 12 do
        Items.Add(LongMonthNames[i]);
      Font.Style:=d2FontBold;
      TextAlign:=d2TextAlignFar;
      ItemIndex:=AMonth - 1;
      OnChange:=DoMonthChange;
    end;
    FYears:=TD2PopupBox.Create(Self);
    with FYears do
    begin
      Parent:=L;
      Width:=40;
      Align:=vaRight;
      Locked:=true;
      Stored:=false;
      DisableFocusEffect:=true;
      Resource:='labelstyle';
      for i:=1 to 10 do
        Items.Add(IntToStr(AYear - i));
      Items.Add(IntToStr(AYear));
      for i:=1 to 10 do
        Items.Add(IntToStr(AYear + i));
      Font.Style:=d2FontBold;
      TextAlign:=d2TextAlignNear;
      ItemIndex:=10;
      OnChange:=DoYearChange;
    end;
  end;
  FWeek:=TD2GridLayout.Create(Self);
  with FWeek do
  begin
    Parent:=Self;
    Locked:=true;
    Stored:=false;
    Height:=19;
    Position.Y:=20;
    ItemHeight:=19;
    Align:=vaTop;
    Padding.Bottom:=2;
    for i:=0 to 6 do
      with TD2Label.Create(Self) do
      begin
        Parent:=FWeek;
        Locked:=true;
        Stored:=false;
        TextAlign:=d2TextAlignCenter;
        WordWrap:=false;
      end;
    ItemWidth:=Width / 7;
  end;
  FWeeks:=TD2GridLayout.Create(Self);
  with FWeeks do
  begin
    Parent:=Self;
    Locked:=true;
    Stored:=false;
    Width:=23;
    Align:=vaMostLeft;
    Padding.Top:=19 + 2;
    ItemHeight:=19;
    ItemWidth:=Width;
    Visible:=false;
    for i:=0 to 5 do
      with TD2Label.Create(Self) do
      begin
        Parent:=FWeeks;
        Locked:=true;
        Stored:=false;
        TextAlign:=d2TextAlignCenter;
        WordWrap:=false;
      end;
  end;
  FDays:=TD2ListBox.Create(Self);
  with FDays do
  begin
    Parent:=Self;
    Locked:=true;
    Stored:=false;
    Position.Y:=50;
    Height:=19 * 6;
    Align:=vaTop;
    Columns:=7;
    ItemHeight:=19;
    AlternatingRowBackground:=true;
    HideSelectionUnfocused:=false;
    ShowScrollBars:=false;
    OnChange:=DoDayChange;
    Resource:='transparentlistboxstyle';
    for i:=1 to 6 * 7 do
      with TD2ListBoxItem.Create(Self) do
      begin
        Parent:=FDays;
        Locked:=true;
        Stored:=false;
        TextAlign:=d2TextAlignFar;
        WordWrap:=false;
      end;
  end;
  FillList;
end;

destructor TD2Calendar.Destroy;
begin
  inherited;
end;

procedure TD2Calendar.FillList;
var
  i:integer;
  AYear,PreMonth,AMonth,ADay:Word;
  Date:TDate;
  //First,Last:integer;	//Deleted by GoldenFox
  First:integer;	//Added by GoldenFox
  A:array[0..1] of char;
  Item:TD2ListBoxItem;
begin
  FDisableDayChange:=FDisableDayChange + 1;
  try
   
    if FFirstDayOfWeek=d2LocaleDefault then
    begin
      {$IFDEF WINDOWS}
      GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IFIRSTDAYOFWEEK,A,SizeOf(A));
      {$ENDIF}
      FFirstDayOfWeekNum:=Ord(A[0]) - Ord('0');
    end
    else
      FFirstDayOfWeekNum:=Ord(FFirstDayOfWeek);
    FFirstDayOfWeekNum:=(8 + FFirstDayOfWeekNum) mod 7;
   
    for i:=0 to 6 do
      TD2Label(FWeek.Children[i]).TextW:=UTF8Encode(ShortDayNames[1 + ((7 + i + FFirstDayOfWeekNum) mod 7)]);

    DecodeDate(FDateTime,AYear,AMonth,ADay);
    PreMonth:=AMonth - 1;
    if PreMonth < 1 then
      PreMonth:=12;
    Date:=EncodeDate(AYear,AMonth,1);
    First:=DayOfWeek(Date);
    if First - FFirstDayOfWeekNum < 3 then
      First:=First + 7;
    if FDays.Count - (First + MonthDays[IsLeapYear(AYear),AMonth] - FFirstDayOfWeekNum) < 3 then
      First:=First - 7;
    FDays.Tag:=First; // store first
    Date:=IncMonth(Date,1);
    //Last:=DayOfWeek(Date);	//Deleted by GoldenFox
    
    for i:=1 to First do
    begin
      Item:=FDays.Items[i - 1];
      if Item=nil then Continue;
      Item.Opacity:=0.3;
      Item.TextW:=IntToStr(MonthDays[IsLeapYear(AYear),PreMonth] - First + i + 1 + FFirstDayOfWeekNum);
    end;
    
    for i:=1 to MonthDays[IsLeapYear(AYear),AMonth] do
    begin
      Item:=FDays.Items[First + i - 2 - FFirstDayOfWeekNum];
      if Item=nil then Continue;
      Item.Opacity:=1;
      Item.TextW:=IntToStr(i);
    end;
    
    for i:=First + MonthDays[IsLeapYear(AYear),AMonth] to FDays.Count + FFirstDayOfWeekNum do
    begin
      Item:=FDays.Items[i - 1 - FFirstDayOfWeekNum];
      if Item=nil then Continue;
      Item.Opacity:=0.3;
      Item.TextW:=IntToStr(i - First - MonthDays[IsLeapYear(AYear),AMonth] + 1);
    end;
    
    
    if FWeekNumbers then
    begin
      FWeeks.Visible:=true;
      DecodeDate(FDateTime,AYear,AMonth,ADay);
      Date:=EncodeDate(AYear,AMonth,1);
      for i:=0 to 5 do
        if WeekOfYear(Date) + i=0 then
          TD2Label(FWeeks.Children[i]).TextW:=IntToStr(52)
        else
        if WeekOfYear(Date)=0 then
          TD2Label(FWeeks.Children[i]).TextW:=IntToStr(i)
        else
        if WeekOfYear(Date) + i > 52 then
          TD2Label(FWeeks.Children[i]).TextW:=IntToStr(WeekOfYear(Date) + i - 52)
        else
          TD2Label(FWeeks.Children[i]).TextW:=IntToStr(WeekOfYear(Date) + i);
    end
    else
      FWeeks.Visible:=false;
   
    FDays.ItemIndex:=First + ADay - 2 - FFirstDayOfWeekNum;
   
    FMonths.ItemIndex:=AMonth - 1;
  
    FYears.Items.Clear;
    
    for i:=10 downto 1 do
      FYears.Items.Add(IntToStr(AYear - i));
      
    FYears.Items.Add(IntToStr(AYear));
    for i:=1 to 10 do
      FYears.Items.Add(IntToStr(AYear + i));
      
    FYears.TextW:=IntToStr(AYear);
  finally
    FDisableDayChange:=FDisableDayChange - 1;
  end;
end;

function TD2Calendar.GetData:Variant;
begin
  Result:=VarFromDateTime(FDateTime);
end;

procedure TD2Calendar.SetData(const Value:Variant);
var
  D:TDateTime;
begin
  if VarIsType(Value,varDate) then
    Date:=VarToDateTime(Value)
  else
    if VarIsStr(Value) and TryStrToDateTime(AnsiString(Value),D) then
      Date:=D;
end;

procedure TD2Calendar.DoDayChange(Sender:TObject);
var
  AYear,AMonth,ADay:Word;
begin
  DecodeDate(FDateTime,AYear,AMonth,ADay);
  if FDays.ItemIndex - FDays.Tag + 2 + FFirstDayOfWeekNum < 1 then
  begin
  end
  else
  if FDays.ItemIndex - FDays.Tag + 2 + FFirstDayOfWeekNum > MonthDays[IsLeapYear(AYear),AMonth] then
  begin
  end
  else
    DateTime:=EncodeDate(AYear,AMonth,FDays.ItemIndex - FDays.Tag + 2 + FFirstDayOfWeekNum);
  if Assigned(FOnDayChange) and (FDisableDayChange=0) then
    FOnDayChange(Self);
end;

procedure TD2Calendar.DoTodayClick(Sender:TObject);
begin
  Date:=Now;
  if Assigned(FOnDayChange) and (FDisableDayChange=0) then
    FOnDayChange(Self);
end;

procedure TD2Calendar.DoNextClick(Sender:TObject);
begin
  Date:=IncMonth(Date,1);
end;

procedure TD2Calendar.DoPrevClick(Sender:TObject);
begin
  Date:=IncMonth(Date,-1);
end;

procedure TD2Calendar.DoMonthChange(Sender:TObject);
var
  AYear,AMonth,ADay:Word;
begin
  DecodeDate(FDateTime,AYear,AMonth,ADay);
  DateTime:=EncodeDate(AYear,FMonths.ItemIndex + 1,ADay);
end;

procedure TD2Calendar.DoYearChange(Sender:TObject);
var
  AYear,AMonth,ADay:Word;
begin
  DecodeDate(FDateTime,AYear,AMonth,ADay);
  DateTime:=EncodeDate(StrToInt(FYears.TextW),AMonth,ADay);
end;

procedure TD2Calendar.Realign;
begin
  inherited;
  if FWeek <> nil then
    FWeek.ItemWidth:=FWeek.Width / 7 - 0.1;
end;

function TD2Calendar.GetDate:TDate;
begin
  Result:=TDate(FDateTime);
end;

procedure TD2Calendar.SetDate(Value:TDate);
begin
  FDisableDayChange:=FDisableDayChange + 1;
  ReplaceTime(TDateTime(Value),FDateTime);
  try
    SetDateTime(Value);
  except
    SetDateTime(FDateTime);
    raise;
  end;
  FDisableDayChange:=FDisableDayChange - 1;
end;

procedure TD2Calendar.SetDateTime(const Value:TDateTime);
begin
  if FDateTime <> Value then
  begin
    FDateTime:=Value;
    FillList;
    if Assigned(FBindingObjects) then
      ToBindingObjects;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2Calendar.SetFirstDayOfWeek(const Value:TD2CalDayOfWeek);
begin
  if FFirstDayOfWeek <> Value then
  begin
    FFirstDayOfWeek:=Value;
    FillList;
  end;
end;

procedure TD2Calendar.SetTodayDefault(const Value:boolean);
begin
  FTodayDefault:=Value;
  if FTodayDefault then Date:=Now;
end;

procedure TD2Calendar.SetWeekNumbers(const Value:boolean);
begin
  if FWeekNumbers <> Value then
  begin
    FWeekNumbers:=Value;
    FillList;
  end;
end;

procedure TD2Calendar.MouseWheel(Shift:TShiftState; WheelDelta:integer;
  var Handled:boolean);
begin
  inherited;
  if not Handled then
  begin
    if WheelDelta > 0 then
      Date:=IncMonth(Date,-1)
    else
      Date:=IncMonth(Date,1);
    Handled:=true;
  end;
end;
        
        
         

            
  
