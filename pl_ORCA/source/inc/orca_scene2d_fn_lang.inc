{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

procedure CollectLangStart;
begin
  ResetLang;
  if varD2CollectLang=nil then
  begin
    varD2CollectLang:=TD2WideStringList.Create;
    varD2CollectLang.Sorted:=true;
    TD2WideStringList(varD2CollectLang).CaseSensitive:=true;
  end;
end;

procedure CollectLangFinish;
begin
  if varD2CollectLang <> nil then FreeAndNil(varD2CollectLang);
end;

function CollectLangStrings:TD2WideStrings;
begin
  Result:=varD2CollectLang;
end;

procedure CollectLangSave;
begin
  if varD2CollectLang <> nil then
  begin
    varD2CollectLang.SaveToFile(ExtractFilePath(ParamStr(0)) + 'lang.lng');
  end;
end;

procedure LoadLangFromFile(AFileName:string);
begin
  if not FileExists(AFileName) then Exit;
  ResetLang;
  if varD2Lang=nil then
  begin
    varD2Lang:=TD2WideStringList.Create;
    varD2Lang.Sorted:=true;
    TD2WideStringList(varD2Lang).CaseSensitive:=true;
  end;
  varD2Lang.LoadFromFile(AFileName);
  UpdateLang;
end;

procedure LoadLangFromStrings(AStr:TD2WideStrings);
begin
  if AStr=nil then Exit;
  ResetLang;
  if varD2Lang=nil then
  begin
    varD2Lang:=TD2WideStringList.Create;
    varD2Lang.Sorted:=true;
    TD2WideStringList(varD2Lang).CaseSensitive:=true;
  end;
  varD2Lang.Assign(AStr);
  UpdateLang;
end;

procedure ResetLang;
begin
  if varD2Lang <> nil then
  begin
    varD2NeedResetLang:=true;
    try
      UpdateLang;
    finally
      varD2NeedResetLang:=false;
      FreeAndNil(varD2Lang);
    end;
  end;
end;

procedure UpdateLang;
var
  i:integer;
begin
  if varD2SceneList <> nil then
  begin
    for i:=0 to varD2SceneList.Count - 1 do
      Id2Scene(varD2SceneList[i]).UpdateResource;
  end;
end;

function Translate(const AText:WideString):WideString;
//var	//Deleted by GoldenFox
  //Idx:integer;	//Deleted by GoldenFox
begin
  if AText='' then
  begin
    Result:='';
    Exit;
  end;
  if Assigned(CustomTranslateProc) then
  begin
    if CustomTranslateProc(AText) <> '' then
      Result:=CustomTranslateProc(AText)
    else
      Result:=AText;
    Exit;
  end;
  if varD2CollectLang <> nil then
  begin
    varD2CollectLang.Add(AText)
  end;
  if varD2Lang <> nil then
  begin
    if not varD2NeedResetLang then
      Result:=varD2Lang.Values[AText]
    else
      if varD2Lang.IndexOfValue(AText) >= 0 then
        Result:=varD2Lang.Names[varD2Lang.IndexOfValue(AText)];
    if Result='' then
      Result:=AText;
  end
  else
    Result:=AText;
end;

function TranslateText(const AText:WideString):WideString;
begin
  Result:=Translate(AText);
end;           
