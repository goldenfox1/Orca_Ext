{%mainunit ../orca_scene2d.pas}
             
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Font =====================================================================}

constructor TD2Font.Create;
begin
  inherited ;
  FSize:=11;
  FFamily:='Tahoma';
  FClearType:=true;
end;

destructor TD2Font.Destroy;
begin
  inherited;
end;

procedure TD2Font.Assign(Source:TPersistent);
begin
  if Source is TD2Font then
  begin
    FFamily:=(Source as TD2Font).Family;
    FSize:=(Source as TD2Font).Size;
    FStyle:=(Source as TD2Font).Style;
    FClearType:=(Source as TD2Font).ClearType;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
  if Source is TFont then
  begin
    FFamily:=TFont(Source).Name;
    FSize:=Abs(TFont(Source).Height);
    if TFont(Source).Style=[] then
      FStyle:=d2FontRegular;
    if TFont(Source).Style=[fsBold] then
      FStyle:=d2FontBold;
    if TFont(Source).Style=[fsItalic] then
      FStyle:=d2FontItalic;
    if TFont(Source).Style=[fsBold,fsItalic] then
      FStyle:=d2FontBoldItalic;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end
  else
    inherited;
end;

procedure TD2Font.AssignTo(Dest:TPersistent);
begin
  if Dest is TFont then
  begin
    (Dest as TFont).Name:=FFamily;
    (Dest as TFont).Height:=-round(FSize);
    case Style of
      d2FontRegular:TFont(Dest).Style:=[];
      d2FontBold:TFont(Dest).Style:=[fsBold];
      d2FontItalic:TFont(Dest).Style:=[fsItalic];
      d2FontBoldItalic:TFont(Dest).Style:=[fsBold,fsItalic];
    end;
  end
  else
    inherited;
end;

function TD2Font.isSizeStored:boolean;
begin
  Result:=FSize <> 11;
end;

function TD2Font.isFamilyStored:boolean;
begin
  Result:=FFamily <> 'Tahoma';
end;

procedure TD2Font.SetFamily(const Value:string);
begin
  if FFamily <> Value then
  begin
    FFamily:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD2Font.SetSize(const Value:single);
begin
  if FSize <> Value then
  begin
    FSize:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD2Font.SetClearType(const Value:boolean);
begin
  if FClearType <> Value then
  begin
    FClearType:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

procedure TD2Font.SetStyle(const Value:TD2FontStyle);
begin
  if FStyle <> Value then
  begin
    FStyle:=Value;
    if Assigned(FOnChanged) then FOnChanged(Self);
  end;
end;

class function TD2Filter.GetFileTypes:string;
begin
  Result:='';
end;

class function TD2Filter.GetImageSize(const AFileName:string):TD2Point;
begin
  Result:=d2Point(0,0);
end;
                          
