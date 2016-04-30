{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//================ TD3Object ==========================

constructor TD3Object.Create(AOwner:TComponent);
begin
  inherited;
  FStored:=true;
end;

destructor TD3Object.Destroy;
begin
  if FParent <> nil then
    FParent.RemoveObject(Self)
  else
    if FScene <> nil then
      FScene.RemoveObject(Self);
  FScene:=nil;
  DeleteChildren;
  FScene:=nil;
  inherited;
end;

function TD3Object.GetChild(Index:integer):TD3Object;
begin
  if (FChildren <> nil) and (Index < FChildren.Count) then
    Result:=TD3Object(FChildren[Index])
  else
    Result:=nil;
end;

function TD3Object.GetChildrenCount:integer;
begin
  if (FChildren <> nil) then
    Result:=FChildren.Count
  else
    Result:=0;
end;

procedure TD3Object.ReaderSetName(Reader:TReader; Component:TComponent;
  var Name:string);
begin
  Name:='';
end;

procedure TD3Object.ReaderError(Reader:TReader; const Message:string;
  var Handled:boolean);
begin
  Handled:=true;
end;

procedure TD3Object.LoadFromStream(const AStream:TStream);
var
  SaveName:string;
  BinStream:TStream;
begin
  { store }
  BinStream:=TMemoryStream.Create;
  try
    ObjectTextToBinary(AStream,BinStream);
    BinStream.Position:=0;
    SaveName:=Name;
    Name:='';
    BinStream.ReadComponent(Self);
    Name:=SaveName;
  finally
    BinStream.Free;
  end;
end;

procedure TD3Object.SaveToStream(const Stream:TStream);
var
  SaveName:string;
  BinStream:TStream;
begin
  { store }
  BinStream:=TMemoryStream.Create;
  try
    SaveName:=Name;
    Name:='';
    BinStream.WriteComponent(Self);
    Name:=SaveName;
    BinStream.Position:=0;
    ObjectBinaryToText(BinStream,Stream);
  finally
    BinStream.Free;
  end;
end;

procedure TD3Object.LoadFromBinStream(const AStream:TStream);
var
  R:TReader;
begin
  R:=TReader.Create(AStream,1024);
  R.OnSetName:=ReaderSetName;
  R.OnError:=ReaderError;
  R.ReadRootComponent(Self);
  R.Free;
end;

procedure TD3Object.SaveToBinStream(const AStream:TStream);
var
  SaveName:string;
begin
  { store }
  SaveName:=Name;
  Name:='';
  AStream.WriteComponent(Self);
  Name:=SaveName;
end;

function TD3Object.Clone(const AOwner:TComponent):TD3Object;
var
  S:TStream;
  R:TReader;
  SaveName:string;
begin
  Result:=nil;
  S:=TMemoryStream.Create;
  try
    { store }
    SaveName:=Name;
    Name:='';
    S.WriteComponent(Self);
    Name:=SaveName;
    S.Position:=0;
    { load }
    R:=TReader.Create(S,1024);
    R.OnSetName:=ReaderSetName;
    Result:=TD3ObjectClass(ClassType).Create(AOwner);
    if Result <> nil then
    begin
      R.ReadRootComponent(Result);
    end;
    R.Free;
  finally
    S.Free;
  end;
end;

procedure TD3Object.CloneChildFromStream(AStream:TStream);
var
  i:integer;
  Obj:TD3Object;
begin
  Obj:=CreateObject3DFromStream(Self,AStream);
  if (Obj <> nil) and (Obj.FChildren <> nil) and (Obj.FChildren.Count > 0) then
  begin
    { delete self childs }
    DeleteChildren;
    { copy parent }
    for i:=0 to Obj.FChildren.Count - 1 do
    begin
      if TD3Object(Obj.FChildren[0]).isVisual then
        TD3VisualObject(Obj.FChildren[0]).Locked:=true;
      TD3Object(Obj.FChildren[0]).Stored:=false;
      TD3Object(Obj.FChildren[0]).Parent:=Self;
    end;
    { realign to new size }
    if Obj.isVisual and (isVisual) then
    begin
      TD3VisualObject(Self).FLastWidth:=TD3VisualObject(Obj).Width;
      TD3VisualObject(Self).FLastHeight:=TD3VisualObject(Obj).Height;
      TD3VisualObject(Self).Realign;
    end;
//    Obj.Free;
  end;
end;

procedure TD3VisualObject.SetLocked(const Value:boolean);
var
  i:integer;
begin
  FLocked:=Value;
{  if (FChildren <> nil) and (FChildren.Count > 0) then
    for i:=0 to FChildren.Count - 1 do
      if TD3Object(FChildren[i]).isVisual then
        TD3VisualObject(FChildren[i]).Locked:=Value;}
end;

procedure TD3Object.ProcessTick(time,deltaTime:single);
var
  c,i:integer;
begin
  if FChildren <> nil then
  begin
    c:=FChildren.Count;
    for i:=FChildren.Count - 1 downto 0 do
    begin
      TD3Object(FChildren[i]).ProcessTick(time,deltaTime);
    end;
  end;
  if Assigned(FOnTick) then
    FOnTick(Self,time,deltaTime);
end;

function TD3Object.Visual:TD3VisualObject;
begin
  Result:=TD3VisualObject(Self);
end;

function TD3Object.IsVisual:boolean;
begin
  Result:=Self is TD3VisualObject;
end;

procedure TD3Object.SetResourceName(const Value:string);
begin
  FResourceName:=Value;
end;

procedure TD3Object.StartAnimation(const AName:String);   // *** DB ***
var
  i:integer;
  E:TD3Animation;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]) is TD3Animation then
        if (CompareText(TD3Animation(FChildren[i]).Name,AName)=0) or
           (CompareText(TD3Animation(FChildren[i]).ResourceName,AName)=0) then
        begin
          E:=TD3Animation(FChildren[i]);
          E.Start;
        end;
    end;
end;

procedure TD3Object.StopAnimation(const AName:String);    // *** DB ***
var
  i:integer;
  E:TD3Animation;
begin
  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
      if TD3Object(FChildren[i]) is TD3Animation then
        if (CompareText(TD3Animation(FChildren[i]).Name,AName)=0) or
           (CompareText(TD3Animation(FChildren[i]).ResourceName,AName)=0) then
        begin
          E:=TD3Animation(FChildren[i]);
          E.Stop;
        end;
end;

procedure TD3Object.StartTriggerAnimation(AInstance:TD3Object; ATrigger:string);
var
  i:integer;
  E:TD3Animation;
begin
  StopTriggerAnimation(AInstance);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]) is TD3Animation then
        TD3Animation(FChildren[i]).StartTrigger(AInstance,ATrigger);
      { locked objects }
      if TD3Object(FChildren[i]).isVisual and TD3VisualObject(FChildren[i]).Locked and not TD3VisualObject(FChildren[i]).HitTest then
      begin
        TD3Object(FChildren[i]).StartTriggerAnimation(AInstance,ATrigger);
      end;
    end;
end;

procedure TD3Object.StopTriggerAnimation(AInstance:TD3Object);
var
  i:integer;
  E:TD3Animation;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]) is TD3Animation then
        if TD3Animation(FChildren[i]).Trigger <> '' then
        begin
          E:=TD3Animation(FChildren[i]);
          E.Stop;
        end;
      { locked objects }
      if TD3Object(FChildren[i]).isVisual and TD3VisualObject(FChildren[i]).Locked and not TD3VisualObject(FChildren[i]).HitTest then
      begin
        TD3Object(FChildren[i]).StopTriggerAnimation(AInstance);
      end;
    end;
end;

{ Property animation }

procedure TD3Object.AnimateStop(const APropertyName:string);
var
  i:integer;
begin
  for i:=0 to ChildrenCount - 1 do
  begin
    if (Children[i] is TD3FloatAnimation) and (CompareText(TD3FloatAnimation(Children[i]).PropertyName,APropertyName)=0) then
    begin
      TD3FloatAnimation(Children[i]).Stop;
    end;
    if (Children[i] is TD3ColorAnimation) and (CompareText(TD3ColorAnimation(Children[i]).PropertyName,APropertyName)=0) then
    begin
      TD3ColorAnimation(Children[i]).Stop;
    end;
  end;
end;

procedure TD3Object.AnimateFloat(const APropertyName:string; const NewValue:single; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD3FloatAnimation;
begin
  AnimateStop(APropertyName);
  A:=TD3FloatAnimation.Create(Self);
  A.Parent:=Self;
  A.OnFinish:=DoAniFinished;
  A.Duration:=Duration;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
end;

procedure TD3Object.AnimateColor(const APropertyName:string; const NewValue:string; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD3ColorAnimation;
begin
  AnimateStop(APropertyName);
  A:=TD3ColorAnimation.Create(Self);
  A.Parent:=Self;
  A.OnFinish:=DoAniFinished;
  A.Duration:=Duration;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
end;

procedure TD3Object.AnimateFloatWait(const APropertyName:string; const NewValue:single; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD3FloatAnimation;
begin
  AnimateStop(APropertyName);
  A:=TD3FloatAnimation.Create(Self);
  A.Parent:=Self;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.Duration:=Duration;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
  while A.FRunning do
  begin
    Application.ProcessMessages;
    Sleep(0);
  end;
  A.Free;
end;

procedure TD3Object.AnimateColorWait(const APropertyName:string; const NewValue:string; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD3ColorAnimation;
begin
  AnimateStop(APropertyName);
  A:=TD3ColorAnimation.Create(Self);
  A.Parent:=Self;
  A.Duration:=Duration;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
  while A.FRunning do
  begin
    Application.ProcessMessages;
    Sleep(0);
  end;
  A.Free;
end;

procedure TD3Object.DoAniFinished(Sender:TObject);
begin
  TD3Animation(Sender).Free;
end;

procedure TD3Object.UpdateChildScene;
var
  i:integer;
begin
  if (FParent <> nil) then
    FScene:=FParent.FScene;
  if (FChildren <> nil) and (FChildren.Count > 0) then
    for i:=0 to FChildren.Count - 1 do
      TD3Object(FChildren[i]).UpdateChildScene;
end;

procedure TD3Object.SetParent(const Value:TD3Object);
begin
  if FParent <> nil then
    FParent.RemoveObject(Self)
  else
    if FScene <> nil then
      FScene.RemoveObject(Self);
  FParent:=Value;
  if FParent <> nil then
  begin
    FParent.AddObject(Self);
    UpdateChildScene;
  end;
end;

procedure TD3Object.SetParentComponent(Value:TComponent);
begin
  inherited ;
  if FParent <> nil then
    FParent.RemoveObject(Self);

  if (Value <> nil) and (Value is TD3Object) then
  begin
    TD3Object(Value).AddObject(Self);
  end
  else
    if (Value <> nil) and (Value is TD3Scene) then
    begin
      TD3Scene(Value).AddObject(Self);
    end;
end;

procedure TD3Object.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  i,j:integer;
begin
  inherited;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]).Stored then
      begin
        Proc(FChildren[i]);
      end;
    end;
end;

function TD3Object.GetParentComponent:TComponent;
begin
  Result:=FParent;
  if Result=nil then
    Result:=FScene;
end;

function TD3Object.HasParent:boolean;
begin
  Result:=true;
end;

function TD3Object.FindObject(const AClass:TD3ObjectClass):TD3Object;
var
  i:integer;
begin
  Result:=nil;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD3Object(FChildren[i]) is AClass then
      begin
        Result:=TD3Object(FChildren[i]);
        Exit;
      end;
      Result:=TD3Object(FChildren[i]).FindObject(AClass);
      if Result <> nil then Exit;
    end;
  end;
end;

function TD3Object.FindResource(const AResource:string):TD3Object;
var
  i:integer;
begin
  Result:=nil;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
    begin
      if CompareText(TD3Object(FChildren[i]).ResourceName,AResource)=0 then
      begin
        Result:=TD3Object(FChildren[i]);
        Exit;
      end;
      Result:=TD3Object(FChildren[i]).FindResource(AResource);
      if Result <> nil then Exit;
    end;
  end;
end;

procedure TD3Object.DeleteChildren;
var
  Child:TD3Object;
begin
  if Assigned(FChildren) then
  begin
    while FChildren.Count > 0 do
    begin
      Child:=TD3Object(FChildren[0]);
      FChildren.Delete(0);
      Child.FParent:=nil;
      Child.Free;
    end;
    FreeAndNil(FChildren); 
  end;
end;

procedure TD3Object.AddObjectsToList(const AList:TList);
var
  i:integer;
begin
  AList.Add(Self);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      TD3Object(FChildren[i]).AddObjectsToList(AList);
end;

procedure TD3Object.AddObject(AObject:TD3Object);
begin
  if FChildren=nil then
    FChildren:=TList.Create;
  FChildren.Add(AObject);
  AObject.FParent:=Self;
  AObject.FScene:=FScene;
  UpdateChildScene;
  if IsVisual and AObject.IsVisual and (TD3VisualObject(Self).Projection=d3ProjectionScreen) then
    TD3VisualObject(AObject).Projection:=TD3VisualObject(Self).Projection;
  if isVisual and not (csLoading in ComponentState) then
  begin
    TD3VisualObject(Self).RecalcAbsolute;
    TD3VisualObject(Self).Realign;
  end;
end;

procedure TD3Object.RemoveObject(AObject:TD3Object);
begin
  if FChildren <> nil then
    FChildren.Remove(AObject);
  if isVisual then
  begin
    TD3VisualObject(Self).Realign;
    TD3VisualObject(Self).Repaint;
  end;
end;

procedure TD3Object.BringToFront;
begin
  if (Parent <> nil) and (Parent.FChildren <> nil) then
  begin
    Parent.FChildren.Remove(Self);
    Parent.FChildren.Add(Self);
    if isVisual then
      TD3VisualObject(Self).Repaint;
  end;
end;

procedure TD3Object.SendToBack;
begin
  if (Parent <> nil) and (Parent.FChildren <> nil) then
  begin
    Parent.FChildren.Remove(Self);
    Parent.FChildren.Insert(0,Self);
    if isVisual then
      TD3VisualObject(Self).Repaint;
  end;
end;




             
           
