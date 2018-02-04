{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//==================== TD2DrakonNode =======================================

//Задать связанный узел ниже
procedure TD2DrakonNode.SetNodeDown(AValue: TD2DrakonNode);
begin
  if FNodeDown=AValue then Exit;
  FNodeDown:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

procedure TD2DrakonNode.SetFill(AValue: TD2Brush);
begin
  FFill.Assign(AValue);
  if FShape<>nil
    then FShape.FFill.Assign(AValue);
  if FShape2<>nil
    then FShape2.FFill.Assign(AValue);
  Repaint;
end;

//Задать связанный узел слева
procedure TD2DrakonNode.SetNodeLeft(AValue: TD2DrakonNode);
begin
  if FNodeLeft=AValue then Exit;
  FNodeLeft:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать связанный узел справа
procedure TD2DrakonNode.SetNodeRight(AValue: TD2DrakonNode);
begin
  if FNodeRight=AValue then Exit;
  FNodeRight:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать связанный узел выше
procedure TD2DrakonNode.SetNodeUp(AValue: TD2DrakonNode);
begin
  if FNodeUp=AValue then Exit;
  FNodeUp:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать тип узела
procedure TD2DrakonNode.SetNodeType(AValue: TD2DrakonNodeType);
var
  t,t2: String;
  fi,fi2: TD2Brush;
  fo,fo2: TD2Font;

begin
  if FNodeType=AValue then Exit;
  FNodeType:=AValue;
  FTextShape:=nil;
  FTextShape2:=nil;
  FShape:=nil;
  FShape2:=nil;
  FNum:=nil;
  case  AValue of     //создаем новый контур
    drAction:    //+Действие
      Resource:='DrakonActionStyle';
    drAddress:   //+Конец ветки-ссылка на другую ветку
      Resource:='DrakonAddressStyle';
    drBeginend:  //+Название алгоритма
      Resource:='DrakonBeginendStyle';
    drBranch:    //+Начало ветки
      Resource:='DrakonBranchStyle';
    drCase:      //+Вариант для множественного выбора
      Resource:='DrakonCaseStyle';
    drComment:   //+Комментарий
      Resource:='DrakonCommentStyle';
    drCommentL:  //+Комментарий слева
      Resource:='DrakonCommentLStyle';
    drCommentR:  //+Комментарий справка ;
      Resource:='DrakonCommentRStyle';
    drCtrlEnd:   //+Конец контрольного срока
      Resource:='DrakonCtrlEndStyle';
    drCtrlStart: //+Начало контрольного срока
      Resource:='DrakonCtrlStartStyle';
    drDuration:  //+Время, длительность
      Resource:='DrakonDurationStyle';
    drEnd:       //+Конец алгоритма
      Resource:='DrakonEndStyle';
    drInput:     //+Ввод
      Resource:='DrakonInputStyle';
    drInsertion: //+Вставка
      Resource:='DrakonInsertionStyle';
    drLoopBegin: //+Начало цикла
      Resource:='DrakonLoopBeginStyle';
    drLoopEnd:   //+Конец цикла
      Resource:='DrakonLoopEndStyle';
    drOutput:    //+Вывод
      Resource:='DrakonOutputStyle';
    drParams:    //+Вхдные параметры
      Resource:='DrakonParamsStyle';
    drPause:     //+Пауза
      Resource:='DrakonPauseStyle';
    drProcess:   //+Парралельный процесс
      Resource:='DrakonProcessStyle';
    drQuestion:  //+Вопрос
      Resource:='DrakonQuestionStyle';
    drSelect:    //+Множественный выбор из нескольких значений
      Resource:='DrakonSelectStyle';
    drShelf:     //+Полка
      Resource:='DrakonShelfStyle';
    drSInput:    //+Простой ввод
      Resource:='DrakonSInputStyle';
    drSOutput:   //+Простой вывод
      Resource:='DrakonSOutputStyle';
    drTimer:     //+Таймер
      Resource:='DrakonTimerStyle';
    drJunction: //точка соединения линий
      Resource:='DrakonJunctionStyle';
    else
         //drNone тип не определен
      Resource:='DrakonNodeStyle';
  end;
end;

procedure TD2DrakonNode.SetStroke(AValue: TD2Brush);
begin
  FStroke.Assign(AValue);
  if FShape<>nil
    then FShape.FStroke.Assign(AValue);
  if FShape2<>nil
    then FShape2.FStroke.Assign(AValue);
  Repaint;
end;

procedure TD2DrakonNode.SetStrokeThickness(AValue: single);
begin
  if FStrokeThickness=AValue then Exit;
  FStrokeThickness:=AValue;
  if FShape<>nil
    then FShape.FStrokeThickness:=AValue;
  if FShape2<>nil
    then FShape2.FStrokeThickness:=AValue;
  Repaint;
end;

procedure TD2DrakonNode.SetText(AValue: String);
begin
  if FText=AValue then Exit;
  FText:=AValue;
  if FTextShape=nil then exit;
  FTextShape.Text:=AValue;
  Realign;
  Repaint;
end;

procedure TD2DrakonNode.SetText2(AValue: String);
begin
  if FText2=AValue then Exit;
  FText2:=AValue;
  if FTextShape2=nil then exit;
  FTextShape2.Text:=AValue;
  Realign;
  Repaint;
end;

procedure TD2DrakonNode.SetTextFill(AValue: TD2Brush);
begin
  FTextFill.Assign(AValue);
  if FTextShape<>nil
    then FTextShape.FFill.Assign(AValue);
  if FTextShape2<>nil
    then FTextShape2.FFill.Assign(AValue);
  Repaint;
end;

procedure TD2DrakonNode.SetTextFont(AValue: TD2Font);
begin
  FTextFont.Assign(AValue);
  if FTextShape<>nil
    then FTextShape.FFont.Assign(AValue);
  if FTextShape2<>nil
    then FTextShape2.FFont.Assign(AValue);
  Realign;
end;

procedure TD2DrakonNode.ApplyStyle;
var S:TD2Object;
begin
  inherited;
  S:=FindResource('shape');
  if (S<>nil) and (S is TD2Shape)
    then FShape:=TD2Shape(S)
    else if (S=nil) and (FResourceLink is TD2Shape)
           then FShape:=TD2Shape(FResourceLink);
  S:=FindResource('shape2');
  if (S<>nil) and (S is TD2Shape)
    then FShape2:=TD2Shape(S);
  S:=FindResource('text');
  if (S<>nil) and (S is TD2Text)
    then FTextShape:=TD2Text(S);
  S:=FindResource('text2');
  if (S<>nil) and (S is TD2Text)
    then FTextShape2:=TD2Text(S);
  S:=FindResource('number');
  if (S<>nil) and (S is TD2Text)
    then FNum:=TD2Text(S);
  Realign;
end;

constructor TD2DrakonNode.Create(AOwner: TComponent);
begin
  inherited;
  FNodeIndex:=-1;
  FNodeType:=drNone;
  FShape:= nil;
  FShape2:= nil;
  FTextShape:= nil;
  FTextShape2:= nil;
  FNodeDown:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  FNodeUp:= nil;
  FText:='';
  FText2:='';
  FTextFont:=TD2Font.Create;
  FTextFill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FTextFill.SolidColor:=$FF000000;
  FFill:=TD2Brush.Create(d2BrushSolid,$FFE0E0E0);
  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FStroke.SolidColor:=$FF000000;
  FStrokeThickness:=1;
end;

destructor TD2DrakonNode.Destroy;
begin
  FNodeDown:= nil;
  FNodeUp:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  inherited;
end;

procedure TD2DrakonNode.Realign;
var
  h,h1: single;

  function AdjustHeight(AText: TD2Text): single;
  var
    R: TD2Rect;
  begin
    if (AText <> nil) and (AText.Canvas <> nil) and (AText.FText <> '') then
      with AText do
      begin
        Canvas.Font.Assign(FFont);
        R:=d2Rect(0,0,Width,1000);
        Canvas.MeasureText(R,R,FText,true,d2TextAlignNear,d2TextAlignNear);
        Result:=R.Bottom;
      end
    else Result:=0;
  end;

begin
  if csDestroying in ComponentState then Exit;
  if FDisableDefaultAlign then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;

  if Width < 40
    then Width:= 40 ;
  if FTextShape2 <> nil
    then begin
           FTextShape2.Text:=FText2;
           FTextShape2.FFont.Assign(FTextFont);
           FTextShape2.FFill.Assign(FTextFill);
           h1:=AdjustHeight(FTextShape2) + FTextShape2.FPadding.FTop + FTextShape2.FPadding.FBottom;
           if h1<15 then h1:=15;
         end
    else h1:=0;
  if FShape2 <> nil
    then begin
           h1:= h1 + FShape2.FMargins.FTop + FShape2.FMargins.FBottom;
           h:= h1 + FShape2.FPadding.FTop + FShape2.FPadding.FBottom;
           FShape2.FFill.Assign(FFill);
           FShape2.FStroke.Assign(FStroke);
           FShape2.FStrokeThickness:=FStrokeThickness;
           FShape2.FHeight:=h1;
         end
    else h:=0;
  if FTextShape <> nil
    then begin
           FTextShape.Text:=FText;
           FTextShape.FFont.Assign(FTextFont);
           FTextShape.FFill.Assign(FTextFill);
           h1:=AdjustHeight(FTextShape) + FTextShape.FPadding.FTop + FTextShape.FPadding.FBottom;
           if h1<15 then h1:=15;
         end
    else h1:=0;
  if FShape <> nil then
  begin
    FShape.FFill.Assign(FFill);
    FShape.FStroke.Assign(FStroke);
    FShape.FStrokeThickness:=FStrokeThickness;
    if FNodeType=drComment
      then with FShape do
             h:= h + h1 + FMargins.FTop + FMargins.FBottom
      else with FShape do
           begin
             h1:= h1 + FMargins.FTop + FMargins.FBottom;
             h:= h + h1 + FPadding.FTop + FPadding.FBottom;
             FHeight:=h1;
           end;
  end;
  if h<15 then h:=15;
  if h>1000 then h:=1000;
  FHeight:=h;
  inherited;
end;






















  
