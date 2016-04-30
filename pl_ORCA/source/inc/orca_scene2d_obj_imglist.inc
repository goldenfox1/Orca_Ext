{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


// ======================TD2ImageList ====================================================

constructor TD2ImageList.Create(AOwner:TComponent);
begin
  inherited;
  FImages:=TList.Create;
  Width:=32;
  Height:=32;
end;

destructor TD2ImageList.Destroy;
begin
  Clear;
  FImages.Free;
  inherited;
end;

procedure TD2ImageList.Clear;
var
  i:integer;
begin
  inherited Clear;

  for i:=0 to FImages.Count - 1 do
    TD2Bitmap(FImages[i]).Free;
  FImages.Clear;
end;

function TD2ImageList.Add(Bitmap:TD2Bitmap):integer;
var
  B:TD2Bitmap;
  R:TD2Rect;
begin
  B:=TD2Bitmap.Create(Width,Height);
  R:=d2Rect(0,0,Bitmap.Width,Bitmap.Height);
  d2FitRect(R,d2Rect(0,0,Width,Height));
  B.Canvas.DrawBitmap(Bitmap,d2Rect(0,0,Bitmap.Width,Bitmap.Height),R,1);

  Result:=FImages.Add(B);
  UpdateList;
end;

procedure TD2ImageList.ReadImage(Stream:TStream);
var
  i:integer;
  S,C:cardinal;
  M:TMemoryStream;
  B:TD2Bitmap;
begin
  Stream.Read(C,4);
  for i:=0 to C - 1 do
  begin
    M:=TMemoryStream.Create;

    Stream.Read(S,4);
    M.SetSize(S);
    Stream.Read(M.Memory^,S);

    B:=TD2Bitmap.CreateFromStream(M);
    FImages.Add(B);

    M.Free;
  end;
  UpdateList;
end;

procedure TD2ImageList.WriteImage(Stream:TStream);
var
  i:integer;
  S:cardinal;
  M:TMemoryStream;
begin
  S:=FImages.Count;
  Stream.Write(S,4);
  for i:=0 to FImages.Count - 1 do
  begin
    M:=TMemoryStream.Create;
    TD2Bitmap(FImages[i]).SaveToStream(M);

    S:=M.Size;
    Stream.Write(S,4);
    Stream.Write(M.Memory^,M.Size);

    M.Free;
  end;
end;

procedure TD2ImageList.DefineProperties(Filer:TFiler);
begin
  inherited DefineProperties(Filer);
  Filer.DefineBinaryProperty('Images',ReadImage,WriteImage,FImages.Count > 0);
end;

function TD2ImageList.GetCount:integer;
begin
  Result:=FImages.Count;
end;

function TD2ImageList.GetBitmap(Index:integer):TD2Bitmap;
begin
  if (Index >= 0) and (Index < Count) then
    Result:=TD2Bitmap(FImages[Index])
  else
    Result:=nil;
end;

procedure TD2ImageList.UpdateList;
var
  i:integer;
  B:TBitmap;
begin
  if not (csDesigning in ComponentState) then Exit;

  inherited Clear;

  for i:=0 to FImages.Count - 1 do
  begin
    if (Images[i].Width <> Width) or (Images[i].Height <> Height) then Continue;

    B:=TBitmap.Create;
    B.Assign(Images[i]);
    inherited AddMasked(B,0);
    B.Free;
  end;
end;

procedure TD2ImageList.ReadData(Stream:TStream);
begin
end;

procedure TD2ImageList.WriteData(Stream:TStream);
begin
end;            
