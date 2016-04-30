{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//===================  TD3Font =========================================

constructor TD3Font.Create;
begin
  inherited ;
  FSize:=12;
  FFamily:='Tahoma';
end;

destructor TD3Font.Destroy;
begin
  inherited;
end;

procedure TD3Font.Assign(Source:TPersistent);
begin
  if Source is TD3Font then
  begin
    FFamily:=(Source as TD3Font).Family;
    FSize:=(Source as TD3Font).Size;
    FStyle:=(Source as TD3Font).Style;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
  if Source is TD2Font then
  begin
    FFamily:=(Source as TD2Font).Family;
    FSize:=(Source as TD2Font).Size;
    FStyle:=TD3FontStyle((Source as TD2Font).Style);
    if Assigned(FOnChanged) then FOnChanged(Self);
  end else
  if Source is TFont then
  begin
    FFamily:=TFont(Source).Name;
    FSize:=Abs(TFont(Source).Height);

    if TFont(Source).Style=[] then         FStyle:=d3FontRegular;
    if TFont(Source).Style=[fsBold] then   FStyle:=d3FontBold;
    if TFont(Source).Style=[fsItalic] then FStyle:=d3FontItalic;
    if TFont(Source).Style=[fsBold,fsItalic] then  FStyle:=d3FontBoldItalic;

    if Assigned(FOnChanged) then FOnChanged(Self);
  end else
    inherited;
end;

procedure TD3Font.AssignTo(Dest:TPersistent);
begin
  if Dest is TFont then
  begin
    (Dest as TFont).Name:=FFamily;
    (Dest as TFont).Height:=-round(FSize);
    case Style of
      d3FontRegular:TFont(Dest).Style:=[];
      d3FontBold:TFont(Dest).Style:=[fsBold];
      d3FontItalic:TFont(Dest).Style:=[fsItalic];
      d3FontBoldItalic:TFont(Dest).Style:=[fsBold,fsItalic];
    end;
  end
  else
  if Dest is TD2Font then
  begin
    TD2Font(Dest).Family:=Family;
    TD2Font(Dest).Size:=Size;
    TD2Font(Dest).Style:=TD2FontStyle(Style);
  end
  else
    inherited;
end;

function TD3Font.isSizeStored:boolean;
begin
  Result:=FSize <> 12;
end;

function TD3Font.isFamilyStored:boolean;
begin
  Result:=FFamily <> 'Tahoma';
end;

procedure TD3Font.SetFamily(const Value:string);
begin
  if FFamily <> Value then
  begin
    FFamily:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD3Font.SetSize(const Value:single);
begin
  if FSize <> Value then
  begin
    FSize:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD3Font.SetStyle(const Value:TD3FontStyle);
begin
  if FStyle <> Value then
  begin
    FStyle:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;
         
           
