{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//==================== TD2DrakonNode =======================================

procedure TD2DrakonNode.SetNodeDown(AValue: TD2DrakonNode);
begin
  if FNodeDown=AValue then Exit;
  FNodeDown:=AValue;
end;

procedure TD2DrakonNode.SetNodeLeft(AValue: TD2DrakonNode);
begin
  if FNodeLeft=AValue then Exit;
  FNodeLeft:=AValue;
end;

procedure TD2DrakonNode.SetNodeRight(AValue: TD2DrakonNode);
begin
  if FNodeRight=AValue then Exit;
  FNodeRight:=AValue;
end;

procedure TD2DrakonNode.SetNodeType(AValue: TD2DrakonNodeType);
begin
  if FNodeType=AValue then Exit;
  FNodeType:=AValue;
end;

procedure TD2DrakonNode.SetNodeUp(AValue: TD2DrakonNode);
begin
  if FNodeUp=AValue then Exit;
  FNodeUp:=AValue;
end;

constructor TD2DrakonNode.Create(AOwner: TComponent);
begin
  FNodeIndex:=-1;
  FNodeType:=drNone;
  FShape:= nil;
  FNodeDown:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  FNodeUp:= nil;
  inherited;
end;

destructor TD2DrakonNode.Destroy;
begin
  FreeAndNil(FNodeDown);
  FreeAndNil(FNodeLeft);
  FreeAndNil(FNodeRight);
  FreeAndNil(FNodeUp);
  FShape:= nil;
  inherited;
end;
        
         

            
  
