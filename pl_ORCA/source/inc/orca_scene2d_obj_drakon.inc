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
begin
  if FNodeType=AValue then Exit;
  FNodeType:=AValue;
  FreeAndNil(FShape);
  case  FNodeType of
    drAction:
      FShape:= TD2DrakonAction.Create(Self); //Действие
    drAddress:   //Конец ветки-ссылка на другую ветку
      FShape:= TD2DrakonAddress.Create(Self);
    drBeginend:  //Название алгоритма
      FShape:= TD2DrakonBeginend.Create(Self);
    drBranch:    //Начало ветки
      FShape:= TD2DrakonBranch.Create(Self);
    drCase:      //Вариант для множественного выбора
      FShape:= TD2DrakonCase.Create(Self);
    drComment:   //Комментарий
      FShape:= TD2DrakonComment.Create(Self);
    drCommentL:  //Комментарий слева
      FShape:= TD2DrakonCommentL.Create(Self);
    drCommentR:  //Комментарий справка ;
      FShape:= TD2DrakonCommentR.Create(Self);
    drCtrlEnd:   //Конец контрольного срока
      FShape:= TD2DrakonCtrlEnd.Create(Self);
    drCtrlStart: //Начало контрольного срока
      FShape:= TD2DrakonCtrlStart.Create(Self);
    drDuration:  //Время, длительность
      FShape:= TD2DrakonDuration.Create(Self);
    drEnd:       //Конец алгоритма
      FShape:= TD2DrakonEnd.Create(Self);
    drInput:     //Ввод
      FShape:= TD2DrakonInput.Create(Self);
    drInsertion: //Вставка
      FShape:= TD2DrakonInsertion.Create(Self);
    drJunction:  //точка соединения линий
      FShape:= TD2DrakonJunction.Create(Self);
    drLoopBegin: //Начало цикла
      FShape:= TD2DrakonLoopBegin.Create(Self);
    drLoopEnd:   //Конец цикла
      FShape:= TD2DrakonLoopEnd.Create(Self);
    drOutput:    //Вывод
      FShape:= TD2DrakonOutput.Create(Self);
    drParams:    //Вхдные параметры
      FShape:= TD2DrakonParams.Create(Self);
    drPause:     //Пауза
      FShape:= TD2DrakonPause.Create(Self);
    drProcess:   //Парралельный процесс
      FShape:= TD2DrakonProcess.Create(Self);
    drQuestion:  //Вопрос
      FShape:= TD2DrakonQuestion.Create(Self);
    drSelect:    //Множественный выбор из нескольких значений
      FShape:= TD2DrakonSelect.Create(Self);
    drShelf:     //Полка
      FShape:= TD2DrakonShelf.Create(Self);
    drSInput:    //Простой ввод
      FShape:= TD2DrakonSInput.Create(Self);
    drSOutput:   //Простой вывод
      FShape:= TD2DrakonSOutput.Create(Self);
    drTimer:     //Таймер
      FShape:= TD2DrakonTimer.Create(Self);
  //else // drNone тип не определен
  end;

  if FShape <> nil then //вставляем текст
    begin


    end;
  if (FEditor <> nil) then
    FEditor.Realign;
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
  FNodeDown:= nil;
  FNodeUp:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;

  FreeAndNil(FShape);
  inherited;
end;


{ TD2DrakonShape }

function TD2DrakonShape.GetText: TD2Text;
begin
  Result:= FText;
end;

procedure TD2DrakonShape.SetText(AValue: TD2Text);
begin
  FText:= AValue;
end;

constructor TD2DrakonShape.Create(AParent: TD2Object);
begin
  inherited Create;
  FText:= TD2Text.Create(AParent);
  if FText = nil then exit;
  FText.Parent:=AParent;
  FText.Locked:=true;
  FText.Align:=vaClient;
  FText.Padding.SetRect(d2Rect(5,5,5,5));
end;

{ TD2DrakonAction }

function TD2DrakonAction.GetText: TD2Text;
begin
  Result:= FText.Text;
end;

//procedure TD2DrakonAction.SetText(AValue: TD2Text);
//begin
//  FText.Text:= AValue;
//end;

constructor TD2DrakonAction.Create(AOwner: TComponent);
begin
  inherited;
  FText:=TD2DrakonShape.Create(Self);
  //FText.CreateText(Self);
end;

destructor TD2DrakonAction.Destroy;
begin
  FText:=nil;
  inherited;
end;
  
