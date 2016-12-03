{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Object =========================================

constructor TD2Object.Create(AOwner:TComponent);
begin
  inherited Create(AOwner);
  AddResource(Self);
  FIndex:=-1;
  FStored:=true;
  FIsVisual:=Self is TD2VisualObject;
  if FIsVisual then
    FVisual:=TD2VisualObject(Self);
end;

destructor TD2Object.Destroy;
var
  i:integer;
begin
//if FScene <> nil then // Notification will recurse components  *** DB ***
//begin                 // ... without regard to our FNotifyList *** DB ***
//  FScene.Notification(Self,opRemove);                         *** DB ***
//end;                                                           *** DB ***

  if FNotifyList <> nil then
  begin
    for i:=FNotifyList.Count - 1 downto 0 do
//  begin     // Deleting an object may throw an exception       *** DB ***
    Try                                                       // *** DB ***
      if (TObject(FNotifyList[i]) is TD2Visual) and (TD2Visual(FNotifyList[i]).VisualObject=Self) then
        TD2Visual(FNotifyList[i]).FVisualObject:=nil;
      if (TObject(FNotifyList[i]) is TD2BrushResource) and (TD2BrushResource(FNotifyList[i]).Resource=Self) then
        TD2BrushResource(FNotifyList[i]).FResource:=nil;
      if (TObject(FNotifyList[i]) is TD2PathData) and (TD2PathData(FNotifyList[i]).Resource=Self) then
        TD2PathData(FNotifyList[i]).FResource:=nil;
      if (TObject(FNotifyList[i]) is TD2Bitmap) and (TD2Bitmap(FNotifyList[i]).Resource=Self) then
        TD2Bitmap(FNotifyList[i]).FResource:=nil;
      if TObject(FNotifyList[i]) is TComponent then
      if TObject(FNotifyList[i]) is TComponent
      then
        THackComponent(FNotifyList[i]).Notification(Self,opRemove);
    except           // Ignore exception                      // *** DB ***
    end;
    FreeAndNil(FNotifyList);
  end;
  i:=varD2ResourceList.IndexOf(Self);
  if i >= 0 then
    varD2ResourceList[i]:=nil;

  if FScene <> nil then     // Now is a better time           // *** DB ***
  begin                     // ... for notifications          // *** DB ***
    FScene.Notification(Self,opRemove);                      // *** DB ***
  end;                                                        // *** DB ***

  if FParent <> nil then
    FParent.RemoveObject(Self)
  else
    if FScene <> nil then
      FScene.RemoveObject(Self);
  FScene:=nil;
  DeleteChildren;
  inherited;
end;

procedure TD2Object.Release(Delay:single=0.1);
var
  T:TTimer;
begin
  T:=TTimer.Create(Application);
  T.OnTimer:=DoReleaseTimer;
  T.Interval:=round(Delay * 1000);
  T.Enabled:=true;
end;

procedure TD2Object.DoReleaseTimer(Sender:TObject);
begin
  Free;
  TTimer(Sender).Enabled:=false;
end;

function TD2Object.ItemClass:string;
begin
  Result:='';
end;

procedure TD2Object.AddFreeNotify(const AObject:TObject);
begin
  if FNotifyList=nil then
    FNotifyList:=TList.Create;
  FNotifyList.Add(AObject);
end;

procedure TD2Object.RemoveFreeNotify(const AObject:TObject);
begin
  if FNotifyList <> nil then
    FNotifyList.Remove(AObject);
end;

procedure TD2Object.ReaderSetName(Reader:TReader; Component:TComponent;
  var Name:string);
begin
  Name:='';
end;

procedure TD2Object.ReaderError(Reader: TReader; const Message: string;
  var Handled: Boolean);
begin
  Handled:=true;
end;

procedure TD2Object.LoadFromStream(const AStream:TStream);
var
  BinStream:TStream;
begin
  { store }
  BinStream:=TMemoryStream.Create;
  try
    ObjectTextToBinary(AStream,BinStream);
    BinStream.Position:=0;
    LoadFromBinStream(BinStream);
  finally
    BinStream.Free;
  end;
end;

procedure TD2Object.SaveToStream(const Stream:TStream);
var
  BinStream:TStream;
begin
  { store }
  BinStream:=TMemoryStream.Create;
  try
    BinStream.WriteComponent(Self);
    BinStream.Position:=0;
    ObjectBinaryToText(BinStream,Stream);
  finally
    BinStream.Free;
  end;
end;

procedure TD2Object.LoadFromBinStream(const AStream:TStream);
var
  R:TReader;
begin
  R:=TReader.Create(AStream,1024);
  R.OnError:=ReaderError;
  try
    R.ReadRootComponent(Self);
  finally
    R.Free;
  end;
end;

procedure TD2Object.SaveToBinStream(const AStream:TStream);
begin
  AStream.WriteComponent(Self);
end;

procedure TD2Object.IntLoadFromBinStream(const AStream:TStream);
var
  R:TReader;
begin
  R:=TReader.Create(AStream,1024);
  R.OnSetName:=ReaderSetName;
  R.OnError:=ReaderError;
  try
    if not (csDesigning in ComponentState) then
    begin
      // need to force set unique name
      SetDesigning(true,false);
      R.ReadRootComponent(Self);
      SetDesigning(false,false);
    end
    else
      R.ReadRootComponent(Self);
  finally
    R.Free;
  end;
end;

procedure TD2Object.IntSaveToBinStream(const AStream:TStream);
var
  SaveName:string;
begin
  { store }
  SaveName:=Name;
  Name:='';
  AStream.WriteComponent(Self);
  Name:=SaveName;
end;

function TD2Object.Clone(const AOwner:TComponent):TD2Object;
var
  S:TStream;
  SaveName:string;
begin
  S:=TMemoryStream.Create;
  try
    { store }
    SaveName:=Name;
    Name:='';
    S.WriteComponent(Self);
    Name:=SaveName;
    S.Position:=0;
    { load }
    Result:=TD2ObjectClass(ClassType).Create(AOwner);
    if Result <> nil then
      Result.IntLoadFromBinStream(S);
  finally
    S.Free;
  end;
end;

procedure TD2Object.CloneChildFromStream(AStream:TStream);
var
  i:integer;
  Obj:TD2Object;
begin
  Obj:=CreateObjectFromStream(Self,AStream);
  if (Obj <> nil) and (Obj.FChildren <> nil) and (Obj.FChildren.Count > 0) then
  begin
    { delete self childs }
    DeleteChildren;
    { copy parent }
    for i:=0 to Obj.FChildren.Count - 1 do
    begin
      if TD2Object(Obj.FChildren[0]).isVisual then
        TD2VisualObject(Obj.FChildren[0]).Locked:=true;
      TD2Object(Obj.FChildren[0]).Stored:=false;
      TD2Object(Obj.FChildren[0]).Parent:=Self;
    end;
    { realign to new size }
    if Obj.isVisual and (isVisual) then
    begin
      TD2VisualObject(Self).FLastWidth:=TD2VisualObject(Obj).Width;
      TD2VisualObject(Self).FLastHeight:=TD2VisualObject(Obj).Height;
      TD2VisualObject(Self).Realign;
    end;
//    Obj.Free;
  end;
end;

procedure TD2VisualObject.SetLocked(const Value:boolean);
begin
  FLocked:=Value;
end;

function TD2Object.GetScene:Id2Scene;
begin
  if FScene <> nil then
    Result:=FScene
  else
    if FParent <> nil then
    begin
      FScene:=FParent.FScene;
      Result:=FScene;
    end
    else
      Result:=nil;
end;

function TD2Object.HasClipParent:TD2VisualObject;
var
  i:integer;
begin
  Result:=nil;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if (TD2Object(FChildren[i]).IsVisual) and (TD2VisualObject(FChildren[i]).ClipParent) then
      begin
        Result:=TD2VisualObject(FChildren[i]);
        Exit;
      end;
end;

function TD2VisualObject.GetEffectsRect:TD2Rect;
var
  i:integer;
begin
  Result:=LocalRect;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) then
        Result:=d2UnionRect(Result,TD2Effect(FChildren[i]).GetRect(LocalRect));
    end;
end;

procedure TD2VisualObject.RecalcHasEffect;
var
  i:integer;
begin
  FHasEffect:=false;
  if FDisableEffect then Exit;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) then
      begin
        if not FHasEffect then
        begin
          UpdateEffects;
          Repaint;
        end;
        FHasEffect:=true;
        Break;
      end;
    end;
end;

function TD2VisualObject.HasDisablePaintEffect:boolean;
var
  i:integer;
begin
  Result:=false;
  if FDisableEffect then Exit;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) and (TD2Effect(FChildren[i]).DisablePaint) then
      begin
        Result:=true;
        Exit;
      end;
end;

function TD2VisualObject.HasAfterPaintEffect:boolean;
var
  i:integer;
begin
  Result:=false;
  if FDisableEffect then Exit;
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      if (TD2Object(FChildren[i]) is TD2Effect) and (TD2Effect(FChildren[i]).Enabled) and (TD2Effect(FChildren[i]).AfterPaint) then
      begin
        Result:=true;
        Exit;
      end;
end;

{ Property animation }

procedure TD2Object.AnimateColor(const APropertyName: string;
  const NewValue: string; Duration: single; AType: TD2AnimationType;
  AInterpolation: TD2InterpolationType);
var
  A:TD2ColorAnimation;
begin
  A:=TD2ColorAnimation.Create(Self);
  A.Parent:=Self;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.OnFinish:=DoAniFinished;
  A.Duration:=Duration;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
end;

procedure TD2Object.AnimateFloat(const APropertyName:string;
  const NewValue:single; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD2FloatAnimation;
begin
  A:=TD2FloatAnimation.Create(Self);
  A.Parent:=Self;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.OnFinish:=DoAniFinished;
  A.Duration:=Duration;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
end;

procedure TD2Object.AnimateFloatDelay(const APropertyName:string; const NewValue:single; Duration:single=0.2;
  Delay:single=0.0; AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD2FloatAnimation;
begin
  A:=TD2FloatAnimation.Create(Self);
  A.Parent:=Self;
  A.AnimationType:=AType;
  A.Interpolation:=AInterpolation;
  A.Delay:=Delay;
  A.Duration:=Duration;
  A.PropertyName:=APropertyName;
  A.StartFromCurrent:=true;
  A.StopValue:=NewValue;
  A.Start;
end;

procedure TD2Object.AnimateFloatWait(const APropertyName:string;
  const NewValue:single; Duration:single=0.2;
  AType:TD2AnimationType=d2AnimationIn; AInterpolation:TD2InterpolationType=d2InterpolationLinear);
var
  A:TD2FloatAnimation;
begin
  A:=TD2FloatAnimation.Create(Self);
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

procedure TD2Object.DoAniFinished(Sender:TObject);
begin
  TD2Animation(Sender).Free;
end;

//====================== Animations ==================================================

procedure TD2Object.StartAnimation(const AName:WideString);
var
  i:integer;
  E:TD2Animation;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2Animation then
        if CompareText(TD2Animation(FChildren[i]).Name,AName)=0 then
        begin
          E:=TD2Animation(FChildren[i]);
          E.Start;
        end;
    end;
end;

procedure TD2Object.StopAnimation(const AName:WideString);
var
  i:integer;
  E:TD2Animation;
begin
  if FChildren <> nil then
    for i:=FChildren.Count - 1 downto 0 do
      if TD2Object(FChildren[i]) is TD2Animation then
        if CompareText(TD2Animation(FChildren[i]).Name,AName)=0 then
        begin
          E:=TD2Animation(FChildren[i]);
          E.Stop;
        end;
end;

procedure TD2Object.StartTriggerAnimation(AInstance:TD2Object; ATrigger:string);
var
  i:integer;
  c:integer;
begin
  if FChildren <> nil then
    c:= FChildren.Count;
  StopTriggerAnimation(AInstance);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2Animation then
        TD2Animation(FChildren[i]).StartTrigger(AInstance,ATrigger);
      { locked objects }
      if TD2Object(FChildren[i]).isVisual and TD2VisualObject(FChildren[i]).Locked and not TD2VisualObject(FChildren[i]).HitTest then
      begin
        TD2Object(FChildren[i]).StartTriggerAnimation(AInstance,ATrigger);
      end;
    end;
end;

procedure TD2Object.StartTriggerAnimationWait(AInstance:TD2Object; ATrigger:string);
var
  i:integer;
begin
  StopTriggerAnimation(AInstance);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2Animation then
      begin
        TD2Animation(FChildren[i]).StartTrigger(AInstance,ATrigger);
        while TD2Animation(FChildren[i]).Running do
        begin
          Application.ProcessMessages;
          Sleep(0);
        end;
      end;
      { locked objects }
      if TD2Object(FChildren[i]).isVisual and TD2VisualObject(FChildren[i]).Locked and not TD2VisualObject(FChildren[i]).HitTest then
      begin
        TD2Object(FChildren[i]).StartTriggerAnimationWait(AInstance,ATrigger);
      end;
    end;
end;

procedure TD2Object.StopTriggerAnimation(AInstance:TD2Object);
var
  i:integer;
  E:TD2Animation;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2Animation then
        if TD2Animation(FChildren[i]).Trigger <> '' then
        begin
          E:=TD2Animation(FChildren[i]);
          E.Stop;
        end;
      { locked objects }
      if TD2Object(FChildren[i]).isVisual and TD2VisualObject(FChildren[i]).Locked and not TD2VisualObject(FChildren[i]).HitTest then
      begin
        TD2Object(FChildren[i]).StopTriggerAnimation(AInstance);
      end;
    end;
end;

procedure TD2Object.ApplyTriggerEffect(AInstance:TD2Object; ATrigger:string);
var
  i:integer;
begin
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if TD2Object(FChildren[i]) is TD2Effect then
        TD2Effect(FChildren[i]).ApplyTrigger(AInstance,ATrigger);
      { locked objects }
      if TD2Object(FChildren[i]).isVisual and TD2VisualObject(FChildren[i]).Locked and not TD2VisualObject(FChildren[i]).HitTest then
      begin
        TD2Object(FChildren[i]).ApplyTriggerEffect(AInstance,ATrigger);
      end;
    end;
end;

{ VCL }

procedure TD2Object.SetNewScene(AScene:Id2Scene);
var
  i:integer;
begin
  FScene:=AScene;
  if (FChildren <> nil) and (FChildren.Count > 0) then
    for i:=0 to FChildren.Count - 1 do
      TD2Object(FChildren[i]).SetNewScene(FScene);
end;

procedure TD2Object.ChangeParent;
begin
end;

procedure TD2Object.SetParent(const Value:TD2Object);
begin
  if Parent <> Value then
  begin
    if FParent <> nil then
      FParent.RemoveObject(Self)
    else
      if FScene <> nil then
        FScene.RemoveObject(Self);
    if Value <> nil then
    begin
      Value.AddObject(Self);
    end
    else
      FParent:=Value;
  end;
end;

function TD2Object.GetChild(Index:integer):TD2Object;
begin
  if (FChildren <> nil) and (Index < FChildren.Count) then
    Result:=TD2Object(FChildren[Index])
  else
    Result:=nil;
end;

function TD2Object.GetChildrenCount:integer;
begin
  if (FChildren <> nil) then
    Result:=FChildren.Count
  else
    Result:=0;
end;

procedure TD2Object.SetParentComponent(Value:TComponent);
var
  SI:Id2Scene;
begin
  inherited ;
  if FParent <> nil then
    FParent.RemoveObject(Self);

  if (Value <> nil) and (Value is TD2Object) then
  begin
    TD2Object(Value).AddObject(Self);
  end
  else
    if (THackComponent(Value).QueryInterface(Id2Scene,SI)=0) and (Assigned(SI)) then
    begin
      SI.AddObject(Self);
    end
end;

procedure TD2Object.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  i,j:integer;
begin
  inherited;
  if (Self is TD2Content) then Exit;

  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
    begin
      if (TD2Object(FChildren[i]) is TD2Content) and (TD2Content(FChildren[i]).FChildren <> nil) then
      begin
        for j:=0 to TD2Content(FChildren[i]).FChildren.Count - 1 do
          if TD2Object(TD2Content(FChildren[i]).FChildren[j]).Stored then
            Proc(TComponent(TD2Content(FChildren[i]).FChildren[j]));
      end;
      if TD2Object(FChildren[i]).Stored then
      begin
        Proc(TComponent(FChildren[i]));
      end;
    end;
end;

function TD2Object.GetParentComponent:TComponent;
begin
  if (FParent <> nil) and (FParent is TD2Popup) and (TD2Popup(FParent).Parent <> nil) and (TD2Popup(FParent).Parent is TD2PopupItem) then
    Result:=TD2Content(FParent).Parent
  else
  if (FParent <> nil) and (FParent is TD2Content) then
    Result:=TD2Content(FParent).Parent
  else
  if (FParent <> nil) and (FParent is TD2ComboListBox)  then
    Result:=TD2ComboListBox(FParent).Parent
  else
    Result:=FParent;
  if (Result=nil) and (FScene <> nil) then
    Result:=FScene.GetComponent;
end;

function TD2Object.HasParent: Boolean;
begin
  Result:=true;
end;

{ binding }

function TD2Object.GetData:Variant;
begin
  Result:=Name;
end;

procedure TD2Object.SetData(const Value:Variant);
begin
end;

function TD2Object.GetBinding(Index:string):Variant;
var
  Obj:TD2Object;
begin
  Obj:=FindBinding(Index);
  if Obj <> nil then
    Result:=Obj.Data
  else
    Result:='';
end;

procedure TD2Object.SetBinding(Index:string; const Value:Variant);
var
  Obj:TD2Object;
begin
  Obj:=FindBinding(Index);
  if Obj <> nil then
  begin
    try
      Obj.Data:=Value;
    except
    end;
  end;
end;

function TD2Object.FindBinding(const ABinding:string):TD2Object;
var
  i:integer;
begin
  Result:=nil;
  if CompareText(BindingName,ABinding)=0 then
  begin
    Result:=Self;
    Exit;
  end;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
    begin
      if CompareText(TD2Object(FChildren[i]).BindingName,ABinding)=0 then
      begin
        Result:=TD2Object(FChildren[i]);
        Exit;
      end;
      Result:=TD2Object(FChildren[i]).FindBinding(ABinding);
      if Result <> nil then Exit;
    end;
  end;
end;

procedure TD2Object.SetBindingName(const Value:string);
begin
  if FBindingName <> Value then
  begin
    FBindingName:=Value;
  end;
end;

{  }

function TD2Object.FindResource(const AResource:string):TD2Object;
var
  i:integer;
begin
  Result:=nil;
  if AResource='' then Exit;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
    begin
      if CompareText(TD2Object(FChildren[i]).ResourceName,AResource)=0 then
      begin
        Result:=TD2Object(FChildren[i]);
        Exit;
      end;
      if TD2Object(FChildren[i]) is TD2Control then Continue;

      Result:=TD2Object(FChildren[i]).FindResource(AResource);
      if Result <> nil then Exit;
    end;
  end;
end;

procedure TD2Object.SetResourceName(const Value:string);
begin
  if FResourceName <> Value then
  begin
    FResourceName:=Value;
  end;
end;

procedure TD2Object.SetStored(const Value:boolean);
var
  i:integer;
begin
  if FStored <> Value then
  begin
    FStored:=Value;
    if (FChildren <> nil) and (FChildren.Count > 0) then
    begin
      for i:=0 to FChildren.Count - 1 do
      begin
        TD2Object(FChildren[i]).Stored:=Value;
      end;
    end;
  end;
end;

procedure TD2Object.UpdateResource;
var
  i:integer;
begin
  if csLoading in ComponentState then Exit;  
  if csDestroying in ComponentState then Exit;
  if (FChildren <> nil) and (FChildren.Count > 0) then
  begin
    for i:=0 to FChildren.Count - 1 do
    begin
      TD2Object(FChildren[i]).UpdateResource;
    end;
  end;
end;

procedure TD2Object.DeleteChildren;
var
  i:integer;
  Child:TD2Object;
begin
  if Assigned(FChildren) then
  begin
    for i:=FChildren.Count - 1 downto 0 do
    begin
      Child:=TD2Object(FChildren[i]);
      Child.FParent:=nil;
      Child.SetNewScene(nil);
      Child.Free;
    end;
    FreeAndNil(FChildren);
  end;
end;

procedure TD2Object.AddObjectsToList(const AList:TList);
var
  i:integer;
begin
  AList.Add(Self);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      TD2Object(FChildren[i]).AddObjectsToList(AList);
end;

procedure TD2Object.AddControlsToList(const AList:TList);
var
  i:integer;
begin
  if (Self is TD2Control) then
    AList.Add(Self);
  if FChildren <> nil then
    for i:=0 to FChildren.Count - 1 do
      TD2Object(FChildren[i]).AddControlsToList(AList);
end;

procedure TD2Object.AddObject(AObject:TD2Object);
begin
  if AObject=nil then Exit;
  if AObject.Parent <> nil then
    AObject.Parent:=nil;
  if FChildren=nil then
  begin
    FChildren:=TList.Create;
    FChildren.Capacity:=10;
  end;
  if FChildren.IndexOf(AObject) >= 0 then Exit;
  FChildren.Add(AObject);
  AObject.FParent:=Self;
  AObject.SetNewScene(FScene);
  AObject.ChangeParent;
  if AObject.IsVisual and not (csLoading in ComponentState) then
  begin
  end;
  if IsVisual and AObject.IsVisual and (AObject.Visual.Align <> vaNone) then
    Self.Visual.FNeedAlign:=true;
  if IsVisual and (AObject is TD2Effect) and (TD2Effect(AObject).Enabled) then
  begin
    Visual.FHasEffect:=true;
    Visual.UpdateEffects;
  end;
  if IsVisual and AObject.IsVisual then
  begin
    if Visual.TempCanvas <> nil then
      AObject.Visual.TempCanvas:=Visual.TempCanvas;
    if Self.IsVisual then
      TD2VisualObject(AObject).FUpdating:=TD2VisualObject(Self).FUpdating;
    AObject.Visual.RecalcOpacity;
    AObject.Visual.RecalcAbsolute;
    AObject.Visual.RecalcUpdateRect;
    if Visual.FHasEffect then
    begin
      Visual.UpdateEffects;
      Visual.Repaint;
    end;
    if AObject.isVisual and (TD2VisualObject(AObject).Align <> vaNone) then
      TD2VisualObject(Self).Realign
    else
      TD2VisualObject(Self).Repaint;
    if TD2VisualObject(Self).FTabList=nil then
      TD2VisualObject(Self).FTabList:=TList.Create;
    TD2VisualObject(Self).FTabList.Add(AObject);
  end;
end;

procedure TD2Object.Sort(Compare:TD2ObjectSortCompare);
begin
  if FChildren <> nil then
    FChildren.Sort(TListSortCompare(Compare));
end;

function TD2Object.GetIndex:integer;
begin
  if (FIndex < 0) and (FParent <> nil) then
    FIndex:=FParent.FChildren.IndexOf(Self);
  Result:=FIndex;
end;

procedure TD2Object.SetIndex(Idx:integer);
var
  i:integer;
begin
  if (Parent <> nil) and (Parent.FChildren.IndexOf(Self) >= 0) then
  begin
    Parent.FChildren.Remove(Self);
    Parent.FChildren.Insert(Idx,Self);
    // recalc Index
    for i:=0 to Parent.FChildren.Count - 1 do
      TD2Object(Parent.FChildren[i]).FIndex:=-1;
    if IsVisual and not (csLoading in ComponentState) and (Parent.IsVisual) then
      Parent.Visual.Realign;
  end;
end;

function TD2Object.GetTagString: string;
begin
  Result:= FTagString;
end;

procedure TD2Object.SetTagString(const Value: string);
begin
  FTagString:=Value;
end;

procedure TD2Object.Exchange(AObject1,AObject2:TD2Object);
var
  Idx:integer;
begin
  if (FChildren <> nil) and (AObject1.Parent=Self) and (AObject2.Parent=Self) then
  begin
    FChildren.Exchange(AObject1.Index,AObject2.Index);
    Idx:=AObject1.FIndex;
    AObject1.FIndex:=AObject2.Index;
    AObject2.FIndex:=Idx;
    if IsVisual and not (csLoading in ComponentState) and (Parent.IsVisual) then
      Parent.Visual.Realign;
  end;
end;

procedure TD2Object.RemoveObject(AObject:TD2Object);
var
  i:integer;
begin
  if (FChildren <> nil) and (FChildren.IndexOf(AObject) < 0) then Exit;
  if IsVisual and AObject.IsVisual and (TD2VisualObject(Self).FTabList <> nil) then
  begin
    TD2VisualObject(AObject).FUpdating:=0;
    TD2VisualObject(Self).FTabList.Remove(AObject);
{    if AObject.isVisual and (TD2VisualObject(AObject).Align <> vaNone) then
      TD2VisualObject(Self).Realign;}
  end;
  if IsVisual and AObject.IsVisual then
    AObject.Visual.Repaint;
  AObject.FParent:=nil;
  AObject.SetNewScene(nil);
  // recalc Index
  if AObject.FIndex >= 0 then
    for i:=AObject.FIndex to FChildren.Count - 1 do
      TD2Object(FChildren[i]).FIndex:=-1;

  if FChildren <> nil then
    FChildren.Remove(AObject);
  if IsVisual then
    Visual.RecalcHasEffect;
  if IsVisual and AObject.IsVisual then
  begin
    TD2VisualObject(Self).RecalcNeedAlign;
    if AObject.Visual.TempCanvas <> nil then
      AObject.Visual.TempCanvas:=nil;
  end;
end;

procedure TD2Object.BringToFront;
var
  i:integer;
begin
  if (Parent <> nil) and (Parent.FChildren <> nil) then
  begin
    Parent.FChildren.Remove(Self);
    Parent.FChildren.Add(Self);
    // recalc Index
    for i:=0 to Parent.FChildren.Count - 1 do
      TD2Object(Parent.FChildren[i]).FIndex:=-1;
    if isVisual then
      TD2VisualObject(Self).Repaint;
  end;
end;

procedure TD2Object.SendToBack;
var
  i:integer;
begin
  if (Parent <> nil) and (Parent.FChildren <> nil) then
  begin
    Parent.FChildren.Remove(Self);
    Parent.FChildren.Insert(0,Self);
    // recalc Index
    for i:=0 to Parent.FChildren.Count - 1 do
      TD2Object(Parent.FChildren[i]).FIndex:=-1;
    if Parent.IsVisual then
      TD2VisualObject(Parent).Realign;
  end;
end;


//=============== TD2BrushObject ==========================================

constructor TD2BrushObject.Create(AOwner:TComponent);
begin
  inherited;
  FBrush:=TD2Brush.Create(d2BrushSolid,$FFFFFFFF);
end;

destructor TD2BrushObject.Destroy;
begin
  FreeAndNil(FBrush);
  inherited;
end;

procedure TD2BrushObject.SetName(const NewName:TComponentName);
begin
  inherited;
  if FResourceName='' then
    FResourceName:=Name;
end;

//=============== TD2PathObject ==========================================

constructor TD2PathObject.Create(AOwner:TComponent);
begin
  inherited;
  FPath:=TD2PathData.Create();
end;

destructor TD2PathObject.Destroy;
begin
  FreeAndNil(FPath);
  inherited;
end;

procedure TD2PathObject.SetName(const NewName:TComponentName);
begin
  inherited;
  if FResourceName='' then
    FResourceName:=Name;
end;

//=============== TD2BitmapObject ======================================

constructor TD2BitmapObject.Create(AOwner:TComponent);
begin
  inherited;
  FBitmap:=TD2Bitmap.Create(1,1);
end;

destructor TD2BitmapObject.Destroy;
begin
  FreeAndNil(FBitmap);
  inherited;
end;

procedure TD2BitmapObject.SetName(const NewName:TComponentName);
begin
  inherited;
  if FResourceName='' then
    FResourceName:=Name;
end;      
          
  
