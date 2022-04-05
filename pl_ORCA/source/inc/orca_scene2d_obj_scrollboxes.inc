{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2ScrollContent ===================================

constructor TD2ScrollContent.Create(AOwner:TComponent);
begin
  inherited;
  ClipChildren:=true;
end;

function TD2ScrollContent.GetClipRect:TD2Rect;
begin
{	//Deleted by GoldenFox
  if (Parent <> nil) and (Parent is TD2ScrollBox) and (TD2ScrollBox(Parent).ContentLayout <> nil) then
  begin
    Result:=TD2ScrollBox(Parent).ContentLayout.LocalRect;
    if (TD2ScrollBox(Parent).VScrollBar <> nil) and (TD2ScrollBox(Parent).VScrollBar.Enabled) then
      d2OffsetRect(Result,0,TD2ScrollBox(Parent).VScrollBar.Value);
    if (TD2ScrollBox(Parent).HScrollBar <> nil) and (TD2ScrollBox(Parent).HScrollBar.Enabled) then
      d2OffsetRect(Result,TD2ScrollBox(Parent).HScrollBar.Value,0);
}
//----------- Block added by GoldenFox -----------
  if (Parent <> nil) and (Parent is TD2CustomScrollBox) and (TD2CustomScrollBox(Parent).ContentLayout <> nil) then
  begin
    Result:=TD2CustomScrollBox(Parent).ContentLayout.LocalRect;
    if (TD2CustomScrollBox(Parent).VScrollBar <> nil) and (TD2CustomScrollBox(Parent).VScrollBar.Enabled) then
      d2OffsetRect(Result,0,TD2CustomScrollBox(Parent).VScrollBar.Value);
    if (TD2CustomScrollBox(Parent).HScrollBar <> nil) and (TD2CustomScrollBox(Parent).HScrollBar.Enabled) then
      d2OffsetRect(Result,TD2CustomScrollBox(Parent).HScrollBar.Value,0);
//-------- End block added by GoldenFox ----------
  end
  else
    Result:=inherited GetClipRect;
end;

function TD2ScrollContent.ObjectByPoint(X,Y:single):TD2VisualObject;
var
  P:TD2Point;
begin
  Result:=inherited ObjectByPoint(X,Y);
  if Result <> nil then
  begin
    P:=AbsoluteToLocal(d2Point(X,Y));
    if not d2PtInRect(P,ClipRect) then
      Result:=nil;
  end;
end;

procedure TD2ScrollContent.Paint;
var
  State:cardinal;
begin
  inherited;
  State:=Canvas.SaveCanvas;
  Canvas.SetMatrix(AbsoluteMatrix);
  Canvas.IntersectClipRect(ClipRect);
  {	//Deleted by GoldenFox
  if (Parent <> nil) and (Parent is TD2ScrollBox) and (TD2ScrollBox(Parent).ContentLayout <> nil) then
  begin
    TD2ScrollBox(Parent).ContentPaint(Canvas,LocalRect);
  end;
  }
//----------- Block added by GoldenFox -----------
  if (Parent <> nil) and (Parent is TD2CustomScrollBox) and (TD2CustomScrollBox(Parent).ContentLayout <> nil) then
  begin
    TD2CustomScrollBox(Parent).ContentPaint(Canvas,LocalRect);
  end;
//-------- End block added by GoldenFox ----------
  Canvas.RestoreCanvas(State);
end;

procedure TD2ScrollContent.PaintChildren;
begin
  inherited ;
end;

procedure TD2ScrollContent.AddObject(AObject:TD2Object);
begin
  inherited;
  {	//Deleted by GoldenFox
    if (Parent <> nil) and (Parent is TD2ScrollBox) then
    TD2ScrollBox(Parent).ContentAddObject(AObject);
	}
//----------- Block added by GoldenFox -----------
  if (Parent <> nil) and (Parent is TD2CustomScrollBox) then
    TD2CustomScrollBox(Parent).ContentAddObject(AObject);
//-------- End block added by GoldenFox ----------
end;

procedure TD2ScrollContent.RemoveObject(AObject:TD2Object);
begin
{	//Deleted by GoldenFox
  if (Parent <> nil) and (Parent is TD2ScrollBox) then
    TD2ScrollBox(Parent).ContentBeforeRemoveObject(AObject);
  inherited;
  if (Parent <> nil) and (Parent is TD2ScrollBox) then
    TD2ScrollBox(Parent).ContentRemoveObject(AObject);
}
//----------- Block added by GoldenFox -----------
  if (Parent <> nil) and (Parent is TD2CustomScrollBox) then
    TD2CustomScrollBox(Parent).ContentBeforeRemoveObject(AObject);
  inherited;
  if (Parent <> nil) and (Parent is TD2CustomScrollBox) then
    TD2CustomScrollBox(Parent).ContentRemoveObject(AObject);
//-------- End block added by GoldenFox ----------	
end;

function TD2ScrollContent.GetUpdateRect:TD2Rect;
begin
{  Result:=inherited GetUpdateRect;
  exit;}
  if FRecalcUpdateRect then
  begin
{	//Deleted by GoldenFox
    if (Parent <> nil) and (Parent is TD2ScrollBox) then
    begin
      if (TD2ScrollBox(Parent).ContentLayout <> nil) then
        FUpdateRect:=TD2ScrollBox(Parent).ContentLayout.UpdateRect
      else
        FUpdateRect:=TD2ScrollBox(Parent).UpdateRect;
}
//----------- Block added by GoldenFox -----------  
    if (Parent <> nil) and (Parent is TD2CustomScrollBox) then
    begin
      if (TD2CustomScrollBox(Parent).ContentLayout <> nil) then
        FUpdateRect:=TD2CustomScrollBox(Parent).ContentLayout.UpdateRect
      else
        FUpdateRect:=TD2CustomScrollBox(Parent).UpdateRect;
//-------- End block added by GoldenFox ----------
    end;
  end;
  Result:=FUpdateRect;
end;

//=============== TD2CustomScrollBox ==========================================

//constructor TD2ScrollBox.Create(AOwner:TComponent);	//Deleted by GoldenFox
constructor TD2CustomScrollBox.Create(AOwner:TComponent);	//Added by GoldenFox
begin
  inherited;
  AutoCapture:=true;
  FAnimated:=true;
  FAutoHide:=true;
  FShowScrollBars:=true;
  FContent:=TD2ScrollContent.Create(Self);
  FContent.Parent:=Self;
  FContent.Stored:=false;
  FContent.Locked:=true;
  FContent.HitTest:=false;
  FScrollDuration:=0.7;
end;

//destructor TD2ScrollBox.Destroy;	//Deleted by GoldenFox
destructor TD2CustomScrollBox.Destroy;	//Added by GoldenFox
begin
  FContent:=nil;
  inherited;
end;

//procedure TD2ScrollBox.FreeStyle;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.FreeStyle;	//Added by GoldenFox
begin
  inherited;
  FContentLayout:=nil;
  FHScrollBar:=nil;
  FVScrollBar:=nil;
end;

//procedure TD2ScrollBox.ApplyStyle;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ApplyStyle;	//Added by GoldenFox
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('sizegrip');
  if (B <> nil) and (B.IsVisual) then
    B.Visual.Visible:=FShowSizeGrip;

  if FUseSmallScrollBars then
  begin
    B:=FindResource('vsmallscrollbar');
    if (B <> nil) and (B is TD2ScrollBar) then
    begin
      FVScrollBar:=TD2ScrollBar(B);
      FVScrollBar.OnChange:=VScrollChange;
      FVScrollBar.Locked:=true;
      if FVScrollBar.Tag=0 then
        FVScrollBar.Tag:=PtrInt(FVScrollBar.Align);
    end;
    B:=FindResource('hsmallscrollbar');
    if (B <> nil) and (B is TD2ScrollBar) then
    begin
      FHScrollBar:=TD2ScrollBar(B);
      FHScrollBar.OnChange:=HScrollChange;
      FHScrollBar.Locked:=true;
      if FHScrollBar.Tag=0 then
        FHScrollBar.Tag:=PtrInt(FHScrollBar.Align);
    end;
    // hide standard
    B:=FindResource('vscrollbar');
    if (B <> nil) and (B.IsVisual) then B.Visual.Visible:=false;
    B:=FindResource('hscrollbar');
    if (B <> nil) and (B.IsVisual) then B.Visual.Visible:=false;
  end;
  if not FUseSmallScrollBars or ((FVScrollBar=nil) or (FHScrollBar=nil)) then
  begin
    B:=FindResource('vscrollbar');
    if (B <> nil) and (B is TD2ScrollBar) then
    begin
      FVScrollBar:=TD2ScrollBar(B);
      FVScrollBar.OnChange:=VScrollChange;
      FVScrollBar.Locked:=true;
      if FVScrollBar.Tag=0 then
        FVScrollBar.Tag:=PtrInt(FVScrollBar.Align);
    end;
    B:=FindResource('hscrollbar');
    if (B <> nil) and (B is TD2ScrollBar) then
    begin
      FHScrollBar:=TD2ScrollBar(B);
      FHScrollBar.OnChange:=HScrollChange;
      FHScrollBar.Locked:=true;
      if FHScrollBar.Tag=0 then
        FHScrollBar.Tag:=PtrInt(FHScrollBar.Align);
    end;
    // hide standard
    B:=FindResource('vsmallscrollbar');
    if (B <> nil) and (B.IsVisual) then B.Visual.Visible:=false;
    B:=FindResource('hsmallscrollbar');
    if (B <> nil) and (B.IsVisual) then B.Visual.Visible:=false;
  end;

  B:=FindResource('content');
  if (B <> nil) and (B.IsVisual) then
    FContentLayout:=B.Visual;

  Realign;
  FVScrollAni:=nil;
  FHScrollAni:=nil;
end;

//function TD2ScrollBox.GetContentBounds:TD2Rect;	//Deleted by GoldenFox
function TD2CustomScrollBox.GetContentBounds:TD2Rect;	//Added by GoldenFox
var
  i:integer;
  R,LocalR:TD2Rect;
begin
  Result:=d2Rect(0,0,Width,Height);
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    R:=ContentLayout.LocalRect;
    for i:=0 to FContent.ChildrenCount - 1 do
      if TD2Object(FContent.Children[i]).isVisual then
        if (TD2VisualObject(FContent.Children[i]).Visible) then
        begin
          LocalR:=TD2VisualObject(FContent.Children[i]).ParentedRect;
          R:=d2UnionRect(R,LocalR);
        end;
    Result:=R;
  end;
end;

//procedure TD2ScrollBox.RealignContent(R:TD2Rect);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.RealignContent(R:TD2Rect);	//Added by GoldenFox
begin
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    FContent.SetBounds(R.Left,R.Top,d2RectWidth(R),d2RectHeight(R));
    FContent.FRecalcUpdateRect:=true; // need to recalc
  end;
end;

//procedure TD2ScrollBox.Realign;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.Realign;	//Added by GoldenFox
 procedure IntAlign;
 var
   R:TD2Rect;
 begin
    R:=GetContentBounds;                               //получить размеры области контента
    if d2RectWidth(R) * d2RectHeight(R)=0 then Exit;   //если площадь 0 то выходим
    d2OffsetRect(R,ContentLayout.Position.X,ContentLayout.Position.Y);
    if (HScrollBar <> nil) and (HScrollBar.Enabled) then
      d2OffsetRect(R,-FScrollDesign.X,0);
    if (VScrollBar <> nil) and (VScrollBar.Enabled) then
      d2OffsetRect(R,0,-FScrollDesign.Y);
    RealignContent(R);
    // realign resource
    if ContentLayout.Parent <> nil then
      ContentLayout.Parent.Visual.BeginUpdate;
    if (VScrollBar <> nil) then
    begin
      VScrollBar.Enabled:=d2RectHeight(R) > ContentLayout.Height;
      if FAutoHide then 
	    VScrollBar.Visible:=VScrollBar.Enabled;
      if not FShowScrollBars then
	    begin
		  VScrollBar.Opacity:=0;
		  VScrollBar.Align:=vaNone;
		end
      else
  	    begin		   
		  VScrollBar.Opacity:=1;
		  VScrollBar.Align:=TD2Align(VScrollBar.Tag);
  	    end;
    end;
    if (HScrollBar <> nil) then
    begin
      HScrollBar.Enabled:=d2RectWidth(R) > ContentLayout.Width;
      if FAutoHide then
  	    HScrollBar.Visible:=HScrollBar.Enabled;
      if not FShowScrollBars then 
	    begin
		  HScrollBar.Opacity:=0;
		  HScrollBar.Align:=vaNone;
	    end
      else
  	    begin		   
		  HScrollBar.Opacity:=1;
		  HScrollBar.Align:=TD2Align(HScrollBar.Tag);
		  if (VScrollBar <> nil) and (VScrollBar.Enabled) then
		    HScrollBar.Padding.right:=VScrollBar.Width;
	  end;
    end;
    if ContentLayout.Parent <> nil then
    begin
      ContentLayout.Parent.Visual.EndUpdate;
      ContentLayout.Parent.Visual.Realign;
    end;
    // align scrollbars
    if (VScrollBar <> nil) then
    begin
      VScrollBar.Enabled:=d2RectHeight(R) > ContentLayout.Height;
      if FAutoHide then 
	    VScrollBar.Visible:=VScrollBar.Enabled;
      if not FShowScrollBars then  
	    begin
		  VScrollBar.Opacity:=0;
		  VScrollBar.Align:=vaNone;
		  VScrollBar.Position.Y:=Width + 100;
	    end
      else
	    begin		   
  		  VScrollBar.Opacity:=1;
		  VScrollBar.HitTest:=true;
		  VScrollBar.Align:=TD2Align(VScrollBar.Tag);
	    end;
      VScrollBar.BringToFront;
      if VScrollBar.Visible and (ContentLayout <> nil) then  
	    begin
		  VScrollBar.Max:=d2RectHeight(R);
		  VScrollBar.ViewportSize:=ContentLayout.Height;
		  VScrollBar.SmallChange:=VScrollBar.ViewportSize / 5;
		  VScrollBar.Value:=FScrollDesign.Y;
	    end	 
      else 
	    begin
		  VScrollBar.Value:=0;
		end;
    end;
    if (HScrollBar <> nil) then
    begin
      HScrollBar.Enabled:=d2RectWidth(R) > ContentLayout.Width;
      HScrollBar.Padding.right:=0;
      if FAutoHide then 
	    HScrollBar.Visible:=HScrollBar.Enabled;
      if not FShowScrollBars then  
	    begin
		  HScrollBar.Opacity:=0;
		  HScrollBar.Align:=vaNone;
		  HScrollBar.Position.Y:=Height + 100;
		end
      else  
		begin   
 		  HScrollBar.Opacity:=1;
		  HScrollBar.Align:=TD2Align(HScrollBar.Tag);
		  if (VScrollBar <> nil) and (VScrollBar.Enabled) then 
		    HScrollBar.Padding.right:=VScrollBar.Width;
	    end;
      HScrollBar.BringToFront;
      if HScrollBar.Visible and (ContentLayout <> nil) then  
		begin
		  HScrollBar.Max:=d2RectWidth(R);
		  HScrollBar.ViewportSize:=ContentLayout.Width;
		  HScrollBar.SmallChange:=HScrollBar.ViewportSize / 5;
		  HScrollBar.Value:=ContentLayout.Position.X - FContent.Position.X;
		end
      else 
	    HScrollBar.Value:=0;
    end;
 end;

var
  R,NewR:TD2Rect;
begin
  if csDestroying in ComponentState then Exit;
  inherited;
  if csLoading in ComponentState then Exit;
  if ContentLayout=nil then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;
  FDisableAlign:=true;
  try
    R:=ContentLayout.LocalRect;
    IntAlign;
    NewR:=ContentLayout.LocalRect;
    if (d2RectWidth(NewR) <> d2RectWidth(R)) or (d2RectHeight(NewR) <> d2RectHeight(R)) then 
	  begin
	    IntAlign;
	  end;		
  finally
    FDisableAlign:=false;
  end;
end;

//function TD2ScrollBox.ContentRect:TD2Rect;	//Deleted by GoldenFox
function TD2CustomScrollBox.ContentRect:TD2Rect;	//Added by GoldenFox
begin
  if ContentLayout <> nil then
    Result:=ContentLayout.ParentedRect
  else
    Result:=LocalRect;
end;

//function TD2ScrollBox.VScrollBarValue:single;	//Deleted by GoldenFox
function TD2CustomScrollBox.VScrollBarValue:single;	//Added by GoldenFox
begin
  if (VScrollBar <> nil) and (VScrollBar.Visible) then
    Result:=VScrollBar.Value
  else
    Result:=0;
end;

//function TD2ScrollBox.HScrollBarValue:single;	//Deleted by GoldenFox
function TD2CustomScrollBox.HScrollBarValue:single;	//Added by GoldenFox
begin
  if (HScrollBar <> nil) and (HScrollBar.Visible) then
    Result:=HScrollBar.Value
  else
    Result:=0;
end;

//procedure TD2ScrollBox.HScrollChange(Sender:TObject);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.HScrollChange(Sender:TObject);	//Added by GoldenFox
var Sav: single;	//Added by GoldenFox
begin
  if ContentLayout=nil then Exit;
  Sav:=FContent.Position.X;	//Added by GoldenFox
  if HScrollBar.Visible then
    FContent.Position.X:=ContentLayout.Position.X - HScrollBar.Value
  else
    FContent.Position.X:=ContentLayout.Position.X;
  FScrollDesign.X:=HScrollBar.Value;
  DoScroll(Sav-FContent.Position.X, 0);	//Added by GoldenFox
end;

//procedure TD2ScrollBox.VScrollChange(Sender:TObject);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.VScrollChange(Sender:TObject);	//Added by GoldenFox
var Sav: single;	//Added by GoldenFox
begin
  if ContentLayout=nil then Exit;
  Sav:=FContent.Position.Y;	//Added by GoldenFox
  if VScrollBar.Visible then
    FContent.Position.Y:=ContentLayout.Position.Y - VScrollBar.Value
  else
    FContent.Position.Y:=ContentLayout.Position.Y;
  FScrollDesign.Y:=VScrollBar.Value;
  DoScroll(0, Sav-FContent.Position.Y);	//Added by GoldenFox
end;

//procedure TD2ScrollBox.CreateHScrollAni;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.CreateHScrollAni;	//Added by GoldenFox
begin
  if FHScrollAni=nil then
  begin
    FHScrollAni:=TD2FloatAnimation.Create(Self);
    FHScrollAni.Parent:=HScrollBar;
    FHScrollAni.AnimationType:=d2AnimationOut;
    FHScrollAni.Interpolation:=d2InterpolationQuadratic;
    //FHScrollAni.Duration:=0.7;	//Deleted by GoldenFox
	FHScrollAni.Duration:=FScrollDuration;	//Added by GoldenFox
    FHScrollAni.PropertyName:='Value';
    FHScrollAni.StartFromCurrent:=true;
  end;
end;

//procedure TD2ScrollBox.CreateVScrollAni;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.CreateVScrollAni;	//Added by GoldenFox
begin
  if FVScrollAni=nil then
  begin
    FVScrollAni:=TD2FloatAnimation.Create(Self);
    FVScrollAni.Parent:=VScrollBar;
    FVScrollAni.AnimationType:=d2AnimationOut;
    FVScrollAni.Interpolation:=d2InterpolationQuadratic;
    //FVScrollAni.Duration:=0.7;	//Deleted by GoldenFox
	FVScrollAni.Duration:=FScrollDuration;	//Added by GoldenFox
    FVScrollAni.PropertyName:='Value';
    FVScrollAni.StartFromCurrent:=true;
  end;
end;

//procedure TD2ScrollBox.MouseDown(Button:TMouseButton; Shift:TShiftState;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.MouseDown(Button:TMouseButton; Shift:TShiftState;	//Added by GoldenFox
  X,Y:single);
begin
  inherited;
  if (Button=mbLeft) and FMouseTracking then
  begin
    FLastDelta:=d2Point(0,0);
    FDownPos:=d2Point(X,Y);
    FCurrentPos:=d2Point(X,Y);
    FDown:=true;
    if (FVScrollAni <> nil) and FVScrollAni.Running then
      FVScrollAni.StopAtCurrent;
    if (FHScrollAni <> nil) and FHScrollAni.Running then
      FHScrollAni.StopAtCurrent;
  end;
end;

//procedure TD2ScrollBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);	//Added by GoldenFox
begin
  inherited;
  if FDown and FMouseTracking then
  begin
    if (VScrollBar <> nil) and (VScrollBar.Visible) then
    begin
      VScrollBar.Value:=VScrollBar.Value - (Y - FCurrentPos.Y);
      FLastDelta.Y:=(Y - FCurrentPos.Y);
    end;
    if (HScrollBar <> nil) and (HScrollBar.Visible) then
    begin
      HScrollBar.Value:=HScrollBar.Value - (X - FCurrentPos.X);
      FLastDelta.X:=(X - FCurrentPos.X);
    end;
    FCurrentPos:=d2Point(X,Y);
  end;
end;

//procedure TD2ScrollBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,	//Deleted by GoldenFox
procedure TD2CustomScrollBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,	//Added by GoldenFox
  Y:single);
begin
  inherited;
  if FDown and FMouseTracking then
  begin
    FDown:=false;
    // animation
    if FAnimated and ((FLastDelta.X <> 0) or (FLastDelta.Y <> 0)) then
    begin
      if (VScrollBar <> nil) and (VScrollBar.Visible) then
      begin
        CreateVScrollAni;
        if FVScrollAni.Running then
          FVScrollAni.StopAtCurrent;
        FVScrollAni.StopValue:=VScrollBar.Value - (FLastDelta.Y * 7);
        FVScrollAni.Start;
      end;
      if (HScrollBar <> nil) and (HScrollBar.Visible) then
      begin
        CreateHScrollAni;
        if FHScrollAni.Running then
          FHScrollAni.StopAtCurrent;
        FHScrollAni.StopValue:=HScrollBar.Value - (FLastDelta.X * 7);
        FHScrollAni.Start;
      end;
    end;
  end;
end;

//procedure TD2ScrollBox.MouseWheel(Shift:TShiftState; WheelDelta:integer;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.MouseWheel(Shift:TShiftState; WheelDelta:integer;	//Added by GoldenFox
  var Handled:boolean);
begin
  inherited;
  if not Handled and not (FDisableMouseWheel) and (VScrollBar <> nil) and (VScrollBar.Visible) then
  begin
    if FAnimated then
    begin
      CreateVScrollAni;
      if FVScrollAni.Running then
        FVScrollAni.StopAtCurrent;
      FVScrollAni.StopValue:=VScrollBar.Value - (VScrollBar.SmallChange * 3 * (WheelDelta / 120));
      FVScrollAni.Start;
    end
    else
      VScrollBar.Value:=VScrollBar.Value - (VScrollBar.SmallChange * 3 * (WheelDelta / 120));
    Handled:=true;
  end;
  if not Handled and not (FDisableMouseWheel) and (HScrollBar <> nil) and (HScrollBar.Visible) then
  begin
    if FAnimated then
    begin
      CreateHScrollAni;
      if FHScrollAni.Running then
        FHScrollAni.StopAtCurrent;
      FHScrollAni.StopValue:=HScrollBar.Value - (HScrollBar.SmallChange * 3 * (WheelDelta / 120));
      FHScrollAni.Start;
    end
    else
      HScrollBar.Value:=HScrollBar.Value - (HScrollBar.SmallChange * 3 * (WheelDelta / 120));
    Handled:=true;
  end;
end;

//procedure TD2ScrollBox.AddObject(AObject:TD2Object);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.AddObject(AObject:TD2Object);	//Added by GoldenFox
begin
  if (FContent <> nil) and (AObject.TagString <> 'self') and (AObject <> FContent) and (AObject <> FResourceLink) and not (AObject is TD2Effect) and not (AObject is TD2Animation) then
	begin
	  FContent.AddObject(AObject);
	end
  else 
    inherited;
end;

//procedure TD2ScrollBox.Loaded;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.Loaded;	//Added by GoldenFox
begin
  inherited;
  // ScrollTo(-FScrollDesign.X,-FScrollDesign.Y);
end;

//procedure TD2ScrollBox.Centre;	//Deleted by GoldenFox
procedure TD2CustomScrollBox.Centre;	//Added by GoldenFox
begin
  if (VScrollBar <> nil) and (VScrollBar.Visible) then
  begin
    VScrollBar.Value:=(VScrollBar.Max - VScrollBar.ViewportSize) / 2;
  end;
  if (HScrollBar <> nil) and (HScrollBar.Visible) then
  begin
    HScrollBar.Value:=(HScrollBar.Max - HScrollBar.ViewportSize) / 2;
  end;
end;

//procedure TD2ScrollBox.ScrollTo(const Dx,Dy:single);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ScrollTo(const Dx,Dy:single);	//Added by GoldenFox
begin
  if (VScrollBar <> nil) and (VScrollBar.Visible) then
    VScrollBar.Value:=VScrollBar.Value - Dy;
  if (HScrollBar <> nil) and (HScrollBar.Visible) then
    HScrollBar.Value:=HScrollBar.Value - Dx;
end;

//procedure TD2CustomScrollBox.InViewRect(const Rect:TD2Rect);	//Deleted by GoldenFox
//begin	//Deleted by GoldenFox
//	//Deleted by GoldenFox
//end;	//Deleted by GoldenFox

//procedure TD2ScrollBox.SetShowScrollBars(const Value:boolean);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.SetShowScrollBars(const Value:boolean);	//Added by GoldenFox
begin
  if FShowScrollBars <> Value then
  begin
    FShowScrollBars:=Value;
    Realign;
  end;
end;

//procedure TD2ScrollBox.SetShowSizeGrip(const Value:boolean);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.SetShowSizeGrip(const Value:boolean);	//Added by GoldenFox
begin
  if FShowSizeGrip <> Value then
  begin
    FShowSizeGrip:=Value;
    ApplyStyle;
  end;
end;

//procedure TD2ScrollBox.DefineProperties(Filer:TFiler);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.DefineProperties(Filer:TFiler);	//Added by GoldenFox
begin
  inherited;
  // Filer.DefineProperty('ScrollDesign',ReadScrollDesign,WriteScrollDesign,(FScrollDesign.X <> 0) and (FScrollDesign.Y <> 0));
end;

//----------- Block added by GoldenFox -----------
procedure TD2CustomScrollBox.DoScroll(DeltaX, DeltaY: single);
begin
  if Assigned(FOnScroll) then
    FOnScroll(Self, DeltaX, DeltaY);
end;
//-------- End block added by GoldenFox ----------

//procedure TD2ScrollBox.ReadScrollDesign(Reader:TReader);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ReadScrollDesign(Reader:TReader);	//Added by GoldenFox
begin
  FScrollDesign:=d2StringToPoint(Reader.ReadString);;
end;

//procedure TD2ScrollBox.WriteScrollDesign(Writer:TWriter);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.WriteScrollDesign(Writer:TWriter);	//Added by GoldenFox
begin
  Writer.WriteString(d2PointToString(FScrollDesign));
end;

//procedure TD2ScrollBox.ContentPaint(const Canvas:TD2Canvas; const ARect:TD2Rect);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ContentPaint(const Canvas:TD2Canvas; const ARect:TD2Rect);	//Added by GoldenFox
begin

end;

//procedure TD2ScrollBox.SetUseSmallScrollBars(const Value:boolean);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.SetUseSmallScrollBars(const Value:boolean);	//Added by GoldenFox
begin
  if FUseSmallScrollBars <> Value then
  begin
    FUseSmallScrollBars:=Value;
    ApplyStyle;
  end;
end;

//function TD2ScrollBox.ClientHeight:single;	//Deleted by GoldenFox
function TD2CustomScrollBox.ClientHeight:single;	//Added by GoldenFox
begin
  if ContentLayout <> nil then
    Result:=ContentLayout.Height
  else
    Result:=Height;
end;

//function TD2ScrollBox.ClientWidth:single;	//Deleted by GoldenFox
function TD2CustomScrollBox.ClientWidth:single;	//Added by GoldenFox
begin
  if ContentLayout <> nil then
    Result:=ContentLayout.Width
  else
    Result:=Width;
end;

//procedure TD2ScrollBox.ContentAddObject(AObject:TD2Object);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ContentAddObject(AObject:TD2Object);	//Added by GoldenFox
begin
end;

//procedure TD2ScrollBox.ContentRemoveObject(AObject:TD2Object);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ContentRemoveObject(AObject:TD2Object);	//Added by GoldenFox
begin
end;

//procedure TD2ScrollBox.ContentBeforeRemoveObject(AObject:TD2Object);	//Deleted by GoldenFox
procedure TD2CustomScrollBox.ContentBeforeRemoveObject(AObject:TD2Object);	//Added by GoldenFox
begin
end;

//=============== TD2VertScrollBox ============================================

constructor TD2VertScrollBox.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='scrollboxstyle';
end;

function TD2VertScrollBox.GetContentBounds:TD2Rect;
//var	//Deleted by GoldenFox
  //i:integer;	//Deleted by GoldenFox
begin
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    FContent.Width:=ContentLayout.Width;
  end;
  Result:=inherited GetContentBounds;
end;

//=============== TD2FramedVertScrollBox ========================================

constructor TD2FramedVertScrollBox.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='framedscrollboxstyle';
end;               
         
         

            
  
