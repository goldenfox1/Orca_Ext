{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

function goodCompareObj(const a1,a2:TObject):integer;
  begin
    Result:=integer(Pointer(a1)) - integer(Pointer(a2));
    if Result <> 0 then
    begin
      if Result < 0 then
        Result:=1
      else
        Result:=-1;
      Exit;
    end;
  end;

//================= TD2WideStrings =======================================

constructor TD2WideStrings.Create;
begin
  inherited Create;
end;

destructor TD2WideStrings.Destroy;
begin
  inherited;
end;

function TD2WideStrings.CompareStrings(const S1,S2:WideString):integer;
begin
  Result:=0;
end;

procedure TD2WideStrings.Delete(Index:integer);
begin
end;

function TD2WideStrings.Add(const S:WideString):integer;
begin
  Result:=GetCount;
  Insert(Result,S);
end;

function TD2WideStrings.Items(AIndex:integer):WideString;
begin
  Result:=Strings[AIndex];
end;

function TD2WideStrings.AddObject(const S:WideString; AObject:TObject):integer;
begin
  Result:=Add(S);
  PutObject(Result,AObject);
end;

procedure TD2WideStrings.Append(const S:WideString);
begin
  Add(S);
end;

procedure TD2WideStrings.AddStrings(Strings:TStrings);
var
  I:integer;
begin
  BeginUpdate;
  try
    for I:=0 to Strings.Count - 1 do AddObject(Strings[I],Strings.Objects[I]);
  finally
    EndUpdate;
  end;
end;

procedure TD2WideStrings.AddStrings(Strings:TD2WideStrings);
var
  I:integer;
begin
  BeginUpdate;
  try
    for I:=0 to Strings.Count - 1 do AddObject(Strings[I],Strings.Objects[I]);
  finally
    EndUpdate;
  end;
end;

procedure TD2WideStrings.Assign(Source:TPersistent);
var
  I:integer;
begin
  if Source is TD2WideStrings then
  begin
    BeginUpdate;
    try
      Clear;
      AddStrings(TD2WideStrings(Source));
    finally
      EndUpdate;
    end;
  end
  else
    if Source is TStrings then
    begin
      BeginUpdate;
      try
        Clear;
        for I:=0 to TStrings(Source).Count - 1 do
          AddObject(TStrings(Source)[I],TStrings(Source).Objects[I]);
      finally
        EndUpdate;
      end;
    end
    else inherited Assign(Source);
end;

procedure TD2WideStrings.AssignTo(Dest:TPersistent);
var
  I:integer;
begin
  if Dest is TStrings then
    with Dest as TStrings do
    begin
      BeginUpdate;
      try
        Clear;
        for I:=0 to Self.Count - 1 do
          AddObject(Self[I],Self.Objects[I]);
      finally
        EndUpdate;
      end;
    end
    else
      if Dest is TD2WideStrings then
        with Dest as TD2WideStrings do
        begin
          BeginUpdate;
          try
            Clear;
            AddStrings(Self);
          finally
            EndUpdate;
          end;
        end
        else inherited;
end;

procedure TD2WideStrings.BeginUpdate;
begin
  if FUpdateCount=0 then
    SetUpdateState(True);
  System.Inc(FUpdateCount);
end;

procedure TD2WideStrings.clear;
begin

end;

procedure TD2WideStrings.DefineProperties(Filer:TFiler);
  function DoWrite:boolean;
  begin
    if Filer.Ancestor <> nil then
    begin
      Result:=True;
      if Filer.Ancestor is TD2WideStrings then
        Result:=not Equals(TD2WideStrings(Filer.Ancestor))
    end
    else
      Result:=Count > 0;
  end;
begin
  Filer.DefineProperty('strings',ReadData,WriteData,DoWrite);
end;

procedure TD2WideStrings.EndUpdate;
begin
  System.Dec(FUpdateCount);
  if FUpdateCount=0 then SetUpdateState(False);
end;

function TD2WideStrings.Equals(Strings:TD2WideStrings):boolean;
var
  I,Count:integer;
begin
  Result:=False;
  Count:=GetCount;
  if Count <> Strings.GetCount then Exit;
  for I:=0 to Count - 1 do
    if Get(I) <> Strings.Get(I) then Exit;
  Result:=True;
end;

procedure TD2WideStrings.Error(const Msg:String; Data:integer);
begin

end;

procedure TD2WideStrings.Exchange(Index1,Index2:integer);
var
  TempObject:TObject;
  TempString:WideString;
begin
  BeginUpdate;
  try
    TempString:=Strings[Index1];
    TempObject:=Objects[Index1];
    Strings[Index1]:=Strings[Index2];
    Objects[Index1]:=Objects[Index2];
    Strings[Index2]:=TempString;
    Objects[Index2]:=TempObject;
  finally
    EndUpdate;
  end;
end;

function TD2WideStrings.GetCapacity:integer;
begin
  Result:=Count;
end;

function TD2WideStrings.GetCommaText:WideString;
var
  S:WideString;
  P:PWideChar;
  I,
  Count:integer;
begin
  Count:=GetCount;
  if (Count=1) and (Get(0)='') then
    Result:=''
  else
  begin
    Result:='';
    for I:=0 to Count - 1 do
    begin
      S:=Get(I);
      P:=PWideChar(S);
      while not (P^ in [d2WideNull..d2WideSpace,System.WideChar('"'),System.WideChar(',')]) do
       System.Inc(P);
//!!!      if (P^ <> WideNull) then S:=WideQuotedStr(S,'"');
      Result:=Result + S + ',';
    end;
    System.Delete(Result,Length(Result),1);
  end;
end;

function TD2WideStrings.GetName(Index:integer):WideString;
var
  P:integer;
begin
  Result:=Get(Index);
  P:=Pos(WideString('='),Result);
  if P > 0 then SetLength(Result,P - 1)
           else Result:='';
end;

function TD2WideStrings.GetObject(Index:integer):TObject;
begin
  Result:=nil;
end;

function TD2WideStrings.GetTextStr:WideString;
var
  I,L,Size,Count:integer;
  P:PWideChar;
  S:WideString;
begin
  Count:=GetCount;
  Size:=0;
  for I:=0 to Count - 1 do
    System.Inc(Size,Length(Get(I)) + 2);
  SetLength(Result,Size);
  P:=Pointer(Result);
  for I:=0 to Count - 1 do
  begin
    S:=Get(I);
    L:=Length(S);
    if L <> 0 then
    begin
      System.Move(Pointer(S)^,P^,2 * L);
      System.Inc(P,L);
    end;
    P^:=d2WideCarriageReturn;
    System.Inc(P);
    P^:=d2WideLineFeed;
    System.Inc(P);
  end;
end;

function TD2WideStrings.GetValueFromIndex(Index:integer):WideString;
begin
  if Index >= 0 then
    Result:=Copy(Get(Index),Length(Names[Index]) + 2,System.MaxInt) else
    Result:='';
end;

procedure TD2WideStrings.SetValueFromIndex(Index:integer; const Value:WideString);
begin
  if Value <> '' then
  begin
    if Index < 0 then Index:=Add('');
    Put(Index,Names[Index] + '=' + Value);
  end
  else
    if Index >= 0 then Delete(Index);
end;

procedure TD2WideStrings.sort;
begin
end;

function TD2WideStrings.GetValue(const Name:WideString):WideString;
var
  I:integer;
begin
  I:=IndexOfName(Name);
  if I >= 0 then Result:=Copy(Get(I),Length(Name) + 2,System.MaxInt)
            else Result:='';
end;

function TD2WideStrings.IndexOf(const S:WideString):integer;
begin
  for Result:=0 to GetCount - 1 do
  begin
    if CompareStrings(Get(Result),S)=0 then Exit;
  end;
  Result:=-1;
end;

function TD2WideStrings.IndexOfName(const Name:WideString):integer;
var
  P:integer;
  S:WideString;
begin
  for Result:=0 to GetCount - 1 do
  begin
    S:=Get(Result);
    P:=Pos(WideString('='),S);
    if (P > 0) and (CompareStrings(Copy(S,1,P - 1),Name)=0) then Exit;
  end;
  Result:=-1;
end;

function TD2WideStrings.IndexOfValue(const Name:WideString):integer;
var
  P:integer;
  S:WideString;
begin
  for Result:=0 to GetCount - 1 do
  begin
    S:=Get(Result);
    P:=Pos(WideString('='),S);
    if (P > 0) and (CompareStrings(Copy(S,P + 1,Length(S)),Name)=0) then Exit;
  end;
  Result:=-1;
end;

function TD2WideStrings.IndexOfObject(AObject:TObject):integer;
begin
  for Result:=0 to GetCount - 1 do
    if GetObject(Result)=AObject then Exit;
  Result:=-1;
end;

procedure TD2WideStrings.InsertObject(Index:integer; const S:WideString; AObject:TObject);
begin
  Insert(Index,S);
  PutObject(Index,AObject);
end;

procedure TD2WideStrings.LoadFromFile(const FileName:String);
var
  Stream:TStream;
begin
  try
    Stream:=TFileStream.Create(FileName,fmOpenRead or fmShareDenyNone);
    try
      LoadFromStream(Stream);
    finally
      Stream.Free;
    end;
  except
    //RaiseLastWin32Error;
  end;
end;

procedure TD2WideStrings.LoadFromStream(Stream:TStream);
var
  Size:integer;
  Order:System.WideChar;
  SW:WideString;
  SA:AnsiString;
begin
  BeginUpdate;
  try
    Size:=Stream.Size - Stream.Position;
    Stream.Read(Order,2);
    if (Order=BOM_LSB_FIRST) or (Order=BOM_MSB_FIRST) then
    begin
      // unicode
      SetLength(SW,(Size - 2) div 2);
      Stream.Read(PWideChar(SW)^,Size - 2);
      SetTextStr(SW);
    end
    else
    begin
      // ansi
      Stream.Position:=Stream.Position - 2;
      SetLength(SA,Size);
      Stream.Read(PAnsiChar(SA)^,Size);
      SetTextStr(SA);
    end;
  finally
    EndUpdate;
  end;
end;

procedure TD2WideStrings.Move(CurIndex,NewIndex:integer);
var
  TempObject:TObject;
  TempString:WideString;
begin
  if CurIndex <> NewIndex then
  begin
    BeginUpdate;
    try
      TempString:=Get(CurIndex);
      TempObject:=GetObject(CurIndex);
      Delete(CurIndex);
      InsertObject(NewIndex,TempString,TempObject);
    finally
      EndUpdate;
    end;
  end;
end;

procedure TD2WideStrings.Put(Index:integer; const S:WideString);
var
  TempObject:TObject;
begin
  TempObject:=GetObject(Index);
  Delete(Index);
  InsertObject(Index,S,TempObject);
end;

procedure TD2WideStrings.PutObject(Index:integer; AObject:TObject);
begin
end;

procedure TD2WideStrings.ReadData(Reader:TReader);
begin
  Reader.ReadListBegin;
  BeginUpdate;
  try
    Clear;
    while not Reader.EndOfList do
      Add(Reader.ReadWideString);
  finally
    EndUpdate;
  end;
  Reader.ReadListEnd;
end;

procedure TD2WideStrings.SaveToFile(const FileName:String);
var
  Stream:TStream;
begin
  Stream:=TFileStream.Create(FileName,fmCreate);
  try
    SaveToStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TD2WideStrings.SaveToAnsiFile(const FileName:String);
var
  S:TStrings;
begin
  S:=TStringList.Create;
  try
    S.Text:=Text;
    S.SaveToFile(FileName);
  finally
    S.Free;
  end;
end;

procedure TD2WideStrings.SaveToStream(Stream:TStream);
var
  SW,BOM:WideString;
begin
  SW:=GetTextStr;
  // only save if allowed
  BOM:=BOM_LSB_FIRST;
  Stream.WriteBuffer(PWideChar(BOM)^,2);
  // SW has already been filled
  Stream.WriteBuffer(PWideChar(SW)^,2 * Length(SW));
end;

procedure TD2WideStrings.SetCapacity(NewCapacity:integer);
begin
end;

procedure TD2WideStrings.SetCaseSensitive(const Value:boolean);
begin
  if Value <> FCaseSensitive then
  begin
    FCaseSensitive:=Value;
    if Sorted then Sort;
  end;
end;

procedure TD2WideStrings.SetCommaText(const Value:WideString);
var
  S:WideString;
  P,P1:PWideChar;
begin
  BeginUpdate;
  try
    Clear;
    P:=PWideChar(Value);
    while P^ in [System.WideChar(#1)..d2WideSpace] do System.Inc(P);
    while P^ <> d2WideNull do
    begin
      if P^='"' then
//!!!        S:=WideExtractQuotedStr(P,'"')
      else
      begin
        P1:=P;
        while (P^ > d2WideSpace) and (P^ <> ',') do System.Inc(P);
        System.SetString(S,P1,P - P1);
      end;
      Add(S);

      while P^ in [System.WideChar(#1)..d2WideSpace] do
        System.Inc(P);
      if P^=',' then
        repeat
          System.Inc(P);
        until not (P^ in [System.WideChar(#1)..d2WideSpace]);
    end;
  finally
    EndUpdate;
  end;
end;

procedure TD2WideStrings.SetItem(index:integer; AText:wideString);
begin
  Strings[index]:=AText;
end;

procedure TD2WideStrings.SetSorted(const Value:boolean);
begin
  if FSorted <> Value then
  begin
    if Value then Sort;
    FSorted:=Value;
  end;
end;

procedure TD2WideStrings.SetText(Text:PWideChar);
begin
  SetTextStr(Text);
end;

procedure TD2WideStrings.SetTextStr(const Value:WideString);
var
  Head,Tail:PWideChar;
  S:WideString;
begin
  BeginUpdate;
  try
    Clear;
    Head:=PWideChar(Value);
    while Head^ <> d2WideNull do
    begin
      Tail:=Head;
      while not (Tail^ in [d2WideNull,d2WideLineFeed,d2WideCarriageReturn,d2WideVerticalTab,d2WideFormFeed]) and
            (Tail^ <> d2WideLineSeparator) and (Tail^ <> d2WideParagraphSeparator) do
        System.Inc(Tail);
      System.SetString(S,Head,Tail - Head);
      Add(S);
      Head:=Tail;
      if Head^ <> d2WideNull then
      begin
        System.Inc(Head);
        if (Tail^=d2WideCarriageReturn) and (Head^=d2WideLineFeed) then
          System.Inc(Head);
      end;
    end;
  finally
    EndUpdate;
  end;
end;

procedure TD2WideStrings.SetUpdateState(Updating:boolean);
begin
end;

procedure TD2WideStrings.SetValue(const Name,Value:WideString);
var
  I :integer;
begin
  I:=IndexOfName(Name);
  if Value <> '' then
  begin
    if I < 0 then I:=Add('');
    Put(I,Name + '=' + Value);
  end
  else
    if I >= 0 then Delete(I);
end;

procedure TD2WideStrings.WriteData(Writer:TWriter);
var
  I:integer;
begin
  Writer.WriteListBegin;
  for I:=0 to Count-1 do
    Writer.WriteWideString(Get(I));
  Writer.WriteListEnd;
end;

//================ TD2WideStringList ================================

destructor TD2WideStringList.Destroy;
begin
  FOnChange:=nil;
  FOnChanging:=nil;
  inherited Destroy;
  if FCount <> 0 then Finalize(FList^[0],FCount);
  FCount:=0;
  SetCapacity(0);
end;

function TD2WideStringList.Add(const S:WideString):integer;
begin
  if not Sorted then
    Result:=FCount
  else
  begin
    if sortByObject then
    begin
      Result:=FCount
    end
    else
    begin
      if Find(S,Result) then
        case Duplicates of
          dupIgnore:
            Exit;
          dupError:
            Error('Doublicate string',0);
        end;
    end;
  end;
  InsertItem(Result,S);
end;

procedure TD2WideStringList.Changed;
begin
  if (FUpdateCount=0) and Assigned(FOnChange) then FOnChange(Self);
end;

procedure TD2WideStringList.Changing;
begin
  if (FUpdateCount=0) and Assigned(FOnChanging) then FOnChanging(Self);
end;

procedure TD2WideStringList.Clear;
begin
  if FCount <> 0 then
  begin
    Changing;
    Finalize(FList^[0],FCount);
    FCount:=0;
    SetCapacity(0);
    Changed;
  end;
end;

function TD2WideStringList.CompareStrings(const S1,S2:WideString):integer;
begin
  {$IFDEF KS_COMPILER6_UP}
  if CaseSensitive then
    Result:=WideCompareStr(S1,S2)
  else
    Result:=WideCompareText(S1,S2);
  {$ELSE}
  if CaseSensitive then
    Result:=CompareStr(S1,S2)
  else
    Result:=CompareText(S1,S2);
  {$ENDIF}
end;

procedure TD2WideStringList.Delete(Index:integer);
begin
  if (Index < 0) or (Index >= FCount) then Error('List index error',Index);
  Changing;
  FList[Index].FString:='';
  System.Dec(FCount);
  if Index < FCount then System.Move(FList[Index + 1],FList[Index],(FCount - Index) * SizeOf(TWideStringItem));
  Changed;
end;

procedure TD2WideStringList.Exchange(Index1,Index2:integer);
begin
  if (Index1 < 0) or (Index1 >= FCount) then Error('List index error',Index1);
  if (Index2 < 0) or (Index2 >= FCount) then Error('List index error',Index2);
  Changing;
  ExchangeItems(Index1,Index2);
  Changed;
end;

procedure TD2WideStringList.ExchangeItems(Index1,Index2:integer);
var 
  P1,P2:Pointer;
begin
  P1:=Pointer(Flist^[Index1].FString);
  P2:=Pointer(Flist^[Index1].FObject);
  Pointer(Flist^[Index1].FString):=Pointer(Flist^[Index2].FString);
  Pointer(Flist^[Index1].FObject):=Pointer(Flist^[Index2].FObject);
  Pointer(Flist^[Index2].FString):=P1;
  Pointer(Flist^[Index2].FObject):=P2;
end;

function TD2WideStringList.Find(const S:WideString; var Index:integer):boolean;
var
  L,H,I,C:integer;
begin
  Result:=False;
  L:=0;
  H:=FCount - 1;
  while L <= H do
  begin
    I:=(L + H) shr 1;
    C:=CompareStrings(FList^[I].FString,S);
    if C < 0 then L:=I + 1
    else
    begin
      H:=I - 1;
      if C=0 then
      begin
        Result:=True;
        if Duplicates <> dupAccept then L:=I;
      end;
    end;
  end;
  Index:=L;
end;

function TD2WideStringList.FindByObject(const S:TObject;
  var Index:integer):boolean;
var
  L,H,I,C:integer;
begin
  Result:=False;
  L:=0;
  H:=FCount - 1;
  while L <= H do
  begin
    I:=(L + H) shr 1;
    C:=goodCompareObj(FList^[I].FObject,S);
    if C < 0 then L:=I + 1 else
    begin
      H:=I - 1;
      if C=0 then
      begin
        Result:=True;
        if Duplicates <> dupAccept then L:=I;
      end;
    end;
  end;
  Index:=L;
end;

function TD2WideStringList.Get(Index:integer):WideString;
begin
  if (Index < 0) or (Index >= FCount) then
  begin
    Error('List index error',Index);
    Result:='';
  end
  else
    Result:=FList[Index].FString;
end;

function TD2WideStringList.GetCapacity:integer;
begin
  Result:=FCapacity;
end;

function TD2WideStringList.GetCount:integer;
begin
  Result:=FCount;
end;

function TD2WideStringList.GetObject(Index:integer):TObject;
begin
  if (Index < 0) or (Index >= FCount) then Error('List index error',Index);
  Result:=FList[Index].FObject;
end;

procedure TD2WideStringList.Grow;
var
  Delta:integer;
begin
  if FCapacity > 64 then Delta:=FCapacity div 4 else
    if FCapacity > 8 then Delta:=16 else
      Delta:=4;
  SetCapacity(FCapacity + Delta);
end;

function TD2WideStringList.IndexOf(const S:WideString):integer;
begin
  if not Sorted then
    Result:=inherited IndexOf(S)
  else
    if not Find(S,Result) then Result:=-1;
end;

procedure TD2WideStringList.Insert(Index:integer; const S:WideString);
begin
  if Sorted then Error('Sorted list error',0);
  if (Index < 0) or (Index > FCount) then Error('List index error',Index);
  InsertItem(Index,S);
end;

procedure TD2WideStringList.InsertItem(Index:integer; const S:WideString);
begin
  Changing;
  if FCount=FCapacity then Grow;
  if Index < FCount then
    System.Move(FList^[Index],FList^[Index + 1],
      (FCount - Index) * SizeOf(TStringItem));
  with FList^[Index] do
  begin
    Pointer(FString):=nil;
    FObject:=nil;
    FString:=S;
  end;
  System.Inc(FCount);
  Changed;
end;

procedure TD2WideStringList.Put(Index:integer; const S:WideString);
begin
  if Sorted then Error('Sorted list error',0);
  if (Index < 0) or (Index >= FCount) then Error('List index error',Index);
  Changing;
  FList[Index].FString:=S;
  Changed;
end;

procedure TD2WideStringList.PutObject(Index:integer; AObject:TObject);
begin
  if (Index < 0) or (Index >= FCount) then Error('List index error',Index);
  Changing;
  FList[Index].FObject:=AObject;
  Changed;
end;

procedure TD2WideStringList.QuickSort(L,R:integer);
var
  I,J:integer;
  //P:WideString;
begin
  repeat
    I:=L;
    J:=R;
    //P:=FList[(L + R) shr 1].FString;
    repeat
      {$IFDEF KS_COMPILER6_UP}
      while WideCompareText(FList[I].FString,P) < 0 do System.Inc(I);
      while WideCompareText(FList[J].FString,P) > 0 do System.Dec(J);
      {ELSE}
      while CompareText(FList[I].FString,P) < 0 do System.Inc(I);
      while CompareText(FList[J].FString,P) > 0 do System.Dec(J);
      {$ENDIF}
      if I <= J then
      begin
        ExchangeItems(I,J);
        System.Inc(I);
        System.Dec(J);
      end;
    until I > J;
    if L < J then QuickSort(L,J);
    L:=I;
  until I >= R;
end;

procedure TD2WideStringList.QuickSortByObject(L,R:integer);
var
  I,J:integer;
  P:TObject;
begin
  repeat
    I:=L;
    J:=R;
    P:=FList[(L + R) shr 1].FObject;
    repeat
      while goodCompareObj(FList[I].FObject,P) < 0 do System.Inc(I);
      while goodCompareObj(FList[J].FObject,P) > 0 do System.Dec(J);
      if I <= J then
      begin
        ExchangeItems(I,J);
        System.Inc(I);
        System.Dec(J);
      end;
    until I > J;
    if L < J then QuickSortByObject(L,J);
    L:=I;
  until I >= R;
end;

procedure TD2WideStringList.SetCapacity(NewCapacity:integer);
begin
  ReallocMem(FList,NewCapacity * SizeOf(TWideStringItem));
  FCapacity:=NewCapacity;
end;

procedure TD2WideStringList.SetUpdateState(Updating:boolean);
begin
  if Updating then Changing
              else Changed;
end;

procedure TD2WideStringList.Sort;
begin
  if not Sorted and (Count > 1) then
  begin
    Changing;
    if FsortByObject then
      QuickSortByObject(0,FCount - 1)
    else
      QuickSort(0,FCount - 1);
    Changed;
  end;
end;

