{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2ControlActionLink =================================================

procedure TD2ControlActionLink.AssignClient(AClient:TObject);
begin
  FClient:=AClient as TD2Control;
end;

function TD2ControlActionLink.DoShowHint(var HintStr:string):boolean;
begin
  Result:=True;
  if Action is TCustomAction then
  begin
    if TCustomAction(Action).DoHint(HintStr) and Application.HintShortCuts and
      (TCustomAction(Action).ShortCut <> scNone) then
    begin
{      if HintStr <> '' then
        HintStr:=Format('%s (%s)',[HintStr,ShortCutToText(TCustomAction(Action).ShortCut)]);}
    end;
  end;
end;

function TD2ControlActionLink.IsCaptionLinked:boolean;
begin
  Result:=inherited IsCaptionLinked and
    (FClient is TD2TextControl) and (TD2TextControl(FClient).TextW=(Action as TCustomAction).Caption);
end;

function TD2ControlActionLink.IsEnabledLinked:boolean;
begin
  Result:=inherited IsEnabledLinked and
    (FClient.Enabled=(Action as TCustomAction).Enabled);
end;

function TD2ControlActionLink.IsHintLinked:boolean;
begin
  Result:=inherited IsHintLinked and
    (FClient.Hint=(Action as TCustomAction).Hint);
end;

function TD2ControlActionLink.IsVisibleLinked:boolean;
begin
  Result:=inherited IsVisibleLinked and
    (FClient.Visible=(Action as TCustomAction).Visible);
end;

function TD2ControlActionLink.IsOnExecuteLinked:boolean;
begin
  Result:=inherited IsOnExecuteLinked and
    (@FClient.OnClick=@Action.OnExecute);
end;

function TD2ControlActionLink.IsHelpLinked:boolean;
begin
  Result:=inherited IsHelpLinked and
    (FClient.HelpContext=TCustomAction(Action).HelpContext) and
    (FClient.HelpKeyword=TCustomAction(Action).HelpKeyword) and
    (FClient.HelpType=TCustomAction(Action).HelpType);
end;

procedure TD2ControlActionLink.SetHelpKeyword(const Value:String);
begin
  if IsHelpLinked then FClient.HelpKeyword:=Value;
end;

procedure TD2ControlActionLink.SetHelpContext(Value:THelpContext);
begin
  if IsHelpLinked then FClient.HelpContext:=Value;
end;

procedure TD2ControlActionLink.SetHelpType(Value:THelpType);
begin
  if IsHelpLinked then FClient.HelpType:=Value;
end;

procedure TD2ControlActionLink.SetCaption(const Value:string);
begin
  if IsCaptionLinked then TD2TextControl(FClient).TextW:=Value;
end;

procedure TD2ControlActionLink.SetEnabled(Value:boolean);
begin
  if IsEnabledLinked then FClient.Enabled:=Value;
end;

procedure TD2ControlActionLink.SetHint(const Value:string);
begin
  if IsHintLinked then FClient.Hint:=Value;
end;

procedure TD2ControlActionLink.SetVisible(Value:boolean);
begin
  if IsVisibleLinked then FClient.Visible:=Value;
end;

procedure TD2ControlActionLink.SetOnExecute(Value:TNotifyEvent);
begin
  if IsOnExecuteLinked then FClient.OnClick:=Value;
end;


//============== TEdtActionStack ===============================

//constructor TEdtActionStack.Create(AOwner:TD2Memo);	//Deleted by GoldenFox
constructor TEdtActionStack.Create(AOwner:TD2CustomMemo);	//Added by GoldenFox
begin
  inherited Create;
  FOwner:=AOwner;
end;

destructor TEdtActionStack.Destroy;
var
  TmpItem :PEdtAction;
begin
  while AtLeast(1) do begin
    TmpItem:=Pop;
    Finalize(TmpItem^);
    FreeMem(TmpItem);
  end;
  inherited;
end;

procedure TEdtActionStack.FragmentDeleted(StartPos:integer; Fragment:WideString);
var
  TmpItem :PEdtAction;
begin
  if Fragment='' then Exit;

  if (not AtLeast(1)) or
      not ((PEdtAction(Peek)^.ActionType=atDelete) and
           (PEdtAction(Peek)^.StartPosition-StartPos-Length(Fragment) <= 1) and
           (PEdtAction(Peek)^.StartPosition-StartPos >= 0)) then
  begin
    New(TmpItem);
    Initialize(TmpItem^);
    Push(TmpItem);

    with TmpItem^ do begin
      ActionType:=atDelete;
      StartPosition:=StartPos;
      DeletedFragment:=Fragment;
      PairedWithPriv:=false;
    end;
  end
  else
    case PEdtAction(Peek)^.ActionType of
      atDelete :begin
        if StartPos > 0 then begin
          if StartPos < PEdtAction(Peek)^.StartPosition then
            PEdtAction(Peek)^.DeletedFragment:=Fragment+PEdtAction(Peek)^.DeletedFragment
          else
            PEdtAction(Peek)^.DeletedFragment:=PEdtAction(Peek)^.DeletedFragment+Fragment;
          PEdtAction(Peek)^.StartPosition:=StartPos;
        end;
      end;
    end;
end;

procedure TEdtActionStack.FragmentInserted(StartPos,FragmentLength:integer; IsPairedWithPriv :boolean);
var
  TmpItem :PEdtAction;
begin
  if FragmentLength=0 then Exit;

  if (not AtLeast(1)) or
      not ((PEdtAction(Peek)^.ActionType=atInsert) and
           (PEdtAction(Peek)^.StartPosition+PEdtAction(Peek)^.Length =StartPos)) then
  begin
    New(TmpItem);
    Initialize(TmpItem^);
    Push(TmpItem);
    with TmpItem^ do begin
      ActionType:=atInsert;
      StartPosition:=StartPos;
      Length:=FragmentLength;
      PairedWithPriv:=IsPairedWithPriv;
    end;
  end else
    case PEdtAction(Peek)^.ActionType of
      atInsert :PEdtAction(Peek)^.Length:=PEdtAction(Peek)^.Length+FragmentLength;
    end;
end;

procedure TEdtActionStack.CaretMovedBy(Shift:integer);
begin

end;

function TEdtActionStack.RollBackAction:boolean;
var
  TmpItem :PEdtAction;
  WasPaired :boolean;
  LTmpOptions :TInsertOptions;
begin
  Result:=AtLeast(1);
  if not(Result and Assigned(FOwner)) then Exit;

  repeat
    TmpItem:=Pop;

    with TmpItem^,FOwner do begin
      if DeletedFragment<>#13+#10 then
        LTmpOptions:=[ioSelected]
      else
        LTmpOptions:=[];

      case ActionType of
        atDelete :InsertAfter(TextPosToPos(StartPosition-1),DeletedFragment,LTmpOptions+[ioMoveCaret]{DeletedFragment<>#13+#10,true,false,false});
        atInsert :DeleteFrom(TextPosToPos(StartPosition),Length,[doMoveCaret]);
      end;
    end;

    WasPaired:=TmpItem^.PairedWithPriv;
    Finalize(TmpItem^);
    Dispose(TmpItem);
  until (not AtLeast(1)) or (not WasPaired);
end;
                    
            
        
         

            
  
