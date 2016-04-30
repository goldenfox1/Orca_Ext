{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2StatusBar =====================================

constructor TD2StatusBar.Create(AOwner:TComponent);
begin
  inherited;
  FShowSizeGrip:=true;
  Height:=22;
  Align:=vaBottom;
end;

procedure TD2StatusBar.ApplyStyle;
var
  sizeGrip:TD2Object;
begin
  inherited;
  sizeGrip:=FindResource('sizegrip');
  if (sizeGrip <> nil) and (sizeGrip is TD2VisualObject) then
  begin
    TD2VisualObject(sizeGrip).visible:=FShowSizeGrip;
    if (Scene <> nil) and not (Scene.GetDesignTime) then
    begin
      TD2VisualObject(sizeGrip).Locked:=false;
      TD2VisualObject(sizeGrip).HitTest:=true;
    end;
  end;
end;

procedure TD2StatusBar.SetShowSizeGrip(const Value:boolean);
begin
  if FShowSizeGrip <> Value then
  begin
    FShowSizeGrip:=Value;
    Resource:=FResource;
  end;
end;

//=============== TD2ToolBar ==============================

constructor TD2ToolBar.Create(AOwner:TComponent);
begin
  inherited;
  Height:=40;
  Align:=vaTop;
end;


//=============== TD2DockBar ==========================================================

constructor TD2DockBar.Create(AOwner:TComponent);
begin
  inherited;
  FMinSize:=32;
  FMaxSize:=64;
end;

destructor TD2DockBar.Destroy;
begin
  inherited;
end;

procedure TD2DockBar.MouseLeave;
begin
  inherited;
  FMousePos:=d2Point(0,0);
  Realign;
end;

procedure TD2DockBar.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;

procedure TD2DockBar.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  FMousePos:=d2Point(X,Y);
  Realign;
end;

procedure TD2DockBar.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;

procedure TD2DockBar.Paint;
begin
  inherited;
end;

procedure TD2DockBar.Realign;
var
  i,j:integer;
  dist,Pos,MaxWidth,Amplitude:single;
  hot:boolean;
  List:TList;
begin
  inherited;
  if FChildren=nil then Exit;
  if FDisableAlign then Exit;
  FDisableAlign:=true;
  try
    { make order left to right list }
    List:=TList.Create;
    for i:=0 to FChildren.Count - 1 do
    begin
      if not TD2Object(FChildren[i]).isVisual then Continue;
      if not TD2VisualObject(FChildren[i]).Visible then Continue;

      if List.Count > 0 then
      begin
        for j:=0 to List.Count - 1 do
        begin
          if TD2VisualObject(FChildren[i]).Position.X < TD2VisualObject(List[j]).Position.X then
            Break;
        end;
        List.Insert(j,FChildren[i]);
      end
      else
        List.Add(FChildren[i]);
    end;
    { align }
    if FMousePos.X=0 then
    begin
      { mouse leave }
      MaxWidth:=List.Count * MinSize;
      for i:=0 to List.Count - 1 do
      begin
        TD2VisualObject(List[i]).Position.Y:=Height - MinSize;
        TD2VisualObject(List[i]).Position.X:=(Width / 2) - (MaxWidth / 2) + (i * MinSize);
        TD2VisualObject(List[i]).Height:=MinSize;
        TD2VisualObject(List[i]).Width:=MinSize;
      end;
    end
    else
    begin
      Amplitude:=FMaxSize * 1.5;
      MaxWidth:=(List.Count * MinSize);
      { check hot object }
      hot:=false;
      for i:=0 to List.Count - 1 do
      begin
        if (FMousePos.X >= TD2VisualObject(List[i]).Position.X) and (FMousePos.X <= TD2VisualObject(List[i]).Position.X + TD2VisualObject(List[i]).Width) then
        begin
          hot:=true;
          Break;
        end
      end;
      { set sizes }
      for i:=0 to List.Count - 1 do
      begin
        if (FMousePos.X >= TD2VisualObject(List[i]).Position.X) and (FMousePos.X <= TD2VisualObject(List[i]).Position.X + TD2VisualObject(List[i]).Width) then
        begin
          TD2VisualObject(List[i]).Width:=MaxSize;
          TD2VisualObject(List[i]).Height:=MaxSize;
          MaxWidth:=MaxWidth + (MaxSize - MinSize);
        end
        else
        begin
          dist:=(TD2VisualObject(List[i]).Position.X + (TD2VisualObject(List[i]).Width / 2)) - FMousePos.X;
          if (Abs(dist) < Amplitude) and (hot) then
          begin
            TD2VisualObject(List[i]).Width:=MinSize + Sin((Pi / 2) + ((dist / Amplitude) * (Pi / 2))) * (MaxSize - MinSize);
            TD2VisualObject(List[i]).Height:=TD2VisualObject(List[i]).Width;
            MaxWidth:=MaxWidth + (TD2VisualObject(List[i]).Width - MinSize);
          end
          else
          begin
            TD2VisualObject(List[i]).Width:=MinSize;
            TD2VisualObject(List[i]).Height:=MinSize;
          end;
        end;
      end;
      { align }
      Pos:=((Width / 2) - (MaxWidth / 2));
      for i:=0 to List.Count - 1 do
      begin
        TD2VisualObject(List[i]).Position.Y:=Height - TD2VisualObject(List[i]).Height - 1;
        TD2VisualObject(List[i]).Position.X:=Pos;
        Pos:=Pos + TD2VisualObject(List[i]).Height;
      end;
    end;
    { }
    List.Free;
  finally
    FDisableAlign:=false;
  end;
end;

procedure TD2DockBar.SetMaxSize(const Value:single);
begin
  FMaxSize:=Value;
end;

procedure TD2DockBar.SetMinSize(const Value:single);
begin
  FMinSize:=Value;
end;
          
                    
