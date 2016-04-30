{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Lang ========================================

constructor TD2Lang.Create(AOwner:TComponent);
begin
  inherited;
  FOriginal:=TD2WideStringList.Create;
  FResources:=TD2WideStringList.Create;
  FAutoSelect:=true;
  FStoreInForm:=true;
end;

destructor TD2Lang.Destroy;
var
  i:integer;
begin
  for i:=0 to FResources.Count - 1 do
    TD2WideStrings(FResources.Objects[i]).Free;
  FResources.Free;
  FOriginal.Free;
  inherited;
end;

procedure TD2Lang.Loaded;
var
  L:string;
begin
  inherited;
  if (FFileName <> '') and (FileExists(ExtractFileName(ParamStr(0)) + FFileName)) then
    LoadFromFile(ExtractFileName(ParamStr(0)) + FFileName);
  if FAutoSelect then
    GetLanguageIDs(L,FLang);
  if FLang <> '' then
    LoadLangFromStrings(LangStr[FLang]);
end;

procedure TD2Lang.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('ResourcesBin',ReadResources,WriteResources,StoreInForm and (FResources.Count > 0));
end;

procedure TD2Lang.ReadResources(Stream:TStream);
var
  Len:cardinal;
  i:integer;
  N:WideString;
  Str:TD2WideStrings;
begin
  FOriginal.Text:=ReadString(Stream);
  Stream.Read(Len,4);
  for i:=0 to Len - 1 do
  begin
    N:=ReadString(Stream);
    Str:=TD2WideStringList.Create;
    Str.Sorted:=true;
    TD2WideStringList(Str).CaseSensitive:=true;
    Str.Text:=ReadString(Stream);
    FResources.AddObject(N,Str);
  end;
end;

procedure TD2Lang.WriteResources(Stream:TStream);
var
  Len:cardinal;
  i:integer;
begin
  WriteString(Stream,FOriginal.Text);
  Len:=FResources.Count;
  Stream.Write(Len,4);
  for i:=0 to Len - 1 do
  begin
    WriteString(Stream,FResources[i]);
    WriteString(Stream,TD2WideStrings(FResources.Objects[i]).Text);
  end;
end;

procedure TD2Lang.LoadFromFile(AFileName:string);
var
  S:TFileStream;
begin
  if FileExists(AFileName) then
  begin
    S:=TFileStream.Create(AFileName,fmOpenRead);
    ReadResources(S);
    S.Free;
  end;
end;

procedure TD2Lang.SaveToFile(AFileName:string);
var
  S:TFileStream;
begin
  S:=TFileStream.Create(AFileName,fmCreate);
  WriteResources(S);
  S.Free;
end;

procedure TD2Lang.AddLang(AName:WideString);
var
  Idx:integer;
  Str:TD2WideStrings;
begin
  Idx:=FResources.IndexOf(AName);
  if Idx < 0 then
  begin
    Str:=TD2WideStringList.Create;
    Str.Sorted:=true;
    TD2WideStringList(Str).CaseSensitive:=true;
    FResources.AddObject(AName,Str);
  end;
end;

function TD2Lang.GetLangStr(Index:WideString):TD2WideStrings;
var
  Idx:integer;
begin
  Idx:=FResources.IndexOf(Index);
  if Idx >= 0 then
    Result:=TD2WideStrings(FResources.Objects[Idx])
  else
    Result:=nil;
end;

procedure TD2Lang.SetLang(const Value:string);
begin
  FLang:=Value;
  if not (csLoading in ComponentState) then
  begin
    if FLang='en' then
      ResetLang
    else
      LoadLangFromStrings(LangStr[FLang]);
  end;
end;
       
        
         

            
  
