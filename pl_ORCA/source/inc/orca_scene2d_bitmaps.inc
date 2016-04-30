{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Bitmap ===================================================================}

constructor TD2Bitmap.Create(const AWidth,AHeight:integer; const APremulAlpha:boolean=true);
begin
  inherited Create;
  FWidth:=AWidth;
  FHeight:=AHeight;
  Recreate;
end;

constructor TD2Bitmap.CreateFromStream(const AStream:TStream);
begin
  Create(0,0);
  LoadFromStream(AStream);
end;

constructor TD2Bitmap.CreateFromBitmapAndMask(const Bitmap,Mask:TD2Bitmap);
var
  i:integer;
begin
  Create(Bitmap.Width,Bitmap.Height);
  if (Bitmap.Width <> Mask.Width) or (Bitmap.Height <> Mask.Height) then Exit;
  for i:=0 to FHeight * FWidth - 1 do
  begin
    PD2ColorRecArray(FBits)[i]:=PD2ColorRecArray(Bitmap.FBits)[i];
    PD2ColorRecArray(FBits)[i].A:=PD2ColorRecArray(Mask.FBits)[i].R;
  end;
end;

destructor TD2Bitmap.Destroy;
begin
  if Assigned(FOnDestroyHandle) then
    FOnDestroyHandle(Self);
  if FResource <> nil then
  begin
    FResource.RemoveFreeNotify(Self);
    FResource:=nil;
  end;
  if Assigned(FCanvas) then
    FreeAndNil(FCanvas);
  Handle:=0;
  if FBits <> nil then
    FreeMem(FBits,FWidth * FHeight * SizeOf(TD2Color));
  inherited;
end;

procedure TD2Bitmap.SetResource(const Value:TD2BitmapObject);
begin
  if FResource <> Value then
  begin
    if FResource <> nil then
      FResource.RemoveFreeNotify(Self);
    FResource:=Value;
    if FResource <> nil then
    begin
      FResourceName:=FResource.ResourceName;
      FResource.AddFreeNotify(Self);
    end
    else
      FResourceName:='';
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

function TD2Bitmap.GetResourceName:string;
begin
  Result:=FResourceName;
end;

procedure TD2Bitmap.SetResourceName(const Value:string);
begin
  if Value <> FResourceName then
  begin
    FResourceName:=Value;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

function TD2Bitmap.GetBitmap:TD2Bitmap;
var
  O:TD2Object;
begin
  Result:=nil;
  if FResource <> nil then
  begin
    Result:=TD2BitmapObject(FResource).Bitmap;
  end
  else
  if FResourceName <> '' then
  begin
    O:=FindResource(FResourceName);
    if O is TD2BitmapObject then
      Resource:=TD2BitmapObject(O);
    if FResource <> nil then
      Result:=TD2BitmapObject(FResource).Bitmap;
  end;
end;

procedure TD2Bitmap.SetSize(const AWidth,AHeight:integer);
begin
  if (FWidth <> AWidth) or (FHeight <> AHeight) then
  begin
    if FBits <> nil then
      FreeMem(FBits,FWidth * FHeight * SizeOf(TD2Color));
    FBits:=nil;
    FWidth:=AWidth;
    FHeight:=AHeight;
    Recreate;
    BitmapChanged;
  end;
end;

procedure TD2Bitmap.Recreate;
begin
  if Assigned(FOnBitmapDestroy) then  FOnBitmapDestroy(Self);
  if Assigned(FOnDestroyHandle) then  FOnDestroyHandle(Self);
  FHandle:=0;
  if Assigned(FCanvas) then  FreeAndNil(FCanvas);
  FCanvas:=nil;
  if FBits <> nil then FreeMem(FBits);
  FBits:=nil;

  if not IsEmpty then
  begin
    GetMem(FBits,FWidth * FHeight * SizeOf(TD2Color));
    Clear(0);
  end;

  FNeedUpdate:=true;
  if Assigned(FOnBitmapCreate) then  FOnBitmapCreate(Self);
end;

procedure TD2Bitmap.Clear(const AColor:TD2Color);
begin
  if FBits <> nil then
  begin
    d2FillLongword(FBits,FWidth * FHeight,d2PremultyAlpha(AColor));
    BitmapChanged;
    FNeedUpdate:=true;
  end;
end;

procedure TD2Bitmap.ClearRect(const ARect:TD2Rect;
  const AColor:TD2Color);
var
  R:TRect;
begin
  if FBits <> nil then
  begin
    R:=Rect(Trunc(ARect.Left),Trunc(ARect.Top),Round(ARect.Right),Round(ARect.Bottom));

    if R.Left < 0 then R.Left:=0;
    if R.Top < 0 then R.Top:=0;
    if R.Right > FWidth then R.Right:=FWidth;
    if R.Bottom > FHeight then R.Bottom:=FHeight;
    if R.Bottom < R.Top then R.Bottom:=R.Top;
    if R.Right < R.Left then R.Right:=R.Left;
    if (R.Right < 0) or (R.Top < 0) or (R.Left > FWidth) or (R.Top > FHeight) then Exit;

    d2FillLongwordRect(FBits,FWidth,FHeight,R.Left,R.Top,R.Right,R.Bottom,d2PremultyAlpha(AColor));
    FNeedUpdate:=true;
  end;
end;

function TD2Bitmap.GetPixels(x,y:integer):TD2Color;
begin
  if (x >= 0) and (y >= 0) and (x < FWidth) and (y < FHeight) and (FBits <> nil) then
    Result:=FBits[x + (y * FHeight)]
  else
    Result:=0;
end;

procedure TD2Bitmap.BitmapChanged;
begin
  if Assigned(FOnChange) then FOnChange(Self);
end;

function TD2Bitmap.IsEmpty:boolean;
begin
  Result:=FWidth * FHeight=0;
end;

procedure TD2Bitmap.DrawGraphic(const Graphic:TGraphic; const DstRect:TD2Rect);
var
  Bitmap:TBitmap;
  SL:PD2ColorArray;
  i,j:integer;
//  xbmp:TLazIntfImage;
begin

  Bitmap:=TBitmap.Create;
  try
    Bitmap.HandleType:=bmDIB;
    Bitmap.PixelFormat:=pf32bit;
    Bitmap.Width:=FWidth;
    Bitmap.Height:=FHeight;
    Bitmap.Canvas.Brush.Color:=RGB(255,0,255);
    Bitmap.Canvas.StretchDraw(Rect(round(DstRect.left),round(DstRect.top),round(DstRect.right),round(DstRect.bottom)),Graphic);

    Bitmap.CreateIntfImage;

    for j:=0 to FHeight - 1 do
    begin
      SL :=Bitmap.ScanLine[j];
      for i:=0 to FWidth - 1 do
        if (TD2ColorRec(SL[i]).R=$FF) and (TD2ColorRec(SL[i]).G=0) and (TD2ColorRec(SL[i]).B=$FF) then
          Continue else
          FBits[i + (j * Width)]:=SL[i];
    end;
    //Bitmap.LoadFromIntfImage(xbmp);
   // xbmp.free;
  finally
    Bitmap.Free;
  end;
end;

procedure TD2Bitmap.Assign(Source:TPersistent);
var
  SLine:PD2ColorArray;
  SLine24:PD2Color24Array;
  i,j:integer;
begin
  if Source is TD2Bitmap then
  begin
    if TD2Bitmap(Source).ResourceBitmap <> nil then
    begin
      Resource:=TD2Bitmap(Source).Resource;
      FResourceName:=TD2Bitmap(Source).ResourceName;
      if Assigned(FOnChange) then FOnChange(Self);
    end
    else
    begin
      Resource:=nil;
      FResourceName:='';
      SetSize(TD2Bitmap(Source).Width,TD2Bitmap(Source).Height);
      d2MoveLongword(TD2Bitmap(Source).FBits,FBits,Width * Height);
      FNeedUpdate:=true;
      if Assigned(FOnChange) then FOnChange(Self);
    end;
  end
  else
    if (Source is TBitmap) and ((Source as TBitmap).PixelFormat=pf32bit) and ((Source as TBitmap).HandleType=bmDIB) then
    with (Source as TBitmap) do
    begin
      Self.SetSize(Width,Height);
      for j:=0 to Height - 1 do
      begin
        SLine:=Scanline[j];
        for i:=0 to Width - 1 do
        begin
          if Transparent and (SLine^[i]=ColorToRGB(TransparentColor)) then
            FBits[i + (j * Width)]:=0
          else
            FBits[i + (j * Width)]:=SLine^[i];
        end;
      end;
      FNeedUpdate:=true;
      if Assigned(FOnChange) then FOnChange(Self);
    end
    else
    if (Source is TBitmap) and ((Source as TBitmap).PixelFormat=pf24bit) then
    with (Source as TBitmap) do
    begin
      Self.SetSize(Width,Height);

      for j:=0 to Height - 1 do
      begin
        SLine24:=PD2Color24Array(Scanline[j]);
        for i:=0 to Width - 1 do
        begin
          if Transparent and (RGB(SLine24[i].R,SLine24^[i].G,SLine24^[i].B)=ColorToRGB(TransparentColor)) then
            FBits[i + (j * Width)]:=0
          else
            FBits[i + (j * Width)]:=$FF000000 + RGB(SLine24^[i].R,SLine24^[i].G,SLine24^[i].B);
        end;
      end;
      FNeedUpdate:=true;
      if Assigned(FOnChange) then FOnChange(Self);
    end
    else
      if Source is TGraphic then
      begin
        SetSize(TGraphic(Source).Width,TGraphic(Source).Height);
        if FBits=nil then Exit;
        DrawGraphic(TGraphic(Source),d2Rect(0,0,FWidth,FHeight));
        d2FillAlpha(FBits,FWidth * FHeight,$FF);
        FNeedUpdate:=true;
        if Assigned(FOnChange) then FOnChange(Self);
      end
      else
        if Source is TPicture then
        begin
          with TPicture(Source) do
          begin
            // icons,metafiles etc...
            Self.SetSize(TPicture(Source).Graphic.Width,TPicture(Source).Graphic.Height);
            if FBits=nil then Exit;
            DrawGraphic(TPicture(Source).Graphic,d2Rect(0,0,FWidth,FHeight));
            d2FillAlpha(FBits,FWidth * FHeight,$FF);
            FNeedUpdate:=true;
            if Assigned(FOnChange) then FOnChange(Self);
          end;
        end
        else
          inherited;
end;

procedure TD2Bitmap.AssignTo(Dest:TPersistent);
var
  i:integer;
  B:TBitmap;
//  xbmp:TLazIntfImage;
begin
  if Dest is TPicture then
  begin
    B:=TBitmap.Create;
    B.HandleType:=bmDIB;
    B.PixelFormat:=pf32bit;
    B.Width:=FWidth;
    B.Height:=FHeight;
  //  xbmp:=B.CreateIntfImage;

    for i:=0 to FHeight - 1 do
      System.Move(Scanline[i]^,B.ScanLine[i]^,Width * 4);

   // B.LoadFromIntfImageX(xbmp);
   // xbmp.free;

    TPicture(Dest).Assign(B);
    B.Free;
  end
  else
  if Dest is TBitmap then
  begin
    TBitmap(Dest).HandleType:=bmDIB;
    TBitmap(Dest).PixelFormat:=pf32bit;
    TBitmap(Dest).Width:=FWidth;
    TBitmap(Dest).Height:=FHeight;
   // xbmp:=TBitmap(Dest).CreateIntfImage;

    for i:=0 to FHeight - 1 do
      System.Move(Scanline[i]^,TBitmap(Dest).ScanLine[i]^,Width * 4);

  //  TBitmap(Dest).LoadFromIntfImage(xbmp);
  //  xbmp.free;
  end
  else
    inherited ;
end;

procedure TD2Bitmap.DefineProperties(Filer:TFiler);
begin
  inherited;
  Filer.DefineBinaryProperty('PNG',ReadBitmap,WriteBitmap,FWidth * FHeight > 0);
end;

procedure TD2Bitmap.ReadBitmap(Stream:TStream);
begin
  LoadFromStream(Stream);
end;

procedure TD2Bitmap.WriteBitmap(Stream:TStream);
begin
  SaveToStream(Stream);
end;

procedure TD2Bitmap.DoLoaded(Sender:TObject);
begin
  if Assigned(FOnThreadLoaded) then
    FOnThreadLoaded(Self);
end;

procedure TD2Bitmap.Rotate(const Angle:single);
var
  temp:TD2Bitmap;
  M,M2:TD2Matrix;
  Pts:array [1..4] of TD2Point;
  R:TD2Rect;
begin
  if Angle=0 then Exit;

  M:=IdentityMatrix;
  M.m31:=-FWidth / 2;
  M.m32:=-FHeight / 2;
  M:=d2MatrixMultiply(M,d2CreateRotationMatrix(d2DegToRad(Angle)));

  Pts[1]:=d2PointFromVector(d2VectorTransform(d2Vector(0,0),M));
  Pts[2]:=d2PointFromVector(d2VectorTransform(d2Vector(FWidth,0),M));
  Pts[3]:=d2PointFromVector(d2VectorTransform(d2Vector(FWidth,FHeight),M));
  Pts[4]:=d2PointFromVector(d2VectorTransform(d2Vector(0,FHeight),M));

  R:=d2NormalizeRect(Pts);

  M2:=IdentityMatrix;
  M2.m31:=d2RectWidth(R) / 2;
  M2.m32:=d2RectHeight(R) / 2;
  M:=d2MatrixMultiply(M,M2);

  temp:=TD2Bitmap.Create(Trunc(d2RectWidth(R)),Trunc(d2RectHeight(R)));
  temp.Clear(0);
  temp.Canvas.BeginScene;
  temp.Canvas.SetMatrix(M);
  temp.Canvas.DrawBitmap(Self,d2Rect(0,0,FWidth,FHeight),d2Rect(0,0,FWidth,FHeight),1);
  temp.Canvas.EndScene;
  Assign(temp);
  temp.Free;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TD2Bitmap.FlipHorizontal;
var
  i:integer;
  tmp:PD2ColorArray;
begin
  GetMem(tmp,Width * 4);
  for i:=0 to (Height - 1) div 2 do
  begin
    System.Move(Scanline[Height - 1 - i][0],tmp[0],Width * 4);
    System.Move(Scanline[i][0],Scanline[Height - 1 - i][0],Width * 4);
    System.Move(tmp[0],Scanline[i][0],Width * 4);
  end;
  if Assigned(FOnChange) then FOnChange(Self);
  FreeMem(tmp,Width * 4);
end;

procedure TD2Bitmap.FlipVertical;
var
  i,j:integer;
  tmp:TD2Color;
begin
  for j:=0 to Height - 1 do
    for i:=0 to (Width - 1) div 2 do
    begin
      tmp:=Scanline[j][Width - 1 - i];
      Scanline[j][Width - 1 - i]:=Scanline[j][i];
      Scanline[j][i]:=tmp;
    end;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TD2Bitmap.InvertAlpha;
var
  i,j:integer;
  Bits:PD2ColorRecArray;
begin
  Bits:=PD2ColorRecArray(Startline);
  for j:=0 to Height - 1 do
    for i:=0 to Width - 1 do
    begin
      Bits[i + (j * Width)].Color:=d2UnpremultyAlpha(Bits[i + (j * Width)].Color);
      TD2ColorRec(Bits[(i) + ((j) * Width)]).A:=$FF - TD2ColorRec(Bits[(i) + ((j) * Width)]).A;
      Bits[i + (j * Width)].Color:=d2PremultyAlpha(Bits[i + (j * Width)].Color);
    end;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TD2Bitmap.FillColor(const Color:TD2Color);
var
  Bits:PD2ColorRecArray;
  i,j:integer;
  a:byte;
begin
  Bits:=PD2ColorRecArray(Startline);
  for j:=0 to Height - 1 do
  begin
    for i:=0 to Width - 1 do
    begin
      {$ifdef FPC_BIG_ENDIAN}
      a:=TD2ColorRec(Bits[(i) + ((j) * Width)]).Color and $FF;
      {$else}
      a:=TD2ColorRec(Bits[(i) + ((j) * Width)]).A;
      {$endif}
      if a > 0 then
      begin
        bits[(i) + ((j) * Width)].Color:=d2PremultyAlpha(d2Opacity(Color,a / $FF));
        {$ifdef FPC_BIG_ENDIAN}
        ReverseBytes(@Bits[(i) + ((j) * Width)].Color,4);
        {$endif}
      end;
    end
  end;
  if Assigned(FOnChange) then FOnChange(Self);
  FNeedUpdate:=true;
end;

function TD2Bitmap.CreateMask:PByteArray;
var
  a:byte;
  Bits:PD2ColorRecArray;
  i,j:integer;
begin
  GetMem(Result,Width * Height);
  FillChar(Result^,Width * Height,0);
  Bits:=PD2ColorRecArray(Startline);
  for j:=0 to Height - 1 do
  begin
    for i:=0 to Width - 1 do
    begin
      {$ifdef FPC_BIG_ENDIAN}
      a:=TD2ColorRec(Bits[(i) + ((j) * Width)]).Color and $FF;
      {$else}
      a:=TD2ColorRec(Bits[(i) + ((j) * Width)]).A;
      {$endif}
      if a > 0 then
      begin
        Result[i + (j * Width)]:=a;
      end;
    end
  end;
end;

procedure TD2Bitmap.ApplyMask(const Mask:PByteArray; const DstX:integer=0; const DstY:integer=0);
var
  Bits:PD2ColorRecArray;
  i,j:integer;
begin
  Bits:=PD2ColorRecArray(Startline);
  for j:=0 to Height - 1 do
  begin
    for i:=0 to Width - 1 do
    begin
      if (i - DstX < 0) or (i - DstX > Width - 1) or (j - DstY < 0) or (j - DstY > Height - 1) then Continue;

      if mask[i - DstX + ((j - DstY) * Width)] > 0 then
      begin
        Bits[i + (j * Width)].Color:=d2PremultyAlpha(d2Opacity(d2UnpremultyAlpha(Bits[i + (j * Width)].Color),($FF - mask[i - DstX + ((j - DstY) * Width)]) / $FF))
      end;
      {$ifdef FPC_BIG_ENDIAN}
      ReverseBytes(@Bits[(i) + ((j) * Width)].Color,4);
      {$endif}
    end
  end;
  if Assigned(FOnChange) then FOnChange(Self);
  FNeedUpdate:=true;
end;

function TD2Bitmap.CreateThumbnail(const Width,Height:integer):TD2Bitmap;
begin
  Result:=TD2Bitmap.Create(Width,Height);
  Result.Canvas.BeginScene;
  Result.Canvas.DrawThumbnail(Self,Width,Height);
  Result.Canvas.EndScene;
end;

procedure TD2Bitmap.LoadFromFile(const AFileName:string; const Rotate:single=0);
var
  Filter:TD2Filter;
begin
  if not FileExists(AFileName) then Exit;
  Filter:=GvarD2DefaultFilterClass.Create;
  if Filter.LoadFromFile(AFileName,Rotate,Self) then
    if Assigned(FOnChange) then FOnChange(Self);
  Filter.Free;
end;

procedure TD2Bitmap.LoadThumbnailFromFile(const AFileName:string; const AFitWidth,AFitHeight:single;
  const UseEmbedded:boolean=true);
var
  Filter:TD2Filter;
begin
  Filter:=GvarD2DefaultFilterClass.Create;
  if Filter.LoadThumbnailFromFile(AFileName,AFitWidth,AFitHeight,UseEmbedded,Self) then
    if Assigned(FOnChange) then FOnChange(Self);
  Filter.Free;
end;

procedure TD2Bitmap.SaveToFile(const AFileName:string; const Params:string='');
var
  Filter:TD2Filter;
begin
  Filter:=GvarD2DefaultFilterClass.Create;
  Filter.SaveToFile(AFileName,Self,Params);
  Filter.Free;
end;

procedure TD2Bitmap.LoadFromStream(Stream:TStream);
var
  Filter:TD2Filter;
  S:TStream;
begin
  if Stream.Position > 0 then
  begin
    S:=TMemoryStream.Create;
    S.CopyFrom(Stream,Stream.Size - Stream.Position);
    S.Position:=0;
    Filter:=GvarD2DefaultFilterClass.Create;
    if Filter.LoadFromStream(S,Self) then
      if Assigned(FOnChange) then FOnChange(Self);
    Filter.Free;
    S.Free;
  end
  else
  begin
    Filter:=GvarD2DefaultFilterClass.Create;
    if Filter.LoadFromStream(Stream,Self) then
      if Assigned(FOnChange) then FOnChange(Self);
    Filter.Free;
  end;
end;

procedure TD2Bitmap.SaveToStream(Stream:TStream);
var
  i:integer;
  hasAlpha:boolean;
  Filter:TD2Filter;
begin
  hasAlpha:=false;
  for i:=0 to FWidth * FHeight - 1 do
    if FBits[i] and $FF000000 <> $FF000000 then
    begin
      hasAlpha:=true;
      Break;
    end;

  if hasAlpha then
  begin
    Filter:=GvarD2DefaultFilterClass.Create;
    Filter.SaveToStream(Stream,Self,'png');
    Filter.Free;
  end
  else
  begin
    Filter:=GvarD2DefaultFilterClass.Create;
    Filter.SaveToStream(Stream,Self,'jpeg','quality=100');
    Filter.Free;
  end;
end;

procedure TD2Bitmap.SetHeight(const Value:integer);
begin
  if FHeight <> Value then
  begin
    FHeight:=Value;
    Recreate;
  end;
end;

procedure TD2Bitmap.SetWidth(const Value:integer);
begin
  if FWidth <> Value then
  begin
    FWidth:=Value;
    Recreate;
  end;
end;

function TD2Bitmap.GetCanvas:TD2Canvas;
begin
  if FCanvas=nil then
  begin
    FCanvas:=GvarD2DefaultCanvasClass.CreateFromBitmap(Self);
    FCanvas.SetMatrix(IdentityMatrix);
  end;
  Result:=FCanvas;
end;

function TD2Bitmap.GetScanline(y:integer):PD2ColorArray;
begin
  Result:=GvarD2DefaultCanvasClass.GetBitmapScanline(Self,y);
end; 

//=============== TD2ImageThread =========================================

constructor TD2ImageThread.Create(const AImage:TD2Image; const AFileName:string; const AUseThumbnails:boolean);
begin
  inherited Create(true);
  FFileName:=AFileName;
  FUseThumbnails:=AUseThumbnails;
  FImage:=AImage;
  Priority:=tpIdle;
  FreeOnTerminate:=true;
end;

destructor TD2ImageThread.Destroy;
begin
  inherited;
end;

procedure TD2ImageThread.Execute;
begin
  FTempBitmap:=TD2Bitmap.Create(0,0);
  if FUseThumbnails then
    FTempBitmap.LoadThumbnailFromFile(FFileName,FImage.Width,FImage.Height,true)
  else
    FTempBitmap.LoadFromFile(FFileName);
  Synchronize(Finished);
end;

procedure TD2ImageThread.Finished;
begin
  if FImage <> nil then
    FImage.Bitmap.Assign(FTempBitmap);
  FTempBitmap.Free;
end;                         
                    
