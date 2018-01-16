{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Resources ================================================

constructor TD2Resources.Create(AOwner:TComponent);
begin
  inherited;
  FResource:=TStringList.Create;
  TStringList(FResource).OnChange:=DoResourceChanged;
  FSceneList:=TList.Create;
end;

destructor TD2Resources.Destroy;
var i: integer;
begin
  if FRoot <> nil then
    FreeAndNil(FRoot);
  if FSceneList <> nil then
  begin
    for i:=0 to FSceneList.Count-1 do
      TObject(FSceneList[i]).Free;
    FreeAndNil(FSceneList);
  end;
  FreeAndNil(FResource);
  inherited;
end;

procedure TD2Resources.Loaded;
begin
  inherited ;
  if FFileName='' then Exit;
  if FileExists(ExtractFilePath(ParamStr(0)) + FFileName) then
    FResource.LoadFromFile(ExtractFilePath(ParamStr(0)) + FFileName)
  else
    if FileExists(FFileName) then
      FResource.LoadFromFile(FFileName);
end;

procedure TD2Resources.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('ResourcesBin',ReadResources,WriteResources,FRoot <> nil);
end;

procedure TD2Resources.ReadResources(Stream:TStream);
begin
  if (FRoot <> nil) then
  begin
    FreeAndNil(FRoot);
  end;

  FRoot:=CreateObjectFromBinStream(nil,Stream);
  if FRoot <> nil then
  begin
    if FRoot.IsVisual then
      FRoot.Visual.UpdateDesignHide(false);
    UpdateScenes;
  end;
end;

procedure TD2Resources.WriteResources(Stream:TStream);
begin
  if FRoot <> nil then
    FRoot.SaveToBinStream(Stream);
end;

procedure TD2Resources.FillStrings;
var
  M:TMemoryStream;
  SaveChanged:TNotifyEvent;
begin
  if FRoot <> nil then
  begin
    M:=TMemoryStream.Create;
    FRoot.SaveToStream(M);
    M.Position:=0;
    SaveChanged:=TStringList(FResource).OnChange;
    TStringList(FResource).OnChange:=nil;
    TStringList(FResource).LoadFromStream(M);
    TStringList(FResource).OnChange:=SaveChanged;
    M.Free;
  end;
end;

procedure TD2Resources.UpdateScenes;
var
  i:integer;
begin
  for i:=0 to FSceneList.Count - 1 do
    Id2Scene(FSceneList[i]).UpdateResource;
end;

procedure TD2Resources.DoResourceChanged(Sender:TObject);
var
  S:TStream;
begin
  if (FRoot <> nil) then
  begin
    FreeAndNil(FRoot);
  end;

  S:=TMemoryStream.Create;
  try
    TStringList(FResource).SaveToStream(S);
    if S.Position > 0 then
    begin
      S.Position:=0;
      FRoot:=CreateObjectFromStream(nil,S);
      if FRoot.IsVisual then
        FRoot.Visual.UpdateDesignHide(false);
    end;
  finally
    S.Free;
  end;
  UpdateScenes;
end;

procedure TD2Resources.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
end;

procedure TD2Resources.SetFileName(const Value:string);
begin
  if FFileName <> Value then
  begin
    FFileName:=Value;
    if not (csLoading in ComponentState) then
    begin
      if FileExists(ExtractFilePath(ParamStr(0)) + FFileName) then
        FResource.LoadFromFile(ExtractFilePath(ParamStr(0)) + FFileName)
      else
        if FileExists(FFileName) then
          FResource.LoadFromFile(FFileName);
    end;
  end;
end;

procedure TD2Resources.SetResource(const Value:TStrings);
begin
  FResource.Assign(Value);
end;

procedure TD2Resources.AddSceneUpdater(const Scene:Id2Scene);
begin
  FSceneList.Add(Pointer(Scene));
end;

procedure TD2Resources.RemoveSceneUpdater(const Scene:Id2Scene);
begin
  if FSceneList <> nil then
    FSceneList.Remove(Pointer(Scene));
end;
              
        
         

            
  
