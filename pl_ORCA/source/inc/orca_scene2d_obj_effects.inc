{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2Effect ====================================

constructor TD2Effect.Create(AOwner:TComponent);
begin
  inherited;
  FEnabled:=true;
end;

destructor TD2Effect.Destroy;
begin
  inherited;
end;

function TD2Effect.GetOffset:TD2Point;          
begin

end;

function TD2Effect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
end;

procedure TD2Effect.ApplyTrigger(AInstance:TD2Object; ATrigger:string);
var
  StartValue:boolean;
  Line,Setter,Prop,Value:AnsiString;
begin
  if FTrigger='' then Exit;
  if AInstance=nil then Exit;
  if Pos(LowerCase(ATrigger),LowerCase(FTrigger))=0 then Exit;

  Line:=FTrigger;
  Setter:=d2GetToken(Line,';');
  StartValue:=false;
  while Setter <> '' do
  begin
    Prop:=d2GetToken(Setter,'=');
    Value:=Setter;
    if GetPropInfo(AInstance,Prop,[tkBool]) <> nil then
    begin

      StartValue:=false;
      if (CompareText(Value,'true')=0) and (GetOrdProp(AInstance,Prop) > 0) then
        StartValue:=true;
      if (CompareText(Value,'false')=0) and (GetOrdProp(AInstance,Prop)=0) then
        StartValue:=true;
    end;
    Setter:=d2GetToken(Line,';');
  end;
  Enabled:=StartValue;
end;

procedure TD2Effect.UpdateParentEffects;
var
  SaveEnabled:boolean;
begin
  if not (csLoading in ComponentState) then
    if (Parent <> nil) and (Parent.isVisual) then
    begin
      TD2VisualObject(Parent).UpdateEffects;
      TD2VisualObject(Parent).FRecalcUpdateRect:=true;
      // update if enabled=false (erase effect )
      SaveEnabled:=FEnabled;
      FEnabled:=true;
      TD2VisualObject(Parent).Repaint;
      FEnabled:=SaveEnabled;
    end;
end;

procedure TD2Effect.ProcessEffect(Canvas:TD2Canvas;
  const Visual:TD2Bitmap; const Data:single);
begin
end;

procedure TD2Effect.SetEnabled(const Value:boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled:=Value;
    if FEnabled then
    begin
      if (Parent <> nil) and (Parent.isVisual) then
        TD2VisualObject(Parent).RecalcHasEffect;
    end;
    UpdateParentEffects;
  end;
end;
    
    
procedure Blur(const Canvas:TD2Canvas; const Bitmap:TD2Bitmap; const Radius:integer; UseAlpha:boolean=true);
var
  pix:PD2ColorArray;
  w,h,wm,hm,wh,vdiv:integer;
  rsum,gsum,bsum,asum,x,y,i,yp,yi,yw:integer;
  P:cardinal;
  divsum:integer;
  stackpointer,stackstart:integer;
  sir:PD2ColorRec;
  rbs,r1,routsum,goutsum,boutsum,aoutsum,rinsum,ginsum,binsum,ainsum:integer;
  dv:PIntArray;
  vmin:PIntArray;
  r,g,b,a:PIntArray;
  stack:PD2ColorArray;
begin
  if (radius<1) then Exit;

  pix:=Bitmap.Scanline[0];

    w:=Bitmap.width;
    h:=Bitmap.height;
    wm:=w - 1;
    hm:=h - 1;
    wh:=w * h;
    vdiv:=radius + radius + 1;

    GetMem(r,wh * SizeOf(Integer));
    GetMem(g,wh * SizeOf(Integer));
    GetMem(b,wh * SizeOf(Integer));
    GetMem(a,wh * SizeOf(Integer));
    GetMem(vmin,max(w,h) * SizeOf(Integer));
    divsum:=(vdiv + 1) shr 1;
    divsum:=divsum * divsum;
    GetMem(dv,256 * divsum * SizeOf(Integer));
    for i:=0 to 256 * divsum - 1 do
      dv[i]:=(i div divsum);

    yw:=0;
    yi:=0;

    GetMem(stack,vdiv * SizeOf(TD2Color));

    r1:=radius + 1;

    for y:=0 to h - 1 do
    begin
      rinsum:=0;
      ginsum:=0;
      binsum:=0;
      ainsum:=0;
      routsum:=0;
      goutsum:=0;
      boutsum:=0;
      aoutsum:=0;
      rsum:=0;
      gsum:=0;
      bsum :=0;
      asum :=0;
      for i:=-radius to radius do
      begin
        p:=pix[yi+min(wm,max(i,0))];
        sir:=@stack[i + radius];
        sir.Color:=p;
        rbs:=r1-abs(i);
        rsum:=rsum + (sir.r*rbs);
        gsum:=gsum + (sir.g*rbs);
        bsum:=bsum + (sir.b*rbs);
        if UseAlpha then
        asum:=asum + (sir.a*rbs);
        if (i > 0) then
        begin
          rinsum:=rinsum + sir.r;
          ginsum:=ginsum + sir.g;
          binsum:=binsum + sir.b;
          if UseAlpha then
          ainsum:=ainsum + sir.a;
        end else
        begin
          routsum:=routsum + sir.r;
          goutsum:=goutsum + sir.g;
          boutsum:=boutsum + sir.b;
          if UseAlpha then
          aoutsum:=aoutsum + sir.a;
        end
      end;
      stackpointer:=radius;

      for x:=0 to w - 1 do
      begin
        r[yi]:=dv[rsum];
        g[yi]:=dv[gsum];
        b[yi]:=dv[bsum];
        if UseAlpha then
        a[yi]:=dv[asum];

        rsum:=rsum - routsum;
        gsum:=gsum - goutsum;
        bsum:=bsum - boutsum;
        if UseAlpha then
        asum:=asum - aoutsum;

        stackstart:=stackpointer-radius+vdiv;
        sir:=@stack[stackstart mod vdiv];

        routsum:=routsum - sir.r;
        goutsum:=goutsum - sir.g;
        boutsum:=boutsum - sir.b;
        if UseAlpha then
        aoutsum:=aoutsum - sir.a;

        if (y=0)then
        begin
          vmin[x]:=min(x+radius+1,wm);
        end;
        p:=pix[yw+vmin[x]];
        sir.color:=p;

        rinsum:=rinsum + sir.r;
        ginsum:=ginsum + sir.g;
        binsum:=binsum + sir.b;
        if UseAlpha then
        ainsum:=ainsum + sir.a;

        rsum:=rsum + rinsum;
        gsum:=gsum + ginsum;
        bsum:=bsum + binsum;
        if UseAlpha then
        asum:=asum + ainsum;

        stackpointer :=(stackpointer+1) mod vdiv;
        sir:=@stack[(stackpointer) mod vdiv];

        routsum:=routsum + sir.r;
        goutsum:=goutsum + sir.g;
        boutsum:=boutsum + sir.b;
        if UseAlpha then
        aoutsum:=aoutsum + sir.a;

        rinsum:=rinsum - sir.r;
        ginsum:=ginsum - sir.g;
        binsum:=binsum - sir.b;
        if UseAlpha then
        ainsum:=ainsum - sir.a;

        yi:=yi + 1;
      end;
      yw:=yw + w;
    end;

    for x:=0 to w - 1 do
    begin
      rinsum:=0;
      ginsum:=0;
      binsum:=0;
      ainsum:=0;
      routsum:=0;
      goutsum:=0;
      boutsum:=0;
      aoutsum:=0;
      rsum:=0;
      gsum:=0;
      bsum :=0;
      asum :=0;
      yp:=-radius * w;
      for i:=-radius to radius do
      begin
        yi:=max(0,yp) + x;

        sir:=@stack[i+radius];

        sir.r:=r[yi];
        sir.g:=g[yi];
        sir.b:=b[yi];
        if UseAlpha then
        sir.a:=a[yi];

        rbs:=r1 - abs(i);

        rsum:=rsum + (r[yi]*rbs);
        gsum:=gsum + (g[yi]*rbs);
        bsum:=bsum + (b[yi]*rbs);
        if UseAlpha then
        asum:=asum + (a[yi]*rbs);

        if (i > 0)then
        begin
          rinsum:=rinsum + sir.r;
          ginsum:=ginsum + sir.g;
          binsum:=binsum + sir.b;
          if UseAlpha then
          ainsum:=ainsum + sir.a;
        end else
        begin
          routsum:=routsum + sir.r;
          goutsum:=goutsum + sir.g;
          boutsum:=boutsum + sir.b;
          if UseAlpha then
          aoutsum:=aoutsum + sir.a;
        end;

        if (i < hm) then
        begin
          yp:=yp + w;
        end
      end;
      yi:=x;
      stackpointer:=radius;
      for y:=0 to h - 1 do
      begin
        pix[yi]:=(dv[asum] shl 24) or (dv[rsum] shl 16) or (dv[gsum] shl 8) or dv[bsum];

        rsum:=rsum - routsum;
        gsum:=gsum - goutsum;
        bsum:=bsum - boutsum;
        if UseAlpha then
        asum:=asum - aoutsum;

        stackstart:=stackpointer-radius+vdiv;
        sir:=@stack[stackstart mod vdiv];

        routsum:=routsum - sir.r;
        goutsum:=goutsum - sir.g;
        boutsum:=boutsum - sir.b;
        if UseAlpha then
        aoutsum:=aoutsum - sir.a;

        if (x=0) then
        begin
          vmin[y]:=min(y+r1,hm)*w;
        end;
        p:=x + vmin[y];

        sir.r:=r[p];
        sir.g:=g[p];
        sir.b:=b[p];
        if UseAlpha then
        sir.a:=a[p];

        rinsum:=rinsum + sir.r;
        ginsum:=ginsum + sir.g;
        binsum:=binsum + sir.b;
        if UseAlpha then
        ainsum:=ainsum + sir.a;

        rsum:=rsum + rinsum;
        gsum:=gsum + ginsum;
        bsum:=bsum + binsum;
        if UseAlpha then
        asum:=asum + ainsum;

        stackpointer:=(stackpointer + 1) mod vdiv;
        sir:=@stack[stackpointer];

        routsum:=routsum + sir.r;
        goutsum:=goutsum + sir.g;
        boutsum:=boutsum + sir.b;
        if UseAlpha then
        aoutsum:=aoutsum + sir.a;

        rinsum:=rinsum - sir.r;
        ginsum:=ginsum - sir.g;
        binsum:=binsum - sir.b;
        if UseAlpha then
        ainsum:=ainsum - sir.a;

        yi:=yi + w;
      end;
    end;
    FreeMem(stack,vdiv * SizeOf(TD2Color));
    FreeMem(dv,256 * divsum * SizeOf(Integer));
    FreeMem(vmin,max(w,h) * SizeOf(Integer));
    FreeMem(a,wh * SizeOf(Integer));
    FreeMem(r,wh * SizeOf(Integer));
    FreeMem(g,wh * SizeOf(Integer));
    FreeMem(b,wh * SizeOf(Integer));
end;

//=============== TD2ShadowEffect =============================================

constructor TD2ShadowEffect.Create(AOwner:TComponent);
begin
  inherited;
//  DisablePaint:=true;
  FShadowColor:=$FF000000;
  FDirection:=45;
  FOpacity:=0.6;
  FSoftness:=0.3;
  FDistance:=3;
end;

destructor TD2ShadowEffect.Destroy;
begin
  inherited;
end;

function TD2ShadowEffect.GetOffset:TD2Point;
var
  S,C:single;
begin
  d2SinCos(d2DegToRad(FDirection),S,C);
  Result:=d2Point(Trunc(FSoftness * 20),Trunc(FSoftness * 20));
end;

function TD2ShadowEffect.GetRect(const ARect:TD2Rect):TD2Rect;
var
  S,C:single;
begin
  Result:=ARect;
  d2InflateRect(Result,Trunc(FSoftness * 20),Trunc(FSoftness * 20));
  d2SinCos(d2DegToRad(FDirection),S,C);
  d2OffsetRect(Result,FDistance * C,FDistance * S);
end;

function TD2ShadowEffect.GetShadowColor:string;
begin
  Result:=d2ColorToStr(FShadowColor);
end;

procedure TD2ShadowEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
var
  Bits:PD2ColorRecArray;
  Mask:PByteArray;
  Sn,Cs:single;
  Off:TD2Point;
begin
  { create mask }
  Mask:=Visual.CreateMask;
  { fill color }
  Visual.FillColor(d2Opacity(d2CorrectColor(FShadowColor),FOpacity));
  { stack blur version }
  Blur(Canvas,Visual,Trunc(5 + FSoftness * 10));
  { apply mask }
  d2SinCos(d2DegToRad(FDirection),Sn,Cs);
  Off:=d2Point(FDistance * Cs * Data,FDistance * Sn * Data);
  Visual.ApplyMask(Mask,-round(Off.X),-round(Off.Y));
  { free mask }
  FreeMem(Mask,Visual.Width * Visual.Height);
end;

procedure TD2ShadowEffect.SetDirection(const Value:single);
begin
  if FDirection <> Value then
  begin
    FDirection:=Value;
    UpdateParentEffects;
  end;
end;

procedure TD2ShadowEffect.SetDistance(const Value:single);
begin
  if FDistance <> Value then
  begin
    FDistance:=Value;
    UpdateParentEffects;
  end;
end;

procedure TD2ShadowEffect.SetOpacity(const Value:single);
begin
  if FOpacity <> Value then
  begin
    FOpacity:=Value;
    if FOpacity < 0 then FOpacity:=0;
    if FOpacity > 1 then FOpacity:=1;
    UpdateParentEffects;
  end;
end;

procedure TD2ShadowEffect.SetShadowColor(const Value:string);
begin
  if FShadowColor <> d2StrToColor(Value) then
  begin
    FShadowColor:=d2StrToColor(Value);
    UpdateParentEffects;
  end;
end;

procedure TD2ShadowEffect.SetSoftness(const Value:single);
begin
  if FSoftness <> Value then
  begin
    FSoftness:=Value;
    if FSoftness < 0 then FSoftness:=0;
    if FSoftness > 3 then FSoftness:=3;
    UpdateParentEffects;
  end;
end;

//=============== TD2BlurEffect ===================================================================}

constructor TD2BlurEffect.Create(AOwner:TComponent);
begin
  inherited;
  DisablePaint:=true;
  FSoftness:=0.3;
end;

destructor TD2BlurEffect.Destroy;
begin
  inherited;
end;

function TD2BlurEffect.GetOffset:TD2Point;
begin
  Result:=d2Point(Trunc(FSoftness * 20),Trunc(FSoftness * 20));
end;

function TD2BlurEffect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
  d2InflateRect(Result,Trunc(FSoftness * 20),Trunc(FSoftness * 20));
end;

procedure TD2BlurEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
begin
  { stack blur version }
  if FSoftness > 0 then
    Blur(Canvas,Visual,Trunc(FSoftness * 15));
end;

procedure TD2BlurEffect.SetSoftness(const Value:single);
begin
  if FSoftness <> Value then
  begin
    FSoftness:=Value;
    if FSoftness < 0 then FSoftness:=0;
    if FSoftness > 9 then FSoftness:=9;
    UpdateParentEffects;
  end;
end;

//=============== TD2GlowEffect ===========================================================

constructor TD2GlowEffect.Create(AOwner:TComponent);
begin
  inherited;
//  DisablePaint:=true;
  FOpacity:=0.9;
  FGlowColor:=$FFFFD700;
  FSoftness:=0.4;
end;

destructor TD2GlowEffect.Destroy;
begin
  inherited;
end;

function TD2GlowEffect.GetGlowColor:string;
begin
  Result:=d2ColorToStr(FGlowColor);
end;

function TD2GlowEffect.GetOffset:TD2Point;
begin
  Result:=d2Point(4 + Trunc(FSoftness * 23),4 + Trunc(FSoftness * 23));
end;

function TD2GlowEffect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
  d2InflateRect(Result,4 + Trunc(FSoftness * 23),4 + Trunc(FSoftness * 23));
end;

procedure TD2GlowEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
var
  Bits:PD2ColorRecArray;
  Mask:PByteArray;
begin
  { create mask }
  Mask:=Visual.CreateMask;
  { fill color }
  Visual.FillColor(d2Opacity(d2CorrectColor(FGlowColor),FOpacity));
  { stack blur version }
  Blur(Canvas,Visual,Trunc(5 + FSoftness * 10));
  { apply mask }
  Visual.ApplyMask(Mask);
  { free mask }
  FreeMem(Mask,Visual.Width * Visual.Height);
end;

procedure TD2GlowEffect.SetGlowColor(const Value:string);
begin
  if FGlowColor <> d2StrToColor(Value) then
  begin
    FGlowColor:=d2StrToColor(Value);
    UpdateParentEffects;
  end;
end;

procedure TD2GlowEffect.SetOpacity(const Value:single);
begin
  if FOpacity <> Value then
  begin
    FOpacity:=Value;
    if FOpacity < 0 then FOpacity:=0;
    if FOpacity > 1 then FOpacity:=1;
    UpdateParentEffects;
  end;
end;

procedure TD2GlowEffect.SetSoftness(const Value:single);
begin
  if FSoftness <> Value then
  begin
    FSoftness:=Value;
    if FSoftness < 0 then FSoftness:=0;
    if FSoftness > 9 then FSoftness:=9;
    UpdateParentEffects;
  end;
end;

//=============== TD2InnerGlowEffect =======================================================

constructor TD2InnerGlowEffect.Create(AOwner:TComponent);
begin
  inherited;
//  DisablePaint:=true;
  AfterPaint:=true;
end;

destructor TD2InnerGlowEffect.Destroy;
begin
  inherited;
end;

function TD2InnerGlowEffect.GetOffset:TD2Point;
begin
  Result:=d2Point(Trunc(FSoftness * 20),Trunc(FSoftness * 20));
end;

function TD2InnerGlowEffect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
  d2InflateRect(Result,Trunc(FSoftness * 20),Trunc(FSoftness * 20));
end;

procedure TD2InnerGlowEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
var
  Bits:PD2ColorRecArray;
  Mask:PByteArray;
begin
  { invert }
  Visual.InvertAlpha;
  { create mask }
  Mask:=Visual.CreateMask;
  { fill color }
  Visual.FillColor(d2Opacity(d2CorrectColor(FGlowColor),FOpacity));
  { stack blur version }
  Blur(Canvas,Visual,Trunc(5 + FSoftness * 10));
  { apply mask }
  Visual.ApplyMask(Mask);
  { free mask }
  FreeMem(Mask,Visual.Width * Visual.Height);
end;

//=============== TD2BevelEffect ========================================================

constructor TD2BevelEffect.Create(AOwner:TComponent);
begin
  inherited;
  DisablePaint:=true;
  FDirection:=45;
  FSize:=10;
end;

destructor TD2BevelEffect.Destroy;
begin
  inherited;
end;

function TD2BevelEffect.GetOffset:TD2Point;
begin
  Result:=d2Point(5,5);
end;

function TD2BevelEffect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
  d2InflateRect(Result,5,5);
end;

function VectorAngleCosine(const V1,V2:TD2Vector):single;
var
  dot,len1,len2:single;
begin
  len1:=sqrt((v1.x*v1.x) + (v1.y*v1.y) + (v1.w*v1.w));
  len2:=sqrt((v2.x*v2.x) + (v2.y*v2.y) + (v2.w*v2.w));
  dot:=(V1.V[0] * V2.V[0] + V1.V[1] * V2.V[1] + V1.V[2] * V2.V[2]);
  Result:=len1 * len2;
  if Abs(Result) > 1e-40 then
    Result:=dot / Result
  else
    Result:=1;
end;

procedure TD2BevelEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
var
  color:cardinal;
  DestBits,Bits:PD2ColorRecArray;
  i,j:integer;
  a,h0,h1,h2,h3:single;
  alpha:byte;
  light,n,v,b:TD2Vector;
  LightMap:TD2Bitmap;
begin
  if FSize=0 then Exit;

  DestBits:=PD2ColorRecArray(Visual.Scanline[0]);
  { create lightmap }
  LightMap:=TD2Bitmap.Create(Visual.Width,Visual.Height);
  Bits:=PD2ColorRecArray(LightMap.Scanline[0]);
  { copy bitmap }
  System.Move(DestBits^,Bits^,Visual.Width * Visual.Height * 4);
  { blur - make HeightMap }
  Blur(Canvas,LightMap,FSize);
  { calculate lighting }
  a:=DegToRad(FDirection);
  light.x:=cos(a);
  light.y:=0;
  light.w:=sin(a);
  { make normalmap from hightmap }
  for j:=Visual.Height - 2 downto 0 do
  begin
    for i:=Visual.Width - 2 downto 0 do
    begin
      // only calc not transparent pixels
      {$ifdef FPC_BIG_ENDIAN}
      alpha:=DestBits[(i) + ((j) * Visual.Width)].Color and $FF;
      {$else}
      alpha:=DestBits[(i) + ((j) * Visual.Width)].A;
      {$endif}
      if alpha > 0 then
      begin
        {$ifdef FPC_BIG_ENDIAN}
        h0:=(Bits[i + ((j + 1) * Visual.Width)].Color and $FF) / $FF; //.height(x  ,z+1);
        h1:=(Bits[(i + 1) + ((j + 1) * Visual.Width)].Color and $FF) / $FF; //height(x+1,z+1);
        h2:=(Bits[(i + 1) + (j * Visual.Width)].Color and $FF) / $FF; //height(x+1, z);
        h3:=(Bits[(i + 1) + ((j + 1) * Visual.Width)].Color and $FF) / $FF; //height(x  , z);
        {$else}
        h0:=(Bits[i + ((j + 1) * Visual.Width)].Color and $FF000000 shr 24) / $FF; //.height(x  ,z+1);
        h1:=(Bits[(i + 1) + ((j + 1) * Visual.Width)].Color and $FF000000 shr 24) / $FF; //height(x+1,z+1);
        h2:=(Bits[(i + 1) + (j * Visual.Width)].Color and $FF000000 shr 24) / $FF; //height(x+1, z);
        h3:=(Bits[(i + 1) + ((j + 1) * Visual.Width)].Color and $FF000000 shr 24) / $FF; //height(x  , z);
        {$endif}

        v.x:=1.0;
        v.y:=h2 - h3;
        v.w:=0;

        b.x:=0;
        b.y:=h0 - h3;
        b.w:=1; // vector length

        // calc normal
        n.x:=b.y*v.w - v.y*b.w;
        n.y:=v.x*b.w - b.x*v.w;
        n.w:=b.x*v.y - v.x*b.y;

        // normalize
        n.x:=n.x / b.w;
        n.y:=n.y / b.w;
        n.w:=n.w / b.w;

        // calc light
        a:=VectorAngleCosine(light,n) * FSize;

        // set value
        {$ifdef FPC_BIG_ENDIAN}
        color:=DestBits[(i) + ((j) * Visual.Width)].Color;
        ReverseBytes(@color,4);
        DestBits[(i) + ((j) * Visual.Width)].Color:=d2PremultyAlpha(d2Opacity(d2ChangeHSL(color,0,0,a * 0.4),alpha / $FF));
        ReverseBytes(@DestBits[(i) + ((j) * Visual.Width)],4);
        {$else}
        DestBits[(i) + ((j) * Visual.Width)].Color:=d2PremultyAlpha(d2Opacity(d2ChangeHSL(DestBits[(i) + ((j) * Visual.Width)].Color,0,0,a * 0.4),alpha / $FF));
        {$endif}
      end;
    end;
  end;
  LightMap.Free;
end;

procedure TD2BevelEffect.SetDirection(const Value:single);
begin
  if FDirection <> Value then
  begin
    FDirection:=Value;
    UpdateParentEffects;
  end;
end;

procedure TD2BevelEffect.SetSize(const Value:integer);
begin
  if FSize <> Value then
  begin
    FSize:=Value;
    UpdateParentEffects;
  end;
end;

//=============== TD2ReflectionEffect ======================================

constructor TD2ReflectionEffect.Create(AOwner:TComponent);
begin
  inherited;
//  DisablePaint:=true;
  FOffset:=0;
  FLength:=0.5;
  FOpacity:=0.5;
end;

destructor TD2ReflectionEffect.Destroy;
begin
  inherited;
end;

function TD2ReflectionEffect.GetOffset:TD2Point;
begin
  Result:=d2Point(2,2);
end;

function TD2ReflectionEffect.GetRect(const ARect:TD2Rect):TD2Rect;
begin
  Result:=ARect;
  d2InflateRect(Result,2,2);
  d2OffsetRect(Result,0,d2RectHeight(ARect) + FOffset);
end;

procedure TD2ReflectionEffect.ProcessEffect(Canvas:TD2Canvas; const Visual:TD2Bitmap; const Data:single);
var
  Bits:PD2ColorRecArray;
  Line:array [0..2048] of TD2Color;
  c,a,i,j:integer;
  alpha:byte;
  color:cardinal;
begin
  Bits:=PD2ColorRecArray(Visual.Scanline[0]);
  c:=Visual.Height - 1;
  a:=round(c * FLength);
  { without Offset }
  Visual.FlipHorizontal;
  { set alpha }
  for j:=0 to c do
    if j > (c - a) then
      for i:=0 to Visual.Width - 1 do
      begin
        // only calc not transparent pixels
        {$ifdef FPC_BIG_ENDIAN}
        alpha:=Bits[i + (Visual.Height - 1 - j) * Visual.Width].Color and $FF;
        {$else}
        alpha:=Bits[i + (Visual.Height - 1 - j) * Visual.Width].A;
        {$endif}
        if alpha > 0 then
        begin
          {$ifdef FPC_BIG_ENDIAN}
          color:=Bits[i + (Visual.Height - 1 - j) * Visual.Width].Color;
          ReverseBytes(@color,4);
          TD2ColorRec(color).A:=Trunc(((j - (c - a)) / a) * FOpacity * alpha);
          Bits[i + (Visual.Height - 1 - j) * Visual.Width].Color:=d2PremultyAlpha(color);
          ReverseBytes(@Bits[i + (Visual.Height - 1 - j) * Visual.Width],4);
          {$else}
          Bits[i + (Visual.Height - 1 - j) * Visual.Width].A:=Trunc(((j - (c - a)) / a) * FOpacity * alpha);
          Bits[i + (Visual.Height - 1 - j) * Visual.Width].Color:=d2PremultyAlpha(Bits[i + (Visual.Height - 1 - j) * Visual.Width].Color);
          {$endif}
        end;
      end
    else
      d2FillLongword(@Bits[(Visual.Height - 1 - j) * Visual.Width],Visual.Width,$0);
end;

procedure TD2ReflectionEffect.SetLength(const Value:single);
begin
  if FLength <> Value then
  begin
    FLength:=Value;
    if FLength < 0.1 then
      FLength:=0.1;
    if FLength > 1 then
      FLength:=1;
    UpdateParentEffects;
  end;
end;

procedure TD2ReflectionEffect.SetOffset(const Value:integer);
begin
  if FOffset <> Value then
  begin
    FOffset:=Value;
    UpdateParentEffects;
  end;
end;

procedure TD2ReflectionEffect.SetOpacity(const Value:single);
begin
  if FOpacity <> Value then
  begin
    FOpacity:=Value;
    UpdateParentEffects;
  end;
end;

  
