{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


{ TD3Designer =================================================================}

procedure TD3Designer.CallDesignSelect(AObject:TObject);
var
  i:integer;
begin
  if FScenes <> nil then
    for i:=0 to FScenes.Count - 1 do
    begin
      TD3Scene(FScenes[i]).DoDesignSelect(AObject);
      TD3Scene(FScenes[i]).AddUpdateRect(d2Rect(0,0,1000,1000));
    end;
end;

procedure TD3Designer.AddScene(const Scene:TD3Scene);
begin
  if FScenes=nil then
    FScenes:=TList.Create;
  if FScenes.IndexOf(Pointer(Scene)) < 0 then
    FScenes.Add(Pointer(Scene));
end;

procedure TD3Designer.RemoveScene(const Scene:TD3Scene);
begin
  FScenes.Remove(Pointer(Scene));
  if FScenes.Count=0 then
    FreeAndNil(FScenes);
end;

procedure TD3Designer.AddObject(AObject:TD3Object);
begin
  //....
end;

procedure TD3Designer.DeleteObject(AObject:TD3Object);
begin
  //....
end;

procedure TD3Designer.AddObject2D(AObject:TD2Object);
begin
  //....
end;
           
