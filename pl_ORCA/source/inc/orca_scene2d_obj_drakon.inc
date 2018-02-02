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
  //if FText<>nil
  //  then begin
  //         t:=FText.Text;
  //         fi:=TD2Brush.Create(d2BrushNone,$FF000000);
  //         fi.Assign(FText.Fill);
  //         fo:=TD2Font.Create;
  //         fo.Assign(FText.Font);
  //       end
  //  else begin
  //         t:='';
  //         fi:=nil;
  //         fo:=nil;
  //       end;
  //if FText2<>nil
  //  then begin
  //         t2:=FText2.Text;
  //         fi2:=TD2Brush.Create(d2BrushNone,$FF000000);
  //         fi2.Assign(FText2.Fill);
  //         fo2:=TD2Font.Create;
  //         fo2.Assign(FText2.Font);
  //       end
  //  else begin
  //         t2:='';
  //         fi2:=nil;
  //         fo2:=nil;
  //       end;
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
  //if FText<>nil then
  //  begin
  //    FText.Text:=t;
  //    FText.Fill.Assign(fi);
  //    FText.Font.Assign(fo);
  //  end;
  //if FText2<>nil then
  //  begin
  //    FText2.Text:=t2;
  //    FText2.Fill.Assign(fi2);
  //    FText2.Font.Assign(fo2);
  //  end;
  //FreeAndNil(fi);
  //FreeAndNil(fo);
  //FreeAndNil(fi2);
  //FreeAndNil(fo2);
end;

procedure TD2DrakonNode.ApplyStyle;
var S:TD2Object;
begin
  inherited;
  S:=FindResource('shape');
  if (S<>nil) and (S is TD2Shape)
    then FShape:=TD2Shape(S)
    else
      if (S=nil) and (FResourceLink is TD2Shape)
        then FShape:=TD2Shape(FResourceLink);
  S:=FindResource('shape2');
  if (S<>nil) and (S is TD2Shape)
    then FShape2:=TD2Shape(S);
  S:=FindResource('text');
  if (S<>nil) and (S is TD2Text)
    then FText:=TD2Text(S);
  S:=FindResource('text2');
  if (S<>nil) and (S is TD2Text)
    then FText2:=TD2Text(S);
end;

constructor TD2DrakonNode.Create(AOwner: TComponent);
begin
  FNodeIndex:=-1;
  FNodeType:=drNone;
  FShape:= nil;
  FText:= nil;
  FNodeDown:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  FNodeUp:= nil;
  inherited;
end;

destructor TD2DrakonNode.Destroy;
begin
  FNodeDown:= nil;
  FNodeUp:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  inherited;
end;

  
