{%mainunit ../orca_scene2d.pas}

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

{ TD2TreeCell }

procedure TD2TreeCell.Realign;
var
  SelectLevel: Integer;        // > 0, если выбран текущий узел или ребенок / внук и т. Д. Выбранного узла. > 0 if current node is selected or child/grandchild etc. of a selected node
begin
  if csDestroying in ComponentState then Exit;
  if FDisableDefaultAlign then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;
  if csLoading in ComponentState then Exit;
  inherited;
  if FGrid = nil then exit;
  if FNode = nil then exit;
  with TD2TreeGrid(FGrid) do
  begin
    if FColumnIndex=FMainColumn  // Некоторые части отображаются только для основного столбца. Some parts are only drawn for the main column.
      then  begin
              //отступ
              if FIndent <> nil then
              begin
                FIndent.Visible := true; //включаем отсуп
                SelectLevel := DetermineLineArrayAndSelectLevel(FNode, FLineArray);  //определяем уровень вложенности и создаем массив линий дерева

                //экспандер
                if FExpander <> nil then
                begin
                  if (toShowButtons in FOptions.FPaintOptions) and
                     (vsHasChildren in FNode.States) and
                      not ((vsAllChildrenHidden in FNode.States) and
                           (toAutoHideButtons in FOptions.FAutoOptions)) and
                     (((FNode.Parent = FRoot) and
                       (toShowRoot in FOptions.FPaintOptions)) or
                      (FNode.Parent <> FRoot))
                    then  begin
                            FExpander.Visible:= true;
                            if vsExpanded in FNode.States //если узел раскрыт
                              then IsExpanded:=true         //установим флаг раскрытия
                              else IsExpanded:=false;       //иначе снимем флаг раскрытия
                          end
                    else FExpander.Visible:= false;
                end;
                CalculateIndent;         //рассчитаем отступ, нарисуем линии дерева
              end;

              //Показывать кнопку узла, если это разрешено, если есть дочерние узлы и хотя бы один
              //из дочерних узлов виден или автоматическое скрытие кнопки отключено.
              // Show node button if allowed, if there child nodes and at least one of the child
              // nodes is visible or auto button hiding is disabled.
              if (toCheckSupport in FOptions.FMiscOptions)
                then  begin
                        case FNode.CheckType of
                          ctNone,             // Узел без отметки.
                          ctButton:           // Узел с кнопкой слева от надписи.
                            begin
                              if FCheck <> nil
                                 then FCheck.Visible := false;
                              if FRadio <> nil
                                 then FRadio.Visible := false;
                            end;
                          ctTriStateCheckBox: // Узел с отметкой типа TCheckBox, которая может иметь 3 состояния (отмечена, grayed, не отмечена).
                            begin
                              if FCheck <> nil
                                 then begin
                                        AllowGrayed:=true;
                                        FCheck.Visible := true;
                                      end;
                              if FRadio <> nil
                                 then FRadio.Visible := false;
                            end;
                          ctCheckBox:         // Узел с отметкой типа TCheckBox, которая может иметь 2 состояния.
                            begin
                              if FCheck <> nil
                                 then  begin
                                         AllowGrayed:=false;
                                         FCheck.Visible := true;
                                       end;
                              if FRadio <> nil
                                 then FRadio.Visible := false;
                            end;
                          ctRadioButton:      // Узел с отметкой типа TRadioButton.
                            begin
                              if FCheck <> nil
                                then  begin
                                        AllowGrayed:=false;
                                        FCheck.Visible := false;
                                      end;
                              if FRadio <> nil
                                then FRadio.Visible := true;
                            end;
                        end;
                        //определяем состояние отметки
                        Case FNode.CheckState of
                          csUncheckedNormal,  // Узел не отмечен и не зажат. unchecked and not pressed
                          csUncheckedPressed: // Узел не отмечен и пользователь зажал кнопку мыши над отметкой. unchecked and pressed
                            begin
                              IsChecked := false;
                              IsGrayed := false;
                            end;

                          csCheckedNormal,    // Узел отмечен и не зажат. checked and not pressed
                          csCheckedPressed:   // Узел отмечен и пользователь зажал кнопку мыши над отметкой. checked and pressed
                              IsChecked := true;

                          csMixedNormal,      // Отметка имеет промежуточное состояние. 3-state check box and not pressed
                          csMixedPressed:      // Отметка имеет промежуточное состояние и пользователь зажал кнопку мыши над отметкой. 3-state check box and pressed
                              IsGrayed := true;
                        end;
                      end
                else  begin
                        if FCheck <> nil
                           then FCheck.Visible := false;
                        if FRadio <> nil
                           then FRadio.Visible := false;
                      end;
            end
      else  begin  //это не главная колонка
              if FIndent <> nil                    //уберем отступ если есть
                then FIndent.Visible := false;
              if FExpander <> nil                  //скроем эеспандер если есть
                then FExpander.Visible := false;
              if FCheck <> nil                     //скроем чекбокс если есть
                then FCheck.Visible := false;
              if FRadio <> nil                     //скроем радиокнопку если есть
                then FRadio.Visible := false;
            end;

    //включить сплиттер если разрешено изменение размеров
    if FSplitter <> nil then
    begin
      if (toNodeHeightResize in FOptions.MiscOptions)
        then FSplitter.Visible:= true
        else FSplitter.Visible:= false;
    end;

  end;
end;

procedure TD2TreeCell.CalculateVerticalAlignments(ShowImages, ShowStateImages: Boolean; out VButtonAlign: Single);
//Вычисляет вертикальное выравнивание узла Node и связанной с ним кнопки развернуть/свернуть
//во время цикла рисования узла в зависимости от стиля выравнивания узлов.
// Calculates the vertical alignment of the given node and its associated expand/collapse button during
// a node paint cycle depending on the required node alignment style.
begin
  //Для абсолютного выравнивания вычисление тривиально. For absolute alignment the calculation is trivial.
  case FGrid.FNodeAlignment of
    naFromTop:
      FVAlign := FNode.Align;
    naFromBottom:
      FVAlign := FNode.NodeHeight - FNode.Align;
    else // naProportional - пропорциональное выравнивание

    //Рассмотрим настройку кнопок и линий, но убедимся, что ни изображение, ни кнопка
    //(в зависимости от того, что выше) не выходят за пределы всей высоты узла (100%
    //означает выравнивание нижнего уровня к границам узла).
    // Consider button and line alignment, but make sure neither the image nor the button (whichever is taller)
    // go out of the entire node height (100% means bottom alignment to the node's bounds).

    //if ShowImages or ShowStateImages
    //  then  begin
    //          if ShowImages
    //            then VAlign := GetNodeImageSize(Node).cy
    //            else VAlign := FStateImages.Height;
    //          VAlign := d2MulDiv((NodeHeight[FNode] - VAlign), FNode.Align, 100) + VAlign / 2;
    //        end
    //  else

      if FExpander.Visible and (FPlus <> nil)
        then FVAlign := d2MulDiv(FNode.NodeHeight - FPlus.Height, FNode.Align, 100) + FPlus.Height / 2
        else FVAlign := d2MulDiv(FNode.NodeHeight, FNode.Align, 100);
  end;

  if FPlus <> nil
    then VButtonAlign := FVAlign - FPlus.Height / 2 {- (FPlus.Height and 1)}
    else VButtonAlign := FVAlign;
end;

procedure TD2TreeCell.CalculateIndent;
//рассчитать отступ експандера и/или отметки узла и нарисовать линии дерева
var
  ButtonY: single;
  IndentSize: integer;
begin
  if FGrid = nil then exit;
  if FIndent = nil then exit;
  IndentSize := Length(FLineArray);                   //кол-во отступов
  if (toFixedIndent in FGrid.FOptions.FPaintOptions)  //если установлен фиксированный отступ,
    then FIndent.Width := FGrid.FIndentWidth                  //то сдвиг только под экспандер
    else FIndent.Width := IndentSize * FGrid.FIndentWidth;    //иначе  делаем полный сдвиг
  //сдвиг экспандера
  CalculateVerticalAlignments({ShowImages, ShowStateImages,} false, false, ButtonY);
  //if (FExpander.Visible) then
  //begin
    //if (FPlus <> nil)
    //  then FPlus.Position.Y:=ButtonY;
    //if (FMinus <> nil)
    //  then FMinus.Position.Y:=ButtonY;
  //end;
  //линии дерева
  if (toShowTreeLines in FGrid.FOptions.FPaintOptions)
    then PaintTreeLines(IfThen(toFixedIndent in FGrid.FOptions.FPaintOptions, 1, IndentSize), FLineArray)
    else FIndent.Data.Data:='';

end;

procedure TD2TreeCell.PaintTreeLines(IndentSize: Integer; LineArray: TD2TreeLineArray);
//нарисовать линии дерева в ячейке Cell в соответствии с массивом линий LineArray
var
  I: Integer;
  XPos,
  Offset: single;
  NewStyles: TD2TreeLineArray;

begin
  if FIndent=nil then exit;
  if not FIndent.Visible then exit;

  FIndent.Data.Data:='';

  {if BidiMode = bdLeftToRight then
  begin
    XPos := CellRect.Left; }

    XPos := 0;
    Offset := FGrid.FIndentWidth;

 { end
  else
  begin
    Offset := -Integer(FIndent);
    XPos := CellRect.Right + Offset;
  end;}

    case FGrid.FLineMode of
      lmBands:
        begin
          NewStyles:= GetBandsLineArray(IndentSize, LineArray);
          FIndent.Stroke.SolidColor:=FGrid.FLineFill.SolidColor;
          FIndent.StrokeDash:=d2DashSolid;

          for i := 0 to IndentSize - 1 do
          begin
            FGrid.DoBeforeDrawTreeLine(FNode, i + Ord(not (toShowRoot in FGrid.TreeOptions.PaintOptions)), XPos);
            DrawTreeLine(XPos, NewStyles[i], false, false {BidiMode <> bdLeftToRight});
            d2IncFloat(XPos, Offset);
          end;
        end;
      else // lmNormal
        FIndent.Stroke.SolidColor:=FTreeLineStrokeColor;
        FIndent.StrokeDash:=FTreeLineStrokeDash;
        for i := 0 to IndentSize - 1 do
        begin
          FGrid.DoBeforeDrawTreeLine(FNode, i + Ord(not (toShowRoot in FGrid.TreeOptions.PaintOptions)), XPos);
          DrawTreeLine(XPos, LineArray[i], ((FExpander.Visible) and (i=IndentSize - 1)), false {BidiMode <> bdLeftToRight});
          d2IncFloat(XPos, Offset);
        end;
     end;
  {$ifdef DEBUG_TreeGrid}
     DebugLn('Y: ' + d2FloatToStr(Position.Y) + ' Column: ' + inttostr(FColumnIndex) + ' Path: ' + FIndent.Data.Data);
  {$endif}
end;

procedure TD2TreeCell.DrawTreeLine(X: Single; Style: TD2TreeLineType; ClipExpander: boolean; Reverse: Boolean);
// Рисует (в зависимости от стиля) один из 5 типов линий дерева.
// Если Reverse = True, то столбец рисуется справа налево, и горизонтальные линии должны быть зеркалированы.
// X и Y описывают левый верхний угол прямоугольника изображения линии, а H обозначает его высоту (и ширину).
// Draws (depending on Style) one of the 5 line types of the tree.
// If Reverse is True then a right-to-left column is being drawn, hence horizontal lines must be mirrored.
// X and Y describe the left upper corner of the line image rectangle, while H denotes its height (and width).

procedure DrawVLine(Top, Bottom, Left: single);
// Рисует вертикальную линию
begin
  FIndent.Data.Data:= FIndent.Data.Data +
                      ' M ' + d2FloatToStr(Left) + ',' + d2FloatToStr(Top) +
                      ' L ' + d2FloatToStr(Left) + ',' + d2FloatToStr(Bottom) + ' Z';
end;

procedure DrawHLine(Left, Right, Top: single);
// Рисует горизонтальную линию.
begin
  FIndent.Data.Data:= FIndent.Data.Data +
                      ' M ' + d2FloatToStr(Left)  + ',' + d2FloatToStr(Top-0.5) +
                      ' L ' + d2FloatToStr(Right) + ',' + d2FloatToStr(Top-0.5) + ' Z';
end;

var
  HalfWidth,
  TargetX: single;

begin
  HalfWidth := FGrid.FIndentWidth / 2;
  if Reverse then
    TargetX := 0
  else
    TargetX := FGrid.FIndentWidth;

  case Style of
    ltBottomRight:
          if ClipExpander and (FPlus<> nil)
            then begin
                   DrawVLine(FVAlign + FPlus.Height /2, FNode.NodeHeight, X + HalfWidth);
                   DrawHLine(X + HalfWidth + FPlus.Width / 2, X + TargetX, FVAlign);
                 end
            else begin
                   DrawVLine(FVAlign, FNode.NodeHeight, X + HalfWidth);
                   DrawHLine(X + HalfWidth, X + TargetX, FVAlign);
                 end;

    ltTopDown:
          if ClipExpander and (FPlus<> nil)
            then begin
                   DrawVLine(0, FVAlign - FPlus.Height /2, X + HalfWidth);
                   DrawVLine(FVAlign + FPlus.Height /2, FNode.NodeHeight, X + HalfWidth);
                 end
            else DrawVLine(0, FNode.NodeHeight, X + HalfWidth);

    ltTopDownRight:
          begin
            if ClipExpander and (FPlus<> nil)
              then begin
                     DrawVLine(0, FVAlign - FPlus.Height /2, X + HalfWidth);
                     DrawVLine(FVAlign + FPlus.Height /2, FNode.NodeHeight, X + HalfWidth);
                     DrawHLine(X + HalfWidth + FPlus.Width / 2, X + TargetX, FVAlign);
                   end
              else begin
                     DrawVLine(0, FNode.NodeHeight, X + HalfWidth);
                     DrawHLine(X + HalfWidth, X + TargetX, FVAlign);
                   end;
          end;

    ltRight:
          if ClipExpander and (FPlus<> nil)
            then DrawHLine(X + HalfWidth + FPlus.Width / 2, X + TargetX, 0 + FVAlign)
            else DrawHLine(X + HalfWidth, X + TargetX, 0 + FVAlign);

    ltTopRight:
          if ClipExpander and (FPlus<> nil)
            then begin
                   DrawVLine(0, FVAlign - FPlus.Height / 2, X + HalfWidth);
                   DrawHLine(X + HalfWidth + FPlus.Width / 2, X + TargetX, FVAlign);
                 end
            else begin
                   DrawVLine(0, FVAlign, X + HalfWidth);
                   DrawHLine(X + HalfWidth, X + TargetX, FVAlign);
                 end;

    ltLeft: // left can also mean right for RTL context
          if Reverse
            then DrawVLine(-1, FNode.NodeHeight, X + FGrid.FIndentWidth)
            else DrawVLine(-1, FNode.NodeHeight, X);

    ltLeftBottom:
          if Reverse
            then DrawVLine(-1, FNode.NodeHeight - 0.5, X + FGrid.FIndentWidth)
            else DrawVLine(-1, FNode.NodeHeight - 0.5, X);
  end;
end;


function TD2TreeCell.GetBandsLineArray(IndentSize: Integer; LineArray: TD2TreeLineArray): TD2TreeLineArray;
//получить альтернативный массив линий дерева для режима LineMode=lmBands
var
  i: Integer;
  NewStyles: TD2TreeLineArray;
begin
  // // Преобразование линейных изображений в правильные полосы. Convert the line images in correct bands.
  NewStyles := nil;
  SetLength(NewStyles, Length(LineArray));
  for i := IndentSize - 1 downto 0 do
  begin
    if (vsExpanded in FNode.States) and not (vsAllChildrenHidden in FNode.States) then
      NewStyles[i] := ltLeft
    else
      case LineArray[i] of
        ltRight,
        ltBottomRight,
        ltTopDownRight,
        ltTopRight:
          NewStyles[i] := ltLeftBottom;
        ltNone:
          // Должно взять изображение справа от него. Ни одна строка не может появляться как
          // последняя запись, поэтому мне не нужна проверка конца здесь.
          // Have to take over the image to the right of this one. A no line entry can never appear as
          // last entry so i don't need an end check here.
          if LineArray[i + 1] in [ltNone, ltTopRight]
            then NewStyles[i] := NewStyles[i + 1]
            else NewStyles[i] := ltLeft;
        ltTopDown:
          // Необходимо проверить изображение справа от него. Верхняя нижняя строка никогда не может появляться
          // последняя запись, поэтому мне не нужна проверка конца здесь.
          // Have to check the image to the right of this one. A top down line can never appear as
          // last entry so i don't need an end check here.
          if LineArray[i + 1] in [ltNone, ltTopRight]
            then NewStyles[i] := NewStyles[i + 1]
            else NewStyles[i] := ltLeft;
      end;
  end;
  Result:= NewStyles;
end;

procedure TD2TreeCell.DoExpanderClick(Sender: TObject);
//обработчик нажатия на экспандер
begin
  if (FNode <> nil) and (FGrid <> nil)
    then FGrid.ToggleNode(FNode);
end;

procedure TD2TreeCell.SetIsExpanded(const Value: boolean);
//установить флаг IsExpanded (узел развернут/свернут) в состояние Value
begin
  if FIsExpanded <> Value then
  begin
    FIsExpanded:=Value;
    //обновим экспандер
    if FExpander <> nil
      then FExpander.StartTriggerAnimation(Self,'IsExpanded');
  end;
end;

procedure TD2TreeCell.SetIsChecked(const Value: boolean);
//установить флаг IsChecked (отметка узла) в состояние Value
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    if Value then IsGrayed:= false;
  //обновим отметку
    if FCheck <> nil
      then  begin
              FCheck.StartTriggerAnimation(Self,'IsChecked');
              FCheck.StartTriggerAnimation(Self,'IsPressed');
            end;
    if FRadio <> nil
      then  begin
              FRadio.StartTriggerAnimation(Self,'IsChecked');
              FRadio.StartTriggerAnimation(Self,'IsPressed');
            end;
  end;
end;

procedure TD2TreeCell.SetIsGrayed(Value: boolean);
//установить флаг IsGrayed (серая (промежуточная) отметка узла) для 3-х позиционного чек-бокса в состояние Value
begin
  if FIsGrayed <> Value then
  begin
    if Value
      then  begin
              IsChecked:= false;
              if FAllowGrayed
                then FIsGrayed:= true
                else FIsGrayed:= false;
            end
      else FIsGrayed:= false;
    //обновим отметку
    if (FCheck <> nil)
      then begin
              FCheck.StartTriggerAnimation(Self,'IsGrayed');
              FCheck.StartTriggerAnimation(Self,'IsPressed');
            end;
  end;
end;

procedure TD2TreeCell.SetAllowGrayed(Value: boolean);
//установить разрешение серого (промежуточного) состояния для 3-х позиционного чек-бокса
begin
  if FAllowGrayed <> Value then
    begin
      FAllowGrayed:=Value;
      if not Value
        then IsGrayed:=false;
    end;
end;

function TD2TreeCell.GetData: Variant;
//получить данные из ячейки
begin
  if FControl<>nil
    then Result:=FControl.GetData
    else Result:=Null;
end;

procedure TD2TreeCell.SetData(const Value: Variant);
//установить данные ячейки в Value
begin
  if FControl<>nil
    then FControl.SetData(Value);
end;

procedure TD2TreeCell.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('indent');
  if (B <> nil) and (B is TD2Path) then
  begin
    FIndent:= TD2Path(B);
    FTreeLineStrokeColor:=FIndent.Stroke.SolidColor;
    FTreeLineStrokeDash:=FIndent.StrokeDash;
  end;

  B:=FindResource('expander');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FExpander:=TD2CustomButton(B);
    FExpander.ApplyResource;
    FExpander.OnClick:=DoExpanderClick;
    FExpander.AutoCapture:= false; //отключить захват нажатия мыши при выходе курсора за пределы элемента
    FExpander.StartTriggerAnimation(Self,'IsExpanded');
    B:=FExpander.FindResource('plus');
    if (B <> nil) and (B is TD2Path)
      then FPlus:= TD2Path(B);
    B:=FExpander.FindResource('minus');
    if (B <> nil) and (B is TD2Path)
      then FMinus:= TD2Path(B);
  end;
  B:=FindResource('check');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FCheck:=TD2CustomButton(B);
    FCheck.ApplyResource;
    FCheck.OnMouseDown:=DoCheckMouseDown;
    FCheck.OnMouseUp:=DoCheckMouseUp;
    FCheck.AutoCapture:= false; //отключить захват нажатия мыши при выходе курсора за пределы элемента
    FCheck.StartTriggerAnimation(Self,'IsChecked');
    FCheck.StartTriggerAnimation(Self,'IsGrayed');
    FCheck.StartTriggerAnimation(Self,'IsPressed');
  end;
  B:=FindResource('radio');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FRadio:=TD2CustomButton(B);
    FRadio.ApplyResource;
    FRadio.OnMouseDown:=DoCheckMouseDown;
    FRadio.OnMouseUp:=DoCheckMouseUp;
    FRadio.AutoCapture:= false;  //отключить захват нажатия мыши при выходе курсора за пределы элемента
    FRadio.StartTriggerAnimation(Self,'IsChecked');
    FCheck.StartTriggerAnimation(Self,'IsPressed');
  end;
  B:=FindResource('content');
  if (B <> nil) and (B is TD2Layout) then
  begin
    FContent:=TD2Layout(B);
    if (FControl <> nil) and (FControl.Parent<>FContent) then
      begin
        FControl.Parent:=FContent;
        FControl.Align:=vaClient;
        FControl.Visible:=true;
      end;
  end;
  if FControl<>nil then
  begin
    B:=FControl.FindResource('splitter');
    if (B <> nil) and (B is TD2SplitLayout) then
    begin
      FSplitter:=TD2SplitLayout(B);
      FSplitter.AutoCapture:= true;  //продолжать изменение размера при выходе курсора за пределы элемента пока нажата кнопка мыши
      FSplitter.OnMouseMove := DoSplitterMouseMove; //обработчик прерывания изменения размера
    end;
  end;
  StartTriggerAnimation(Self,'IsSelected');
end;

procedure TD2TreeCell.FreeStyle;
begin
  inherited;
  if FControl<>nil
    then FControl.Parent:=Self;
  FIndent:=nil;
  FExpander:=nil;
  FPlus:=nil;
  FMinus:=nil;
  FCheck:=nil;
  FRadio:=nil;
  FContent:=nil;
  FSplitter:=nil;
end;

procedure TD2TreeCell.KeyDown(var Key: Word; var KeyChar: System.WideChar;
  Shift: TShiftState);
var NewCheckState: TD2CheckState;
begin

  case Key of
    VK_SPACE:
      if (Shift = []) then  //Нажат только пробел
        begin
          if (FGrid<>nil) and (toCheckSupport in FGrid.FOptions.FMiscOptions) and
             (FNode<>nil) and (FNode.CheckType <> ctNone) then
            begin
              if (FGrid.FStates * [tsKeyCheckPending, tsMouseCheckPending] = []) and
                not (vsDisabled in FNode.States) then
              begin
                NewCheckState := DetermineNextCheckState(FNode.CheckType, FNode.CheckState);
                if FGrid.DoChecking(FNode, NewCheckState) then    //если узел может изменить состояние (определяется прложением)
                begin
                  FGrid.DoStateChange([tsKeyCheckPending]);
                  FGrid.FCheckNode := FNode;
                  FGrid.FPendingCheckState := NewCheckState;
                  FNode.CheckState := PressedState[FNode.CheckState];
                end;
              end;
            end;
        end;
    else  begin
            inherited;
            if FControl <> nil then
              TD2HackVisualObject(FControl).KeyDown(Key, KeyChar, Shift);
          end;
  end;
end;

procedure TD2TreeCell.DoCheckMouseDown(Sender: TObject; Button: TMouseButton;
                                       Shift: TShiftState; X, Y:single);
//обработчик нажатия кнопок мыши над отметкой узла
var NewCheckState: TD2CheckState;
begin
  if {(Button = mbLeft) and }(Shift = [ssLeft]) and
     (FGrid<>nil) and (toCheckSupport in FGrid.FOptions.FMiscOptions) and
     (FNode<>nil) and not (vsDisabled in FNode.States) and (FNode.CheckType<>ctNone) and
     (FGrid.FStates * [tsMouseCheckPending, tsKeyCheckPending] = [])
    then  begin
            NewCheckState := DetermineNextCheckState(FNode.CheckType, FNode.CheckState);
            if FGrid.DoChecking(FNode, NewCheckState) then    //если узел может изменить состояние (определяется прложением)
              begin
                FGrid.DoStateChange([tsMouseCheckPending]);
                FGrid.FCheckNode := FNode;
                FGrid.FPendingCheckState := NewCheckState;
                FNode.CheckState := PressedState[FNode.CheckState];
              end;
          end;
end;

procedure TD2TreeCell.DoCheckMouseUp(Sender: TObject; Button: TMouseButton;
                                     Shift: TShiftState; X, Y: single);
//обработчик отпускания кнопок мыши над отметкой узла
begin

  if (FGrid<>nil) and (tsMouseCheckPending in FGrid.FStates) then
    begin
      FGrid.DoStateChange([], [tsMouseCheckPending]);
      //Нужна проверка на nil, номер # 285, т.к. когда кнопка мыши нажата на флажке,
      //но еще не отпущена, и в этот момент грид начинает перестраиваться по таймеру????,
      //то после этого, когда кнопка мыши отпускается FCheckNode уже равен nil
      //  Need check for nil, issue #285
      //  because when mouse down on checkbox but not yet released
      //  and in this time list starts to rebuild by timer
      //  after this when mouse release  FCheckNode equal nil
      if (FGrid.FCheckNode<>nil) and (FNode<>nil) then
      begin
        if (FGrid.FCheckNode=FNode) and (FCheck.MouseInObject or FRadio.MouseInObject) //мышь все еще над тем же узлом?  Is the mouse still over the same node?
          then FGrid.DoCheckClick(FNode, FGrid.FPendingCheckState)
          else FGrid.FCheckNode.CheckState := UnpressedState[FGrid.FCheckNode.CheckState];
      end;
      FGrid.FCheckNode := nil;
    end;
end;

function TD2TreeCell.DetermineNextCheckState(CheckType: TD2CheckType;
  CheckState: TD2CheckState): TD2CheckState;
//Определяет следующее состояние отметки если пользователь щелкнет на значек отметки или нажмет клавишу пробел.
// Determines the next check state in case the user click the check image or pressed the space key.
begin
  case CheckType of
    ctTriStateCheckBox:
      if CheckState = csUncheckedNormal
        then Result:= csMixedNormal
        else if CheckState = csCheckedNormal
               then Result:= csUncheckedNormal
               else Result:= csCheckedNormal;
    ctCheckBox:
      if CheckState = csCheckedNormal
        then Result := csUncheckedNormal
        else Result := csCheckedNormal;
    ctRadioButton:
      Result := csCheckedNormal;
  else
    Result := csUncheckedNormal;
  end;
end;

constructor TD2TreeCell.Create(AOwner: TComponent);
begin
  inherited;

  FCheck:=nil;             //указатель на чекбокс
  FColumnIndex:= -1;       //индекс связанной колонки
  FContent:=nil;           //указатель на клиентскую область
  FControl:=nil;           //указатель на контрл, отображающий и редактирующий данные
  FExpander:=nil;          //указатель на кнопку разворачивания узла
  FGrid:=nil;              //указатель на грид
  FIndent:=nil;            //указатель на отступ
  FIsChecked:=false;       //true - чекбокс отмечен
  FIsExpanded:=false;      //true - узел развернут
  SetLength(FLineArray,0); //массив линий дерева
  FMinus:=nil;             //указатель на кнопку "-" экспандера
  FNode:=nil;              //указатель на связанный узел дерева
  FPlus:=nil;              //указатель на кнопку "+" экспандера
  FRadio:=nil;             //указатель на радиокнопку
  FSplitter:=nil;          //указатель на сплиттер
  FVAlign:= 0;             //сдвиг горизонтальной линии дерева
end;

destructor TD2TreeCell.Destroy;
begin
  FreeAndNil(FControl);
  inherited Destroy;
end;

procedure TD2TreeCell.DoSplitterMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y, Dx, Dy: single);
var
  NewSize:single;
begin
  if TD2HackVisualObject(FSplitter).FPressed then
  begin
    if (FNode <> nil) and (FGrid <> nil) then
    begin
      NewSize:=AbsoluteToLocal(FSplitter.LocalToAbsolute(d2Point(X,Y))).Y;
      if (NewSize < FGrid.FRowHeight) then NewSize:=FGrid.FRowHeight;
      if (NewSize <> FNode.NodeHeight) then
      begin
        //Height := NewSize;
        FGrid.SetNodeHeight(FNode,NewSize); //установить новый размер узла
        FGrid.Realign;                      //перестроить грид
      end;
    end;
  end;
end;

{ TD2TreeColumn }

procedure TD2TreeColumn.DoChangeExpander(Sender: TObject);
begin
  if (FGrid <> nil) then
    with TD2TreeCell(Sender) do
      if (FNode <> nil) and
         (IsExpanded <> (vsExpanded in FNode.States))
        then TD2TreeGrid(Self.FGrid).ToggleNode(FNode);
end;

function TD2TreeColumn.GetIsMainColumn: boolean;
begin
  if (FGrid <> nil) and (Index = TD2TreeGrid(FGrid).FMainColumn)
    then  Result:= true
    else  Result:= false;
end;

function TD2TreeColumn.CreateCellControl: TD2Control;
begin
  Result:=TD2TreeCell.Create(Self);
end;

procedure TD2TreeColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  if (ACellControl <> nil) and
     (ACellControl is TD2TreeCell) and
     (TD2TreeCell(ACellControl).FControl <> nil) and
     (TD2TreeCell(ACellControl).FControl is TD2Control) then
    begin
      ACellControl.OnEnterFocus:=nil;
      ACellControl.OnKillFocus:=nil;
      ACellControl.OnKeyDown:=nil;
      ACellControl.OnCanFocused:=nil;
      with TD2TreeCell(ACellControl) do
      begin
        FControl.ApplyResource;
        FControl.OnCanFocused:=DoCanFocused;
        FControl.OnEnterFocus:=DoEnterFocus;
        FControl.OnKillFocus:=DoKillFocus;
        FControl.OnKeyDown:=DoKeyDown;
        //OnChangeCheck:=DoChangeCheck;
        //OnChangeExpander:=DoChangeExpander;
        ApplyResource;
      end;
    end;
end;

procedure TD2TreeColumn.UpdateColumn;
//Обновление колонки
var
  Node: PD2TreeNode;
  BaseOffset: Single;          // Верхнее положение верхнего узла для рисования, заданное в абсолютных координатах дерева. top position of the top node to draw given in absolute tree coordinates
  Cell: TD2TreeCell;           //текущий контрол ячейки
  i: integer;

  //UseBackground,
  //ShowImages,
  //ShowStateImages,
  //ShowCheckImages: Boolean;

begin
  if FGrid = nil then exit; //нет грида - выходим
  if FGrid.ContentLayout=nil then exit; //нет окна для вывода колонок - выходим
  FUpdateColumn:=true;      //начать обновление колонки (для совместимости с CustomGrid)

  with TD2TreeGrid(FGrid) do
  begin
  //  if not (tsPainting in FStates) then
  //  begin
  //    DoStateChange([tsPainting]);
      try
        ////Для быстрой проверки используются промежуточные переменные. For quick checks some intermediate variables are used.
        //UseBackground := (toShowBackground in FOptions.FPaintOptions) and (FBackground.Graphic is TBitmap) ;
        //ShowImages := Assigned(FImages);
        //ShowStateImages := Assigned(FStateImages);
        //ShowCheckImages := Assigned(FCheckImages) and} (toCheckSupport in FOptions.FMiscOptions);

        BaseOffset:= 0;
        i:= 0;  //индекс текущей видимой ячейки

        for Node in FVisibleRowsList do // ----------- основной цикл. Пока есть узел main node paint loop
        begin
          if i >= FCellControls.Count            //если текущий индекс больше чем кол-во созданных ячеек
             then begin                          //то
                    Cell:=TD2TreeCell(CreateCellControl); //создадим ячеку
                    Cell.SendToBack;                    //переместим ячейку выше всех
                    InitCellControl(Cell);         //настроим ячеку
                    FCellControls.Add(Cell);       //добавим ячейку к списку ячеек
                  end
             else Cell:= FCellControls.Items[i]; //иначе возьмем текущую ячейку
          Cell.Visible:=true;
          Cell.FGrid:=TD2CustomTreeGrid(FGrid);
          Cell.FColumnIndex:= Self.Index;
          Cell.FNode:=Node;
          Cell.Data:= GetValue(Node, Self.Index);
          Cell.SetBounds(0, BaseOffset, Self.Width, Node.NodeHeight); //задаем границы ячейки
          Cell.Realign;
          // Заключительный штрих для этого узла: отметьте его, если он является текущим целевым узлом перетаскивания. Final touch for this node: mark it if it is the current drop target node.
          if (Node = FDropTargetNode) and (toShowDropmark in FOptions.FPaintOptions)
             then PaintDropMark(Cell);

          inc(i);
          d2IncFloat(BaseOffset, Node.NodeHeight);
        end;    //-------------- конец основного цикла

        for i:=i to FCellControls.Count - 1 do  //Cкрыть остальные ячейки, если есть,
          TD2Control(FCellControls.Items[i]).Visible:=false;
        UpdateSelected;
      finally
        //DoStateChange([], [tsPainting]);
        FUpdateColumn:=false;  //обновление колонки закончено (для совместимости с CustomGrid)
      end;
    //end;
  end;
end;

procedure TD2TreeColumn.PaintDropMark(const Cell: TD2TreeCell);
//Нарисовать прямоугольник целевого узла перетаскивания
begin

end;

procedure TD2TreeColumn.NodeSetFocus(Node: PD2TreeNode);
begin
  // Установить фокус в видимую ячейку соответствующую узлу Node. Виртуальный метод: определяется в потомках
end;

function TD2TreeColumn.CellControlByNode(Node: PD2TreeNode): TD2Control;
//Получить видимую ячейку дерева, соответствующую узлу Node
var
  i:integer;
begin
  Result:=nil;
  if Grid=nil then Exit;
  for i:=0 to FCellControls.Count - 1 do
    with TD2TreeCell(FCellControls.Items[i]) do
      if Visible and (FNode=Node) then
      begin
        Result:=TD2Control(FCellControls.Items[i]);
        Exit;
      end;
end;

{ TD2TreeTextColumn }

function TD2TreeTextColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  if Result <> nil then
    begin
      TD2TreeCell(Result).FControl:=TD2TextCell.Create(Result);
      if TD2TreeCell(Result).FControl <> nil then
        with TD2TreeCell(Result).FControl do
        begin
          HitTest:=false;
          CanFocused:=false;
          Locked:=true;
        end;
    end;
end;

procedure TD2TreeTextColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  if TD2TreeCell(ACellControl).FControl <> nil then
    TD2TextCell(TD2TreeCell(ACellControl).FControl).OnChange:=DoTextChanged;
end;

procedure TD2TreeTextColumn.DoTextChanged(Sender: TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  with TD2TreeCell(TD2TextCell(Sender).Owner) do
  begin
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,Data);
    if Assigned(TD2TreeGrid(Grid).FOnEdititingDone) then
      TD2TreeGrid(Grid).FOnEdititingDone(Grid,FNode,FColumnIndex);
  end;
end;

procedure TD2TreeTextColumn.NodeSetFocus(Node: PD2TreeNode);
var C: TD2Control;
begin
  inherited;
  C:=CellControlByNode(Node);
  if (C <> nil) and (C is TD2TreeCell) and (TD2TreeCell(C).FControl <> nil) then
  begin
    TD2TreeCell(C).FControl.HitTest:=true;
    TD2TreeCell(C).FControl.CanFocused:=true;
    TD2TreeCell(C).FControl.SetFocus;
  end;
end;

{ TD2CustomTreeGrid }

constructor TD2CustomTreeGrid.Create(AOwner: TComponent);
begin
  //if not Initialized then
  //  InitializeGlobalStructures;

  inherited;
  FResource:='gridstyle';
  RowCount:=0;
  //ControlStyle := ControlStyle - [csSetCaption] + [csCaptureMouse, csOpaque, csReplicatable, csDisplayDragImage, csReflector];
  FTotalInternalDataSize := 0;
  FNodeDataSize := -1;
  Width := 200;
  Height := 100;
  FDefaultNodeHeight := 21;
  FFocusedColumn := NoColumn;
  //FDragImageKind := diComplete;
  FLastSelectionLevel := -1;
  //FSelectionBlendFactor := 128;
  FIndentWidth := 20;
  FVisibleRowsList:= TList.Create;


  //BorderStyle := bsSingle;
  //FButtonStyle := bsRectangle;
  //FButtonFillMode := fmTreeColor;

  //FCheckImageKind := ckSystemDefault;
  //FCheckImages := SystemCheckImages;
  //
  //FImageChangeLink := TChangeLink.Create;
  //FImageChangeLink.OnChange := ImageListChange;
  //FStateChangeLink := TChangeLink.Create;
  //FStateChangeLink.OnChange := ImageListChange;
  //FCustomCheckChangeLink := TChangeLink.Create;
  //FCustomCheckChangeLink.OnChange := ImageListChange;

  //FAutoExpandDelay := 1000;
  //FAutoScrollDelay := 1000;
  //FAutoScrollInterval := 1;

  //FBackground := TPicture.Create;

  FDefaultPasteMode := amAddChildLast;
  //FMargin := 4;
  //FTextMargin := 4;
  //FLastDragEffect := DROPEFFECT_NONE;
  //FDragType := dtOLE;
  //FDragHeight := 350;
  //FDragWidth := 200;

  //FColors := TVTColors.Create(Self);
  //FEditDelay := 1000;
  //
  //FDragImage := TVTDragImage.Create(Self);
  //with FDragImage do
  //begin
  //  Fade := True;
  //  PostBlendBias := 0;
  //  PreBlendBias := 0;
  //  Transparency := 200;
  //end;

  SetLength(FSingletonNodeArray, 1);
  //FAnimationDuration := 200;
  //FSearchTimeout := 1000;
  //FSearchStart := ssFocusedNode;
  FNodeAlignment := naProportional;
  //FLineStyle := lsDotted;
  //FIncrementalSearch := isNone;
  //FClipboardFormats := TClipboardFormats.Create(Self);
  FOptions := GetOptionsClass.Create(Self);

  {$ifdef EnableThreadSupport}
  AddThreadReference;
  {$endif}

  //FVclStyleEnabled := False;
  // XE2+ VCL Style
  {$ifdef VCLStyleSupport}
  FSetOrRestoreBevelKindAndBevelWidth := False;
  FSavedBevelKind := bkNone;
  FSavedBorderWidth := 0;
  {$ifend}
end;

destructor TD2CustomTreeGrid.Destroy;
begin
  InterruptValidation();
  Exclude(FOptions.FMiscOptions, toReadOnly);
  {$ifdef EnableThreadSupport}
  ReleaseThreadReference(Self);
  {$endif}
  //StopWheelPanning;
  //lcl
  //FPanningWindow.Free;

  // Just in case it didn't happen already release the edit link.
  //FEditLink := nil;
  //FClipboardFormats.Free;
  // Clear will also free the drag manager if it is still alive.
  Clear;
  FVisibleRowsList.Free;

  //FDragImage.Free;
  //FColors.Free;
  //FBackground.Free;
  //FImageChangeLink.Free;
  //FStateChangeLink.Free;
  //FCustomCheckChangeLink.Free;
  //FScrollBarOptions.Free;

  // The window handle must be destroyed before the header is freed because it is needed in WM_NCDESTROY.
  //todo_lcl_check
  {
  if HandleAllocated then
    DestroyWindowHandle;
  }

  // Release FDottedBrush in case WM_NCDESTROY hasn't been triggered.
  //if FDottedBrush <> 0 then
  //  DeleteObject(FDottedBrush);
  //FDottedBrush := 0;

  FOptions.Free;
  //FreeAndNil(FHeader);

  FreeMem(FRoot);

  //FPlusBM.Free;
  //FHotPlusBM.Free;
  //FMinusBM.Free;
  //FHotMinusBM.Free;

  inherited;
end;

function TD2CustomTreeGrid.ItemClass: string;
begin
  Result:='TD2TreeTextColumn;TD2TreeCheckColumn;TD2TreePopupColumn;TD2TreeImageColumn;TD2TreeProgressColumn';
end;


{ -------------------- TD2CustomVirtualTreeOptions -------------------- }

constructor TD2CustomTreeOptions.Create(AOwner: TD2CustomTreeGrid);

begin
  FOwner := AOwner;
  FPaintOptions := DefaultTreePaintOptions;
  FAnimationOptions := DefaultTreeAnimationOptions;
  FAutoOptions := DefaultTreeAutoOptions;
  FSelectionOptions := DefaultTreeSelectionOptions;
  FMiscOptions := DefaultTreeMiscOptions;
end;

procedure TD2CustomTreeOptions.SetAnimationOptions(const Value: TD2TreeAnimationOptions);
begin
  FAnimationOptions := Value;
end;

procedure TD2CustomTreeOptions.SetAutoOptions(const Value: TD2TreeAutoOptions);
//Установить опиции автоматической обработки определенных ситуаций.

var ChangedOptions: TD2TreeAutoOptions;
begin
  if FAutoOptions <> Value then
  begin
    // Exclusive ORing to get all entries wich are in either set but not in both.
    ChangedOptions := FAutoOptions + Value - (FAutoOptions * Value);
    FAutoOptions := Value;
    if Assigned(FOwner) then
      with FOwner do
        if (toAutoSpanColumns in ChangedOptions) and not (csLoading in ComponentState) {and HandleAllocated then}
          then Realign;     //    Invalidate;
  end;
end;

procedure TD2CustomTreeOptions.SetMiscOptions(const Value: TD2TreeMiscOptions);  //установить прочие опции
var
  ToBeSet,
  ToBeCleared: TD2TreeMiscOptions;
begin
  if FMiscOptions <> Value then
  begin
    ToBeSet := Value - FMiscOptions;
    ToBeCleared := FMiscOptions - Value;
    FMiscOptions := Value;
    {$ifndef Windows}
    Exclude(FMiscOptions,toAcceptOLEDrop);
    Exclude(ToBeCleared,toAcceptOLEDrop);
    Exclude(ToBeSet,toAcceptOLEDrop);
    {$endif}
    if Assigned(FOwner) then
      with FOwner do
        if not (csLoading in ComponentState) {and HandleAllocated} then
        begin
          if toCheckSupport in ToBeSet + ToBeCleared
            then Realign; //Invalidate;


          //if not (csDesigning in ComponentState) then
          //begin
          //  {!!! проверить необходимость данной опции. Вероятно не нужна
          //  if toFullRepaintOnResize in (ToBeSet + ToBeCleared) then
          //    //todo_lcl_check
          //    RecreateWnd(FOwner);
          //    }
          //  {!!! не понятно как получить Handle дерева. Есть только Handle сцены
          //  if toAcceptOLEDrop in ToBeSet then
          //    RegisterDragDrop(Handle, DragManager as IDropTarget);
          //  if toAcceptOLEDrop in ToBeCleared then
          //    RevokeDragDrop(Handle);
          //  }
          //end;
        end;
  end;
end;

procedure TD2CustomTreeOptions.SetPaintOptions(const Value: TD2TreePaintOptions);
var
  ToBeSet,
  ToBeCleared: TD2TreePaintOptions;
  Run: PD2TreeNode;
  //HandleWasAllocated: Boolean;
begin
  if FPaintOptions <> Value then
  begin
    ToBeSet := Value - FPaintOptions;
    ToBeCleared := FPaintOptions - Value;
    FPaintOptions := Value;
    if (toFixedIndent in ToBeSet) then
    begin
      // Fixes issue #388
      Include(FPaintOptions, toShowRoot);
      Include(ToBeSet, toShowRoot);
    end;//if
    if Assigned(FOwner) then
      with FOwner do
      begin
        if not (csLoading in ComponentState) and (toShowFilteredNodes in ToBeSet + ToBeCleared) then
        begin
          //Repaint;
          InterruptValidation;
          Run := GetFirstNoInit;
          while Assigned(Run) do
          begin
            if (vsFiltered in Run.States) then
            begin
              if FullyVisible[Run] then
              begin
                if toShowFilteredNodes in ToBeSet then
                  Inc(FVisibleCount)
                else
                  Dec(FVisibleCount);
              end;
              if toShowFilteredNodes in ToBeSet then
                AdjustTotalHeight(Run, Run.NodeHeight, True)
              else
                AdjustTotalHeight(Run, -Run.NodeHeight, True);
            end;
            Run := GetNextNoInit(Run);
          end;
          //if HandleWasAllocated then
            //EndUpdate;
        end;
        {!!! не актурально
        if IsWinVistaOrAbove and ((tsUseThemes in FStates) or
           ((toThemeAware in ToBeSet) and StyleServices.Enabled)) and
           (toUseExplorerTheme in (ToBeSet + ToBeCleared)) and not VclStyleEnabled then
          if (toUseExplorerTheme in ToBeSet) then
          begin
            SetWindowTheme('explorer');
            DoStateChange([tsUseExplorerTheme]);
          end
          else
            if toUseExplorerTheme in ToBeCleared then
            begin
              SetWindowTheme('');
              DoStateChange([], [tsUseExplorerTheme]);
            end;

        if not (csLoading in ComponentState) then
        begin
          if ((toThemeAware in ToBeSet + ToBeCleared) or (toUseExplorerTheme in ToBeSet + ToBeCleared) or VclStyleEnabled) then
          begin
            if ((toThemeAware in ToBeSet) and StyleServices.Enabled) or VclStyleEnabled then
              DoStateChange([tsUseThemes])
            else
              if (toThemeAware in ToBeCleared) then
                DoStateChange([], [tsUseThemes]);

            PrepareBitmaps(True, False);
            RedrawWindow(Handle, nil, 0, RDW_INVALIDATE or RDW_VALIDATE or RDW_FRAME);
          end;
             }
          if toChildrenAbove in ToBeSet + ToBeCleared then
          begin
            InvalidateCache;
            if FUpdateCount = 0 then
            begin
              ValidateCache;
              Realign; //Invalidate;
            end;
          end;
          Realign; //Invalidate;
        end;
      end;
end;

procedure TD2CustomTreeOptions.SetSelectionOptions(const Value: TD2TreeSelectionOptions);
var
  ToBeSet,
  ToBeCleared: TD2TreeSelectionOptions;

begin
  if FSelectionOptions <> Value then
  begin
    ToBeSet := Value - FSelectionOptions;
    ToBeCleared := FSelectionOptions - Value;
    FSelectionOptions := Value;

    with FOwner do
    begin
      if (toMultiSelect in (ToBeCleared + ToBeSet)) or
        ([toLevelSelectConstraint, toSiblingSelectConstraint] * ToBeSet <> []) then
        ClearSelection;

      if (toExtendedFocus in ToBeCleared) and (FFocusedColumn > 0) {and HandleAllocated }
         then begin
                FFocusedColumn := FMainColumn;
                Realign; //Invalidate;
              end;

      if not (toExtendedFocus in FSelectionOptions) then
        FFocusedColumn := FMainColumn;
    end;
  end;
end;

procedure TD2CustomTreeOptions.AssignTo(Dest: TPersistent);
begin
  if Dest is TD2CustomTreeOptions then
  begin
    with TD2CustomTreeOptions(Dest) do
    begin
      PaintOptions := Self.PaintOptions;
      AnimationOptions := Self.AnimationOptions;
      AutoOptions := Self.AutoOptions;
      SelectionOptions := Self.SelectionOptions;
      MiscOptions := Self.MiscOptions;
    end;
  end
  else inherited;
end;
//--------------------------------------------------------------

{ -------------------- TD2CustomTreeGrid -------------------- }

procedure TD2CustomTreeGrid.AdjustTotalCount(Node: PD2TreeNode;
  Value: Integer; Relative: Boolean);
//Изменяет общее кол-во узлов (TotalCount) узла Node и всех его родителей в соответствии со значеним Value.
//При Relative = true - Value = величина изменения, иначе Value = абсолютное значение
//(на самом деле, корректировка делается итеративно, чтобы избежать накладных расходов на вызов функции).
// Sets a node's total count to the given value and recursively adjusts the parent's total count
// (actually, the adjustment is done iteratively to avoid function call overheads).
var
  Difference: Integer;
  Run: PD2TreeNode;
begin
  if Relative
    then Difference := Value
    else Difference := Value - Integer(Node.TotalCount);
  if Difference <> 0 then
  begin
    Run := Node;
    //Корневой узел имеет дерево в качестве родителя. Root node has as parent the tree view.
    while Assigned(Run) and (Run <> Pointer(Self)) do
    begin
      Inc(Integer(Run.TotalCount), Difference);
      Run := Run.Parent;
    end;
  end;
end;

procedure TD2CustomTreeGrid.AdjustTotalHeight(Node: PD2TreeNode; Value: Single; Relative: Boolean);
//Устанавливает высоту узла в заданное значение и рекурсивно изменяет общую высоту всех его родителей.
// Sets a node's total height to the given value and recursively adjusts the parent's total height.
var
  Difference: Single;
  Run: PD2TreeNode;
begin
  if Relative                 //если относительное значение
    then Difference := Value                              //то изменение высоты узла задана явно
    else Difference := Value - Node.TotalHeight; //иначе задана новая высота узла. вычислим изменение высоты узла
  if Difference <> 0 then    //если изменение не 0, то
  begin
    Run := Node;    //текущий узел равен заданному
    repeat          //для узла и всех его родителей
      //Inc(Integer(Run.TotalHeight), Difference);  //изменяем общую высоту узла
      Run.TotalHeight:=Run.TotalHeight+Difference;
      // If the node is not visible or the parent node is not expanded or we are already at the top
      // then nothing more remains to do.
      if not (vsVisible in Run.States) or        //Если узел не виден или
         (Run = FRoot) or (Run.Parent = nil) or  //мы уже на вершине
         not (vsExpanded in Run.Parent.States)   //или родительский узел не расширен
        then Break;                                //то входим из цикла

      Run := Run.Parent;                         //иначе переходим к родителю
    until False;         //перейти в начало цикла
  end;
  UpdateVerticalRange;
end;

function TD2CustomTreeGrid.CalculateCacheEntryCount: Integer;
// Вычисляет размер кэша позиции.
// Calculates the size of the position cache.
begin
  if FVisibleCount > 1
     then Result := Ceil(FVisibleCount / CacheThreshold)
     else Result := 0;
end;

function TD2CustomTreeGrid.ChangeCheckState(Node: PD2TreeNode;
  Value: TD2CheckState): Boolean;
//Устанавливает состояние проверки узла в соответствии с заданным значением и типом проверки узла.
//Если состояние проверки должно распространяться на родительские узлы, и один из них отказывается
//изменяться, то ничего не происходит и возвращается False, иначе True.
// Sets the check state of the node according to the given value and the node's check type.
// If the check state must be propagated to the parent nodes and one of them refuses to change then
// nothing happens and False is returned, otherwise True.

var
  Run: PD2TreeNode;
  UncheckedCount,
  MixedCheckCount,
  CheckedCount: Cardinal;

begin
  Result := not (vsChecking in Node.States);
  with Node^ do
  if Result then
  begin
    Include(States, vsChecking);
    try
      if not (vsInitialized in States) then
        InitNode(Node)
      else if CheckState = Value then
      begin
        // Value didn't change and node was initialized, so nothing to do
        Result := False;
        Exit;
      end;//if

      // Indicate that we are going to propagate check states up and down the hierarchy.
      if FCheckPropagationCount = 0 then // WL, 05.02.2004: Do not enter tsCheckPropagation more than once
        DoStateChange([tsCheckPropagation]);
      Inc(FCheckPropagationCount); // WL, 05.02.2004
      // Do actions which are associated with the given check state.
      case CheckType of
        // Check state change with additional consequences for check states of the children.
        ctTriStateCheckBox:
          begin
            // Propagate state down to the children.
            if toAutoTristateTracking in FOptions.FAutoOptions then
              case Value of
                csUncheckedNormal:
                  if Node.ChildCount > 0 then
                  begin
                    Run := FirstChild;
                    CheckedCount := 0;
                    MixedCheckCount := 0;
                    UncheckedCount := 0;
                    while Assigned(Run) do
                    begin
                      if Run.CheckType in [ctCheckBox, ctTriStateCheckBox] then
                      begin
                        SetCheckState(Run, csUncheckedNormal);
                        // Check if the new child state was set successfully, otherwise we have to adjust the
                        // node's new check state accordingly.
                        case Run.CheckState of
                          csCheckedNormal:
                            Inc(CheckedCount);
                          csMixedNormal:
                            Inc(MixedCheckCount);
                          csUncheckedNormal:
                            Inc(UncheckedCount);
                        end;
                      end;
                      Run := Run.NextSibling;
                    end;

                    // If there is still a mixed state child node checkbox then this node must be mixed checked too.
                    if MixedCheckCount > 0 then
                      Value := csMixedNormal
                    else
                      // If nodes are normally checked child nodes then the unchecked count determines what
                      // to set for the node itself.
                      if CheckedCount > 0 then
                        if UncheckedCount > 0 then
                          Value := csMixedNormal
                        else
                          Value := csCheckedNormal;
                  end;
                csCheckedNormal:
                  if Node.ChildCount > 0 then
                  begin
                    Run := FirstChild;
                    CheckedCount := 0;
                    MixedCheckCount := 0;
                    UncheckedCount := 0;
                    while Assigned(Run) do
                    begin
                      if Run.CheckType in [ctCheckBox, ctTriStateCheckBox] then
                      begin
                        SetCheckState(Run, csCheckedNormal);
                        // Check if the new child state was set successfully, otherwise we have to adjust the
                        // node's new check state accordingly.
                        case Run.CheckState of
                          csCheckedNormal:
                            Inc(CheckedCount);
                          csMixedNormal:
                            Inc(MixedCheckCount);
                          csUncheckedNormal:
                            Inc(UncheckedCount);
                        end;
                      end;
                      Run := Run.NextSibling;
                    end;

                    // If there is still a mixed state child node checkbox then this node must be mixed checked too.
                    if MixedCheckCount > 0 then
                      Value := csMixedNormal
                    else
                      // If nodes are normally checked child nodes then the unchecked count determines what
                      // to set for the node itself.
                      if CheckedCount > 0 then
                        if UncheckedCount > 0 then
                          Value := csMixedNormal
                        else
                          Value := csCheckedNormal;
                  end;
              end;
          end;
        // radio button check state change
        ctRadioButton:
          if Value = csCheckedNormal then
          begin
            Value := csCheckedNormal;
            // Make sure only this node is checked.
            Run := Parent.FirstChild;
            while Assigned(Run) do
            begin
              if Run.CheckType = ctRadioButton then
                Run.CheckState := csUncheckedNormal;
              Run := Run.NextSibling;
            end;
            Realign; //вместо Invalidate;
          end;
      end;

      if Result then
        CheckState := Value // Set new check state
      else
        CheckState := UnpressedState[CheckState]; // Reset dynamic check state.

      // Propagate state up to the parent.
      if not (vsInitialized in Parent.States) then
        InitNode(Parent);
      if (toAutoTristateTracking in FOptions.FAutoOptions) and ([vsChecking, vsDisabled] * Parent.States = []) and
        (CheckType in [ctCheckBox, ctTriStateCheckBox]) and (Parent <> FRoot) and
        (Parent.CheckType = ctTriStateCheckBox) then
        Result := CheckParentCheckState(Node, Value)
      else
        Result := True;

      InvalidateNode(Node);

      Dec(FCheckPropagationCount); // WL, 05.02.2004
      if FCheckPropagationCount = 0 then // WL, 05.02.2004: Allow state change event after all check operations finished
        DoStateChange([], [tsCheckPropagation]);
    finally
      Exclude(States, vsChecking);
    end;
  end;
end;

function TD2CustomTreeGrid.CompareNodePositions(Node1, Node2: PD2TreeNode;
  ConsiderChildrenAbove: Boolean): Integer;
//Определяет находится ли позиция Node1 до позиции Node2 в дереве.
//Если ConsiderChildrenAbove = True позиции узлов сравниватся в их визуальном порядке.
//Возвращает 0, если Node1 = Node2, < 0, если Node1 расположен перед Node2, иначе > 0.
// Tries hard and smart to quickly determine whether Node1's structural position is before Node2's position.
// If ConsiderChildrenAbove is True, the nodes will be compared with their visual order in mind.
// Returns 0 if Node1 = Node2, < 0 if Node1 is located before Node2 else > 0.

var
  Run1,
  Run2: PD2TreeNode;
  Level1,
  Level2: Cardinal;

begin
  Assert(Assigned(Node1) and Assigned(Node2), 'Nodes must never be nil.');

  if Node1 = Node2 then
    Result := 0
  else
  begin
    if HasAsParent(Node1, Node2) then
      Result := IfThen(ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions), -1, 1)
    else
      if HasAsParent(Node2, Node1) then
        Result := IfThen(ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions), 1, -1)
      else
      begin
        // the given nodes are neither equal nor are they parents of each other, so go up to FRoot
        // for each node and compare the child indices of the top level parents
        // Note: neither Node1 nor Node2 can be FRoot at this point as this (a bit strange) circumstance would
        //       be caught by the previous code.

        // start lookup at the same level
        Level1 := GetNodeLevel(Node1);
        Level2 := GetNodeLevel(Node2);
        Run1 := Node1;
        while Level1 > Level2 do
        begin
          Run1 := Run1.Parent;
          Dec(Level1);
        end;
        Run2 := Node2;
        while Level2 > Level1 do
        begin
          Run2 := Run2.Parent;
          Dec(Level2);
        end;

        // now go up until we find a common parent node (loop will safely stop at FRoot if the nodes
        // don't share a common parent)
        while Run1.Parent <> Run2.Parent do
        begin
          Run1 := Run1.Parent;
          Run2 := Run2.Parent;
        end;
        Result := Integer(Run1.Index) - Integer(Run2.Index);
      end;
  end;
end;

function TD2CustomTreeGrid.FindInPositionCache(Node: PD2TreeNode;
  var CurrentPos: Single): PD2TreeNode;
// Просматривает кеш позиции и возвращает узел, ближайший к Node (позиция меньше или равна заданной)
// CurrentPos - положение найденного узла
// Looks through the position cache and returns the node whose top position is the largest one which is smaller or equal
// to the position of the given node.

var
  L, H, I: Integer;

begin
  L := 0;
  H := High(FPositionCache);
  while L <= H do
  begin
    I := (L + H) shr 1;
    if CompareNodePositions(FPositionCache[I].Node, Node) <= 0 then
      L := I + 1
    else
      H := I - 1;
  end;
  if L = 0 then // High(FPositionCache) = -1
  begin
    Result := nil;
    CurrentPos := 0;
  end
  else
  begin
    Result := FPositionCache[L - 1].Node;
    CurrentPos := FPositionCache[L - 1].AbsoluteTop;
  end;
end;

function TD2CustomTreeGrid.FindInPositionCache(Position: Single;
  var CurrentPos: Single): PD2TreeNode;
// Просматривает кеш позиции и возвращает ближайший к заданной Position позиции узел (позиция меньше или равна заданной)
// CurrentPos - положение найденного узла
// Возвращенный узел не обязательно занимает данную позицию, но является ближайшей
// итерация от подхода к реальному узлу для заданной позиции. CurrentPos получает
// фактическое положение найденного узла, которое необходимо для дальнейшей итерации.

// Looks through the position cache and returns the node whose top position is the
// largest one which is smaller or equal to the given vertical position.  The returned
// node does not necessarily occupy the given position but is the nearest one to start
// iterating from to approach the real node for a given position. CurrentPos receives
// the actual position of the found node which is needed for further iteration.

var
  L, H, I: Integer;

begin
  L := 0;
  H := High(FPositionCache);
  while L <= H do
  begin
    I := (L + H) shr 1;
    if FPositionCache[I].AbsoluteTop <= Position then
      L := I + 1
    else
      H := I - 1;
  end;
  if L = 0 then // High(FPositionCache) = -1
  begin
    Result := nil;
    CurrentPos := 0;
  end
  else
  begin
    Result := FPositionCache[L - 1].Node;
    CurrentPos := FPositionCache[L - 1].AbsoluteTop;
  end;
end;

procedure TD2CustomTreeGrid.FixupTotalCount(Node: PD2TreeNode);
//Пересчитать общую сумму узла Node и его детей
//Вызывается после загрузки поддерева из потока. Количество детей в каждом узле
//уже установлено, но не их общая сумма.
// Called after loading a subtree from stream. The child count in each node is already set but not
// their total count.
var Child: PD2TreeNode;
begin
  // Initial total count is set to one on node creation.
  Child := Node.FirstChild;
  while Assigned(Child) do
  begin
    FixupTotalCount(Child);
    Inc(Node.TotalCount, Child.TotalCount);
    Child := Child.NextSibling;
  end;
end;

//----------------------------------------------------------------------------------------------------------------------

procedure TD2CustomTreeGrid.FixupTotalHeight(Node: PD2TreeNode);
// Пересчитать общую высоту узла Node
// Вызывается после загрузки поддерева из потока. Индивидуальная высота каждого
// узла уже установлена, но их общая высота нуждается в корректировке в зависимости
// от их состояния видимости.
// Called after loading a subtree from stream. The individual height of each node is set already,
// but their total height needs an adjustment depending on their visibility state.
var Child: PD2TreeNode;
begin
  // Initial total height is set to the node height on load.
  Child := Node.FirstChild;

  if vsExpanded in Node.States
    then  begin
            while Assigned(Child) do
            begin
              FixupTotalHeight(Child);
              if vsVisible in Child.States
                 //then Inc(Node.TotalHeight, Child.TotalHeight);
                 then Node.TotalHeight:=Node.TotalHeight + Child.TotalHeight;
              Child := Child.NextSibling;
            end;
          end
    else  begin
            // The node is collapsed, so just update the total height of its child nodes.
            while Assigned(Child) do
            begin
              FixupTotalHeight(Child);
              Child := Child.NextSibling;
            end;
          end;
end;

function TD2CustomTreeGrid.GetBottomNode: PD2TreeNode;
//получить ссылку на нижний видимый узел
begin
  Result := InternalGetNodeAt(0, ClientHeight);
end;

function TD2CustomTreeGrid.GetCheckedCount: Cardinal;
//Получить количество отмеченных узлов в дереве
var Node: PD2TreeNode;
begin
  Result := 0;
  Node := GetFirstChecked;
  while Assigned(Node) do
  begin
     Inc(Result);
     Node := GetNextChecked(Node);
  end;
end;

function TD2CustomTreeGrid.GetCheckState(Node: PD2TreeNode): TD2CheckState;
//Получить состояние отметки узла Node
begin
  Result := Node.CheckState;
end;

function TD2CustomTreeGrid.GetCheckType(Node: PD2TreeNode): TD2CheckType;
//Получить тип отметки узла Node
begin
  Result := Node.CheckType;
end;

function TD2CustomTreeGrid.GetChildCount(Node: PD2TreeNode): Cardinal;
//Получить кол-во детей узла Node
begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.ChildCount
  else
    Result := Node.ChildCount;
end;

function TD2CustomTreeGrid.GetChildrenInitialized(Node: PD2TreeNode): Boolean;
//true - установлено наличие детей для узла
begin
  Result := not (vsHasChildren in Node.States) or (Node.ChildCount > 0);
end;

function TD2CustomTreeGrid.GetCutCopyCount: Cardinal;
//Получить кол-во копируемых узлов
var Node: PD2TreeNode;
begin
  Result := 0;
  Node := GetFirstCutCopy;
  while Assigned(Node) do
  begin
     Inc(Result);
     Node := GetNextCutCopy(Node);
  end;
end;

function TD2CustomTreeGrid.GetDisabled(Node: PD2TreeNode): Boolean;
//true - узел Node отключен
begin
  Result := Assigned(Node) and (vsDisabled in Node.States);
end;

function TD2CustomTreeGrid.GetExpanded(Node: PD2TreeNode): Boolean;
//true - узел Node развернут
begin
  if Assigned(Node) then
    Result := vsExpanded in Node.States
  else
    Result := False;
end;

function TD2CustomTreeGrid.GetFiltered(Node: PD2TreeNode): Boolean;
//true - узел Node отфильтрован
begin
  Result := vsFiltered in Node.States;
end;

function TD2CustomTreeGrid.GetFullyVisible(Node: PD2TreeNode): Boolean;
//True - узел Node видим и все его родители видимы и развернуты.
// Determines whether the given node has the visibility flag set as well as all its parents are expanded.
begin
  Assert(Assigned(Node), 'Invalid parameter GetFullyVisible.');
  Result := vsVisible in Node.States;
  if Result and (Node <> FRoot)
    then Result := VisiblePath[Node];
end;

function TD2CustomTreeGrid.GetHasChildren(Node: PD2TreeNode): Boolean;
//true - узел Node имеет дочерние узлы
begin
  if Assigned(Node) then
    Result := vsHasChildren in Node.States
  else
    Result := vsHasChildren in FRoot.States;
end;

function TD2CustomTreeGrid.GetMainColumn: integer;
//Получить колонку, отображающая структуру дерева
begin
  if ColumnCount > 0
    then Result := FMainColumn
    else Result := NoColumn;
end;

function TD2CustomTreeGrid.GetMultiline(Node: PD2TreeNode): Boolean;
//true - текст узла Node многострочный
begin
  Result := Assigned(Node) and (Node <> FRoot) and (vsMultiline in Node.States);
end;

function TD2CustomTreeGrid.GetNodeHeight(Node: PD2TreeNode): Single;
//Получить высоту узла Node
begin
  if Assigned(Node) and (Node <> FRoot)
    then  begin
            if (toVariableNodeHeight in FOptions.FMiscOptions) and not (vsDeleting in Node.States) then
            begin
              if not (vsInitialized in Node.States)
                then InitNode(Node);

              // Ensure the node's height is determined.
              MeasureItemHeight(Node);
            end;
            Result := Node.NodeHeight;
          end
    else  Result := 0;
end;

function TD2CustomTreeGrid.GetNodeParent(Node: PD2TreeNode): PD2TreeNode;
//Получить родителя узла Node
begin
  if Assigned(Node) and (Node.Parent <> FRoot)
    then Result := Node.Parent
    else Result := nil;
end;

function TD2CustomTreeGrid.GetOffsetXY: TD2Point;
//Получить смещение прокрутки слева и сверху
begin
  Result := d2Point(HScrollBarValue, VScrollBarValue);
end;

function TD2CustomTreeGrid.GetRangeX: Single;
begin
  //UpdateHorizontalRange;
  Result := Max(0, FRangeX);
end;

function TD2CustomTreeGrid.GetRootNodeCount: Cardinal;
//получить кол-во детей узла Root
begin
  Result := FRoot.ChildCount;
end;

function TD2CustomTreeGrid.GetSelected(Node: PD2TreeNode): Boolean;
//True если узел существует и выбран
begin
  Result := Assigned(Node) and (vsSelected in Node.States);
end;

function TD2CustomTreeGrid.GetTopNode: PD2TreeNode;
//Получить верхний видимый узел
begin
  //Result := InternalGetNodeAt(0, 0);
  Result := InternalGetNodeAt(0, 0, True, FTopRowY, FTopRow);
  if Result = nil
    then FTopRowY:= 0
    else d2IncFloat(FTopRowY, VScrollBarValue);
end;

function TD2CustomTreeGrid.GetTotalCount: Cardinal;
//Получить общее кол-во узлов в дереве (не считая узел Root)
begin
  Inc(FUpdateCount);
  try
    ValidateNode(FRoot, True);
  finally
    Dec(FUpdateCount);
  end;
  // The root node itself doesn't count as node.
  Result := FRoot.TotalCount - 1;
end;

function TD2CustomTreeGrid.GetVerticalAlignment(Node: PD2TreeNode): Single;
//Получить вертикальное смещение кнопки открытия/сворачивания узла Node
begin
  Result := Node.Align;
end;

function TD2CustomTreeGrid.GetVisible(Node: PD2TreeNode): Boolean;
//True - узел Node видимый.
// Determines if the given node is marked as being visible.
begin
  if Node = nil then Node := FRoot;
  if not (vsInitialized in Node.States)
    then InitNode(Node);
  Result := vsVisible in Node.States;
end;

function TD2CustomTreeGrid.GetVisiblePath(Node: PD2TreeNode): Boolean;
//True - все родители узла Node развернуты и видимы.
// Determines if all parents of the given node are expanded and have the visibility flag set.
begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameters SetFullyVisible.'); //если Node = Nil то прервать выполение
  // Узел FRoot всегда развернут. FRoot is always expanded
  repeat
    Node := Node.Parent;
  until (Node = FRoot) or not (vsExpanded in Node.States) or not (vsVisible in Node.States);
  Result := Node = FRoot;
end;

function TD2CustomTreeGrid.HasVisibleNextSibling(Node: PD2TreeNode): Boolean;
//True - если следующий за узлом Node одноуровневый узел видимый.
//Требуется, чтобы сделать правильные линии дерева
// Helper method to determine if the given node has a visible next sibling.
// This is needed to draw correct tree lines.
begin
  // Check if there is a sibling at all.
  Result := Assigned(Node.NextSibling);

  if Result then
  begin
    repeat
      Node := Node.NextSibling;
      Result := IsEffectivelyVisible[Node];
    until Result or (Node.NextSibling = nil);
  end;
end;

function TD2CustomTreeGrid.HasVisiblePreviousSibling(Node: PD2TreeNode): Boolean;
//True - если предыдущий перед узлом Node одноуровневый узел видимый.
//Требуется, чтобы сделать правильные линии дерева
// Helper method to determine if the given node has a visible previous sibling.
// This is needed to draw correct tree lines.

begin
  // Check if there is a sibling at all.
  Result := Assigned(Node.PrevSibling);

  if Result then
  begin
    repeat
      Node := Node.PrevSibling;
      Result := IsEffectivelyVisible[Node];
    until Result or (Node.PrevSibling = nil);
  end;
end;

procedure TD2CustomTreeGrid.InterruptValidation;
//прервать валидацию???
var WasValidating: Boolean;
begin
  DoStateChange([tsStopValidation], [tsUseCache]);
  {$ifdef EnableThreadSupport}
  // Проверка существования рабочего потока. Он может быть уже уничтожен (как правило при уничтожении последнего дерева).
  // Check the worker thread existance. It might already be gone (usually on destruction of the last tree).
  if Assigned(WorkerThread) then
  begin
    WasValidating := (tsValidating in FStates);
    WorkerThread.RemoveTree(Self);
    if WasValidating then
      InvalidateCache();
  end;
  {$endif}
end;

procedure TD2CustomTreeGrid.InitRootNode(OldSize: Cardinal);
//Реинициализирует корневой узел.
// Reinitializes the root node.
var NewSize: Cardinal;
begin
  NewSize := TreeNodeSize + FTotalInternalDataSize;
  if FRoot = nil then
    FRoot := AllocMem(NewSize)
  else
  begin
    ReallocMem(FRoot, NewSize);
    FillChar(PAnsiChar(PAnsiChar(FRoot) + OldSize)^, NewSize - OldSize, 0);
  end;

  with FRoot^ do
  begin
    // Indication that this node is the root node.
    PrevSibling := FRoot;
    NextSibling := FRoot;
    Parent := Pointer(Self);
    States := [vsInitialized, vsExpanded, vsHasChildren, vsVisible];
    TotalHeight := FDefaultNodeHeight;
    TotalCount := 1;
    NodeHeight := FDefaultNodeHeight;
    Align := 50;
  end;
end;

function TD2CustomTreeGrid.IsFirstVisibleChild(Parent, Node: PD2TreeNode ): Boolean;
//True - если узел Node является первым видимым ребенком узла Parent.
// Helper method to check if Node is the same as the first visible child of Parent.
var Run: PD2TreeNode;
begin
  // Find first visible child.
  Run := Parent.FirstChild;
  while Assigned(Run) and not IsEffectivelyVisible[Run] do
    Run := Run.NextSibling;

  Result := Assigned(Run) and (Run = Node);
end;

function TD2CustomTreeGrid.IsLastVisibleChild(Parent, Node: PD2TreeNode): Boolean;
//True - если узел Node является последним видимым ребенком узла Parent.
// Helper method to check if Node is the same as the last visible child of Parent.
var Run: PD2TreeNode;
begin
  // Find last visible child.
  Run := Parent.LastChild;
  while Assigned(Run) and not IsEffectivelyVisible[Run] do
    Run := Run.PrevSibling;

  Result := Assigned(Run) and (Run = Node);
end;

function TD2CustomTreeGrid.MakeNewNode: PD2TreeNode;
//Создает новый узел дерева и возвращает его указатель
var Size: Cardinal;
begin
  Size := TreeNodeSize;
  if not (csDesigning in ComponentState) then
  begin
    if FNodeDataSize = -1 then ValidateNodeDataSize(FNodeDataSize); //Убедимся, что FNodeDataSize является действительным. Make sure FNodeDataSize is valid.
    Inc(Size, FNodeDataSize); //Учтем размер данных узла. Take record alignment into account.
  end;
  Result := AllocMem(Size + FTotalInternalDataSize);  //выделяем память под узел

  with Result^ do // Заполнение значений по умолчанию. Fill in some default values.
  begin
    TotalCount := 1;
    TotalHeight := FDefaultNodeHeight;
    NodeHeight := FDefaultNodeHeight;
    States := [vsVisible];
    Align := 50;
  end;
end;

{$ifdef PACKARRAYPASCAL}

function TD2BaseVirtualTree.PackArray(const TheArray: TD2NodeArray; Count: Integer): Integer;
//Удаляет узлы, которые больше не используются, из массива выбора. Возвращает кол-во оставшихся узлов?
var
  Source, Dest: ^PD2TreeNode;
  ConstOne: NativeInt;
begin
  Source := Pointer(TheArray);
  ConstOne := 1;
  Result := 0;
  // Выполнить быстрое сканирование чтобы найти первую запись
  // Do the fastest scan possible to find the first entry
  while (Count <> 0) and {not Odd(NativeInt(Source^))} (NativeInt(Source^) and ConstOne = 0) do
  begin
    Inc(Result);
    Inc(Source);
    Dec(Count);
  end;

  if Count <> 0 then
  begin
    Dest := Source;
    repeat
      // Skip odd entries
      if {not Odd(NativeInt(Source^))} NativeInt(Source^) and ConstOne = 0 then
      begin
        Dest^ := Source^;
        Inc(Result);
        Inc(Dest);
      end;
      Inc(Source); // Point to the next entry
      Dec(Count);
    until Count = 0;
  end;
end;
{$else}

{$IMPLICITEXCEPTIONS OFF}

function TD2CustomTreeGrid.PackArray(TheArray: TD2NodeArray; Count: Integer): Integer; assembler;
// Удаляет узлы, которые больше не используются, из массива выбора. Возвращает кол-во оставшихся узлов.
// Массив выбора должен быть отсортирован для работы этого алгоритма. У удаляемых узлов
// установлен бит 0 (LSB). Этот маленький трюк работает, потому что выделение памяти всегда
// выровнены на DWORD. Поскольку массив выбора должен быть отсортирован при определении
// элементов, которые нужно удалить, гораздо эффективнее увеличивать рассматриваемую
// запись, а не устанавливать ее на нуль (что нарушит упорядоченное появление списка).

//На входе EAX содержит ссылку на себя, в EDX адрес массива TheArray и ECX содержит Count.
//Возвращаемое значение количество оставшихся записей в массиве, так что абонент
//может перераспределить (сократить) массив выбора, если это необходимо, или -1,
//если ничего не нужно изменять.

// Removes all entries from the selection array which are no longer in use. The selection array must be sorted for this
// algo to work. Values which must be removed are marked with bit 0 (LSB) set. This little trick works because memory
// is always allocated DWORD aligned. Since the selection array must be sorted while determining the entries to be
// removed it is much more efficient to increment the entry in question instead of setting it to nil (which would break
// the ordered appearance of the list).
//
// On enter EAX contains self reference, EDX the address to TheArray and ECX Count
// The returned value is the number of remaining entries in the array, so the caller can reallocate (shorten)
// the selection array if needed or -1 if nothing needs to be changed.

asm
        PUSH    EBX
        PUSH    EDI
        PUSH    ESI
        {$if FPC_FULLVERSION >= 30100}
        MOV     ESI, EDX
        {$else}
        MOV     ECX, EDX               //fpc < 3.1: count is in EDX. Move to ECX
        MOV     ESI, [EBP+8]           //fpc < 3.1: TheArray is in EBP+8
        {$endif}
        MOV     EDX, -1
        JCXZ    @@Finish               // Empty list?
        INC     EDX                    // init remaining entries counter
        MOV     EDI, ESI               // source and destination point to the list memory
        MOV     EBX, 1                 // use a register instead of immediate operant to check against
@@PreScan:
        TEST    [ESI], EBX             // do the fastest scan possible to find the first entry
                                       // which must be removed
        JNZ     @@DoMainLoop
        INC     EDX
        ADD     ESI, 4
        DEC     ECX
        JNZ     @@PreScan
        JMP     @@Finish

@@DoMainLoop:
        MOV     EDI, ESI
@@MainLoop:
        TEST    [ESI], EBX             // odd entry?
        JNE     @@Skip                 // yes, so skip this one
        MOVSD                          // else move the entry to new location
        INC     EDX                    // count the moved entries
        DEC     ECX
        JNZ     @@MainLoop             // do it until all entries are processed
        JMP     @@Finish

@@Skip:
        ADD     ESI, 4                 // point to the next entry
        DEC     ECX
        JNZ     @@MainLoop             // do it until all entries are processed
@@Finish:
        MOV     EAX, EDX               // prepare return value
        POP     ESI
        POP     EDI
        POP     EBX
end;

procedure TD2CustomTreeGrid.SetBottomNode(Node: PD2TreeNode);
//Установить узел Node самым нижним
var
  Run: PD2TreeNode;
  R: TD2Rect;

begin
  if Assigned(Node) then
  begin
    // make sure all parents of the node are expanded
    Run := Node.Parent;
    while Run <> FRoot do
    begin
      if not (vsExpanded in Run.States) then
        ToggleNode(Run);
      Run := Run.Parent;
    end;
    R := GetDisplayRect(Node, MainColumn, True);
    VScrollBar.Value := R.Bottom - ClientHeight - 1;
  end;
end;

procedure TD2CustomTreeGrid.SetBottomSpace(const Value: Single);
//Установить высоту дополнительного пространства ниже последнего видимого узла
begin
  if FBottomSpace <> Value then
  begin
    FBottomSpace := Value;
    //UpdateVerticalScrollBar(True);
    Realign;
  end;
end;

procedure TD2CustomTreeGrid.SetMainColumn(Value: Integer);
//Изменить колонку, отображающую структуру дерева
begin
  if csLoading in ComponentState
     then FMainColumn := Value
     else begin
            if Value < 0 then Value := 0;
            if Value > ColumnCount - 1 then Value := ColumnCount - 1;
            if Value <> FMainColumn then
            begin
              FMainColumn := Value;
              MainColumnChanged;
              if not (toExtendedFocus in FOptions.FSelectionOptions)
                 then FFocusedColumn := Value;
              Realign;
            end;
          end;
end;


procedure TD2CustomTreeGrid.SetShowCheckboxes(const Value: boolean);
//Установить видимость чек-боксов: true - показывать; false - не показывать
begin
  if FShowCheckboxes <> Value then
  begin
    FShowCheckboxes := Value;
    Realign;
  end;
end;

procedure TD2CustomTreeGrid.SetCheckState(Node: PD2TreeNode; Value: TD2CheckState);
//Установить состояние отметки для узла Node
begin
  if (Node.CheckState <> Value) and not (vsDisabled in Node.States) and DoChecking(Node, Value) then
    DoCheckClick(Node, Value);
end;

procedure TD2CustomTreeGrid.SetCheckType(Node: PD2TreeNode; Value: TD2CheckType);
//Установить тип отметки для узла Node
begin
  if (Node.CheckType <> Value) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Node.CheckType := Value;
    if (Value <> ctTriStateCheckBox) and (Node.CheckState in [csMixedNormal, csMixedPressed]) then
      Node.CheckState := csUncheckedNormal;// reset check state if it doesn't fit the new check type
    // For check boxes with tri-state check box parents we have to initialize differently.
    if (toAutoTriStateTracking in FOptions.FAutoOptions) and (Value in [ctCheckBox, ctTriStateCheckBox]) and
      (Node.Parent <> FRoot) then
    begin
      if not (vsInitialized in Node.Parent.States) then
        InitNode(Node.Parent);
      if (Node.Parent.CheckType = ctTriStateCheckBox) and
        (Node.Parent.CheckState in [csUncheckedNormal, csCheckedNormal]) then
        CheckState[Node] := Node.Parent.CheckState;
    end;
    InvalidateNode(Node);
  end;
end;


{$IMPLICITEXCEPTIONS ON}

{$endif}

procedure TD2CustomTreeGrid.SetChildCount(Node: PD2TreeNode;
  NewChildCount: Cardinal);
//Установка кол-ва потомков узла Node в NewChildCount с изменением их структуры.
//И спользуется для добавления или удаления дочерних узлов в / из конца списка дочернего узла.
//Чтобы вставить или удалить конкретный узел используется отдельная процедура.
// Changes a node's child structure to accomodate the new child count. This is used to add or delete
// child nodes to/from the end of the node's child list. To insert or delete a specific node a separate
// routine is used.
var
  Remaining: Cardinal;
  Index: Cardinal;
  Child: PD2TreeNode;
  Count: Integer;
  NewHeight: Single;
  lNodeHeight: Single;
begin
  if not (toReadOnly in FOptions.FMiscOptions) then
  begin
    if Node = nil then Node := FRoot;

    if NewChildCount = 0
      then DeleteChild(Node)
    else
    begin
      //Если ничего не изменится, то ничего не делать. If nothing changed then do nothing.
      if NewChildCount <> Node.ChildCount then
      begin
        InterruptValidation;
        NewHeight := 0;

        if NewChildCount > Node.ChildCount then
        begin
          Remaining := NewChildCount - Node.ChildCount;
          Count := Remaining;

          //Новые узлы для добавления. New nodes to add.
          if Assigned(Node.LastChild) then
            Index := Node.LastChild.Index + 1
          else
            begin
              Index := 0;
              Include(Node.States, vsHasChildren);
            end;
          Node.States := Node.States - [vsAllChildrenHidden, vsHeightMeasured];

          //Новые узлы по умолчанию видны, видимость проверять не нужно. New nodes are by default always visible, so we don't need to check the visibility.
          while Remaining > 0 do
          begin
            Child := MakeNewNode;
            Child.Index := Index;
            Child.PrevSibling := Node.LastChild;
            if Assigned(Node.LastChild) then
              Node.LastChild.NextSibling := Child;
            Child.Parent := Node;
            Node.LastChild := Child;
            if Node.FirstChild = nil then
              Node.FirstChild := Child;
            Dec(Remaining);
            Inc(Index);

            if (toVariableNodeHeight in FOptions.FMiscOptions) then
            begin
              lNodeHeight := Child.NodeHeight;
              DoMeasureItem(Child, lNodeHeight);
              Child.NodeHeight := lNodeHeight;
              Child.TotalHeight := lNodeHeight;
            end;
            //Inc(NewHeight, Child.NodeHeight);
            NewHeight:=NewHeight + Child.NodeHeight;
          end;

          if vsExpanded in Node.States then
          begin
            AdjustTotalHeight(Node, NewHeight, True);
            if FullyVisible[Node] then
              Inc(Integer(FVisibleCount), Count);
          end;

          AdjustTotalCount(Node, Count, True);
          Node.ChildCount := NewChildCount;
          //if (FUpdateCount = 0) and (toAutoSort in FOptions.FAutoOptions) and
          //   (FHeader.FSortColumn > InvalidColumn)
          //  then Sort(Node, FHeader.FSortColumn, FHeader.FSortDirection, True);

          InvalidateCache;
        end
        else
        begin
          // Nodes have to be deleted.
          Remaining := Node.ChildCount - NewChildCount;
          while Remaining > 0 do
          begin
            DeleteNode(Node.LastChild);
            Dec(Remaining);
          end;
        end;

        if FUpdateCount = 0 then
        begin
          ValidateCache;
          //UpdateScrollBars(True);
          //Invalidate;
          Realign; //вместо UpdateScrollBars(True) и Invalidate
        end;

        if Node = FRoot then
          StructureChange(nil, crChildAdded)
        else
          StructureChange(Node, crChildAdded);
      end;
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetDefaultNodeHeight(Value: Single);
//Установить высоту узла по умолчанию
begin
  if Value = 0 then
    Value := FRowHeight;
  if FDefaultNodeHeight <> Value then
  begin
    d2IncFloat(FRoot.TotalHeight, Value - FDefaultNodeHeight);
    d2IncFloat(FRoot.NodeHeight, Value - FDefaultNodeHeight);

    FDefaultNodeHeight := Value;
    InvalidateCache;
    if (FUpdateCount = 0) {and HandleAllocated} and not (csLoading in ComponentState) then
    begin
      ValidateCache;
      //UpdateScrollBars(True);
      ScrollIntoView(FFocusedNode, toCenterScrollIntoView in FOptions.SelectionOptions, True);
      Realign; //вместо UpdateScrollBars(True) и Invalidate;
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetDisabled(Node: PD2TreeNode; Value: Boolean);
//Устанавливает флаг vsDisabled (отключен) у узла Node в соответствии с Value.
begin
  if Assigned(Node) and (Value xor (vsDisabled in Node.States)) then
  begin
    if Value then
      Include(Node.States, vsDisabled)
    else
      Exclude(Node.States, vsDisabled);

    if FUpdateCount = 0 then
      InvalidateNode(Node);
  end;
end;

procedure TD2CustomTreeGrid.SetExpanded(Node: PD2TreeNode; Value: Boolean);
//Устанавливает флаг vsExpanded (развернутый) у узла Node в соответствии с Value
begin
 if Assigned(Node) and (Node <> FRoot) and (Value xor (vsExpanded in Node.States)) then
    ToggleNode(Node);
end;

procedure TD2CustomTreeGrid.SetFiltered(Node: PD2TreeNode; Value: Boolean);
//Устанавливает флаг vsFiltered (отфильтрованный) у узла Node в соответствии с Value и обновляет все зависимые статусы.
// Sets the 'filtered' flag of the given node according to Value and updates all dependent states.
var NeedUpdate: Boolean;
begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  // Initialize the node if necessary as this might change the filtered state.
  if not (vsInitialized in Node.States) then
    InitNode(Node);

  if Value <> (vsFiltered in Node.States) then
  begin
    InterruptValidation;
    NeedUpdate := False;
    if Value then
    begin
      Include(Node.States, vsFiltered);
      if not (toShowFilteredNodes in FOptions.FPaintOptions) then
      begin
        AdjustTotalHeight(Node, -Integer(NodeHeight[Node]), True);
        if FullyVisible[Node] then
        begin
          Dec(FVisibleCount);
          NeedUpdate := True;
        end;
      end;

      if FUpdateCount = 0 then
        DetermineHiddenChildrenFlag(Node.Parent)
      else
        Include(FStates, tsUpdateHiddenChildrenNeeded);
    end
    else
    begin
      Exclude(Node.States, vsFiltered);
      if not (toShowFilteredNodes in FOptions.FPaintOptions) then
      begin
        AdjustTotalHeight(Node, Integer(NodeHeight[Node]), True);
        if FullyVisible[Node] then
        begin
          Inc(FVisibleCount);
          NeedUpdate := True;
        end;
      end;

      if vsVisible in Node.States then
        // Update the hidden children flag of the parent.
        // Since this node is now visible we simply have to remove the flag.
        Exclude(Node.Parent.States, vsAllChildrenHidden);
    end;

    InvalidateCache;
    if NeedUpdate and (FUpdateCount = 0) then
    begin
      ValidateCache;
      //UpdateScrollBars(True);
      //Invalidate;
      Realign; //вместо UpdateScrollBars(True) и Invalidate
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetFocusedNode(Value: PD2TreeNode);
//Передать фокус узлу Value
var WasDifferent: Boolean;
begin
  WasDifferent := Value <> FFocusedNode;
  DoFocusNode(Value, True);
  // Do change event only if there was actually a change.
  if WasDifferent and (FFocusedNode = Value) then
    DoFocusChange(FFocusedNode, FFocusedColumn);
end;

procedure TD2CustomTreeGrid.SetFullyVisible(Node: PD2TreeNode; Value: Boolean);
//При Value=True узел Node и все его родители становятся видимыми, а также все его родители раскрываются
//При Value=False флаг видимости сбрасывается, без изменения состояния развернутости и видимости родительских узлов.
// This method ensures that a node is visible and all its parent nodes are expanded and also visible
// if Value is True. Otherwise the visibility flag of the node is reset but the expand state
// of the parent nodes stays untouched.
begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter SetFullyVisible'); //если Node = Nil то прервать выполение

  IsVisible[Node] := Value; //установить видимость узла в Value
  if Value then             //Если узел должен быть видим
  begin
    repeat
      Node := Node.Parent;               //устанавливаем текущим родительский узел
      if Node = FRoot then Break;        //если текущий узел - корень, то выходим
      if not (vsExpanded in Node.States) //если узел свернут
        then ToggleNode(Node);             //то развернем узел
      if not (vsVisible in Node.States)  //если узел скрыт
        then IsVisible[Node] := True;      //то покажем узел
    until False;
  end;
end;

procedure TD2CustomTreeGrid.SetHasChildren(Node: PD2TreeNode; Value: Boolean);
//Установить флаг vsHasChildren (наличие детей) в Value у узла Node
begin
  if Assigned(Node) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    if Value then
      Include(Node.States, vsHasChildren)
    else
    begin
      Exclude(Node.States, vsHasChildren);
      DeleteChild(Node);
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetIndentWidth(Value: Single);
//установить отступ границы вложенного узла от границы родителя
begin
  if FIndentWidth <> Value then
  begin
    FIndentWidth := Value;
    if not (csLoading in ComponentState) and (FUpdateCount = 0) {and HandleAllocated}
       then begin
              //UpdateScrollBars(True);
              Realign; //вместо Invalidate;
            end;
  end;
end;

procedure TD2CustomTreeGrid.SetLineMode(const Value: TD2TreeLineMode);
//Установить тип линий деревьева: дерево или полосы и т.д.
begin
  if FLineMode <> Value
    then  begin
            FLineMode := Value;
            if (csLoading in ComponentState)
              then Realign;
          end;
end;

procedure TD2CustomTreeGrid.SetNodeAlignment(const Value: TD2TreeNodeAlignment);
//Установить тип выравнивария для узлов
begin
  if FNodeAlignment <> Value then
  begin
    FNodeAlignment := Value;
    if {HandleAllocated and} not (csReading in ComponentState)
      then Realign; //вместо Invalidate;
  end;
end;

procedure TD2CustomTreeGrid.SetNodeDataSize(Value: Integer);
//Установить кол-во байт для распределения с каждым узлом. Если -1 то делать обратный вызов
var LastRootCount: Cardinal;
begin
  if Value < -1 then
    Value := -1;
  if FNodeDataSize <> Value then
  begin
    FNodeDataSize := Value;
    if not (csLoading in ComponentState) and not (csDesigning in ComponentState) then
    begin
      LastRootCount := FRoot.ChildCount;
      Clear;
      SetRootNodeCount(LastRootCount);
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetNodeHeight(Node: PD2TreeNode; Value: Single);
//Установить в Value высоту узла Node
var Difference: Single;
begin
  if Assigned(Node) and (Node <> FRoot) and (Node.NodeHeight <> Value) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Difference := Value - Node.NodeHeight;
    Node.NodeHeight := Value;

    // Если узел эффективно отфильтрован, ничего делать не нужно, т.к. он все равно не виден.
    // If the node is effectively filtered out, nothing else has to be done, as it is not visible anyway.
    if not IsEffectivelyFiltered[Node] then
    begin
      AdjustTotalHeight(Node, Difference, True);

      Realign; //всесто всего что ниже
      //// Если в настоящий момент активна операция редактирования, обновите границы редакторов.
      //// If an edit operation is currently active then update the editors boundaries as well.
      //UpdateEditBounds;
      //InvalidateCache;
      //// Держитесь подальше от касания кеша узла во время проверки.
      //// Stay away from touching the node cache while it is being validated.
      //if not (tsValidating in FStates) and FullyVisible[Node] and not IsEffectivelyFiltered[Node] then
      //begin
      //  if (FUpdateCount = 0) and ([tsPainting, tsSizing] * FStates = []) then
      //  begin
      //    ValidateCache;
      //    InvalidateToBottom(Node);
      //    Realign; //вместо UpdateScrollBars(True)
      //  end;
      //end;
    end;
  end;
end;

procedure TD2CustomTreeGrid.SetNodeParent(Node: PD2TreeNode; const Value: PD2TreeNode);
//Задать родителя Value для узла Node
begin
  if Assigned(Node) and Assigned(Value) and (Node.Parent <> Value) then
    MoveTo(Node, Value, amAddChildLast, False);
end;

procedure TD2CustomTreeGrid.SetMultiline(Node: PD2TreeNode; const Value: Boolean);
//Задать многострочный режим текста в Value для узла Node
begin
  if Assigned(Node) and (Node <> FRoot) then
    if Value <> (vsMultiline in Node.States) then
    begin
      if Value then
        Include(Node.States, vsMultiline)
      else
        Exclude(Node.States, vsMultiline);

      if FUpdateCount = 0 then
        InvalidateNode(Node);
    end;
end;

procedure TD2CustomTreeGrid.SetOffsetX(const Value: Single);
//Установить значение прокрутки слева
begin
  DoSetOffsetXY(d2Point(Value, VScrollBarValue), DefaultScrollUpdateFlags);
end;

procedure TD2CustomTreeGrid.SetOffsetXY(const Value: TD2Point);
//Установить значение прокрутки слева и сверху
begin
  DoSetOffsetXY(Value, DefaultScrollUpdateFlags);
end;

procedure TD2CustomTreeGrid.SetOffsetY(const Value: Single);
//Установить значение прокрутки сверху
begin
  DoSetOffsetXY(d2Point(HScrollBarValue, Value), DefaultScrollUpdateFlags);
end;

procedure TD2CustomTreeGrid.SetOptions(const Value: TD2CustomTreeOptions);
//Установить опции поведения дерева
begin
  FOptions.Assign(Value);
end;

procedure TD2CustomTreeGrid.SetRootNodeCount(Value: Cardinal);
//установить кол-во детей для узла Root
begin
  // Не устанавливайте кол-во для узла Root до тех пор, пока не будут установлены все другие свойства (в частности, событие OnInitNode).
  // Don't set the root node count until all other properties (in particular the OnInitNode event) have been set.
  if csLoading in ComponentState then
  begin
    FRoot.ChildCount := Value;
    DoStateChange([tsNeedRootCountUpdate]);
  end
  else
    if FRoot.ChildCount <> Value then
    begin
      BeginUpdate;
      InterruptValidation;
      SetChildCount(FRoot, Value);
      EndUpdate;
    end;
end;

procedure TD2CustomTreeGrid.SetSelected(Node: PD2TreeNode; Value: Boolean);
//Установить статус "выбран" узла Node в соответствие с Value
begin
  if not FSelectionLocked and Assigned(Node) and (Node <> FRoot) and (Value xor (vsSelected in Node.States)) then
  begin
    if Value
      then  begin
              if FSelectionCount = 0
                then FRangeAnchor := Node
                else if not (toMultiSelect in FOptions.FSelectionOptions)
                       then ClearSelection;
              AddToSelection(Node);

              // Make sure there is a valid column selected (if there are columns at all).
              //if ((FFocusedColumn < 0) or not (coVisible in FHeader.Columns[FFocusedColumn].Options)) and
              //  (FHeader.MainColumn > NoColumn) then
              //  if ([coVisible, coAllowFocus] *  FHeader.Columns[FHeader.MainColumn].Options = [coVisible, coAllowFocus]) then
              //    FFocusedColumn := FHeader.MainColumn
              //  else
              //    FFocusedColumn := FHeader.Columns.GetFirstVisibleColumn(True);
              if FRangeAnchor = nil
                then FRangeAnchor := Node;
            end
      else  begin
              RemoveFromSelection(Node);
              if FSelectionCount = 0
                then ResetRangeAnchor;
            end;
    if FullyVisible[Node] and not IsEffectivelyFiltered[Node]
      then InvalidateNode(Node);
  end;
end;

procedure TD2CustomTreeGrid.SetTopNode(Node: PD2TreeNode);
//Установить узел Node верхним видимым
var
  R: TD2Rect;
  Run: PD2TreeNode;
begin
  if Assigned(Node) then
  begin
    // make sure all parents of the node are expanded
    Run := Node.Parent;
    while Run <> FRoot do
    begin
      if not (vsExpanded in Run.States) then
        ToggleNode(Run);
      Run := Run.Parent;
    end;
    R := GetDisplayRect(Node, MainColumn, True);
    //header
    //if ShowHeader then d2DecFloat(R.Top, FHeader.Height);
    VScrollBar.Value := R.Top;
  end;
end;

procedure TD2CustomTreeGrid.SetVerticalAlignment(Node: PD2TreeNode; Value: Single);
//Установить в Value значение вертикального смещения кнопки экспандера для узла Node
begin
  if Value > 100 then
    Value := 100;
  if Node.Align <> Value then
  begin
    Node.Align := Value;
    if FullyVisible[Node] and not IsEffectivelyFiltered[Node] then
      InvalidateNode(Node);
  end;
end;

procedure TD2CustomTreeGrid.SetVisible(Node: PD2TreeNode; Value: Boolean);
//Устанавливает видимость узла Node в соответствии с Value.
// Sets the visibility style of the given node according to Value.
var NeedUpdate: Boolean;
begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter SetVisible.');  //если Node = Nil то прервать выполение

  if Value <> (vsVisible in Node.States) then
  begin
    InterruptValidation;
    NeedUpdate := False;
    if Value then
    begin
      Include(Node.States, vsVisible);
      if vsExpanded in Node.Parent.States then
        AdjustTotalHeight(Node.Parent, Node.TotalHeight, True);
      if VisiblePath[Node] then
      begin
        Inc(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
        NeedUpdate := True;
      end;

      // Update the hidden children flag of the parent.
      // Since this node is now visible we simply have to remove the flag.
      if not IsEffectivelyFiltered[Node] then
        Exclude(Node.Parent.States, vsAllChildrenHidden);
    end
    else
    begin
      if vsExpanded in Node.Parent.States then
        AdjustTotalHeight(Node.Parent, -Integer(Node.TotalHeight), True);
      if VisiblePath[Node] then
      begin
        Dec(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
        NeedUpdate := True;
      end;
      Exclude(Node.States, vsVisible);

      if FUpdateCount = 0 then
        DetermineHiddenChildrenFlag(Node.Parent)
      else
        Include(FStates, tsUpdateHiddenChildrenNeeded);
    end;

    InvalidateCache;
    if NeedUpdate and (FUpdateCount = 0) then
    begin
      ValidateCache;
      //UpdateScrollBars(True);
      //Invalidate;
      Realign; //вместо UpdateScrollBars(True) и Invalidate
    end;
  end;
end;


procedure TD2CustomTreeGrid.SetVisiblePath(Node: PD2TreeNode; Value: Boolean);
//При Value = True разворачиваются все родительские узлы для узла Node.
//При Value = False ни какие измененя не происходят
// If Value is True then all parent nodes of Node are expanded.
begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter SetVisiblePath.'); //если Node = Nil то прервать выполение

  if Value then
  begin
    repeat
      Node := Node.Parent;
      if Node = FRoot then Break;
      if not (vsExpanded in Node.States) then ToggleNode(Node);
    until False;
  end;
end;

//function TD2CustomTreeGrid.ToggleCallback(Step, StepSize: Integer;
//  Data: Pointer): Boolean;
//var
//  Column: Integer;
//  Run: TRect;
//  SecondaryStepSize: Integer;
//
//  //--------------- local functions -------------------------------------------
//
//  procedure EraseLine;
//
//  var
//    LocalBrush: HBRUSH;
//
//  begin
//    with TD2ToggleAnimationData(Data^), FHeader.FColumns do
//    begin
//      // Iterate through all columns and erase background in their local color.
//      // LocalBrush is a brush in the color of the particular column.
//      Column := GetFirstVisibleColumn;
//      while (Column > InvalidColumn) and (Run.Left < ClientWidth) do
//      begin
//        GetColumnBounds(Column, Run.Left, Run.Right);
//        if coParentColor in Items[Column].FOptions then
//          FillRect(DC, Run, Brush)
//        else
//        begin
//          if VclStyleEnabled then
//            LocalBrush := CreateSolidBrush(ColorToRGB(FColors.BackGroundColor))
//          else
//            LocalBrush := CreateSolidBrush(ColorToRGB(Items[Column].Color));
//          FillRect(DC, Run, LocalBrush);
//          DeleteObject(LocalBrush);
//        end;
//        Column := GetNextVisibleColumn(Column);
//      end;
//    end;
//  end;
//
//  //---------------------------------------------------------------------------
//
//  procedure DoScrollUp(DC: HDC; Brush: HBRUSH; Area: TRect; Steps: Integer);
//
//begin
//    {$ifndef INCOMPLETE_WINAPI}
//    ScrollDC(DC, 0, -Steps, Area, Area, 0, nil);
//    {$endif}
//
//    if Step = 0 then
//      if not FHeader.UseColumns then
//        FillRect(DC, Rect(Area.Left, Area.Bottom - Steps - 1, Area.Right, Area.Bottom), Brush)
//      else
//      begin
//        Run := Rect(Area.Left, Area.Bottom - Steps - 1, Area.Right, Area.Bottom);
//        EraseLine;
//      end;
//  end;
//
//  //---------------------------------------------------------------------------
//
//  procedure DoScrollDown(DC: HDC; Brush: HBRUSH; Area: TRect; Steps: Integer);
//
//      begin
//      {$ifndef INCOMPLETE_WINAPI}
//      ScrollDC(DC, 0, Steps, Area, Area, 0, nil);
//      {$endif}
//
//        if Step = 0 then
//          if not FHeader.UseColumns then
//        FillRect(DC, Rect(Area.Left, Area.Top, Area.Right, Area.Top + Steps + 1), Brush)
//          else
//          begin
//        Run := Rect(Area.Left, Area.Top, Area.Right, Area.Top + Steps + 1);
//            EraseLine;
//          end;
//      end;
//
//  //--------------- end local functions ---------------------------------------
//
//begin
//  Result := True;
//  if StepSize > 0 then
//  begin
//    SecondaryStepSize := 0;
//    with TToggleAnimationData(Data^) do
//    begin
//      if Mode1 <> tamNoScroll then
//      begin
//        if Mode1 = tamScrollUp then
//          DoScrollUp(DC, Brush, R1, StepSize)
//        else
//          DoScrollDown(DC, Brush, R1, StepSize);
//
//        if (Mode2 <> tamNoScroll) and (ScaleFactor > 0) then
//        begin
//          // As this routine is able to scroll two independent areas at once, the missing StepSize is
//          // computed in that case. To ensure the maximal accuracy the rounding error is accumulated.
//          SecondaryStepSize := Round((StepSize + MissedSteps) * ScaleFactor);
//          MissedSteps := MissedSteps + StepSize * ScaleFactor - SecondaryStepSize;
//        end;
//      end
//      else
//        SecondaryStepSize := StepSize;
//
//      if Mode2 <> tamNoScroll then
//        if Mode2 = tamScrollUp then
//          DoScrollUp(DC, Brush, R2, SecondaryStepSize)
//        else
//          DoScrollDown(DC, Brush, R2, SecondaryStepSize);
//    end;
//  end;
//end;

procedure TD2CustomTreeGrid.AddToSelection(Node: PD2TreeNode);
//Добавляет узел Node в массив текущего выбора.
var Changed: Boolean;
begin
  if not FSelectionLocked then
  begin
    Assert(Assigned(Node), 'Node must not be nil!');
    FSingletonNodeArray[0] := Node;
    Changed := InternalAddToSelection(FSingletonNodeArray, 1, False);
    if Changed then
    begin
      InvalidateNode(Node);
      Change(Node);
    end;
  end;
end;

//----------------------------------------------------------------------------------------------------------------------

procedure TD2CustomTreeGrid.AddToSelection(const NewItems: TD2NodeArray;
  NewLength: Integer; ForceInsert: Boolean = False);
//Добавляет узлы, указанные в массиве NewItems в массив текущего выбора.
//NewLength - количество узлов, для добавления (необходимо, чтобы позволить
//NewItems быть больше, чем фактические используемые записи).
//ForceInsert = True, если узлы должны быть вставлены без учета ограничения
//уровня выбора или уже установленных флагов (например, при загрузке из потока).
//Внимание! В случае ForceInsert = True вызывающий несет ответственность за то,
//          чтобы новые узлы уже не были в массиве выбора!

// Adds the given items all at once into the current selection array. NewLength is the amount of
// nodes to add (necessary to allow NewItems to be larger than the actual used entries).
// ForceInsert is True if nodes must be inserted without consideration of level select constraint or
// already set selected flags (e.g. when loading from stream).
// Note: In the case ForceInsert is True the caller is responsible for making sure the new nodes aren't already in the
//       selection array!
var Changed: Boolean;
begin
  Changed := InternalAddToSelection(NewItems, NewLength, ForceInsert);
  if Changed then
  begin
    if NewLength = 1
    then  begin
            InvalidateNode(NewItems[0]);
            Change(NewItems[0]);
          end
    else  begin
            //Invalidate;
            Realign; //вместо Invalidate
            Change(nil);
          end;
  end;
end;

procedure TD2CustomTreeGrid.AdviseChangeEvent(StructureChange: Boolean;
  Node: PD2TreeNode; Reason: TD2ChangeReason);
//Используется для регистрации отложенного событий изменения.
//Если StructureChange = False, то имеется событие изменения выбора узлов
//(без конкретной причины) иначе это изменение структуры.
// Used to register a delayed change event. If StructureChange is False then
// we have a selection change event (without a specific reason) otherwise it
//is a structure change.
begin
  if StructureChange
   then begin
          if tsStructureChangePending in FStates then
              begin
                {if HandleAllocated
                  then KillTimer(Handle,StructureChangeTimer)//;}
              end
            else
              DoStateChange([tsStructureChangePending]);

          FLastStructureChangeNode := Node;
          if FLastStructureChangeReason = crIgnore
            then FLastStructureChangeReason := Reason
            else if Reason <> crIgnore
                   then FLastStructureChangeReason := crAccumulated;
        end
   else begin
          if tsChangePending in FStates then
            //KillTimer(Handle, ChangeTimer)
          else
            DoStateChange([tsChangePending]);
          FLastChangedNode := Node;
        end;
end;

function TD2CustomTreeGrid.AllocateInternalDataArea(Size: Cardinal): Cardinal;
// Простой метод регистрации будет называться каждым потомком, чтобы претендовать на свою внутреннюю область данных.
//Смещение от начала узла до внутренней области данных вызывающего класса дерева.

// Simple registration method to be called by each descendant to claim their internal data area.
// Result is the offset from the begin of the node to the internal data area of the calling tree class.

begin
  Assert((FRoot = nil) or (FRoot.ChildCount = 0), 'Internal data allocation must be done before any node is created.');
  {$ifdef DEBUG_VTV}Logger.Send('FTotalInternalDataSize BEFORE',FTotalInternalDataSize);{$endif}
  {$ifdef DEBUG_VTV}Logger.Send('Size',Size);{$endif}
  {$ifdef DEBUG_VTV}Logger.Send('TreeNodeSize',TreeNodeSize);{$endif}
  Result := TreeNodeSize + FTotalInternalDataSize;
  {$ifdef DEBUG_VTV}Logger.Send('Result',Result);{$endif}
  Inc(FTotalInternalDataSize, (Size + (SizeOf(Pointer) - 1)) and not (SizeOf(Pointer) - 1));
  {$ifdef DEBUG_VTV}Logger.Send('FTotalInternalDataSize AFTER', FTotalInternalDataSize);{$endif}
  InitRootNode(Result);
end;

//procedure TD2CustomTreeGrid.Animate(Steps, Duration: Single;
//  Callback: TD2VTAnimationCallback; Data: Pointer);
//begin
//
//end;

procedure TD2CustomTreeGrid.Change(Node: PD2TreeNode);
begin
  AdviseChangeEvent(False, Node, crIgnore);

  if FUpdateCount = 0 then
  begin
    {if (FChangeDelay > 0) and not (tsSynchMode in FStates)
      then SetTimer(Handle, ChangeTimer, FChangeDelay, nil)
      else} DoChange(Node);
  end;
end;

function TD2CustomTreeGrid.CheckParentCheckState(Node: PD2TreeNode;
  NewCheckState: TD2CheckState): Boolean;
//Проверяет все братья и сестры узла Node что бы определить кокое состояние отметки должен получить родитель.
// Checks all siblings of node to determine which check state Node's parent must get.

var
  CheckCount,
  BoxCount: Cardinal;
  PartialCheck: Boolean;
  Run: PD2TreeNode;

begin
  CheckCount := 0;
  BoxCount := 0;
  PartialCheck := False;
  Run := Node.Parent.FirstChild;
  while Assigned(Run) do
  begin
    if Run = Node then
    begin
      // The given node cannot be checked because it does not yet have its new check state (as this depends
      // on the outcome of this method). Instead NewCheckState is used as this contains the new state the node
      // will get if this method returns True.
      if Run.CheckType in [ctCheckBox, ctTriStateCheckBox] then
      begin
        Inc(BoxCount);
        if NewCheckState in [csCheckedNormal, csCheckedPressed] then
          Inc(CheckCount);
        PartialCheck := PartialCheck or (NewCheckState = csMixedNormal);
      end;
    end
    else
      if Run.CheckType in [ctCheckBox, ctTriStateCheckBox] then
      begin
        Inc(BoxCount);
        if Run.CheckState in [csCheckedNormal, csCheckedPressed] then
          Inc(CheckCount);
        PartialCheck := PartialCheck or (Run.CheckState = csMixedNormal);
      end;
    Run := Run.NextSibling;
  end;

  if (CheckCount = 0) and not PartialCheck then
    NewCheckState := csUncheckedNormal
  else
    if CheckCount < BoxCount then
      NewCheckState := csMixedNormal
    else
      NewCheckState := csCheckedNormal;

  Node := Node.Parent;
  Result := DoChecking(Node, NewCheckState);
  if Result then
  begin
    DoCheckClick(Node, NewCheckState);
    // Recursively adjust parent of parent.
    // This is already done in the function DoCheckClick() called in the above line
    // We revent unnecessary upward recursion by commenting this code.
    //    with Node^ do
    //    begin
    //      if not (vsInitialized in Parent.States) then
    //        InitNode(Parent);
    //      if ([vsChecking, vsDisabled] * Parent.States = []) and (Parent <> FRoot) and
    //        (Parent.CheckType = ctTriStateCheckBox) then
    //        Result := CheckParentCheckState(Node, NewCheckState);
    //    end;
  end;
end;

procedure TD2CustomTreeGrid.ClearTempCache;
//Обеспечивает надежное состояние кэша временного узла.
//make sure the temporary node cache is in a reliable state
begin
  FTempNodeCache := nil;
  FTempNodeCount := 0;
end;

function TD2CustomTreeGrid.CountLevelDifference(Node1, Node2: PD2TreeNode): Integer;
// Этот метод подсчитывает, на сколько уровней отступа данные узлы отстоят друг от друга.
// Если оба узла имеют одного родителя, то разница 0 в противном случае результат
// GetNodeLevel (Node2) - GetNodeLevel (Node1), но со знаком.
// Если результат отрицательный, то уровень Node2 меньше, чем Node1.

// This method counts how many indentation levels the given nodes are apart.
// If both nodes have the same parent then the difference is 0 otherwise the result
// is basically GetNodeLevel(Node2) - GetNodeLevel(Node1), but with sign.
// If the result is negative then Node2 is less intended than Node1.

var
  Level1, Level2: Integer;

begin
  Assert(Assigned(Node1) and Assigned(Node2), 'Both nodes must be Assigned.');

  Level1 := 0;
  while Node1.Parent <> FRoot do
  begin
    Inc(Level1);
    Node1 := Node1.Parent;
  end;

  Level2 := 0;
  while Node2.Parent <> FRoot do
  begin
    Inc(Level2);
    Node2 := Node2.Parent;
  end;

  Result := Level2 - Level1;
end;

function TD2CustomTreeGrid.CountVisibleChildren(Node: PD2TreeNode): Cardinal;
//Возвращает количество видимых дочерних узлов узла Node.
// Returns the number of visible child nodes of the given node.
begin
  Result := 0;

  // The node's direct children...
  if vsExpanded in Node.States then
  begin
    // ...and their children.
    Node := Node.FirstChild;
    while Assigned(Node) do
    begin
      if vsVisible in Node.States then
        Inc(Result, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
      Node := Node.NextSibling;
    end;
  end;
end;

procedure TD2CustomTreeGrid.DetermineHiddenChildrenFlag(Node: PD2TreeNode);
// Обновление флага vsAllChildrenHidden (все дети скрыты) у узла Node.
// Update the hidden children flag of the given node.
var Run: PD2TreeNode;
begin
  if Node.ChildCount = 0 //если кол-во детей = 0
   then begin
          if vsHasChildren in Node.States                //если установлен флаг наличия детей
            then Exclude(Node.States, vsAllChildrenHidden)  //то исключаем флаг vsAllChildrenHidden из статуса узла
            else Include(Node.States, vsAllChildrenHidden); //иначе включаем флаг vsAllChildrenHidden в статус узла
        end
   else begin  //инече детей > 0
          //Перебрать всех братьев и сестер и остановиться, когда найден видимый узел или все проверены.
          // Iterate through all siblings and stop when one visible is found.
          Run := Node.FirstChild;
          while Assigned(Run) and not IsEffectivelyVisible[Run] do
            Run := Run.NextSibling;
          if Assigned(Run)                                //если найден эффективно видимый узел
            then Exclude(Node.States, vsAllChildrenHidden)   //то исключаем флаг vsAllChildrenHidden из статуса
            else Include(Node.States, vsAllChildrenHidden);  //иначе включаем флаг vsAllChildrenHidden в статус узла
        end;
end;

procedure TD2CustomTreeGrid.DetermineHiddenChildrenFlagAllNodes;
// Обновление флага vsAllChildrenHidden (все дети скрыты) у всех неинициализированных узлов.
var Run: PD2TreeNode;
begin
  Run := GetFirstNoInit(False);
  while Assigned(Run) do
  begin
    DetermineHiddenChildrenFlag(Run);
    Run := GetNextNoInit(Run);
  end;
end;

function TD2CustomTreeGrid.DetermineLineArrayAndSelectLevel(Node: PD2TreeNode;
  var LineArray: TD2TreeLineArray): Integer;
//Функция используется во время циклов рисования и инициализирует массив LineArray идентификаторов типа линии.
//Эти идентификаторы используются для рисования древовидных линий перед узлом Node
//Возвращает уровень вложенности узла, используемый для рисования.

// This method is used during paint cycles and initializes an array of line type IDs. These IDs are used to paint
// the tree lines in front of the given node.
// Additionally an initial count of selected parents is determined and returned which is used for specific painting.

var
  X: Integer;
  //Indent: Integer;
  Run: PD2TreeNode;

begin
  Result := 0;
  if toShowRoot in FOptions.FPaintOptions then
    X := 1
  else
    X := 0;
  Run := Node;
  // Определить уровень отступа верхнего узла.
  // Determine indentation level of top node.
  while Run.Parent <> FRoot do
  begin
    Inc(X);
    Run := Run.Parent;
    // Считаем выбранные узлы (FRoot никогда не выбирается).
    // Count selected nodes (FRoot is never selected).
    if vsSelected in Run.States then
      Inc(Result);
  end;

  // Устанавливаем начальный размер массива линий, это автоматически инициализирует все записи в ltNone ???.
  // Set initial size of line index array, this will automatically initialized all entries to ltNone.
  SetLength(LineArray, X);
  //Indent := X - 1;

  // Использовать линии только если указано.
  // Only use lines if requested.
  if (toShowTreeLines in FOptions.FPaintOptions) {and
     (not (toHideTreeLinesIfThemed in FOptions.FPaintOptions) or not (tsUseThemes in FStates))} then
  begin
    if toChildrenAbove in FOptions.FPaintOptions then
    begin
      Dec(X);
      if not HasVisiblePreviousSibling(Node) then
      begin
        if (Node.Parent <> FRoot) or HasVisibleNextSibling(Node) then
          LineArray[X] := ltBottomRight
        else
          LineArray[X] := ltRight;
      end
      else
        if (Node.Parent = FRoot) and (not HasVisibleNextSibling(Node)) then
          LineArray[X] := ltTopRight
        else
          LineArray[X] := ltTopDownRight;

      // Теперь подходим к корню, чтобы определить остальное.
      // Now go up to the root to determine the rest.
      Run := Node.Parent;
      while Run <> FRoot do
      begin
        Dec(X);
        if HasVisiblePreviousSibling(Run) then
          LineArray[X] := ltTopDown
        else
          LineArray[X] := ltNone;

        Run := Run.Parent;
      end;
    end
    else
    begin

      // При необходимости перейдем через родительский обход.
      // Start over parent traversal if necessary.
      Run := Node;

      if Run.Parent <> FRoot then
      begin
        // Самое последнее изображение (тот, что находится непосредственно перед меткой элемента), отличается.
        // The very last image (the one immediately before the item label) is different.
        if HasVisibleNextSibling(Run) then
          LineArray[X - 1] := ltTopDownRight
        else
          LineArray[X - 1] := ltTopRight;
        Run := Run.Parent;

        // Теперь поднимемся на всех родителей.
        // Now go up all parents.
        repeat
          if Run.Parent = FRoot then
            Break;
          Dec(X);
          if HasVisibleNextSibling(Run) then
            LineArray[X - 1] := ltTopDown
          else
            LineArray[X - 1] := ltNone;
          Run := Run.Parent;
        until False;
      end;

      // Подготовить корневой уровень. Выполнять пункты на этом этапе до узла верхнего уровня.
      // Prepare root level. Run points at this stage to a top level node.
      if (toShowRoot in FOptions.FPaintOptions) and ((toShowTreeLines in FOptions.FPaintOptions) {and
         (not (toHideTreeLinesIfThemed in FOptions.FPaintOptions) or not (tsUseThemes in FStates))}) then
      begin
        //
        // Верхний узел Root? Is the top node a root node?
        if Run = Node then
        begin
          // Первый ребенок получает нижнее правое растровое изображение, если оно не является также единственным потомком.
          // First child gets the bottom-right bitmap if it isn't also the only child.
          if IsFirstVisibleChild(FRoot, Run) then
            // Это единственный ребенок? Is it the only child?
            if IsLastVisibleChild(FRoot, Run) then
              LineArray[0] := ltRight
            else
              LineArray[0] := ltBottomRight
          else
            // Реальный последний ребенок. real last child
            if IsLastVisibleChild(FRoot, Run) then
              LineArray[0] := ltTopRight
            else
              LineArray[0] := ltTopDownRight;
        end
        else
        begin
          //Нет, верхний узел не является узлом верхнего уровня. Поэтому нам нужна другая живопись.
          // No, top node is not a top level node. So we need different painting.
          if HasVisibleNextSibling(Run) then
            LineArray[0] := ltTopDown
          else
            LineArray[0] := ltNone;
        end;
      end;
    end;
  end;

  //if (tsUseExplorerTheme in FStates) and HasChildren[Node] and (Indent >= 0) then
  //  LineArray[Indent] := ltNone;
end;

procedure TD2CustomTreeGrid.DoBeforeDrawTreeLine(Node: PD2TreeNode; Level: integer; var XPos: single);
begin
  if Assigned(FOnBeforeDrawTreeLine)
    then FOnBeforeDrawTreeLine(Self, Node, Level, XPos);
end;

function TD2CustomTreeGrid.DoCancelEdit: Boolean;
//Отменяет текущие действие редактирования или отложенного редактирования.
// Called when the current edit action or a pending edit must be cancelled.
begin
  ////KillTimer(Handle, EditTimer);
  DoStateChange([], [tsEditPending]);
  Result := (tsEditing in FStates) {and FEditLink.CancelEdit};
  if Result then
  begin
    DoStateChange([], [tsEditing]);
    if Assigned(FOnEditCancelled) then
      FOnEditCancelled(Self, FEditColumn);
    //FEditLink := nil;
  end;
end;

procedure TD2CustomTreeGrid.DoChange(Node: PD2TreeNode);
//Вызывает прерывание OnChange
begin
  //KillTimer(Handle, ChangeTimer);
  if Assigned(FOnChange) then
    FOnChange(Self, Node);

  // This is a good place to reset the cached node. This is the same as the node passed in here.
  // This is necessary to allow descendants to override this method and get the node then.
  DoStateChange([], [tsChangePending]);
  FLastChangedNode := nil;
end;

procedure TD2CustomTreeGrid.DoCheckClick(Node: PD2TreeNode; NewCheckState: TD2CheckState);
//Устанавливает состояние отметки узла Node в NewCheckState
begin
  if ChangeCheckState(Node, NewCheckState) then
    DoChecked(Node);
end;

procedure TD2CustomTreeGrid.DoChecked(Node: PD2TreeNode);

begin
  if Assigned(FOnChecked) then
    FOnChecked(Self, Node);

  {$ifdef EnableAccessible}
  NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}
end;

function TD2CustomTreeGrid.DoChecking(Node: PD2TreeNode; var NewCheckState: TD2CheckState): Boolean;
//Определяет может ли узел Node изменить состояние отметки проверки на NewCheckState и вызывает прерывание OnChecking
// Determines if a node is allowed to change its check state to NewCheckState.
begin
  if toReadOnly in FOptions.FMiscOptions
    then Result := False
    else  begin
            Result := True;
            if Assigned(FOnChecking)
              then FOnChecking(Self, Node, NewCheckState, Result);
          end;
end;

procedure TD2CustomTreeGrid.DoCollapsed(Node: PD2TreeNode);
//Вызывает прерывание после сворачиванием узла (OnCollapsed).
//При установленной опции toAlwaysSelectNode выбирается узел, заданный в FNextNodeToSelect
var
  lFirstSelected: PD2TreeNode;
  lParent: PD2TreeNode;
begin
  if Assigned(FOnCollapsed) then
    FOnCollapsed(Self, Node);
  {$ifdef EnableAccessible}
  if Assigned(FAccessibleItem) then
    NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}

  if (toAlwaysSelectNode in TreeOptions.SelectionOptions) then
  begin
    {Выбрать следующий видимый родительский элемент, если текущий выбранный узел становится невидимым из-за сворачивания
     Это делает VT более похожим Win32 элемент управления TreeView
     Это означает, что не допускается множественный выбор и если есть выбранный узел вообще }
    {Select the next visible parent if the currently selected node gets invisible due to a collapse
     This makes the VT behave more like the Win32 custom TreeView control
     This makes only sense no no multi selection is allowed and if there is a selected node at all}
    lFirstSelected := GetFirstSelected();
    if Assigned(lFirstSelected) and not FullyVisible[lFirstSelected] then
    begin
      lParent := GetVisibleParent(lFirstSelected);
      Selected[lParent] := True;
      Selected[lFirstSelected] := False;
    end;//if
    //Если нет (еще нет) выбранного узла,то использовать FNextNodeToSelect чтобы выбрать
    //if there is (still) no selected node, then use FNextNodeToSelect to select one
    if SelectedCount = 0 then
      EnsureNodeSelected();
  end;//if
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2CustomTreeGrid.DoCollapsing(Node: PD2TreeNode): Boolean;
//Вызывает прерывание перед сворачиванием узла (OnCollapsing)
begin
  Result := True;
  if Assigned(FOnCollapsing) then
    FOnCollapsing(Self, Node, Result);
end;

function TD2CustomTreeGrid.DoCompare(Node1, Node2: PD2TreeNode; Column: Integer): Integer;
//Вызывает прерывание сравнения узлов (OnCompareNodes)
begin
  Result := 0;
  if Assigned(FOnCompareNodes) then
    FOnCompareNodes(Self, Node1, Node2, Column, Result);
end;

procedure TD2CustomTreeGrid.DoContentPaint(Sender: TObject;
  const Canvas: TD2Canvas; const ARect: TD2Rect);
//отрисовка альтернативного фона нечетных строк
var
  i:integer;
  P:TD2Point;
  Offset: single;
  Node: PD2TreeNode;
begin
  FAlternatingRowBackground:=toOtherBackgroundOddRow in TreeOptions.PaintOptions;
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    if FAlternatingRowBackground then
    begin
      Canvas.Fill.Assign(FOddFill);
      Offset:=GetTopRowY;
      i:=TopRow;
      for Node in FVisibleRowsList do
      begin
        if Odd(i) then
        begin
          P:=FContent.LocalToAbsolute(d2Point(0,Offset));
          P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
          Canvas.FillRect(d2Rect(P.X,P.Y,P.X + FContent.Width,P.Y + Node.NodeHeight),0,0,[],AbsoluteOpacity);
        end;
        d2IncFloat(Offset, Node.NodeHeight);
        inc(i);
      end;
    end;
  end;
end;

procedure TD2CustomTreeGrid.DoContentPaint2(Sender: TObject;
  const Canvas: TD2Canvas; const ARect: TD2Rect);
//отрисовка горизонтальных и вертикальных линий
var
  i,t, m:integer;
  P,K:TD2Point;
  State:integer;
  s, Offset: single;
  Node: PD2TreeNode;
  //l: TD2TreeLineType;

  NewStyles: TD2TreeLineArray;

begin
  FShowHorzLines:= toShowHorzGridLines in TreeOptions.PaintOptions;
  FShowVertLines:= toShowVertGridLines in TreeOptions.PaintOptions;
  if (FContent <> nil) and (ContentLayout <> nil) and (FShowVertLines or FShowHorzLines) then
  begin
    State:=Canvas.SaveCanvas;
    Canvas.IntersectClipRect(ARect);
    Canvas.Stroke.Assign(FLineFill);
    Offset:=GetTopRowY;
    t:=ColumnCount;
    if FShowHorzLines
      then  begin
              for i:=0 to FVisibleRowsList.Count-1 do
              begin
                d2IncFloat(Offset, PD2TreeNode(FVisibleRowsList[i]).NodeHeight);
                P:=FContent.LocalToAbsolute(d2Point(0,Offset));
                P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
                if (FLineMode = lmNormal) or (t <= 0) //нормальный режим или нет колонок
                  then Canvas.DrawLine(d2Point(P.X,P.Y - 0.5),d2Point(P.X + FContent.Width,P.Y - 0.5),AbsoluteOpacity)
                  else  if t > 0 then // lmBands и есть колонки
                        begin
                          s:=0;
                          K:=Columns[FMainColumn].LocalToAbsolute(d2Point(0,0));
                          K:=TD2VisualObject(Sender).AbsoluteToLocal(K);
                          with TD2TreeCell(Columns[FMainColumn].FCellControls[i]) do
                            if FIndent <> nil
                              then begin
                                     NewStyles:= GetBandsLineArray( IfThen(toFixedIndent in FGrid.FOptions.FPaintOptions,
                                                                           1, Length(FLineArray)), FLineArray);
                                     s:= FIndent.Width;
                                     for m:=Length(NewStyles)-1 downto 0 do
                                        if (NewStyles[m] = ltLeftBottom) //преобразуются в ltLeftBottom
                                           then d2DecFloat(s, FIndentWidth);
                                     s:= Max(s, 0);
                                   end;
                          if FMainColumn=0
                            then  Canvas.DrawLine(d2Point(P.X + s, P.Y - 0.5),d2Point(P.X + FContent.Width,P.Y - 0.5),AbsoluteOpacity)
                            else  begin
                                    Canvas.DrawLine(d2Point(P.X, P.Y - 0.5),d2Point(K.X, P.Y - 0.5),AbsoluteOpacity);
                                    Canvas.DrawLine(d2Point(K.X + s, P.Y - 0.5),d2Point(P.X + FContent.Width, P.Y - 0.5),AbsoluteOpacity);
                                  end;
                        end;
              end;
            end
      else if FShowVertLines then
             for Node in FVisibleRowsList do
               d2IncFloat(Offset, Node.NodeHeight);

    if FShowVertLines
      then  for i:=0 to t - 1 do
            begin
              P:=Columns[i].LocalToAbsolute(d2Point(Columns[i].Width,0));
              P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
              if (toFullVertGridLines in TreeOptions.PaintOptions)
                 then Canvas.DrawLine(d2Point(P.X + 0.5,P.Y),d2Point(P.X + 0.5,P.Y + ContentLayout.Height),AbsoluteOpacity)
                 else Canvas.DrawLine(d2Point(P.X + 0.5,P.Y),d2Point(P.X + 0.5,P.Y + Offset - GetTopRowY),AbsoluteOpacity);
            end;

    Canvas.RestoreCanvas(State);
  end;
end;


procedure TD2CustomTreeGrid.UpdateSelection;   //обновить маркеры выбора строк
var
  i:integer;
  l,b: single;
  P:TD2Point;
  R:TD2Rect;
  SelRects:array of TD2Rect; //массив прямоугольников
  Clone:TD2VisualObject;

  Offset: single;
  Node: PD2TreeNode;
  bSelect, bPreSelect: boolean;
begin

  //inherited UpdateSelection;

  if FSelection=nil then Exit;
  if (FVisibleCount=0) or (ColumnCount=0) then
  begin
    FSelectedRows[0]:=0;			//обнулить длину массива выделенных строк
    if FSelections <> nil then
      begin
        for i:=0 to FSelections.Count - 1 do
          TD2VisualObject(FSelections[i]).Visible:=false;	//сркыть все маркеры выделения
      end;
    if FPreSelection <> nil then FPreSelection.Visible := false;  //скрыть маркер предвыделения
    if FFocus <> nil then
      FFocus.Visible:=false;
    Exit;
  end;
  //делаем массив прямоугольников
  SetLength(SelRects,0);
  Offset:=GetTopRowY;    //координата Y верхнего видимого в окне узла
  FPreSelection.Visible:=false;
  for i:=0 to FVisibleRowsList.Count-1 do //перебираем все видимые в окне узлы
  begin
    if vsSelected in PD2TreeNode(FVisibleRowsList[i]).States then //если текущий узел выделен
    begin
      P := FContent.LocalToAbsolute(d2Point(0, Offset)); //верхнюю левую точку узла преобразуем в координаты контента
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual  //если есть владелец и он визуальный,
        then P := FSelection.Parent.Visual.AbsoluteToLocal(P);        //то преобразуем в координаты владельца FSelection
      if FContent.Width < ClientWidth
        then R:= d2Rect(P.X, P.Y, P.X + ClientWidth,    P.Y + PD2TreeNode(FVisibleRowsList[i]).NodeHeight)
        else R:= d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + PD2TreeNode(FVisibleRowsList[i]).NodeHeight);
      SetLength(SelRects,Length(SelRects) + 1);
      SelRects[High(SelRects)]:=R;
    end;

    //установим границы маркера пред.выбора
    if (FPreSelection <> nil) and             //если маркер предвыбора имеется и
       ( (FIsDragOver    and (FDropTargetNode = PD2TreeNode(FVisibleRowsList[i]))) or     //маркер предвыбора влючен и предвыбранный узел равен текущему, или
         (FIsPreSelected and (FLastSelected   = PD2TreeNode(FVisibleRowsList[i]))) and not FIsDragOver) then  //осуществляется перетаскивание и целевой узел равен текущему, то
      begin
        P := FContent.LocalToAbsolute(d2Point(0, Offset)); //верхнюю левую точку узла преобразуем в координаты контента
        if (FPreSelection.Parent <> nil) and FPreSelection.Parent.IsVisual then  //если есть владелец и он визуальный,
          P := FPreSelection.Parent.Visual.AbsoluteToLocal(P);                     //то преобразуем в координаты владельца FPreSelection
        if (FContent.Width < ClientWidth) or FIsDragOver
          then R:= d2Rect(P.X, P.Y, P.X + ClientWidth,    P.Y + PD2TreeNode(FVisibleRowsList[i]).NodeHeight)
          else R:= d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + PD2TreeNode(FVisibleRowsList[i]).NodeHeight);
        FPreSelection.SetBounds(R.Left, R.Top, R.Right - R.Left, R.Bottom - R.Top); //установим границы маркера пред.выбора
        FPreSelection.Visible:=true;
      end;
    d2IncFloat(Offset, PD2TreeNode(FVisibleRowsList[i]).NodeHeight); //координата Y следующего узла
  end;

  // Create selection list
  //создаем список маркеров выделения если его нет
  if FSelections=nil
    then FSelections:=TList.Create;

  // создаем маркеры выделения create selections
  if FSelections.Count < Length(SelRects) then	//если кол-во маркеров меньше чем кол-во прямоуголькиков
    for i:=FSelections.Count to Length(SelRects) - 1 do	//то создадим недостающие маркеры и добавим их в список
    begin
      Clone:=TD2VisualObject(FSelection.Clone(Self));
      Clone.ResourceName:='';
      FSelections.Add(Clone);
      Clone.Parent:=FSelection.Parent;
      Clone.Stored:=false;
    end;
  // скроем лишние если нужно hide if not need
  if Length(SelRects) < FSelections.Count then 		//если кол-во маркеров больше чем кол-во прямоугольников
    for i:=Length(SelRects) to FSelections.Count - 1 do	//то лишние скроем
      TD2VisualObject(FSelections[i]).Visible:=false;

  // расставим маркеры выделения align selections
  for i:=0 to High(SelRects) do
  begin
    TD2VisualObject(FSelections[i]).Visible := true; //влючим очередной маркер
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left, Top, Right - Left, Bottom - Top); //установим границы маркера как у соотв. прямоугольника
      if (FFocus <> nil) then
      begin
        if FShowSelectedCell and               // если разрешено выделять ячейку
           IsOneRowSelected and                // и выделена только 1 строка
           (FocusedColumn > NoColumn) then       // и есть выделенная колонка
          begin
            FFocus.Visible:=true;
  	      FFocus.BringToFront;
  	      P:=d2Point(Columns[FocusedColumn].Position.X,0);
  	      P:=FContent.LocalToAbsolute(P);
    	      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
  	      P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
            if FShowHorzLines then b:=1 else b:=0;
            if (FShowVertLines) and (FocusedColumn>0) then l:=1 else l:=0;
            with SelRects[i] do                                                                  //установить координаты маркера ячейки
              FFocus.SetBounds(P.X + FFocus.Padding.Left+l, SelRects[i].Top + FFocus.Padding.Top,
                              Columns[FFocusedColumn].Width - (FFocus.Padding.Left + FFocus.Padding.Right)-l,
                              SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom)-b);
          end
        else FFocus.Visible := false;  // иначе скрыть маркер ячейки
    end;
  end;

  //переместим вперед маркер пред.выбора если он включен
    if (FPreSelection <> nil) and (FPreSelection.Visible)
      then FPreSelection.BringToFront;

  //обновим колонки
  for i := 0 to ColumnCount - 1 do
    Columns[i].UpdateSelected;
end;

//function TD2CustomTreeGrid.DoCreateEditor(Node: PD2TreeNode;
//  Column: Integer): ID2VTEditLink;
//begin
//  Result := nil;
//  if Assigned(FOnCreateEditor) then
//    FOnCreateEditor(Self, Node, Column, Result);
//end;

procedure TD2CustomTreeGrid.DoCanEdit(Node: PD2TreeNode;
  Column: Integer; var Allowed: Boolean);
begin
  if Assigned(FOnEditing)
    then FOnEditing(Self, Node, Column, Allowed);
end;

procedure TD2CustomTreeGrid.DoEdit;
//Начать редактирование узла,имеющего фокус
begin
  //Application.CancelHint;
  ////KillTimer(Handle, ScrollTimer);
  ////KillTimer(Handle, EditTimer);
  //DoStateChange([], [tsEditPending]);
  //if Assigned(FFocusedNode) and not (vsDisabled in FFocusedNode.States) and
  //  not (toReadOnly in FOptions.FMiscOptions) and (FEditLink = nil) then
  //begin
  //  FEditLink := DoCreateEditor(FFocusedNode, FEditColumn);
  //  if Assigned(FEditLink) then
  //  begin
  //    DoStateChange([tsEditing], [tsDrawSelecting, tsDrawSelPending, tsToggleFocusedSelection, tsOLEDragPending,
  //      tsOLEDragging, tsClearPending, tsScrollPending, tsScrolling, tsMouseCheckPending]);
  //    ScrollIntoView(FFocusedNode, toCenterScrollIntoView in FOptions.SelectionOptions,
  //      not (toDisableAutoscrollOnEdit in FOptions.AutoOptions));
  //    if FEditLink.PrepareEdit(Self, FFocusedNode, FEditColumn) then
  //    begin
  //      UpdateEditBounds;
  //      // Node needs repaint because the selection rectangle and static text must disappear.
  //      InvalidateNode(FFocusedNode);
  //      if not FEditLink.BeginEdit then
  //        DoStateChange([], [tsEditing]);
  //    end
  //    else
  //      DoStateChange([], [tsEditing]);
  //    if not (tsEditing in FStates) then
  //      FEditLink := nil;
  //  end;
  //end;
end;

function TD2CustomTreeGrid.DoEndEdit: Boolean;
//Закончить редактирование и вызвать предывание OnEdited
begin
  ////KillTimer(Handle, EditTimer);
  Result := (tsEditing in FStates) {and FEditLink.EndEdit};
  if Result then
  begin
    DoStateChange([], [tsEditing]);
    //FEditLink := nil;
    if Assigned(FOnEdited) then
      FOnEdited(Self, FFocusedNode, FEditColumn);
  end;
  DoStateChange([], [tsEditPending]);
end;

procedure TD2CustomTreeGrid.DoEndOperation(OperationKind: TD2TreeOperationKind);
//Вызывает прерывание OnEndOperation (вызывается при окончании длительной операции)
begin
  if Assigned(FOnEndOperation) then
    FOnEndOperation(Self, OperationKind)
end;

procedure TD2CustomTreeGrid.DoEnter;
//Выделить узел при при получении деревом фокуса
begin
  EnsureNodeSelected();
end;

procedure TD2CustomTreeGrid.DoFocusChange(Node: PD2TreeNode; Column: Integer);
//Вызывает прерывание смены фокуса узла
begin
  if Assigned(FOnFocusChanged) then
    FOnFocusChanged(Self, Node, Column);
  {$ifdef EnableAccessible}
  NotifyWinEvent(EVENT_OBJECT_LOCATIONCHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  NotifyWinEvent(EVENT_OBJECT_VALUECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  NotifyWinEvent(EVENT_OBJECT_SELECTION, Handle, OBJID_CLIENT, CHILDID_SELF);
  NotifyWinEvent(EVENT_OBJECT_FOCUS, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}
end;

function TD2CustomTreeGrid.DoFocusChanging(OldNode, NewNode: PD2TreeNode;
  OldColumn, NewColumn: Integer): Boolean;
//Вызывает прерывание перед изменением фокуса узла и/или колонки
begin
  //Result := (OldColumn = NewColumn) or FHeader.AllowFocus(NewColumn);
  if Assigned(FOnFocusChanging) then
    FOnFocusChanging(Self, OldNode, NewNode, OldColumn, NewColumn, Result);
end;

procedure TD2CustomTreeGrid.DoFocusNode(Node: PD2TreeNode; Ask: Boolean);
//Вызывает прерывание при изменениии фокуса узла
begin
  if not (tsEditing in FStates) or EndEditNode then
  begin
    if Node = FRoot then
      Node := nil;
    if (FFocusedNode <> Node) and (not Ask or DoFocusChanging(FFocusedNode, Node, FFocusedColumn, FFocusedColumn)) then
    begin
      if Assigned(FFocusedNode) then
      begin
        //Автоматическое свертывание последнего сфокусированного узла, если включено. Это, однако, выполняется
        //только в том случае, если старый и новый сфокусированный узел имеет общий родительский узел.
        // Do automatic collapsing of last focused node if enabled. This is however only done if
        // old and new focused node have a common parent node.
        if (toAutoExpand in FOptions.FAutoOptions) and Assigned(Node) and (Node.Parent = FFocusedNode.Parent) and
          (vsExpanded in FFocusedNode.States) then
          ToggleNode(FFocusedNode)
        else
          InvalidateNode(FFocusedNode);
      end;
      FFocusedNode := Node;
    end;

    //Необходимо прокручивать узел в представлении, даже если он тот же узел, что и раньше.
    // Have to scroll the node into view, even it is the same node as before.
    if Assigned(FFocusedNode) then
    begin
      //Убедитесь, что допустимый столбец установлен, если используются столбцы, и в настоящий момент ни один столбец не находится в фокусе.
      // Make sure a valid column is set if columns are used and no column has currently the focus.
      //if FHeader.UseColumns and (not FHeader.FColumns.IsValidColumn(FFocusedColumn)) then
      //  FFocusedColumn := FHeader.MainColumn;

      //Делаем автоматическое расширение вновь сфокусированного узла, если он включен.
      // Do automatic expansion of the newly focused node if enabled.
      if (toAutoExpand in FOptions.FAutoOptions) and not (vsExpanded in FFocusedNode.States) then
        ToggleNode(FFocusedNode);
      InvalidateNode(FFocusedNode);
      if (FUpdateCount = 0) and not (toDisableAutoscrollOnFocus in FOptions.FAutoOptions) then
        ScrollIntoView(FFocusedNode, (toCenterScrollIntoView in FOptions.SelectionOptions) and
          (MouseButtonDown * FStates = []), not (toFullRowSelect in FOptions.SelectionOptions) );
    end;

    //При необходимости замените анкер.
    // Reset range anchor if necessary.
    if FSelectionCount = 0 then
      ResetRangeAnchor;
  end;
end;

procedure TD2CustomTreeGrid.DoFreeNode(Node: PD2TreeNode);
//Вызывает прерывание при освобождении узла
begin
  // Prevent invalid references
  if Node = FLastChangedNode then FLastChangedNode := nil;
  //if Node = FCurrentHotNode then FCurrentHotNode := nil;
  if Node = FDropTargetNode then FDropTargetNode := nil;
  if Node = FLastStructureChangeNode then FLastStructureChangeNode := nil;
  if Node = FNextNodeToSelect then FNextNodeToSelect := nil;
  if Self.UpdateCount = 0 then  // Пропустить если дерево в состоянии BeginUpdate для увеличения производительности
  begin
    // Omit this stuff if the control is in a BeginUpdate/EndUpdate bracket to increase performance
    // We now try
    // Make sure that CurrentNode does not point to an invalid node
    // Убедитесь, что CurrentNode не указывает на недопустимую узел
    if (toAlwaysSelectNode in TreeOptions.SelectionOptions) and (Node = GetFirstSelected()) then
    begin
      if Assigned(FNextNodeToSelect) then
        //Выбираем новый узел, если выбранный освобождается. Select a new node if the currently selected node gets freed
        Selected[FNextNodeToSelect] := True
      else
      begin
        FNextNodeToSelect := Self.NodeParent[GetFirstSelected()];
        if Assigned(FNextNodeToSelect) then
          Selected[FNextNodeToSelect] := True;
      end;//else
    end;//if
  end;

  // Вызываем прерывание. fire event
  if Assigned(FOnFreeNode) and
     ([vsInitialized, vsOnFreeNodeCallRequired] * Node.States <> [])
    then FOnFreeNode(Self, Node);
  FreeMem(Node);
  if Self.UpdateCount = 0 then EnsureNodeSelected();
end;

function TD2CustomTreeGrid.DoGetNodeWidth(Node: PD2TreeNode; Column: Integer;
           Canvas: TD2Canvas = nil): Single;
// Возвращает ширину узла. Виртуальный метод переопределяемый в потомках
// Returns the pixel width of a node.

begin
  Result := 0;
end;

function TD2CustomTreeGrid.DoInitChildren(Node: PD2TreeNode;
  var ChildCount: Cardinal): Boolean;
//Вызывает OnInitChildren и возвращает истину, если обработчик события определен;
// The function calls the OnInitChildren and returns True if the event was called;
// it returns False if the caller can expect that no changes have been made to ChildCount
begin
  if Assigned(FOnInitChildren)
    then  begin
            FOnInitChildren(Self, Node, ChildCount);
            Result := True;
          end
    else Result := False;
end;

procedure TD2CustomTreeGrid.DoInitNode(Parent, Node: PD2TreeNode; var InitStates: TD2TreeNodeInitStates);
//Вызов прерывания OnInitNode
begin
  if Assigned(FOnInitNode) then
    FOnInitNode(Self, Parent, Node, InitStates);
end;

procedure TD2CustomTreeGrid.DoLoadUserData(Node: PD2TreeNode; Stream: TStream);
//Вызов прерывания после загрузки узла из потока (FOnLoadNode) для загрузки из потока Stream данных узла Node
begin
  if Assigned(FOnLoadNode) then
    if Node = FRoot
      then FOnLoadNode(Self, nil, Stream)
      else FOnLoadNode(Self, Node, Stream);
end;

procedure TD2CustomTreeGrid.DoMeasureItem(Node: PD2TreeNode; var NodeHeight: Single);
//Вызов прерывания определения высоты узла
begin
  if Assigned(FOnMeasureItem) then
  FOnMeasureItem(Self, Node, NodeHeight);
end;

procedure TD2CustomTreeGrid.DoNodeCopied(Node: PD2TreeNode);
//Вызов прерывания OnNodeCopied (вызывается после копирования узла)
begin
  if Assigned(FOnNodeCopied) then
    FOnNodeCopied(Self, Node);
end;

function TD2CustomTreeGrid.DoNodeCopying(Node, NewParent: PD2TreeNode): Boolean;
//Вызов прерывания OnNodeCopying (вызывается, перед копированием узла на другой родительский узел)
begin
  Result := True;
  if Assigned(FOnNodeCopying) then
    FOnNodeCopying(Self, Node, NewParent, Result);
end;

procedure TD2CustomTreeGrid.DoNodeMoved(Node: PD2TreeNode);
//Вызов прерывания OnNodeMoved после перемещения узла Node к новому родителю
begin
  if Assigned(FOnNodeMoved) then
    FOnNodeMoved(Self, Node);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2CustomTreeGrid.DoNodeMoving(Node, NewParent: PD2TreeNode): Boolean;
//Вызов прерывания OnNodeMoving (вызывается перед перемещением узла Node к новому родителю NewParent)
begin
  Result := True;
  if Assigned(FOnNodeMoving) then
    FOnNodeMoving(Self, Node, NewParent, Result);
end;

procedure TD2CustomTreeGrid.DoRemoveFromSelection(Node: PD2TreeNode);
//Вызов прерывание OnRemoveFromSelection (вызывается когда узел удаляется из списка выбранных)
begin
  if Assigned(FOnRemoveFromSelection) then
    FOnRemoveFromSelection(Self, Node);
end;

procedure TD2CustomTreeGrid.DoReset(Node: PD2TreeNode);
//Вызов прерывания nResetNode (вызывается, когда узел устанавливается как неинициализированный)
begin
  if Assigned(FOnResetNode) then
    FOnResetNode(Self, Node);
end;

procedure TD2CustomTreeGrid.DoSaveUserData(Node: PD2TreeNode;
  Stream: TStream);
//Вызов прерывания OnSaveNode (вызывается при сериализации узла в поток) при записи узла Node в поток Stream
begin
  if Assigned(FOnSaveNode)
    then if Node = FRoot
           then FOnSaveNode(Self, nil, Stream)
           else FOnSaveNode(Self, Node, Stream);
end;

function TD2CustomTreeGrid.DoSetOffsetXY(Value: TD2Point;
  Options: TD2ScrollUpdateOptions{; ClipRect: PRect}): Boolean;
// Фактический установщик смещения, используемый для прокрутки клиентской области, обновления полос прокрутки и аннулирования заголовка (все необязательные).
// Возвращает True, если смещение действительно изменилось, иначе возвращается False.
// Actual offset setter used to scroll the client area, update scroll bars and invalidating the header (all optional).
// Returns True if the offset really changed otherwise False is returned.

var
  DeltaX: Single;
  DeltaY: Single;
  DWPStructure: THandle;//HDWP;
  I: Integer;
  P: TD2Point;
  R: TD2Rect;

begin
  {$ifdef DEBUG_VTV}Logger.EnterMethod([lcScroll],'DoSetOffsetXY');{$endif}
  {$ifdef DEBUG_VTV}Logger.Send([lcScroll],'Value',Value);{$endif}
  //{$ifdef DEBUG_VTV}Logger.SendCallStack([lcScroll],'CallStack');{$endif}
  // Range check, order is important here.
  if Value.X < (ClientWidth - FRangeX)
    then Value.X := ClientWidth - FRangeX;
  if Value.X > 0
    then Value.X := 0;
  DeltaX := Value.X - HScrollBarValue;
  //if UseRightToLeftAlignment then
  //  DeltaX := -DeltaX;
  if Value.Y < (ClientHeight - FRangeY)
    then Value.Y := ClientHeight - FRangeY;
  if Value.Y > 0
    then Value.Y := 0;
  DeltaY := Value.Y - VScrollBarValue;
  {$ifdef DEBUG_VTV}Logger.Send([lcScroll],'FOffsetX: %d FOffsetY: %d',[FOffsetX,FOffsetY]);{$endif}
  {$ifdef DEBUG_VTV}Logger.Send([lcScroll],'DeltaX: %d DeltaY: %d',[DeltaX,DeltaY]);{$endif}
  Result := (DeltaX <> 0) or (DeltaY <> 0);
  if Result then
  begin
    HScrollBar.Value := Value.X;
    VScrollBar.Value := Value.Y;
    if tsHint in Self.FStates
      then Application.CancelHint;

    if FUpdateCount = 0 then
    begin
      ScrollTo(DeltaX, DeltaY);
      Realign;
      //остатки от родной функции VirtualTree
      //// The drag image from VCL controls need special consideration.
      //{$ifndef INCOMPLETE_WINAPI}
      //if tsVCLDragging in FStates then
      //  ImageList_DragShowNolock(False);
      //{$endif}
      //
      //if (suoScrollClientArea in Options) and not (tsToggling in FStates) then
      //begin
      //  // Have to invalidate the entire window if there's a background.
      //  if (toShowBackground in FOptions.FPaintOptions) and (FBackground.Graphic is TBitmap) then
      //  begin
      //    //todo: reimplement
      //    {$ifndef INCOMPLETE_WINAPI}
      //    // Since we don't use ScrollWindow here we have to move all client windows ourselves.
      //    DWPStructure := BeginDeferWindowPos(ControlCount);
      //    for I := 0 to ControlCount - 1 do
      //      if Controls[I] is TWinControl then
      //      begin
      //        with Controls[I] as TWinControl do
      //          DWPStructure := DeferWindowPos(DWPStructure, Handle, 0, Left + DeltaX, Top + DeltaY, 0, 0,
      //            SWP_NOZORDER or SWP_NOACTIVATE or SWP_NOSIZE);
      //        if DWPStructure = 0 then
      //          Break;
      //      end;
      //    if DWPStructure <> 0 then
      //      EndDeferWindowPos(DWPStructure);
      //    InvalidateRect(Handle, nil, False);
      //    {$endif}
      //  end
      //  else
      //  begin
      //    if (DeltaX <> 0) and (GetVisibleFixedWidth > 0) then
      //    begin
      //      // When fixed columns exists we have to scroll separately horizontally and vertically.
      //      // Horizontally is scroll only the client area not occupied by fixed columns and
      //      // vertically entire client area (or clipping area if one exists).
      //      R := ClientRect;
      //      R.Left := GetVisibleFixedWidth;
      //      //lclheader
      //      if hoVisible in FHeader.FOptions then
      //      begin
      //        Inc(R.Top,FHeader.Height);
      //        Inc(R.Bottom,FHeader.Height);
      //      end;
      //      //scrollwindow implementation under gtk is broken
      //      {$ifdef Gtk}
      //      InvalidateRect(Handle, nil, True);
      //      {$else}
      //      ScrollWindow(Handle, DeltaX, 0, @R, @R);
      //      if DeltaY <> 0 then
      //        ScrollWindow(Handle, 0, DeltaY, @R, @R);
      //      {$endif}
      //    end
      //    else
      //    begin
      //      //lclheader
      //      if ClipRect <> nil then
      //      begin
      //        {$ifdef DEBUG_VTV}Logger.SendWarning([lcWarning], 'DoSetOffsetXY called with a non nil ClipRect');{$endif}
      //        R := ClipRect^;
      //      end
      //      else
      //        R := ClientRect;
      //      if hoVisible in FHeader.FOptions then
      //      begin
      //        Inc(R.Top, FHeader.Height);
      //        Inc(R.Bottom, FHeader.Height);
      //      end;
      //      {$ifdef DEBUG_VTV}Logger.Send([lcScroll], 'Rect to Scroll', R);{$endif}
      //      //todo: temporary hack to avoid some drawing problems.
      //      //Will be removed when scrollwindowex is properly implemented in all widgets
      //      {$ifdef LCLQt}
      //      ScrollWindow(Handle, DeltaX, DeltaY, @R, @R);
      //      {$else}
      //      {$ifdef Gtk}
      //      InvalidateRect(Handle, nil, True);
      //      {$else}
      //      ScrollWindowEx(Handle, DeltaX, DeltaY, @R, @R,0, nil, SW_INVALIDATE or SW_SCROLLCHILDREN);
      //      {$endif}
      //      {$endif}
      //    end;
      //  end;
      //end;
      //
      //if suoUpdateNCArea in Options then
      //begin
      //  if DeltaX <> 0 then
      //  begin
      //    if (suoRepaintHeader in Options) and (hoVisible in FHeader.FOptions) then
      //      FHeader.Invalidate(nil);
      //    if not (tsSizing in FStates) and (FScrollBarOptions.ScrollBars in [ssHorizontal, ssBoth]) then
      //      UpdateHorizontalScrollBar(suoRepaintScrollBars in Options);
      //  end;
      //
      //  if (DeltaY <> 0) and ([tsThumbTracking, tsSizing] * FStates = []) then
      //  begin
      //    UpdateVerticalScrollBar(suoRepaintScrollBars in Options);
      //    if not (FHeader.UseColumns or IsMouseSelecting) and
      //      (FScrollBarOptions.ScrollBars in [ssHorizontal, ssBoth]) then
      //      UpdateHorizontalScrollBar(suoRepaintScrollBars in Options);
      //  end;
      //end;
      //{$ifndef INCOMPLETE_WINAPI}
      //if tsVCLDragging in FStates then
      //  ImageList_DragShowNolock(True);
      //{$endif}
    end;

    //// Finally update "hot" node if hot tracking is activated
    //GetCursorPos(P);
    //P := ScreenToClient(P);
    //if PtInRect(ClientRect, P) then
    //  HandleHotTrack(P.X, P.Y);
    //
    //DoScroll(DeltaX, DeltaY);
    //Perform(CM_UPDATE_VCLSTYLE_SCROLLBARS,0,0);
  end;
  {$ifdef DEBUG_VTV}Logger.ExitMethod([lcScroll],'DoSetOffsetXY');{$endif}
end;

procedure TD2CustomTreeGrid.DoStartOperation(OperationKind: TD2TreeOperationKind);
//Вызов прерывания OnStartOperation
begin
  if Assigned(FOnStartOperation) then
    FOnStartOperation(Self, OperationKind);
end;

procedure TD2CustomTreeGrid.DoStateChange(Enter: TD2TreeStates; Leave: TD2TreeStates);
//Изменяет текущие флаги состояния дерева: Enter - добавляемые, Leave - исключаемые
var
  ActualEnter,
  ActualLeave: TD2TreeStates;
begin
  if Assigned(FOnStateChange) then
  begin
    ActualEnter := Enter - FStates;
    ActualLeave := FStates * Leave;
    if (ActualEnter + ActualLeave) <> []
      then FOnStateChange(Self, Enter, Leave);
  end;
  FStates := FStates + Enter - Leave;
end;

procedure TD2CustomTreeGrid.DoStructureChange(Node: PD2TreeNode;
  Reason: TD2ChangeReason);
//Вызов прерывания OnStructureChange при изменении структуры дерева
begin
  //if HandleAllocated then
  //  KillTimer(Handle, StructureChangeTimer);
  if Assigned(FOnStructureChange) then
    FOnStructureChange(Self, Node, Reason);

  // Это хорошее место для сброса кэшированных узлов и причина. Эти же значения здесь.
  // Это необходимо, чтобы потомки, чтобы переопределить этот метод и получить их.
  // This is a good place to reset the cached node and reason. These are the same as the values passed in here.
  // This is necessary to allow descendants to override this method and get them.
  DoStateChange([], [tsStructureChangePending]);
  FLastStructureChangeNode := nil;
  FLastStructureChangeReason := crIgnore;
end;

function TD2CustomTreeGrid.DoValidateCache: Boolean;
//Этот метод заполняет кеш, который используется для ускорения поиска узлов.
//Стратегия проста: взять текущее количество видимых узлов и равномерно
//распределить количество меток (которые хранятся в FPositionCache), так чтобы
//итерация через дерево не занимала слишком много времени.
//Если в дереве меньше узлов чем значение CacheThreshold, то кеш остается пустым.
//Результат True, если кеш был заполнен без перерыва, иначе False.
//Примечание. Вы можете настроить максимальное количество узлов между двумя
//            элементами кэша, изменив CacheThreshold.

// This method fills the cache, which is used to speed up searching for nodes.
// The strategy is simple: Take the current number of visible nodes and distribute evenly a number of marks
// (which are stored in FPositionCache) so that iterating through the tree doesn't cost too much time.
// If there are less than 'CacheThreshold' nodes in the tree then the cache remains empty.
// Result is True if the cache was filled without interruption, otherwise False.
// Note: You can adjust the maximum number of nodes between two cache entries by changing CacheThreshold.

var
  CurrentTop: Single;
  EntryCount,
  Index: Cardinal;
  CurrentNode,
  Temp: PD2TreeNode;

begin
  EntryCount := 0;
  if not (tsStopValidation in FStates) then
  begin
    if FStartIndex = 0 then
      FPositionCache := nil;

    EntryCount := CalculateCacheEntryCount;
    SetLength(FPositionCache, EntryCount);
    if FStartIndex > EntryCount then
      FStartIndex := EntryCount;

    // Optimize validation by starting with FStartIndex if set.
    if (FStartIndex > 0) and Assigned(FPositionCache[FStartIndex - 1].Node) then
    begin
      // Index is the current entry in FPositionCache.
      Index := FStartIndex - 1;
      // Running term for absolute top value.
      CurrentTop := FPositionCache[Index].AbsoluteTop;
      // Running node pointer.
      CurrentNode := FPositionCache[Index].Node;
    end
    else
    begin
      // Index is the current entry in FPositionCache.
      Index := 0;
      // Running term for absolute top value.
      CurrentTop := 0;
      // Running node pointer.
      CurrentNode := GetFirstVisibleNoInit(nil, True);
    end;

    // EntryCount serves as counter for processed nodes here. This value can always start at 0 as
    // the validation either starts also at index 0 or an index which is always a multiple of CacheThreshold
    // and EntryCount is only used with modulo CacheThreshold.
    EntryCount := 0;
    if Assigned(CurrentNode) then
    begin
      while not (tsStopValidation in FStates) do
      begin
        // If the cache is full then stop the loop.
        if (Integer(Index) > Length(FPositionCache)) then    // ADDED: 17.09.2013 - Veit Zimmermann
          Break;                                             // ADDED: 17.09.2013 - Veit Zimmermann
        if (EntryCount mod CacheThreshold) = 0 then
        begin
          // New cache entry to set up.
          with FPositionCache[Index] do
          begin
            Node := CurrentNode;
            AbsoluteTop := CurrentTop;
          end;
          Inc(Index);
        end;

        d2IncFloat(CurrentTop, NodeHeight[CurrentNode]);
        // Advance to next visible node.
        Temp := GetNextVisibleNoInit(CurrentNode, True);
        // If there is no further node then stop the loop.
        if (Temp = nil) then       // CHANGED: 17.09.2013 - Veit Zimmermann
          Break;                   // CHANGED: 17.09.2013 - Veit Zimmermann

        CurrentNode := Temp;
        Inc(EntryCount);
      end;
    end;
    // Finalize the position cache so no nil entry remains there.
    if not (tsStopValidation in FStates) and (Integer(Index) <= High(FPositionCache)) then
    begin
      SetLength(FPositionCache, Index + 1);
      with FPositionCache[Index] do
      begin
        Node := CurrentNode;
        AbsoluteTop := CurrentTop;
      end;
    end;
  end;

  Result := (EntryCount > 0) and not (tsStopValidation in FStates);

  // In variable node height mode it might have happend that some or all of the nodes have been adjusted in their
  // height. During validation updates of the scrollbars is disabled so let's do this here.
  if Result and (toVariableNodeHeight in FOptions.FMiscOptions) then
  begin
    Realign; //UpdateScrollBars(True);
  end;
end;

procedure TD2CustomTreeGrid.DragEnter(const Data: TD2DragObject;
  const Point: TD2Point);
begin
  inherited;
  UpdateSelection;
end;

procedure TD2CustomTreeGrid.DragLeave;
begin
  inherited;
  UpdateSelection;
end;

procedure TD2CustomTreeGrid.DragDrop(const Data: TD2DragObject;
  const Point: TD2Point);
begin
  inherited;
  if (FPreSelection<>nil) and (FPreSelection is TD2Rectangle) then
    begin
      TD2Rectangle(FPreSelection).Sides:=[];
      TD2Rectangle(FPreSelection).Stroke.Style:= d2BrushNone;
      UpdateSelection;
    end;
end;

procedure TD2CustomTreeGrid.DragOver(const Data: TD2DragObject;
  Shift: TShiftState; const Point: TD2Point; var Accept: Boolean);
var
  T: Single;
  N: integer;
  H: Single;
  P: TD2Point;
begin
  Accept:=false;            //разрешение отсутствует
  FLastDropMode:=dmNowhere; //режим падения - никуда
  if (FIsDragOver) and (FContentLayout<>nil) and          //если курсор над гридом и есть объект контента и
     (FContentLayout.PointInObject(Point.X,Point.Y)) then //курсор мыши над ним
    begin
      P:=FContentLayout.AbsoluteToLocal(Point);           //преобразуем координаты мыши в локальные
      FDropTargetNode:=GetNodeAt(P.X,P.Y,true,T,N); //найдем целевой узел

      if Assigned(FDropTargetNode) then  //если целевой узел найден
        begin
          //if (GetTopNode=FDropTargetNode) //если целевой узел является верхним
          //  then StartDragScroll(scLineUp);  //то запустим отложенную прокрутку вверх
          //  else if (GetBottomNode=FDropTargetNode) //иначе если целевой узел является нижним
          //         then StartDragScroll(scLineDown);          //то запустим отложенную прокрутку вниз
          H:=FDropTargetNode.NodeHeight/3;            //найдем 1/3 высоты узла
          if (P.Y <= T + H)                           //если курсор в верхней трети узла
            then FLastDropMode:=dmAbove                 //то режим - "выше узла"
            else if (P.Y >= T + 2*H)                    //иначе если курсор в нижней трети узла
                   then FLastDropMode:=dmBelow                   //то режим - "ниже узла"
                   else FLastDropMode:=dmOnNode;                 //иначе режим - "на узел" (курсор в середине узла)
        end;
    end
    else FDropTargetNode:=nil;   //иначе целового узла нет (или контента нет или курсор вне его)

  if Assigned(OnDragOver) then
    OnDragOver(Self,Data,Shift,Point,FDropTargetNode,FLastDropMode,Accept);

  if (FPreSelection<>nil) and (FPreSelection is TD2Rectangle) then
    case FLastDropMode of
      dmNowhere: begin
                   TD2Rectangle(FPreSelection).Sides:=[];
                   TD2Rectangle(FPreSelection).Stroke.Style:= d2BrushNone;
                   UpdateSelection;
                 end;
        dmAbove: begin
                   TD2Rectangle(FPreSelection).Sides:=[d2SideTop];
                   TD2Rectangle(FPreSelection).Corners:=[d2CornerTopLeft,d2CornerTopRight];
                   TD2Rectangle(FPreSelection).Stroke.Style:= d2BrushSolid;
                   TD2Rectangle(FPreSelection).Stroke.SolidColor:=$FF3752FD;;
                   TD2Rectangle(FPreSelection).StrokeThickness:=4;
                   UpdateSelection;
                 end;
        dmBelow: begin
                   TD2Rectangle(FPreSelection).Sides:=[d2SideBottom];
                   TD2Rectangle(FPreSelection).Corners:=[d2CornerBottomLeft,d2CornerBottomRight];
                   TD2Rectangle(FPreSelection).Stroke.Style:= d2BrushSolid;
                   TD2Rectangle(FPreSelection).Stroke.SolidColor:=$FF3752FD;
                   TD2Rectangle(FPreSelection).StrokeThickness:=5;
                   UpdateSelection;
                 end;
       dmOnNode: begin
                   TD2Rectangle(FPreSelection).Sides:= AllSides;
                   TD2Rectangle(FPreSelection).Corners:=AllCorners;
                   TD2Rectangle(FPreSelection).Stroke.Style:= d2BrushSolid;
                   TD2Rectangle(FPreSelection).Stroke.SolidColor:=$FF3752FD;
                   TD2Rectangle(FPreSelection).StrokeThickness:=7;
                   UpdateSelection;
                 end;
    end;

end;

procedure TD2CustomTreeGrid.EndOperation(OperationKind: TD2TreeOperationKind);
//Вызывается для индикации завершения длительной операции.
// Called to indicate that a long-running operation has finished.

begin
  Assert(FOperationCount > 0, 'EndOperation must not be called when no operation in progress.');
  Dec(FOperationCount);
  DoEndOperation(OperationKind);
end;

function TD2CustomTreeGrid.FindNodeInSelection(P: PD2TreeNode;
  var Index: Integer; LowBound, HighBound: Integer): Boolean;
//Поиск узла P в массиве выбора. LowBound и HighBound нижняя и верхняя границы диапазона поиска.
//LowBound = -1 или HighBound = -1 - максимальный диапазон, иначе нужно LowBound <= HighBound.
// Search routine to find a specific node in the selection array.
// LowBound and HighBound determine the range in which to search the node.
// Either value can be -1 to denote the maximum range otherwise LowBound must be less or equal HighBound.
var L, H, I: PtrInt;
begin
  Result := False;
  if LowBound >= 0
    then L := LowBound
    else L := 0;
  if HighBound >= 0
    then H := HighBound
    else H := FSelectionCount - 1;

  while L <= H do
  begin
    I := (L + H) shr 1;
    if PAnsiChar(FSelectionNodes[I]) < PAnsiChar(P)
      then L := I + 1
      else  begin
              H := I - 1;
              if FSelectionNodes[I] = P then
              begin
                Result := True;
                L := I;
              end;
            end;
  end;
  Index := L;
end;

procedure TD2CustomTreeGrid.FinishChunkHeader(Stream: TStream; StartPos,
  EndPos: Integer);
//Используется при потоковой передаче узла для завершающей записи размер блока
// used while streaming out a node to finally write out the size of the chunk
var Size: Integer;
begin
  Stream.Position := StartPos + SizeOf(Size);  //переместить ко второму входу в заголовке блока. seek back to the second entry in the chunk header
  Size := EndPos - StartPos - SizeOf(TD2ChunkHeader); //определить размер (Size) блока без заголовка. determine size of chunk without the chunk header
  Stream.Write(Size, SizeOf(Size)); //запись  размер. write the size...
  Stream.Position := EndPos; // ... и переместься к последнему конечному положению. and seek to the last endposition
end;

function TD2CustomTreeGrid.GetContentHeight: Single;
begin
  //UpdateVerticalRange;
  Result:=FRangeY;
end;

function TD2CustomTreeGrid.GetOptionsClass: TD2TreeOptionsClass;
//Получить класс опций дерева. Может быть переопределено у потомков
begin
  Result := TD2TreeOptions;
end;

function TD2CustomTreeGrid.GetOperationCanceled: Boolean;
//Получить состояния флага необходимости завершения длительной операции: true - операция должна быть завершена
begin
  Result := FOperationCanceled and (FOperationCount > 0);
end;

function TD2CustomTreeGrid.GetTopRow: integer;
begin
  Result:=FTopRow;
end;

function TD2CustomTreeGrid.GetValue(Node: PD2TreeNode; Col: integer): Variant;
begin
  Result:=NULL;
  if Assigned(FOnGetValue) then
    FOnGetValue(Self, Node, Col, Result);
end;

procedure TD2CustomTreeGrid.SetValue(Node: PD2TreeNode; Col: integer;
  const Value: Variant);
begin
    if Assigned(FOnSetValue) then
    FOnSetValue(Self, Node, Col, Value);
end;

procedure TD2CustomTreeGrid.InitChildren(Node: PD2TreeNode);
//Инициализация дочерних узлов для узла Node.
// Initiates the initialization of the child number of the given node.
var Count: Cardinal;
begin
  if Assigned(Node) and (Node <> FRoot) and (vsHasChildren in Node.States) then
  begin
    Count := Node.ChildCount;
    if DoInitChildren(Node, Count) then
    begin
      SetChildCount(Node, Count);
      if Count = 0 then
        Exclude(Node.States, vsHasChildren);
    end;
  end;
end;

procedure TD2CustomTreeGrid.InitNode(Node: PD2TreeNode);
//Инициализация узла Node для обеспечения загрузки приложением его данных.
// Initiates the initialization of the given node to allow the application to load needed data for it.
var InitStates: TD2TreeNodeInitStates;
begin
  with Node^ do
  begin
    InitStates := [];
    if vsInitialized in States                 //если узел уже инициализирован,
      then Include(InitStates, ivsReInit);       //то переинициализировать
    Include(States, vsInitialized);            //установить статус узла "инициализирован"
    if Parent = FRoot                          //если родитель Root
      then DoInitNode(nil, Node, InitStates)     //то выполнить прерывание OnInitNode без родителя
      else DoInitNode(Parent, Node, InitStates); //иначе выполнить прерывание OnInitNode с родителем

    if ivsDisabled in InitStates          //если узел отключен
      then Include(States, vsDisabled);     //то добавим флаг в статус
    if ivsHasChildren in InitStates       //если у узла есть дети
      then Include(States, vsHasChildren);   //то добавим флаг в статус
    if ivsSelected in InitStates then     //если узел выбран
    begin                                    //то добавим узел к списку выбранных
      FSingletonNodeArray[0] := Node;
      InternalAddToSelection(FSingletonNodeArray, 1, False);
    end;
    if ivsMultiline in InitStates then  //если многострочный
      Include(States, vsMultiline);       //то добавим флаг в статус
    if ivsFiltered in InitStates then   //если отфилльтрованный, то
    begin
      Include(States, vsFiltered);        //добавить фильтрацию в статус узла
      if not (toShowFilteredNodes in FOptions.FPaintOptions) then   //если не установлен флаг отключения фильтра, то
      begin
        AdjustTotalHeight(Node, -NodeHeight, True);  // установим в 0 общую высоту узла и уменьшаем общую высоту его родителей
        if FullyVisible[Node] then
          Dec(FVisibleCount);
        //UpdateScrollBars(True);
        Realign; //вместо UpdateScrollBars(True)
      end;
    end;

    // Expanded may already be set (when called from ReinitNode) or be set in DoInitNode, allow both.
    if (vsExpanded in Node.States) xor (ivsExpanded in InitStates) then
    begin
      // Expand node if not yet done (this will automatically initialize child nodes).
      if ivsExpanded in InitStates
        then ToggleNode(Node)
        else
          // If the node already was expanded then explicitly trigger child initialization.
          if vsHasChildren in Node.States
            then InitChildren(Node);
    end;
  end;
end;

procedure TD2CustomTreeGrid.InternalAddFromStream(Stream: TStream;
  Version: Integer; Node: PD2TreeNode);
// Загружает все детали узла Node (в том числе его детей) из потока Stream.
// Поскольку новые узлы могут быть выбраны этот метод также фиксирует массив выбора.
// Loads all details for Node (including its children) from the given stream.
// Because the new nodes might be selected this method also fixes the selection array.
var
  Stop: PD2TreeNode;
  Index: Integer;
  LastTotalHeight: Single;
  WasFullyVisible: Boolean;

begin
  Assert(Node <> FRoot, 'The root node cannot be loaded from stream.');

  // Keep the current total height value of Node as it has already been applied
  // but might change in the load and fixup code. We have to adjust that afterwards.
  LastTotalHeight := Node.TotalHeight;
  WasFullyVisible := FullyVisible[Node] and not IsEffectivelyFiltered[Node];

  // Read in the new nodes.
  ReadNode(Stream, Version, Node);

  // One time update of node-internal states and the global visibility counter.
  // This is located here to ease and speed up the loading process.
  FixupTotalCount(Node);
  AdjustTotalCount(Node.Parent, Node.TotalCount - 1, True); // -1 because Node itself was already set.
  FixupTotalHeight(Node);
  AdjustTotalHeight(Node.Parent, Node.TotalHeight - LastTotalHeight, True);

  // New nodes are always visible, so the visible node count has been increased already.
  // If Node is now invisible we have to take back this increment and don't need to add any visible child node.
  if not FullyVisible[Node] or IsEffectivelyFiltered[Node] then
  begin
    if WasFullyVisible then
      Dec(FVisibleCount);
  end
  else
    // It can never happen that the node is now fully visible but was not before as this would require
    // that the visibility state of one of its parents has changed, which cannot happen during loading.
    Inc(FVisibleCount, CountVisibleChildren(Node));

  // Fix selection array.
  ClearTempCache;
  if Node = FRoot then
    Stop := nil
  else
    Stop := Node.NextSibling;

  if toMultiSelect in FOptions.FSelectionOptions then
  begin
    // Add all nodes which were selected before to the current selection (unless they are already there).
    while Node <> Stop do
    begin
      if (vsSelected in Node.States) and not FindNodeInSelection(Node, Index, 0, High(FSelectionNodes)) then
        InternalCacheNode(Node);
      Node := GetNextNoInit(Node);
    end;
    if FTempNodeCount > 0 then
      AddToSelection(FTempNodeCache, FTempNodeCount, True);
    ClearTempCache;
  end
  else // No further selected nodes allowed so delete the corresponding flag in all new nodes.
    while Node <> Stop do
    begin
      Exclude(Node.States, vsSelected);
      Node := GetNextNoInit(Node);
    end;
end;

function TD2CustomTreeGrid.InternalAddToSelection(Node: PD2TreeNode;
  ForceInsert: Boolean): Boolean;
// Внутренняя версия метода AddToSelection, не вызвающая событие OnChange
begin
  Assert(Assigned(Node), 'Node must not be nil!'); //прервать выполение если Node=Nil
  FSingletonNodeArray[0] := Node;
  Result := InternalAddToSelection(FSingletonNodeArray, 1, ForceInsert);  //передать выполнение во внутреннюю процедуру
end;

procedure TD2CustomTreeGrid.InternalDisconnectNode(Node: PD2TreeNode;
  KeepFocus: Boolean; Reindex: Boolean);
// Отключает узел Node от его родителя и братьев и сестер. Указатель на узел не
// сбрасываются и он по-прежнему может быть использован после возврата из прцедуры
// (вероятно, только очень короткое время!).
// Если KeepFocus = True, то узел сохраняет фокус. Это полезно, если этот узел вновь
// подсоединяется к дереву сразу после возврата из прцедуры и он должен оставаться
// сфокусированным, если это было раньше.
// Внимание! Node не должен быть Nil или корневым узлом.

// Disconnects the given node from its parent and siblings. The node's pointer
// are not reset so they can still be used after return from this method (probably a very short time only!).
// If KeepFocus is True then the focused node is not reset. This is useful if the given node is reconnected to the tree
// immediately after return of this method and should stay being the focused node if it was it before.
// Note: Node must not be nil or the root node.

var
  Parent,
  Run: PD2TreeNode;
  Index: Integer;
  AdjustHeight: Boolean;

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Node must neither be nil nor the root node.');

  if (Node = FFocusedNode) and not KeepFocus then
  begin
    DoFocusNode(nil, False);
    DoFocusChange(FFocusedNode, FFocusedColumn);
  end;

  if Node = FRangeAnchor then
    ResetRangeAnchor;

  // Update the hidden children flag of the parent.
  if (Node.Parent <> FRoot) and not (vsClearing in Node.Parent.States) then
    if FUpdateCount = 0 then
      DetermineHiddenChildrenFlag(Node.Parent)
    else
      Include(FStates, tsUpdateHiddenChildrenNeeded);

  if not (vsDeleting in Node.States) then
  begin
    // Some states are only temporary so take them out.
    Node.States := Node.States - [vsChecking];
    Parent := Node.Parent;
    Dec(Parent.ChildCount);
    AdjustHeight := (vsExpanded in Parent.States) and (vsVisible in Node.States);
    if Parent.ChildCount = 0 then
    begin
      Parent.States := Parent.States - [vsAllChildrenHidden, vsHasChildren];
      if (Parent <> FRoot) and (vsExpanded in Parent.States) then
        Exclude(Parent.States, vsExpanded);
    end;
    AdjustTotalCount(Parent, -Integer(Node.TotalCount), True);
    if AdjustHeight then
      AdjustTotalHeight(Parent, -Integer(Node.TotalHeight), True);
    if FullyVisible[Node] then
      Dec(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));

    if Assigned(Node.PrevSibling) then
      Node.PrevSibling.NextSibling := Node.NextSibling
    else
      Parent.FirstChild := Node.NextSibling;

    if Assigned(Node.NextSibling) then
    begin
      Node.NextSibling.PrevSibling := Node.PrevSibling;
      // Reindex all following nodes.
      if Reindex then
      begin
        Run := Node.NextSibling;
        Index := Node.Index;
        while Assigned(Run) do
        begin
          Run.Index := Index;
          Inc(Index);
          Run := Run.NextSibling;
        end;
      end;
    end
    else
      Parent.LastChild := Node.PrevSibling;
  end;
end;

function TD2CustomTreeGrid.InternalGetNodeAt(X, Y: Single): PD2TreeNode;
//Получить узел по координатам (внутренняя версия функции GetNodeAt).
//X и Y задаются в координатах клиентской области
var
  Dummy: Single;
  DummyI: Integer;
begin
  Result := InternalGetNodeAt(X, Y, True, Dummy, DummyI);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2CustomTreeGrid.GetNodeAtTree(Row: integer): PD2TreeNode;
//Получить узел по номеру Row видимой строки (перегруженный вариант  функции GetNodeAtTree).
var
  Num: integer;
begin
  Result:=nil;
  if (Row < 0) or (Row > VisibleCount-1) then exit;

  Result := GetFirstVisibleNoInit(nil, True);
  Num := 0;
  while (Num < Row) and (Result <> nil) do
  begin
    Result := GetNextVisibleNoInit(nil, True);
    inc(Num);
  end;
end;

function TD2CustomTreeGrid.InternalGetNodeAt(X, Y: Single; Relative: Boolean;
  var NodeTop: Single; var NodeNum: integer): PD2TreeNode;
//Получить узел по координатам (внутренняя версия функции GetNodeAt).
//При Relative = True  X и Y задаются в координатах клиентской области ,
//иначе в абсолютных координатах всего виртуального дерева (без смещения в окне дерева).
//NodeTop получает значение Position.Y возвращаемого узла или не изменняется если узел не найден

//lclheader this is the original version of GetNodeAt used internally since expects coordinates
//  relative to the image tree. In LCL the image tree and control coordinates are different
//  when header is visible

// This method returns the node that occupies the specified point, or nil if there's none.
// If Relative is True then X and Y are given in client coordinates otherwise they are considered as being
// absolute values into the virtual tree image (regardless of the current offsets in the tree window).
// NodeTop gets the absolute or relative top position of the node returned or is untouched if no node
// could be found.

var
  AbsolutePos,
  CurrentPos: Single;
begin
  AbsolutePos := Y;
  if (not Relative) and (AbsolutePos < 0)
     then AbsolutePos := 0;

  if Relative
     then d2IncFloat(AbsolutePos, VScrollBarValue); //было then d2IncFloat(AbsolutePos, -VScrollBarValue);

  // CurrentPos отслеживает текущую позиции для проверки.
  // Он всегда соответствует верхней позиции текущего рассматриваемого узла
  // CurrentPos tracks a running term of the current position to test for.
  // It corresponds always to the top position of the currently considered node.
  CurrentPos := 0;
  NodeNum := -1;
  // Использовать кеш, если доступен.
  // If the cache is available then use it.
  if tsUseCache in FStates
     then Result := FindInPositionCache(AbsolutePos, CurrentPos)
     else Result := GetFirstVisibleNoInit(nil, True);

  // Определим узел, положение и высота которого наиболее точно соответствуют позиции прокрутки.
  // Determine node, of which position and height corresponds to the scroll position most closely.
  while Assigned(Result) and (Result <> FRoot) do
  begin
    if AbsolutePos < (CurrentPos + NodeHeight[Result])
       then Break;
    d2IncFloat(CurrentPos, NodeHeight[Result]);
    inc(NodeNum);
    Result := GetNextVisibleNoInit(Result, True);
  end;
  if Result = FRoot
     then Result := nil;

  //Возвращаем коородинату Y найденного узла, т.к. она вероятно не совпадает с заданной Y
  // Since the given vertical position is likely not the same as the top position
  // of the found node this top position is returned.
  if Assigned(Result) then
  begin
    NodeTop := CurrentPos;
    if Relative then
      d2DecFloat(NodeTop, VScrollBarValue); //было d2IncFloat(NodeTop, VScrollBarValue);
    //{$ifdef DEBUG_VTV}Logger.Send([lcPaintHeader],'GetNodeAt Result: ',Result^.Index);{$endif}
  end;
end;

procedure TD2CustomTreeGrid.InternalRemoveFromSelection(Node: PD2TreeNode);
//Внутренняя версия метода RemoveFromSelection для удаления узла Node из массива выбранных.
//Для окончательного удаления узла необходимо использовать функцию PackArray.
// Special version to mark a node to be no longer in the current selection. PackArray must
// be used to remove finally those entries.

var Index: Integer;
begin
  //Т.к. указатели всегда выровнены по DWORD, то мы можем просто увеличить все те,
  //которые мы хотим удалить (см. также PackArray), и сохранить порядок в списке.
  // Because pointers are always DWORD aligned we can simply increment all those
  // which we want to have removed (see also PackArray) and still have the
  // order in the list preserved.
  if FindNodeInSelection(Node, Index, -1, -1) then
  begin
    Exclude(Node.States, vsSelected);
    Inc(PAnsiChar(FSelectionNodes[Index]));
    DoRemoveFromSelection(Node);
    AdviseChangeEvent(False, Node, crIgnore);
  end;
end;

procedure TD2CustomTreeGrid.RemoveFromSelection(Node: PD2TreeNode);
//Удаляет узел Node из массива выбранных узлов
var Index: Integer;
begin
  if not FSelectionLocked then
  begin
    Assert(Assigned(Node), 'Node must not be nil!');
    if vsSelected in Node.States then
    begin
      Exclude(Node.States, vsSelected);
      if FindNodeInSelection(Node, Index, -1, -1) and (Index < FSelectionCount - 1) then
        Move(FSelectionNodes[Index + 1], FSelectionNodes[Index], (FSelectionCount - Index - 1) * SizeOf(Pointer));
      if FSelectionCount > 0 then
        Dec(FSelectionCount);
      SetLength(FSelectionNodes, FSelectionCount);

      if FSelectionCount = 0 then
        ResetRangeAnchor;

      if FSelectionCount <= 1 then
        UpdateNextNodeToSelect(Node);

      DoRemoveFromSelection(Node);
      Change(Node);
    end;
  end;
end;

function TD2CustomTreeGrid.InternalAddToSelection(const NewItems: TD2NodeArray;
  NewLength: Integer; ForceInsert: Boolean): Boolean;
// Внутренняя версия метода AddToSelection, не вызвающая событие OnChange
// Internal version of method AddToSelection which does not trigger OnChange events

var
  I, J: Integer;
  CurrentEnd: Integer;
  Constrained,
  SiblingConstrained: Boolean;

begin
  // The idea behind this code is to use a kind of reverse merge sort. QuickSort is quite fast
  // and would do the job here too but has a serious problem with already sorted lists like FSelectionNodes.

  // 1) Remove already selected items, mark all other as being selected.
  if ForceInsert then
  begin
    for I := 0 to NewLength - 1 do
    begin
      Include(NewItems[I].States, vsSelected);
      if Assigned(FOnAddToSelection) then
        FOnAddToSelection(Self, NewItems[I]);
    end;
  end
  else
  begin
    Constrained := toLevelSelectConstraint in FOptions.FSelectionOptions;
    if Constrained and (FLastSelectionLevel = -1) then
      FLastSelectionLevel := GetNodeLevel(NewItems[0]);
    SiblingConstrained := toSiblingSelectConstraint in FOptions.FSelectionOptions;
    if SiblingConstrained and (FRangeAnchor = nil) then
      FRangeAnchor := NewItems[0];

    for I := 0 to NewLength - 1 do
      if ([vsSelected, vsDisabled] * NewItems[I].States <> []) or
         (Constrained and (Cardinal(FLastSelectionLevel) <> GetNodeLevel(NewItems[I]))) or
         (SiblingConstrained and (FRangeAnchor.Parent <> NewItems[I].Parent)) then
        Inc(PAnsiChar(NewItems[I]))
      else
      begin
        Include(NewItems[I].States, vsSelected);
        if Assigned(FOnAddToSelection) then
          FOnAddToSelection(Self, NewItems[I]);
      end;
  end;

  I := PackArray(NewItems, NewLength);
  if I > -1 then
    NewLength := I;

  Result := NewLength > 0;
  if Result then
  begin
    // 2) Sort the new item list so we can easily traverse it.
    if NewLength > 1 then
      QuickSort(NewItems, 0, NewLength - 1);
    // 3) Make room in FSelectionNodes for the new items.
    if FSelectionCount + NewLength >= Length(FSelectionNodes) then
      SetLength(FSelectionNodes, FSelectionCount + NewLength);

    // 4) Merge in new items
    J := NewLength - 1;
    CurrentEnd := FSelectionCount - 1;

    while J >= 0 do
    begin
      // First insert all new entries which are greater than the greatest entry in the old list.
      // If the current end marker is < 0 then there's nothing more to move in the selection
      // array and only the remaining new items must be inserted.
      if CurrentEnd >= 0 then
      begin
        while (J >= 0) and (PAnsiChar(NewItems[J]) > PAnsiChar(FSelectionNodes[CurrentEnd])) do
        begin
          FSelectionNodes[CurrentEnd + J + 1] := NewItems[J];
          Dec(J);
        end;
        // early out if nothing more needs to be copied
        if J < 0 then
          Break;
      end
      else
      begin
        // insert remaining new entries at position 0
        Move(NewItems[0], FSelectionNodes[0], (J + 1) * SizeOf(Pointer));
        // nothing more to do so exit main loop
        Break;
      end;

      // find the last entry in the remaining selection list which is smaller then the largest
      // entry in the remaining new items list
      FindNodeInSelection(NewItems[J], I, 0, CurrentEnd);
      Dec(I);
      // move all entries which are greater than the greatest entry in the new items list up
      // so the remaining gap travels down to where new items must be inserted
      Move(FSelectionNodes[I + 1], FSelectionNodes[I + J + 2], (CurrentEnd - I) * SizeOf(Pointer));
      CurrentEnd := I;
    end;
    Inc(FSelectionCount, NewLength);
  end;
end;

procedure TD2CustomTreeGrid.InternalCacheNode(Node: PD2TreeNode);
// Добавляет узел Node в массив временных узлов (используется при сборе возможно большого количества узлов).
// Adds the given node to the temporary node cache (used when collecting possibly large amounts of nodes).
var Len: Cardinal;
begin
  Len := Length(FTempNodeCache);
  if FTempNodeCount = Len then
  begin
    if Len < 100
      then Len := 100
      else Len := Len + Len div 10;
    SetLength(FTempNodeCache, Len);
  end;
  FTempNodeCache[FTempNodeCount] := Node;
  Inc(FTempNodeCount);
end;

procedure TD2CustomTreeGrid.InternalConnectNode(Node, Destination: PD2TreeNode;
  Target: TD2CustomTreeGrid; Mode: TD2TreeNodeAttachMode);
//Присоединить узел Node к узлу Destination в дереве Target в зависимости от Mode.
//Ни какие проверки не проводятся. Узлы Node и Destination должны быть действительным.
//Узлы Node и Destination не должен быть корневыми если Mode amInsertBefore или amInsertAfter.
// Connects Node with Destination depending on Mode.
// No error checking takes place. Node as well as Destination must be valid.
// Node must never be a root node and Destination must not be a root node if Mode
// is amInsertBefore or amInsertAfter.
var Run: PD2TreeNode;
begin
  // Keep in mind that the destination node might belong to another tree.
  with Target do
  begin
    case Mode of
      amInsertBefore:
        begin
          Node.PrevSibling := Destination.PrevSibling;
          Destination.PrevSibling := Node;
          Node.NextSibling := Destination;
          Node.Parent := Destination.Parent;
          Node.Index := Destination.Index;
          if Node.PrevSibling = nil then
            Node.Parent.FirstChild := Node
          else
            Node.PrevSibling.NextSibling := Node;

          // reindex all following nodes
          Run := Destination;
          while Assigned(Run) do
          begin
            Inc(Run.Index);
            Run := Run.NextSibling;
          end;

          Inc(Destination.Parent.ChildCount);
          Include(Destination.Parent.States, vsHasChildren);
          AdjustTotalCount(Destination.Parent, Node.TotalCount, True);

          // Add the new node's height only if its parent is expanded.
          if FullyVisible[Node] then
          begin
            AdjustTotalHeight(Destination.Parent, Node.TotalHeight, True);
            Inc(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
          end;
        end;
      amInsertAfter:
        begin
          Node.NextSibling := Destination.NextSibling;
          Destination.NextSibling := Node;
          Node.PrevSibling := Destination;
          Node.Parent := Destination.Parent;
          if Node.NextSibling = nil then
            Node.Parent.LastChild := Node
          else
            Node.NextSibling.PrevSibling := Node;
          Node.Index := Destination.Index;

          // reindex all following nodes
          Run := Node;
          while Assigned(Run) do
          begin
            Inc(Run.Index);
            Run := Run.NextSibling;
          end;

          Inc(Destination.Parent.ChildCount);
          Include(Destination.Parent.States, vsHasChildren);
          AdjustTotalCount(Destination.Parent, Node.TotalCount, True);

          // Add the new node's height only if its parent is expanded.
          if FullyVisible[Node] then
          begin
            AdjustTotalHeight(Destination.Parent, Node.TotalHeight, True);
            Inc(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
          end;
        end;
      amAddChildFirst:
        begin
          if Assigned(Destination.FirstChild) then
          begin
            // If there's a first child then there must also be a last child.
            Destination.FirstChild.PrevSibling := Node;
            Node.NextSibling := Destination.FirstChild;
            Destination.FirstChild := Node;
          end
          else
          begin
            // First child node at this location.
            Destination.FirstChild := Node;
            Destination.LastChild := Node;
            Node.NextSibling := nil;
          end;
          Node.PrevSibling := nil;
          Node.Parent := Destination;
          Node.Index := 0;
          // reindex all following nodes
          Run := Node.NextSibling;
          while Assigned(Run) do
          begin
            Inc(Run.Index);
            Run := Run.NextSibling;
          end;

          Inc(Destination.ChildCount);
          Include(Destination.States, vsHasChildren);
          AdjustTotalCount(Destination, Node.TotalCount, True);
          // Add the new node's height only if its parent is expanded.
          if FullyVisible[Node] then
          begin
            AdjustTotalHeight(Destination, Node.TotalHeight, True);
            Inc(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
          end;
        end;
      amAddChildLast:
        begin
          if Assigned(Destination.LastChild) then
          begin
            // If there's a last child then there must also be a first child.
            Destination.LastChild.NextSibling := Node;
            Node.PrevSibling := Destination.LastChild;
            Destination.LastChild := Node;
          end
          else
          begin
            // first child node at this location
            Destination.FirstChild := Node;
            Destination.LastChild := Node;
            Node.PrevSibling := nil;
          end;
          Node.NextSibling := nil;
          Node.Parent := Destination;
          if Assigned(Node.PrevSibling) then
            Node.Index := Node.PrevSibling.Index + 1
          else
            Node.Index := 0;
          Inc(Destination.ChildCount);
          Include(Destination.States, vsHasChildren);
          AdjustTotalCount(Destination, Node.TotalCount, True);
          // Add the new node's height only if its parent is expanded.
          if FullyVisible[Node] then
          begin
            AdjustTotalHeight(Destination, Node.TotalHeight, True);
            Inc(FVisibleCount, CountVisibleChildren(Node) + Cardinal(IfThen(IsEffectivelyVisible[Node], 1)));
          end;
        end;
    else
      // amNoWhere: do nothing
    end;

    // Remove temporary states.
    Node.States := Node.States - [vsChecking, vsCutOrCopy, vsDeleting, vsClearing];

    // Update the hidden children flag of the parent.
    if (Mode <> amNoWhere) and (Node.Parent <> FRoot) then
    begin
      // If we have added a visible node then simply remove the all-children-hidden flag.
      if IsEffectivelyVisible[Node] then
        Exclude(Node.Parent.States, vsAllChildrenHidden)
      else
        // If we have added an invisible node and this is the only child node then
        // make sure the all-children-hidden flag is in a determined state.
        // If there were child nodes before then no action is needed.
        if Node.Parent.ChildCount = 1 then
          Include(Node.Parent.States, vsAllChildrenHidden);
    end;
  end;
end;

procedure TD2CustomTreeGrid.InternalClearSelection;
//Внутренняя процедура очистки массива выбора узлов
var Count: Integer;
begin
  //Вполне возможно, что есть недействительные ссылки на узлы в массиве выбора
  //если обновление дерева заблокировано и были внесены изменения в структуре.
  //Обработаем эту потенциально опасную ситуацию путем явной упаковки массива выбора.
  // It is possible that there are invalid node references in the selection array
  // if the tree update is locked and changes in the structure were made.
  // Handle this potentially dangerous situation by packing the selection array explicitely.
  if FUpdateCount > 0 then
  begin
    Count := PackArray(FSelectionNodes, FSelectionCount);
    if Count > -1 then
    begin
      FSelectionCount := Count;
      SetLength(FSelectionNodes, FSelectionCount);
    end;
  end;

  while FSelectionCount > 0 do
  begin
    Dec(FSelectionCount);
    Exclude(FSelectionNodes[FSelectionCount].States, vsSelected);
    DoRemoveFromSelection(FSelectionNodes[FSelectionCount]);
  end;
  ResetRangeAnchor;
  FSelectionNodes := nil;
  DoStateChange([], [tsClearPending]);
end;

procedure TD2CustomTreeGrid.InvalidateCache;
//Пометить кэш недействительным.
// Marks the cache as invalid.
begin
  DoStateChange([tsValidationNeeded], [tsUseCache]);
end;

function TD2CustomTreeGrid.IsOneRowSelected: boolean;
begin
  Result:= (FSelectionCount <= 1);
end;

function TD2CustomTreeGrid.IsSelected(Row: integer): boolean;
begin
  Result:=GetSelected(GetNodeAtTree(Row));
end;

function TD2CustomTreeGrid.IsSelected(const X, Y: single): boolean;
begin
  Result:=GetSelected(GetNodeAtTree(X, Y));
end;

procedure TD2CustomTreeGrid.MainColumnChanged;
//Виртуальный метод, изменяемый в потомках, вызываемый при изменении колонки, отображающая структуру дерева
begin
  DoCancelEdit;
end;

procedure TD2CustomTreeGrid.MarkCutCopyNodes;
//Устанавливает флаг vsCutOrCopy в каждом выбранном в данный момент узле, кроме
//недействительных чтобы указать, что они является частью операции с буфером обмена.
// Sets the vsCutOrCopy style in every currently selected but not disabled node
// to indicate it is now part of a clipboard operation.
var
  Nodes: TD2NodeArray;
  I: Integer;

begin
  Nodes := nil;
  if FSelectionCount > 0 then
  begin
    // need the current selection sorted to exclude selected nodes which are children, grandchildren etc. of
    // already selected nodes
    Nodes := GetSortedSelection(False);
    for I := 0 to High(Nodes) do
      with Nodes[I]^ do
        if not (vsDisabled in States) then
          Include(States, vsCutOrCopy);
  end;
end;

function TD2CustomTreeGrid.ReadChunk(Stream: TStream; Version: Integer;
  Node: PD2TreeNode; ChunkType, ChunkSize: Integer): Boolean;
//Вызывается при чтении структуры дерева, узел Node уже действителен (распределен) на данный момент.
//Возвращает True, если блок обработан, иначе false.
//Функция обрабатывает базовый и пользовательский блоки, любой другой блок помечается как неизвестный
//(результат будет False) и пропускается. Потомки могут переопределить этот метод.

// Called while loading a tree structure, Node is already valid (allocated) at this point.
// The function handles the base and user chunks, any other chunk is marked as being unknown
// (result becomes False) and skipped. descendants may handle them by overriding this method.
// Returns True if the chunk could be handled, otherwise False.
var
  ChunkBody: TD2BaseChunkBody;
  Run: PD2TreeNode;
  LastPosition: Integer;
begin
  case ChunkType of
    BaseChunk:
      begin
        // Load base chunk's body (chunk header has already been consumed).
        Stream.Read(ChunkBody, SizeOf(ChunkBody));
        with Node^ do
        begin
          // Set states first, in case the node is invisible.
          States := ChunkBody.States;
          NodeHeight := ChunkBody.NodeHeight;
          TotalHeight := NodeHeight;
          Align := ChunkBody.Align;
          CheckState := ChunkBody.CheckState;
          CheckType := ChunkBody.CheckType;
          ChildCount := ChunkBody.ChildCount;

          // Create and read child nodes.
          while ChunkBody.ChildCount > 0 do
          begin
            Run := MakeNewNode;

            Run.PrevSibling := Node.LastChild;
            if Assigned(Run.PrevSibling) then
              Run.Index := Run.PrevSibling.Index + 1;
            if Assigned(Node.LastChild) then
              Node.LastChild.NextSibling := Run
            else
              Node.FirstChild := Run;
            Node.LastChild := Run;
            Run.Parent := Node;

            ReadNode(Stream, Version, Run);
            Dec(ChunkBody.ChildCount);
          end;
        end;
        Result := True;
      end;
    UserChunk:
      if ChunkSize > 0 then
      begin
        // need to know whether the data was read
        LastPosition := Stream.Position;
        DoLoadUserData(Node, Stream);
        // compare stream position to learn whether the data was read
        Result := Stream.Position > LastPosition;
        // Improve stability by advancing the stream to the chunk's real end if
        // the application did not read what has been written.
        if not Result or (Stream.Position <> (LastPosition + ChunkSize)) then
          Stream.Position := LastPosition + ChunkSize;
      end
      else
        Result := True;
  else
    // unknown chunk, skip it
    Stream.Position := Stream.Position + ChunkSize;
    Result := False;
  end;
end;

procedure TD2CustomTreeGrid.ReadNode(Stream: TStream; Version: Integer; Node: PD2TreeNode);
// Считывает якорь блока каждого узла и начинает читать подблоки узла Node из потока Stream
// Reads the anchor chunk of each node and initiates reading the sub chunks for this node
var
  Header: TD2ChunkHeader;
  EndPosition: Integer;
begin
  with Stream do
  begin
    // Read anchor chunk of the node.
    Stream.Read(Header, SizeOf(Header));
    if Header.ChunkType = NodeChunk then
    begin
      EndPosition := Stream.Position + Header.ChunkSize;
      // Read all subchunks until the indicated chunk end position is reached in the stream.
      while Position < EndPosition do
      begin
        // Read new chunk header.
        Stream.Read(Header, SizeOf(Header));
        ReadChunk(Stream, Version, Node, Header.ChunkType, Header.ChunkSize);
      end;
      // If the last chunk does not end at the given end position then there is something wrong.
      if Position <> EndPosition then
        ShowError(SCorruptStream2, hcTFCorruptStream2);
    end
    else
      ShowError(SCorruptStream1, hcTFCorruptStream1);
  end;
end;

procedure TD2CustomTreeGrid.ResetRangeAnchor;
//Вызывается, когда больше нет выбранного узла и для якоря массива выбора узлов нужно новое значение.
// Called when there is no selected node anymore and the selection range anchor needs a new value.
begin
  FRangeAnchor := FFocusedNode;
  FLastSelectionLevel := -1;
end;

procedure TD2CustomTreeGrid.SelectNode(Node: PD2TreeNode);
//Делает выбранным узел Node и отменяет выделение всех ранее выделенных узлов
begin
  if FSelectionCount > 0
     then ClearSelection;
  Selected[Node]:=true;
end;

procedure TD2CustomTreeGrid.SelectNodes(StartNode, EndNode: PD2TreeNode;
  AddOnly: Boolean);
//Выбирает диапазон узлов от узла StartNode до узла EndNode и отменяет выделение всех
//ранее выбранных узлов, которые не находятся в этом диапазоне, если AddOnly = false.
//EndNode должен быть виден в то время как StartNode не обязательно, как и в том случае,
//когда последним сфокусированным был узел StartNode, но он является дочерним узлом,
//который был ранее свернут. В этом случае первый видимый родительский узел используется
//в качестве начального узла. StartNode может быть нулевым, в этом случае используется
//самый первый узел в дереве.
// Selects a range of nodes and unselects all other eventually selected nodes which
// are not in this range if AddOnly is False.
// EndNode must be visible while StartNode does not necessarily as in the case where
// the last focused node is the start node but it is a child of a node which has been
// collapsed previously. In this case the first visible parent node is used as start
// node. StartNode can be nil in which case the very first node in the tree is used.

var
  NodeFrom,
  NodeTo,
  LastAnchor: PD2TreeNode;
  Index: Integer;

begin
  //Assert(Assigned(EndNode), 'EndNode must not be nil!');
  if (Assigned(EndNode)) and (not FSelectionLocked) then
  begin
    ClearTempCache;
    if StartNode = nil then
      StartNode := GetFirstVisibleNoInit(nil, True)
    else
      if not FullyVisible[StartNode] then
      begin
        StartNode := GetPreviousVisible(StartNode, True);
        if StartNode = nil then
          StartNode := GetFirstVisibleNoInit(nil, True);
      end;

    if CompareNodePositions(StartNode, EndNode, True) < 0 then
    begin
      NodeFrom := StartNode;
      NodeTo := EndNode;
    end
    else
    begin
      NodeFrom := EndNode;
      NodeTo := StartNode;
    end;

    // The range anchor will be reset by the following call.
    LastAnchor := FRangeAnchor;
    if not AddOnly then
      InternalClearSelection;

    while NodeFrom <> NodeTo do
    begin
      InternalCacheNode(NodeFrom);
      NodeFrom := GetNextVisible(NodeFrom, True);
    end;
    // select last node too
    InternalCacheNode(NodeFrom);
    // now add them all in "one" step
    AddToSelection(FTempNodeCache, FTempNodeCount);
    ClearTempCache;
    if Assigned(LastAnchor) and FindNodeInSelection(LastAnchor, Index, -1, -1) then
     FRangeAnchor := LastAnchor;
  end;
end;

procedure TD2CustomTreeGrid.SetSelectedByPoint(const X, Y: single);
//сделать выбранной строку по координатам X, Y
begin
  //InternalClearSelection;
  FLastSelected:=GetNodeAtTree(X, Y);
  SelectNode(FLastSelected);
end;

procedure TD2CustomTreeGrid.SetSelectedMoreRowByPoint(const X, Y: single);
//добавить к выбранным строки начиная с текущей до строки с координатами X, Y с очисткой ранее выбранных строк
begin
  FLastSelected:=GetNodeAtTree(X, Y);
  if Assigned(FLastSelected) then
    begin
      if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
        then SelectNodes(FRangeAnchor, FLastSelected, false)
        else SelectNode(FLastSelected);
    end;
end;

procedure TD2CustomTreeGrid.SetSelectedExtraRowByPoint(const X, Y: single);
//добавить к выбранным строки начиная с текущей до строки с координатами X, Y без очистки ранее выбранных строк
begin
  FLastSelected:=GetNodeAtTree(X, Y);
  if Assigned(FLastSelected) then
    begin
      if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
        then SelectNodes(FRangeAnchor, FLastSelected, true)
        else Selected[FLastSelected]:= true;
    end;
end;

function TD2CustomTreeGrid.ChangeSelectionRowByPoint(const X, Y: single): boolean;
//Инвертировать выделение строки по координатам X,Y. Результат: true - строка выделена, false - развыделена
var s: boolean;
begin
  FLastSelected:=GetNodeAtTree(X, Y);
  if Assigned(FLastSelected) then
  begin
    s:= not GetSelected(FLastSelected);
    Selected[FLastSelected]:= s;
    if SelectedCount > 0
      then FRangeAnchor:=FLastSelected
      else FRangeAnchor:=nil;
    Result:=s;
  end;
end;

procedure TD2CustomTreeGrid.KeyDown(var Key: Word; var KeyChar: System.WideChar; Shift: TShiftState);
var
  MultiSelect: boolean;
  ANode: PD2TreeNode;
begin
  inherited;
  if FVisibleCount > 0 then
  begin
    MultiSelect:= (toMultiSelect in FOptions.FSelectionOptions);
    if IsOneRowSelected then
    begin
      if (KeyChar <> #0) and not (ReadOnly) and not (Columns[FocusedColumn].ReadOnly) then
      begin
        if (Columns[FocusedColumn] <> nil) and
           (TD2TreeColumn(Columns[FocusedColumn]).CellControlByNode(FLastSelected) <> nil) then
        begin
          if (KeyChar = ' ')
            then  if (FocusedColumn = FMainColumn)
                    then Key:=VK_SPACE
                    else KeyChar:= #0
            else TD2TreeColumn(Columns[FocusedColumn]).NodeSetFocus(FLastSelected);
          TD2HackVisualObject(TD2TreeColumn(Columns[FocusedColumn]).CellControlByNode(FLastSelected)).KeyDown(Key,KeyChar,Shift);
          Key:=0;
        end;
      end;
      case Key of
        VK_F2:
          begin
            if Columns[FocusedColumn] <> nil
               then TD2TreeColumn(Columns[FocusedColumn]).NodeSetFocus(FLastSelected);
            Key:=0;
          end;
        VK_LEFT:
          begin
            if (Shift = []) then                    //нажата только влево
            begin
              if Assigned(FLastSelected) then //если есть выделенная ячейка
              begin
                if (FFocusedColumn = FMainColumn) and          // если это главная колонка и
                       (HasChildren[FLastSelected]) and             // есть дети и
                            (Expanded[FLastSelected])               // узел разсвернут
                  then Expanded[FLastSelected]:=false            // то свернуть узел
                  else begin                                     // иначе
                          if (FocusedColumn > 0)                   // если это не первая колонка
                            then FocusedColumn := FocusedColumn - 1; // то выделить предыдущую ячейку строки
                        end;
              end;
            end;
            if (Shift = [ssCtrl]) and (FocusedColumn <> 0)  //нажата Ctrl+влево
              then FocusedColumn := 0;
            Key:=0;
          end;
        VK_RIGHT:
          begin
            if (Shift = []) then                     //нажата только вправо
            begin
              if Assigned(FLastSelected) then              // если есть выделенная ячейка
              begin
                if (FFocusedColumn = FMainColumn) and        // если это главная колонка и
                       (HasChildren[FLastSelected]) and           // есть дети и узел свернут и
                            (not Expanded[FLastSelected])         // узел свернут
                  then Expanded[FLastSelected]:=true           // то развернуть узел
                  else begin                                     // иначе
                          if (FocusedColumn < ColumnCount - 1)     //если это не последняя колонка
                            then FocusedColumn := FocusedColumn + 1; //то выделить следующую ячейку строки
                        end;
              end;
            end;
            if (Shift = [ssCtrl]) and (FocusedColumn <> ColumnCount - 1) //нажата Ctrl+вправо
               then FocusedColumn := ColumnCount - 1;
            Key:=0;
          end;
      end;
    end;
    case Key of
      VK_SPACE:
        if (Shift = [ssCtrl]) and MultiSelect then   //Ctrl + пробел
        begin
          FIsPreSelected := Selected[FLastSelected];
          Selected[FLastSelected]:= not FIsPreSelected;
          Key:=0;
        end;
      VK_A:
        if (Shift = [ssCtrl]) and MultiSelect then //Ctrl + A
          begin
            SelectAll(false);
            FIsPreSelected:=false;
            Key:=0;
          end;
      VK_UP:
        begin
          if Shift = [] then  //нажата только вверх
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetPreviousVisible(FLastSelected);
              if not Assigned(FLastSelected)
                then FLastSelected:=GetFirstVisible;
              SelectNode(FLastSelected);
              Key:=0;
            end;
          if Shift = [ssShift] then  //если Shift + ввех
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetPreviousVisible(FLastSelected);
              if not Assigned(FLastSelected)
                 then FLastSelected:=GetFirstVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:= true;
              Key:=0;
            end;
          if Shift = [ssCtrl]  then //Ctrl + ввех
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetPreviousVisible(FLastSelected);
              if not Assigned(FLastSelected)
                then FLastSelected:=GetFirstVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;
      VK_DOWN:
        begin
          if Shift = [] then  //нажата только вниз
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetNextVisible(FLastSelected);
              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              SelectNode(FLastSelected);
              Key:=0;
            end;
          if Shift = [ssShift] then  //если Shift + вниз
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetNextVisible(FLastSelected);
              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:= true;
              Key:=0;
            end;
          if Shift = [ssCtrl]  then //Ctrl + вниз
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetNextVisible(FLastSelected);
              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;
      VK_HOME:
        begin
          if Shift = [] then  //нажата только Home
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetFirstVisible;
              SelectNode(FLastSelected);
              Key:=0;
            end;
          if Shift = [ssShift] then  //если Shift + Home
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetFirstVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:= true;
              Key:=0;
            end;
          if Shift = [ssCtrl] then //Ctrl + Home и выбрана не первая строка
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetFirstVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;
      VK_END:
        begin
          if Shift = [] then  //нажата только End
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetLastVisible;
              SelectNode(FLastSelected);
              Key:=0;
            end;
          if Shift = [ssShift] then  //если Shift + End
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetLastVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:=true;
              Key:=0;
            end;
          if Shift = [ssCtrl]  then //Ctrl + End и выбрана не последняя строка
            begin
              FIsPreSelected:=false;
              FLastSelected:=GetLastVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;
    end;
    ScrollIntoView(FLastSelected,toCenterScrollIntoView in TreeOptions.FSelectionOptions,false);

    case Key of
      VK_PRIOR:
        begin
          if Shift = [] then   //нажата только PgUp
            begin
              FIsPreSelected:=false;
              ANode:=GetTopNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetTopNode(FLastSelected);
                      end
                else  begin
                        SetBottomNode(FLastSelected);
                        FLastSelected:=GetTopNode;
                        SetTopNode(FLastSelected);
                      end;
              if not Assigned(FLastSelected)
                then FLastSelected:=GetFirstVisible;
              SelectNode(FLastSelected);
              Key:=0;
            end;

          if Shift = [ssShift] then    //если Shift + PgUp
            begin
              FIsPreSelected:=false;
              ANode:=GetTopNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetTopNode(FLastSelected);
                      end
                else  begin
                        SetBottomNode(FLastSelected);
                        FLastSelected:=GetTopNode;
                        SetTopNode(FLastSelected);
                      end;
              if not Assigned(FLastSelected)
                then FLastSelected:=GetFirstVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:= true;
              Key:=0;
            end;

          if Shift = [ssCtrl]  then  //Ctrl + PgUp и выбрана не последняя строка
            begin
              FIsPreSelected:=false;
              ANode:=GetTopNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetTopNode(FLastSelected);
                      end
                else  begin
                        SetBottomNode(FLastSelected);
                        FLastSelected:=GetTopNode;
                        SetTopNode(FLastSelected);
                      end;
              if not Assigned(FLastSelected)
                then FLastSelected:=GetFirstVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;

      VK_NEXT:
        begin
          if Shift = [] then  //нажата только PgDn
            begin
              FIsPreSelected:=false;
              ANode:=GetBottomNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetBottomNode(FLastSelected);
                      end
                else  begin
                        SetTopNode(FLastSelected);
                        FLastSelected:=GetBottomNode;
                        SetBottomNode(FLastSelected);
                      end;
              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              SelectNode(FLastSelected);

              Key:=0;
            end;

          if Shift = [ssShift] then    //если Shift + PgDn
            begin
              FIsPreSelected:=false;
              ANode:=GetBottomNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetBottomNode(FLastSelected);
                      end
                else  begin
                        SetTopNode(FLastSelected);
                        FLastSelected:=GetBottomNode;
                        SetBottomNode(FLastSelected);
                      end;

              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              if (toMultiSelect in TreeOptions.SelectionOptions) and  Assigned(FRangeAnchor)
                then SelectNodes(FRangeAnchor, FLastSelected, false)
                else Selected[FLastSelected]:= true;
              Key:=0;
            end;

          if Shift = [ssCtrl]  then   //Ctrl + PgDn и выбрана не последняя строка
            begin
              FIsPreSelected:=false;
              ANode:=GetBottomNode;
              if ANode <> FLastSelected
                then  begin
                        FLastSelected:=ANode;
                        SetBottomNode(FLastSelected);
                      end
                else  begin
                        SetTopNode(FLastSelected);
                        FLastSelected:=GetBottomNode;
                        SetBottomNode(FLastSelected);
                      end;
              if not Assigned(FLastSelected)
                then FLastSelected:=GetLastVisible;
              if MultiSelect
                then begin
                       FIsPreSelected:=true;
                       Realign;
                     end
                else SelectNode(FLastSelected);
              Key:=0;
            end;
        end;
      else Exit;
    end;

    Key:=0;
  end;


end;

procedure TD2CustomTreeGrid.KeyUp(var Key: Word; var KeyChar: System.WideChar;
  Shift: TShiftState);
begin
  If (Key=VK_SPACE) and (tsKeyCheckPending in FStates)
    then  begin
            DoStateChange([], [tskeyCheckPending]);
            if (FCheckNode <> nil) then
            begin
              if (FCheckNode = FLastSelected)
                then DoCheckClick(FCheckNode, FPendingCheckState)
                else FCheckNode.CheckState := UnpressedState[FCheckNode.CheckState];
              FCheckNode := nil;
            end;
          end
    else inherited;
end;

procedure TD2CustomTreeGrid.MouseUp(Button: TMouseButton; Shift: TShiftState;
  X, Y: single);
begin
  if tsMouseCheckPending in FStates
    then  begin
            DoStateChange([], [tsMouseCheckPending]);
            if (FCheckNode <> nil)
              then FCheckNode.CheckState := UnpressedState[FCheckNode.CheckState];
            FCheckNode := nil;
          end
    else inherited;
end;

procedure TD2CustomTreeGrid.SetFocusedNodeAndColumn(Node: PD2TreeNode; Column: Integer);
//Устанавливает фокус на узел Node и колонку Column
//var
//  OldColumn: Integer;
//  WasDifferent: Boolean;

begin
  //if not FHeader.AllowFocus(Column) then
  //  Column := FFocusedColumn;
  //
  //WasDifferent := (Node <> FFocusedNode) or (Column <> FFocusedColumn);
  //
  //OldColumn := FFocusedColumn;
  //FFocusedColumn := Column;
  //
  //DoFocusNode(Node, True);
  //
  //// Check if the change was accepted.
  //if FFocusedNode = Node then
  //begin
  //  CancelEditNode;
  //  if WasDifferent then
  //    DoFocusChange(FFocusedNode, FFocusedColumn);
  //end
  //else
  //  // If the user did not accept the new cell to focus then set also the focused column back
  //  // to its original state.
  //  FFocusedColumn := OldColumn;
end;

procedure TD2CustomTreeGrid.SkipNode(Stream: TStream);
//Игнорирует данные для следующего узла в потоке Stream (в том числе и дочерние узлы).
// Skips the data for the next node in the given stream (including the child nodes).
var Header: TD2ChunkHeader;
begin
  with Stream do
  begin
    // read achor chunk of the node
    Stream.Read(Header, SizeOf(Header));
    if Header.ChunkType = NodeChunk then
      Stream.Position := Stream.Position + Header.ChunkSize
    else
      ShowError(SCorruptStream1, hcTFCorruptStream1);
  end;
end;

procedure TD2CustomTreeGrid.StartOperation(OperationKind: TD2TreeOperationKind);
// Вызывается, чтобы указать, что была начата длительная операция.
// Called to indicate that a long-running operation has been started.
begin
  Inc(FOperationCount);
  DoStartOperation(OperationKind);
  if FOperationCount = 1 then
    FOperationCanceled := False;
end;

procedure TD2CustomTreeGrid.StructureChange(Node: PD2TreeNode;
  Reason: TD2ChangeReason);
//Изменить структуру узла Node в соответствии с параметрами Reason
begin
  AdviseChangeEvent(True, Node, Reason);

  if FUpdateCount = 0 then
  begin
    {if (FChangeDelay > 0) and not (tsSynchMode in FStates)
      then SetTimer(Handle, StructureChangeTimer, FChangeDelay, nil)
      else }DoStructureChange(Node, Reason);
  end;
end;

procedure TD2CustomTreeGrid.ValidateCache;
//Запуск проверки кэша, если это еще не сделано, добавляя экземпляр рабочих потоков
//в список ожидания (если он еще не существует) и сигнализации нить может начать проверку.
// Starts cache validation if not already done by adding this instance to the worker thread's waiter list
// (if not already there) and signalling the thread it can start validating.

begin
  // Ожидание потока, для остановки проверки кэша этого дерева, если она в настоящее идет.
  // Wait for thread to stop validation if it is currently validating this tree's cache.
  InterruptValidation;

  FStartIndex := 0;
  {$ifdef EnableThreadSupport}
  if (tsValidationNeeded in FStates) and (FVisibleCount > CacheThreshold) then
  begin
    // Tell the thread this tree needs actually something to do.
    WorkerThread.AddTree(Self);
    WorkEvent.SetEvent;
  end;
  {$endif}
end;

procedure TD2CustomTreeGrid.ValidateNodeDataSize(var Size: Integer);
//Определяет размер области данных узла дерева
begin
  Size := SizeOf(Pointer);
  if Assigned(FOnGetNodeDataSize) then
    FOnGetNodeDataSize(Self, Size);
end;

procedure TD2CustomTreeGrid.UpdateNextNodeToSelect(Node: PD2TreeNode);
//Обновить узел для последующего выбора после удаления теущего выбранного узела.
//Это заставит VT вести себя как Win32 TreeView, который всегда выбирает новый узел,
//если выбранный узел будет удален.
// save a potential node to select after the currently selected node will be deleted.
// This will make the VT to behave more like the Win32 TreeView, which always selecta a new node if the currently
// selected one gets deleted.

begin
  if not (toAlwaysSelectNode in TreeOptions.SelectionOptions) then Exit;
  if GetNextSibling(Node) <> nil
    then  FNextNodeToSelect := GetNextSibling(Node)
    else  if GetPreviousSibling(Node) <> nil
            then  FNextNodeToSelect := GetPreviousSibling(Node)
            else  if GetNodeLevel(Node) > 0
                    then FNextNodeToSelect := Node.Parent
                    else FNextNodeToSelect := GetFirstChild(Node);
end;//if Assigned(Node);

procedure TD2CustomTreeGrid.WriteChunks(Stream: TStream; Node: PD2TreeNode);
//Записывает основные элементы узла Node в поток Stream.
//Внимание! Потомки могут опционально переопределить этот метод для добавления
//          других специфический элементов собственных узлов. Имейте в виду,
//          что этот метод также вызывается для корневого узла. Используя этот
//          факт в потомках вы можете создать своего рода "глобальные" элементы
//          узла непосредственно не привязаные к конкретному узлу.
// Writes the core chunks for Node into the stream.
// Note: descendants can optionally override this method to add other node specific
//       chunks. Keep in mind that this method is also called for the root node.
//       Using this fact in descendants you can create a kind of "global" chunks
//       not directly bound to a specific node.

var
  Header: TD2ChunkHeader;
  LastPosition,
  ChunkSize: Integer;
  Chunk: TD2BaseChunk;
  Run: PD2TreeNode;

begin
  with Stream do
  begin
    // 1. The base chunk...
    LastPosition := Position;
    Chunk.Header.ChunkType := BaseChunk;
    with Node^, Chunk do
    begin
      Body.ChildCount := ChildCount;
      Body.NodeHeight := NodeHeight;
      // Some states are only temporary so take them out as they make no sense at the new location.
      Body.States := States - [vsChecking, vsCutOrCopy, vsDeleting, vsOnFreeNodeCallRequired, vsHeightMeasured];
      Body.Align := Align;
      Body.CheckState := CheckState;
      Body.CheckType := CheckType;
      Body.Reserved := 0;
    end;
    // write the base chunk
    Write(Chunk, SizeOf(Chunk));

    // 2. ... directly followed by the child node chunks (actually they are child chunks of
    //   the base chunk)
    if vsInitialized in Node.States then
    begin
      Run := Node.FirstChild;
      while Assigned(Run) do
      begin
        WriteNode(Stream, Run);
        Run := Run.NextSibling;
      end;
    end;

    FinishChunkHeader(Stream, LastPosition, Position);

    // 3. запись пользовательских данных. write user data
    LastPosition := Position;
    Header.ChunkType := UserChunk;
    Write(Header, SizeOf(Header));
    DoSaveUserData(Node, Stream);
    // check if the application actually wrote data
    ChunkSize := Position - LastPosition - SizeOf(TD2ChunkHeader);
    // seek back to start of chunk if nothing has been written
    if ChunkSize = 0 then
    begin
      Position := LastPosition;
      Size := Size - SizeOf(Header);
    end
    else
      FinishChunkHeader(Stream, LastPosition, Position);
  end;
end;

procedure TD2CustomTreeGrid.WriteNode(Stream: TStream; Node: PD2TreeNode);
//Записывает основной элемент "обертку" узла Node в поток Stream и инициирует
//запись дочерних узлов и элементов.
// Writes the "cover" chunk for Node to Stream and initiates writing child nodes and chunks.
var
  LastPosition: Integer;
  Header: TD2ChunkHeader;
begin
  // Initialize the node first if necessary and wanted.
  if toInitOnSave in FOptions.FMiscOptions then
  begin
    if not (vsInitialized in Node.States)
      then InitNode(Node);
    if (vsHasChildren in Node.States) and (Node.ChildCount = 0)
      then InitChildren(Node);
  end;

  with Stream do
  begin
    LastPosition := Position;
    // Emit the anchor chunk.
    Header.ChunkType := NodeChunk;
    Write(Header, SizeOf(Header));
    // Write other chunks to stream taking their size into this chunk's size.
    WriteChunks(Stream, Node);
    // Update chunk size.
    FinishChunkHeader(Stream, LastPosition, Position);
  end;
end;

procedure TD2CustomTreeGrid.UpdateColumns;
var
  Node: PD2TreeNode;
  MaximumBottom: Single; //Координата нижней границы клиентской области . maximum vertical target position
  BaseOffset: Single;    //Координата X текущего видимого узла, в абсолютных координатах дерева. top position of the top node to draw given in absolute tree coordinates

begin
  if (ContentLayout = nil) then exit;
  if (tsPainting in FStates) then exit;
  DoStateChange([tsPainting]);

  FVisibleRowsList.Clear;
  Node := TopNode;  // Определим начальный узел для рисования. Determine node to start drawing with.
  MaximumBottom:= ContentLayout.LocalRect.Bottom + FVScrollBar.Value - GetTopRowY;  //координата нижней границы видимой области в абсолютных координатах дерева.
  BaseOffset:= 0;

  while Assigned(Node) do  //Пока есть узел
  begin
    if not (vsInitialized in Node.States) then InitNode(Node); // Инициализировать узел, если еще не выполнено. Initialize node if not already done.
    MeasureItemHeight(Node); // Убедиться, что высота узла определена.  Ensure the node's height is determined.
    FVisibleRowsList.Add(Node);

    d2IncFloat(BaseOffset, Node.NodeHeight);
    if BaseOffset > MaximumBottom then Break; //если верхняя координата следующего узла нижи чем нижний край видимого окна, то выходим из цикла
    Node := GetNextVisible(Node, True); // Переход к следующему видимому узлу. Advance to next visible node.
  end;
  inherited;
  DoStateChange([], [tsPainting]);

end;

procedure TD2CustomTreeGrid.ToggleSelection(StartNode, EndNode: PD2TreeNode);
// Переключает состояние выбора диапазона узлов от StartNode до EndNode
// Примечание. Этот метод специально разработан для выбора диапазонов с клавиатурой
//             и поэтому учитывает анкер диапазона.

// Switchs the selection state of a range of nodes.
// Note: This method is specifically designed to help selecting ranges with the keyboard and considers therefore
//       the range anchor.

var
  NodeFrom,
  NodeTo: PD2TreeNode;
  NewSize: Integer;
  Position: Integer;

begin
  if not FSelectionLocked then
  begin
    Assert(Assigned(EndNode), 'EndNode must not be nil!');
    if StartNode = nil then
      StartNode := FRoot.FirstChild
    else
      if not FullyVisible[StartNode] then
        StartNode := GetPreviousVisible(StartNode, True);

    Position := CompareNodePositions(StartNode, EndNode);
    // nothing to do if start and end node are the same
    if Position <> 0 then
    begin
      if Position < 0 then
      begin
        NodeFrom := StartNode;
        NodeTo := EndNode;
      end
      else
      begin
        NodeFrom := EndNode;
        NodeTo := StartNode;
      end;

      ClearTempCache;

      // 1) toggle the start node if it is before the range anchor
      if CompareNodePositions(NodeFrom, FRangeAnchor) < 0 then
        if not (vsSelected in NodeFrom.States) then
          InternalCacheNode(NodeFrom)
        else
          InternalRemoveFromSelection(NodeFrom);

      // 2) toggle all nodes within the range
      NodeFrom := GetNextVisible(NodeFrom, True);
      while NodeFrom <> NodeTo do
      begin
        if not (vsSelected in NodeFrom.States) then
          InternalCacheNode(NodeFrom)
        else
          InternalRemoveFromSelection(NodeFrom);
        NodeFrom := GetNextVisible(NodeFrom, True);
      end;

      // 3) toggle end node if it is after the range anchor
      if CompareNodePositions(NodeFrom, FRangeAnchor) > 0 then
        if not (vsSelected in NodeFrom.States) then
          InternalCacheNode(NodeFrom)
        else
          InternalRemoveFromSelection(NodeFrom);

      // Do some housekeeping if there was a change.
      NewSize := PackArray(FSelectionNodes, FSelectionCount);
      if NewSize > -1 then
      begin
        FSelectionCount := NewSize;
        SetLength(FSelectionNodes, FSelectionCount);
      end;
      // If the range went over the anchor then we need to reselect it.
      if not (vsSelected in FRangeAnchor.States) then
        InternalCacheNode(FRangeAnchor);
      if FTempNodeCount > 0 then
        AddToSelection(FTempNodeCache, FTempNodeCount);
      ClearTempCache;
    end;
  end;
end;

procedure TD2CustomTreeGrid.UnselectNodes(StartNode, EndNode: PD2TreeNode);
//Отменяет выбор диапазона узлов от StartNode до EndNode.
//EndNode должен быть видимым, при этом StartNode может быть не видим. Например, когда
//StartNode является последним сфокусированным и дочерним узлом ранее свернутого узла.
//В этом случае первый видимый родительский узел используется как начальный.
//StartNode может быть nil, в этом случае используется самый первый узел в дереве.

// Deselects a range of nodes.
// EndNode must be visible while StartNode must not as in the case where the last focused node is the start node
// but it is a child of a node which has been collapsed previously. In this case the first visible parent node
// is used as start node. StartNode can be nil in which case the very first node in the tree is used.

var
  NodeFrom,
  NodeTo: PD2TreeNode;
  NewSize: Integer;

begin
  if not FSelectionLocked then
  begin
    Assert(Assigned(EndNode), 'EndNode must not be nil!');

    if StartNode = nil then
      StartNode := FRoot.FirstChild
    else
      if not FullyVisible[StartNode] then
      begin
        StartNode := GetPreviousVisible(StartNode, True);
        if StartNode = nil then
          StartNode := FRoot.FirstChild;
      end;

    if CompareNodePositions(StartNode, EndNode) < 0 then
    begin
      NodeFrom := StartNode;
      NodeTo := EndNode;
    end
    else
    begin
      NodeFrom := EndNode;
      NodeTo := StartNode;
    end;

    while NodeFrom <> NodeTo do
    begin
      InternalRemoveFromSelection(NodeFrom);
      NodeFrom := GetNextVisible(NodeFrom, True);
    end;
    // Deselect last node too.
    InternalRemoveFromSelection(NodeFrom);

    // Do some housekeeping.
    NewSize := PackArray(FSelectionNodes, FSelectionCount);
    if NewSize > -1 then
    begin
      FSelectionCount := NewSize;
      SetLength(FSelectionNodes, FSelectionCount);
    end;
  end;
end;

function TD2CustomTreeGrid.GetTopRowY: single;
begin
  //if InternalGetNodeAt(0, VScrollBarValue, false, Result) = nil
  //  then Result:= 0;
  Result:= FTopRowY;   //вычисляется в процедуре TopNode;
end;

procedure TD2CustomTreeGrid.DoExpanded(Node: PD2TreeNode);
//Вызвать прерывание после раскрытия узла (FOnExpanded)
begin
  if Assigned(FOnExpanded) then
    FOnExpanded(Self, Node);
  {$ifdef EnableAccessible}
  NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}
end;

function TD2CustomTreeGrid.DoExpanding(Node: PD2TreeNode): Boolean;
//Вызывает прерывание перед разворачиванием узла (OnExpanding)
begin
  Result := True;
  if Assigned(FOnExpanding) then
    FOnExpanding(Self, Node, Result);
end;

function TD2CustomTreeGrid.AbsoluteIndex(Node: PD2TreeNode): Cardinal;
//Возвращает абсолютный № узла Node в дереве
begin
  Result := 0;
  while Assigned(Node) and (Node <> FRoot) do
  begin
    if not (vsInitialized in Node.States) then
      InitNode(Node);
    if Assigned(Node.PrevSibling) then
    begin
      // if there's a previous sibling then add its total count to the result
      Node := Node.PrevSibling;
      Inc(Result, Node.TotalCount);
    end
    else
    begin
      Node := Node.Parent;
      if Node <> FRoot then
        Inc(Result);
    end;
  end;
end;

function TD2CustomTreeGrid.AddChild(Parent: PD2TreeNode; UserData: Pointer): PD2TreeNode;
//Добавляет новый дочерний узел к родительскому узлу Parent и увеличивает кол-во
//дочерних узлов. Если Parent равен Nil, то новый узел добавляется в качестве
//последнего узла верхнего уровня.
//UserData может быть использован для указания первоого байта SizeOf(Pointer)
//области данных пользователя в исходное значение, которое можно использовать
//в OnInitNode и также приведет к инициированию события OnFreeNode (если <> NIL),
//даже если узел инициализируется не "официально".
//AddChild представляет собой метод совместимости и будет неявно проверять
//родительский узел. Это виртуальный метод и, следовательно, его непосредственное
//использование не рекомендуется.

// Adds a new node to the given parent node. This is simply done by increasing the child count of the
// parent node. If Parent is nil then the new node is added as (last) top level node.
// UserData can be used to set the first SizeOf(Pointer) bytes of the user data area to an initial
// value which can be used in OnInitNode and will also cause to trigger the OnFreeNode event (if <> nil)
// even if the node is not yet "officially" initialized.
// AddChild is a compatibility method and will implicitly validate the parent node. This is however
// against the virtual paradigm and hence I dissuade from its usage.

var
  NodeData: ^Pointer;

begin
  if not (toReadOnly in FOptions.FMiscOptions) then
  begin
    CancelEditNode;

    if Parent = nil then
      Parent := FRoot;
    if not (vsInitialized in Parent.States) then
      InitNode(Parent);

    //Локальная остановака обновлений дерева, чтобы избежать использования нового узла, прежде чем он правильно настроен.
    //Если счетчик изменений 0 тогда будет правильное обновление в конце этого метода.
    // Locally stop updates of the tree in order to avoid usage of the new node before it is correctly set up.
    // If the update count was 0 on enter then there will be a correct update at the end of this method.
    Inc(FUpdateCount);
    try
      SetChildCount(Parent, Parent.ChildCount + 1);  //увеличим кол-во дочерних узлов у Parent
      // Update the hidden children flag of the parent. Nodes are added as being visible by default.
      Exclude(Parent.States, vsAllChildrenHidden); // Отключим у родителья флаг "Все дети скрыты". По умолчанию узлы добавляются как видимые.
    finally
      Dec(FUpdateCount); //уменьшим счетчки обновлений
    end;
    Result := Parent.LastChild; //вернем последний дочерний узела Parent

    // Check if there is initial user data and there is also enough user data space allocated.
    if Assigned(UserData) then    //проверим наличие данных пользователя и достаточно ли для них выделенно памяти
      if FNodeDataSize >= SizeOf(Pointer) then
      begin
        NodeData := Pointer(PByte(@Result.Data) + FTotalInternalDataSize);
        NodeData^ := UserData;
        Include(Result.States, vsOnFreeNodeCallRequired);
      end
      else
        ShowError(SCannotSetUserData, hcTFCannotSetUserData);

    InvalidateCache;  //пометить кэш недействительным
    if FUpdateCount = 0 then    //если обновлений сейчас нет, то
    begin
      ValidateCache;
      if tsStructureChangePending in FStates then
      begin
        if Parent = FRoot then
          StructureChange(nil, crChildAdded)
        else
          StructureChange(Parent, crChildAdded);
      end;

      //if (toAutoSort in FOptions.FAutoOptions) and (FHeader.FSortColumn > InvalidColumn) then
      //  Sort(Parent, FHeader.FSortColumn, FHeader.FSortDirection, True);

      InvalidateToBottom(Parent);
      //lcl
      //Calling UpdateHorizontalScrollBar without a header leads to a
      //wrong NodeWidth because the node is not initialized at this time.
      //As result the horizontal scrollbar is not correctly
      //sized and the node can not be selected by a click.

      //if HandleAllocated then
        //UpdateVerticalScrollBar(True)
        Realign; //вместо UpdateScrollBars(True)
   end;
  end
  else Result := nil;
end;

procedure TD2CustomTreeGrid.AddFromStream(Stream: TStream;
  TargetNode: PD2TreeNode);
//Загружает узлы из потока Stream и добавляет их к TargetNode. Текущее содержимое
//не очищается до начала процесса загрузки (см также LoadFromStream)
// loads nodes from the given stream and adds them to TargetNode the current content
// is not cleared before the load process starts (see also LoadFromStream)
var
  ThisID: TD2MagicID;
  Version,
  Count: Cardinal;
  Node: PD2TreeNode;
begin
  if not (toReadOnly in FOptions.FMiscOptions) then
  begin
    // check first whether this is a stream we can read
    Stream.ReadBuffer(ThisID, SizeOf(TD2MagicID));
    if (ThisID[0] = MagicID[0]) and
       (ThisID[1] = MagicID[1]) and
       (ThisID[2] = MagicID[2]) and
       (ThisID[5] = MagicID[5]) then
    begin
      Version := Word(ThisID[3]);
      if Version <= TreeStreamVersion  then
      begin
        BeginUpdate;
        try
          if Version < 2 then
            Count := MaxInt
          else
            Stream.ReadBuffer(Count, SizeOf(Count));

          while (Stream.Position < Stream.Size) and (Count > 0) do
          begin
            Dec(Count);
            Node := MakeNewNode;
            InternalConnectNode(Node, TargetNode, Self, amAddChildLast);
            InternalAddFromStream(Stream, Version, Node);
          end;
          if TargetNode = FRoot then
            DoNodeCopied(nil)
          else
            DoNodeCopied(TargetNode);
        finally
          EndUpdate;
        end;
      end
      else
        ShowError(SWrongStreamVersion, hcTFWrongStreamVersion);
    end
    else
      ShowError(SWrongStreamVersion, hcTFWrongStreamVersion);
  end;
end;

procedure TD2CustomTreeGrid.AfterConstruction;
//Вызывается после создания экземпляра дерева для инициализации корневого узла
begin
  inherited;
  if FRoot = nil then
    InitRootNode;
end;

function TD2CustomTreeGrid.CancelEditNode: Boolean;
//Вызывается приложением или текущим редактором для отмены редактирования.
// Called by the application or the current edit link to cancel the edit action.

begin
  if {HandleAllocated and }([tsEditing, tsEditPending] * FStates <> []) then
    Result := DoCancelEdit
  else
    Result := True;
end;

function TD2CustomTreeGrid.CanEdit(Node: PD2TreeNode; Column: Integer
  ): Boolean;
// True - если данный узел может быть отредактирован.
// Returns True if the given node can be edited.
begin
  //Result := (toEditable in FOptions.FMiscOptions) and Enabled and not (toReadOnly in FOptions.FMiscOptions)
  //  and ((Column < 0) or (coEditable in FHeader.Columns[Column].Options));
  DoCanEdit(Node, Column, Result);
end;

procedure TD2CustomTreeGrid.CancelOperation;
//Вызывается приложением для отмены длительной операции.
// Called by the application to cancel a long-running operation.
begin
  if FOperationCount > 0 then
    FOperationCanceled := True;
end;

procedure TD2CustomTreeGrid.Clear;
//полное удаление всех узлов дерева
begin
  if not (toReadOnly in FOptions.FMiscOptions) or (csDestroying in ComponentState) then
  begin
    BeginUpdate;
    try
      InterruptValidation;
      if IsEditing then
        CancelEditNode;

      if ClipboardStates * FStates <> [] then
      begin
        OleSetClipboard(nil);
        DoStateChange([], ClipboardStates);
      end;
      ClearSelection;
      FFocusedNode := nil;
      FLastSelected := nil;
      //FCurrentHotNode := nil;
      FDropTargetNode := nil;
      FLastChangedNode := nil;
      FRangeAnchor := nil;
      //FCheckNode := nil;
      FLastVCLDragTarget := nil;
      FLastSearchNode := nil;
      DeleteChild(FRoot, True);
      if FHScrollBar<> nil
         then HScrollBar.Value := 0;
      if FVScrollBar<> nil
         then VScrollBar.Value := 0;

    finally
      EndUpdate;
    end;
  end;
end;

procedure TD2CustomTreeGrid.ClearChecked;
//Снимает отметку у всех узлов
var Node: PD2TreeNode;
begin
  Node := RootNode.FirstChild;
  while Assigned(Node) do
  begin
    if Node.CheckState <> csUncheckedNormal then
      CheckState[Node] := csUncheckedNormal;
    Node := GetNextNoInit(Node);
  end;
end;

procedure TD2CustomTreeGrid.ClearSelection;
//Очистить массив выбора узлов
var
  Node: PD2TreeNode;
  R: TD2Rect;
  Counter: Integer;
begin
  if not FSelectionLocked and (FSelectionCount > 0) and not (csDestroying in ComponentState) then
  begin
    //if (FUpdateCount = 0) {and HandleAllocated} and (FVisibleCount > 0) then
    //begin
    //  // Итерация всех видимыех в настоящее время узлов в клиентской области, и их аннулирование.
    //  // Iterate through nodes currently visible in the client area and invalidate them.
    //  Node := TopNode;
    //  if Assigned(Node) then
    //    R := GetDisplayRect(Node, NoColumn, False);
    //  Counter := FSelectionCount;
    //
    //  while Assigned(Node) do
    //  begin
    //    R.Bottom := R.Top + NodeHeight[Node];
    //    if vsSelected in Node.States then
    //    begin
    //      //InvalidateRect(Handle, @R, False);
    //      InvalidateRect(R);
    //      Dec(Counter);
    //      // Only try as many nodes as are selected.
    //      if Counter = 0 then Break;
    //    end;
    //    R.Top := R.Bottom;
    //    if R.Top > ClientHeight then
    //      Break;
    //    Node := GetNextVisibleNoInit(Node, True);
    //  end;
    //end;

    InternalClearSelection;
    Change(nil);
  end;
end;

function TD2CustomTreeGrid.CopyTo(Source: PD2TreeNode; Tree: TD2CustomTreeGrid;
  Mode: TD2TreeNodeAttachMode; ChildrenOnly: Boolean): PD2TreeNode;
// Упрощенный метод CopyTo, чтобы скопировать узел Source в корень другого дерева Tree.
// A simplified CopyTo method to allow to copy nodes to the root of another tree.
begin
  Result := CopyTo(Source, Tree.FRoot, Mode, ChildrenOnly);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2CustomTreeGrid.CopyTo(Source, Target: PD2TreeNode;
  Mode: TD2TreeNodeAttachMode; ChildrenOnly: Boolean): PD2TreeNode;
//Копирует Source и всех его детей в узел Target.
//Mode используется для указания, где добавить новый узел (как одноуровневый или
//дочерний к Target). При ChildrenOnly = False результатом является вновь созданный
//узел - копия источника. При ChildrenOnly = true - результат содержит Target.
//ChildrenOnly определяет, следует ли копировать в том числе и узел Source или
//только его дочерние узлы.

// Copies Source and all its child nodes to Target.
// Mode is used to specify further where to add the new node actually (as sibling
// of Target or as child of Target). Result is the newly created node to which source
// has been copied if ChildrenOnly is False or just contains Target in the other case.
// ChildrenOnly determines whether to copy also the source node or only its child nodes.
var
  TargetTree: TD2CustomTreeGrid;
  Stream: TMemoryStream;
begin
  Assert(TreeFromNode(Source) = Self, 'The source tree must contain the source node.');

  Result := nil;
  if (Mode <> amNoWhere) and Assigned(Source) and (Source <> FRoot) then
  begin
    // Assume that an empty destination means the root in this (the source) tree.
    if Target = nil then
    begin
      TargetTree := Self;
      Target := FRoot;
      Mode := amAddChildFirst;
    end
    else
      TargetTree := TreeFromNode(Target);

    if not (toReadOnly in TargetTree.FOptions.FMiscOptions) then
    begin
      if Target = TargetTree.FRoot then
      begin
        case Mode of
          amInsertBefore:
            Mode := amAddChildFirst;
          amInsertAfter:
            Mode := amAddChildLast;
        end;
      end;

      Stream := TMemoryStream.Create;
      try
        // Write all nodes into a temprary stream depending on the ChildrenOnly flag.
        if not ChildrenOnly then
          WriteNode(Stream, Source)
        else
        begin
          Source := Source.FirstChild;
          while Assigned(Source) do
          begin
            WriteNode(Stream, Source);
            Source := Source.NextSibling;
          end;
        end;
        // Now load the serialized nodes into the target node (tree).
        TargetTree.BeginUpdate;
        try
          Stream.Position := 0;
          while Stream.Position < Stream.Size do
          begin
            Result := TargetTree.MakeNewNode;
            InternalConnectNode(Result, Target, TargetTree, Mode);
            TargetTree.InternalAddFromStream(Stream, TreeStreamVersion, Result);
            if not DoNodeCopying(Result, Target) then
            begin
              TargetTree.DeleteNode(Result);
              Result := nil;
            end
            else
              DoNodeCopied(Result);
          end;
          if ChildrenOnly then
            Result := Target;
        finally
          TargetTree.EndUpdate;
        end;
      finally
        Stream.Free;
      end;

      with TargetTree do
      begin
        InvalidateCache;
        if FUpdateCount = 0 then
        begin
          ValidateCache;
          //UpdateScrollBars(True);
          //Invalidate;
          Realign; //вместо UpdateScrollBars(True) и Invalidate
        end;
        StructureChange(Source, crNodeCopied);
      end;
    end;
  end;
end;

procedure TD2CustomTreeGrid.DeleteChild(Node: PD2TreeNode; ResetHasChildren: Boolean);
//Удаление всех детей и их детей узла Node из памяти, не меняя флмг vsHasChildren узла.
// Removes all children and their children from memory without changing the vsHasChildren style by default.
var
  Run,
  Mark: PD2TreeNode;
  LastTop,
  LastLeft:Single;
  NewSize: Integer;
  ParentVisible: Boolean;
begin
  if Assigned(Node) and (Node.ChildCount > 0) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Assert(not (tsIterating in FStates), 'Deleting nodes during tree iteration leads to invalid pointers.');

    // The code below uses some flags for speed improvements which may cause invalid pointers if updates of
    // the tree happen. Hence switch updates off until we have finished the operation.
    Inc(FUpdateCount);
    try
      InterruptValidation;
      LastLeft := -HScrollBarValue;
      LastTop := VScrollBarValue;

      // Make a local copy of the visibility state of this node to speed up
      // adjusting the visible nodes count.
      ParentVisible := Node = FRoot;
      if not ParentVisible then
        ParentVisible := FullyVisible[Node] and (vsExpanded in Node.States);

      // Show that we are clearing the child list, to avoid registering structure change events.
      Include(Node.States, vsClearing);
      Run := Node.LastChild;
      while Assigned(Run) do
      begin
        if ParentVisible and IsEffectivelyVisible[Run] then
          Dec(FVisibleCount);

        Include(Run.States, vsDeleting);
        Mark := Run;
        Run := Run.PrevSibling;
        // Important, to avoid exchange of invalid pointers while disconnecting the node.
        if Assigned(Run) then
          Run.NextSibling := nil;
        DeleteNode(Mark);
      end;
      Exclude(Node.States, vsClearing);
      if ResetHasChildren then
        Exclude(Node.States, vsHasChildren);
      if Node <> FRoot then
        Exclude(Node.States, vsExpanded);
      Node.ChildCount := 0;
      if (Node = FRoot) or (vsDeleting in Node.States) then
      begin
        Node.TotalHeight := FDefaultNodeHeight + NodeHeight[Node];
        Node.TotalCount := 1;
      end
      else
      begin
        AdjustTotalHeight(Node, NodeHeight[Node]);
        AdjustTotalCount(Node, 1);
      end;
      Node.FirstChild := nil;
      Node.LastChild := nil;
    finally
      Dec(FUpdateCount);
    end;

    InvalidateCache;
    if FUpdateCount = 0 then
    begin
      NewSize := PackArray(FSelectionNodes, FSelectionCount);
      if NewSize > -1 then
      begin
        FSelectionCount := NewSize;
        SetLength(FSelectionNodes, FSelectionCount);
      end;

      ValidateCache;
      //UpdateScrollBars(True);
      Realign; //вместо UpdateScrollBars(True)
      // Invalidate entire tree if it scrolled e.g. to make the last node also the
      // bottom node in the treeview.
      if (LastLeft <> HScrollBarValue) or (LastTop <> VScrollBarValue)
        then {Invalidate} Realign
        else InvalidateToBottom(Node);
    end;
    StructureChange(Node, crChildDeleted);
  end
  else if ResetHasChildren then
    Exclude(Node.States, vsHasChildren);
end;

procedure TD2CustomTreeGrid.DeleteNode(Node: PD2TreeNode; Reindex: Boolean);
//Удаление узла Node и всех его детей и их детей и т.д.
var
  LastTop,
  LastLeft: Single;
  LastParent: PD2TreeNode;
  WasInSynchMode: Boolean;
  ParentClearing: Boolean;

begin
  if Assigned(Node) and (Node <> FRoot) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Assert(not (tsIterating in FStates), 'Deleting nodes during tree iteration leads to invalid pointers.');

    // Determine parent node for structure change notification.
    ParentClearing := vsClearing in Node.Parent.States;
    LastParent := Node.Parent;

    if not ParentClearing then
    begin
      if LastParent = FRoot then
        StructureChange(nil, crChildDeleted)
      else
        StructureChange(LastParent, crChildDeleted);
    end;

    LastLeft := -HScrollBarValue;
    LastTop := VScrollBarValue;

    if vsSelected in Node.States then
    begin
      if FUpdateCount = 0 then
      begin
        // Временный переход в режим синхронизации, чтобы избежать события с задержкой изменения для узла при отмене выбора.
        // Go temporarily into sync mode to avoid a delayed change event for the node
        // when unselecting.
        WasInSynchMode := tsSynchMode in FStates;
        Include(FStates, tsSynchMode);
        RemoveFromSelection(Node);
        if not WasInSynchMode then
          Exclude(FStates, tsSynchMode);
        InvalidateToBottom(LastParent);
      end
      else InternalRemoveFromSelection(Node);
    end
    else InvalidateToBottom(LastParent);

    if tsHint in FStates then
    begin
      Application.CancelHint;
      DoStateChange([], [tsHint]);
    end;

    if not ParentClearing then InterruptValidation;

    DeleteChild(Node);
    InternalDisconnectNode(Node, False, Reindex);
    DoFreeNode(Node);

    if not ParentClearing then
    begin
      DetermineHiddenChildrenFlag(LastParent);
      InvalidateCache;
      if FUpdateCount = 0 then
      begin
        ValidateCache;
        //UpdateScrollBars(True);
        // Invalidate entire tree if it scrolled e.g. to make the last node also the
        // bottom node in the treeview.
        //if (LastLeft <> HScrollBarValue) or (LastTop <> VScrollBarValue) then
        //  //Invalidate;
          Realign; //вместо UpdateScrollBars(True) и Invalidate
      end;
    end;
  end;

end;

procedure TD2CustomTreeGrid.DeleteSelectedNodes;
// Удаляет все выбранные узлы (включая их дочерние узлы).
// Deletes all currently selected nodes (including their child nodes).
var
  Nodes: TD2NodeArray;
  I: Integer;
  LevelChange: Boolean;
begin
  Nodes := nil;
  if (FSelectionCount > 0) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    BeginUpdate;
    try
      Nodes := GetSortedSelection(True);
      for I := High(Nodes) downto 1 do
      begin
        LevelChange := Nodes[I].Parent <> Nodes[I - 1].Parent;
        DeleteNode(Nodes[I], LevelChange);
      end;
      DeleteNode(Nodes[0]);
    finally
      EndUpdate;
    end;
  end;
end;

function TD2CustomTreeGrid.EditNode(Node: PD2TreeNode;
  Column: Integer): Boolean;
//Начать редактирование узла Node в колонке Column.
//Возвращает True, если начато редактирование иначе - False.
// Application triggered edit event for the given node.
// Returns True if the tree started editing otherwise False.

begin
  Assert(Assigned(Node), 'Node must not be nil.');
  //Assert((Column > InvalidColumn) and (Column < FHeader.Columns.Count),
  //  'Column must be a valid column index (-1 if no header is shown).');

  Result := tsEditing in FStates;
  // If the tree is already editing then we don't disrupt this.
  if not Result and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    FocusedNode := Node;
    if Assigned(FFocusedNode) and (Node = FFocusedNode) and CanEdit(FFocusedNode, Column) then
    begin
      FEditColumn := Column;
      if not (vsInitialized in Node.States) then
        InitNode(Node);
      DoEdit;
      Result := tsEditing in FStates;
    end
    else
      Result := False;
  end;
end;

function TD2CustomTreeGrid.EndEditNode: Boolean;
// Вызывается, чтобы закончить редактирование узла или остановить таймер ожидания редактирования.
// Called to finish a current edit action or stop the edit timer if an edit operation is pending.

begin
  if [tsEditing, tsEditPending] * FStates <> [] then
    Result := DoEndEdit
  else
    Result := True;
end;

procedure TD2CustomTreeGrid.EnsureNodeSelected;
//Обеспечить выделение узла при потере выбора текущим узлом (например при удалении)
begin
  if (toAlwaysSelectNode in TreeOptions.SelectionOptions) and
     (GetFirstSelected() = nil) and not SelectionLocked then
  begin
    if Assigned(FNextNodeToSelect)
      then Selected[FNextNodeToSelect] := True
      else if Self.isFocused  //вместо Focused
             then Selected[GetFirstVisible] := True;
  end;
end;

procedure TD2CustomTreeGrid.FullCollapse(Node: PD2TreeNode);
//Эта процедура сворачивает все развернутые узлы в поддереве узла Node или всего
//дерева если Node = FRoot или Nil. Свернуты будут только те узлы, которые были
//развернуты. Это исключает неинициализированные узлы, но узлы помеченые как видимые
//также будет сворачиваться, если они развернуты.
// This routine collapses all expanded nodes in the subtree given by Node or the whole tree
// if Node is FRoot or nil. Only nodes which are expanded will be collapsed. This excludes
// uninitialized nodes but nodes marked as visible will still be collapsed if they are expanded.
var Stop: PD2TreeNode;
begin
  if FRoot.TotalCount > 1 then
  begin
    if Node = FRoot then
      Node := nil;

    DoStateChange([tsCollapsing]);
    BeginUpdate;
    try
      Stop := Node;
      Node := GetLastVisibleNoInit(Node, True);

      if Assigned(Node) then
      begin
        repeat
          if [vsHasChildren, vsExpanded] * Node.States = [vsHasChildren, vsExpanded] then
            ToggleNode(Node);
          Node := GetPreviousNoInit(Node, True);
        until (Node = Stop) or not Assigned(Node);

        // Collapse the start node too.
        if Assigned(Stop) and ([vsHasChildren, vsExpanded] * Stop.States = [vsHasChildren, vsExpanded]) then
          ToggleNode(Stop);
      end;
    finally
      EndUpdate;
      DoStateChange([], [tsCollapsing]);
    end;
  end;
end;

procedure TD2CustomTreeGrid.FullExpand(Node: PD2TreeNode = nil);
//Эта процедура разворачивает все свернутые узлы в поддереве узла Node или всего дерева,
//если Node = Froot или Nil. Все узлы на пути вниз инициализируются поэтому процедура
//может занять длительное время. Поскольку все узлы проверяются, дерево не может
//использовать оптимизацию, что непродуктивно. Поэтому старайтесь избегать её использования.
// This routine expands all collapsed nodes in the subtree given by Node or the whole tree if Node is FRoot or nil.
// All nodes on the way down are initialized so this procedure might take a long time.
// Since all nodes are validated, the tree cannot make use of optimatizations. Hence it is counter productive and you
// should consider avoiding its use.
var Stop: PD2TreeNode;
begin
  if FRoot.TotalCount > 1 then
  begin
    DoStateChange([tsExpanding]);
    BeginUpdate;
    try
      if Node = nil then
      begin
        Node := FRoot.FirstChild;
        Stop := nil;
      end
      else
      begin
        Stop := Node.NextSibling;
        if Stop = nil then
        begin
          Stop := Node;
          repeat
            Stop := Stop.Parent;
          until (Stop = FRoot) or Assigned(Stop.NextSibling);
          if Stop = FRoot then
            Stop := nil
          else
            Stop := Stop.NextSibling;
        end;
      end;

      // Initialize the start node. Others will be initialized in GetNext.
      if not (vsInitialized in Node.States) then
        InitNode(Node);

      repeat
        if not (vsExpanded in Node.States) then
          ToggleNode(Node);
        Node := GetNext(Node);
      until Node = Stop;
    finally
      EndUpdate;
      DoStateChange([], [tsExpanding]);
    end;
  end;
end;

function TD2CustomTreeGrid.GetDisplayRect(Node: PD2TreeNode; Column: Integer; TextOnly: Boolean; Unclipped: Boolean;
                                          ApplyCellContentMargin: Boolean): TD2Rect;
// Определяет координаты клиентской области, которые занимает узел Node, в зависимости от положения скроллеров, развернутости узла и т.д.
// Если узел не может быть найден (потому что один из его родителей свернут или он невидим),
// то возвращается пустой прямоугольник.
// Если TextOnly = True, возвращаются только текстовые границы, т.е. левая и правая границы прямоугольника
// в соответствии с BidiMode (направление письма), выравниванием и шириной текста узла.
// Если Unclipped = True (только при TextOnly = True), тогда вычисленный прямоугольник текста не обрезается,
// если текст не полностью вписывается в текстовое поле. Это необходимо для вывода подсказок.
// Если ApplyCellContentMargin = True (только при TextOnly = True), тогда область для текста учитывает отступы ячейки
// Если Column = -1, то для определения ширины используется ширина всей клиентской области, иначе используется указанный столбец.
// Примечание. Столбец должен быть допустимым столбцом и используется независимо от того, является ли заголовок видимым или нет.

// Determines the client coordinates the given node covers, depending on scrolling, expand state etc.
// If the given node cannot be found (because one of its parents is collapsed or it is invisible) then an empty
// rectangle is returned.
// If TextOnly is True then only the text bounds are returned, that is, the resulting rectangle's left and right border
// are updated according to bidi mode, alignment and text width of the node.
// If Unclipped is True (which only makes sense if also TextOnly is True) then the calculated text rectangle is
// not clipped if the text does not entirely fit into the text space. This is special handling needed for hints.
// If ApplyCellContentMargin is True (which only makes sense if also TextOnly is True) then the calculated text
// rectangle respects the cell content margin.
// If Column is -1 then the entire client width is used before determining the node's width otherwise the bounds of the
// particular column are used.
// Note: Column must be a valid column and is used independent of whether the header is visible or not.

var
  Temp: PD2TreeNode;
  CacheIsAvailable: Boolean;
  Offset,
  Indent,
  TextWidth: Single;
  MainColumnHit: Boolean;
  //CurrentBidiMode: TBidiMode;

  CurrentAlignment: TAlignment;
  MaxUnclippedHeight: Single;
  TM: TTextMetric;
  ExtraVerticalMargin: Single;
  PColumn: TD2TreeColumn;
begin
  //{$ifdef DEBUG_VTV}Logger.EnterMethod([lcPaintHeader],'GetDisplayRect');{$endif}
  Assert(Assigned(Node), 'Node must not be nil.');
  Assert(Node <> FRoot, 'Node must not be the hidden root node.');

  MainColumnHit := (Column + 1) in [0, 1];
  if not (vsInitialized in Node.States) then
    InitNode(Node);

  Result := d2Rect(0, 0, 0, 0);

  //Проверка узла на видимость (определение уровня отступа)
  // Check whether the node is visible (determine indentation level btw.).
  if not IsEffectivelyVisible[Node] then Exit;
  Temp := Node;
  Indent := 0;

  if not (toFixedIndent in FOptions.FPaintOptions) then
  begin
    while Temp <> FRoot do
    begin
      if not (vsVisible in Temp.States) or                  //если узел не видим
         not (vsExpanded in Temp.Parent.States) then Exit;  //или если родитель свернут то выходим
      Temp := Temp.Parent;
      if MainColumnHit and (Temp <> FRoot)
        then d2IncFloat(Indent, FIndentWidth);
    end;
  end;//if not toFixedIndent

  // Здесь мы знаем, что узел виден.
  // Here we know the node is visible.
  Offset := 0;
  CacheIsAvailable := False;
  if tsUseCache in FStates then
  begin
    // Если можно использовать кеш позиции, то выполнить двоичный поиск, чтобы найти кешированный узел,
    // который как можно ближе к текущему узлу. Затем итерировать все следующие видимые узлы и суммировать их высоты.
    // If we can use the position cache then do a binary search to find a cached node which is as close as possible
    // to the current node. Iterate then through all following and visible nodes and sum up their heights.
    Temp := FindInPositionCache(Node, Offset);
    CacheIsAvailable := Assigned(Temp);
    while Assigned(Temp) and (Temp <> Node) do
    begin
      d2IncFloat(Offset, NodeHeight[Temp]);
      Temp := GetNextVisibleNoInit(Temp, True);
    end;
  end;
  if not CacheIsAvailable then
  begin
    // Если кеш недоступен, проходим через все узлы до корня и суммируем их высоты.
    // If the cache is not available then go straight through all nodes up to the root and sum up their heights.
    Temp := Node;
    repeat
      Temp := GetPreviousVisibleNoInit(Temp, True);
      if Temp = nil then
        Break;
      d2IncFloat(Offset, NodeHeight[Temp]);
    until False;
  end;

  Result := d2Rect(0, Offset, Max(FRangeX, ClientWidth), Offset + NodeHeight[Node]);

  // Ограничить левую и правую границы в указанном столбце (если есть). Limit left and right bounds to the given column (if any).
  if Column > NoColumn then GetColumnBounds(Column, Result.Left, Result.Right);

  // Ограничить границы слева и справа, если требуется только текстовая область. Limit left and right bounds further if only the text area is required.
  if TextOnly then
  begin
    // Начнем со смещения текста в столбце и рассмотрим также уровень отступа.
    // Если текст узла задействован, мы также должны учитывать направленность и выравнивание.
    // Start with the offset of the text in the column and consider the indentation level too.
    // If the text of a node is involved then we have to consider directionality and alignment too.
    if Column <= NoColumn
       then begin
              Offset := FMargins.Left + Indent;
              //CurrentBidiMode := BidiMode;
              //CurrentAlignment := Alignment;
            end
       else begin
              PColumn:=TD2TreeColumn(GetColumn(Column));
              Offset := PColumn.FMargins.Left + Indent;
              //CurrentBidiMode := PColumn.BidiMode;
              //CurrentAlignment := PColumn.Alignment;
              PColumn:=nil;
            end;

    if MainColumnHit then
    begin
      if toShowRoot in FOptions.FPaintOptions
         then d2IncFloat(Offset, FIndentWidth);
      //if (toCheckSupport in FOptions.FMiscOptions) {and Assigned(FCheckImages)} and (Node.CheckType <> ctNone)
      //   then d2IncFloat(Offset, FCheckImages.Width + 2);
    end;

    //// Consider associated images.
    //if Assigned(FStateImages) and HasImage(Node, ikState, Column) then
    //  d2IncFloat(Offset, FStateImages.Width + 2);
    //if Assigned(FImages) and HasImage(Node, ikNormal, Column) then
    //  d2IncFloat(Offset, GetNodeImageSize(Node).cx + 2);

    // Offset теперь содержит расстояние от левой или правой границы прямоугольника (в зависимости от режима bidi).
    // Теперь рассмотрим выравнивание и вычислим конечный результат.
    // Offset contains now the distance from the left or right border of the rectangle (depending on bidi mode).
    // Now consider the alignment too and calculate the final result.
    {if CurrentBidiMode = bdLeftToRight then
    begin}
      d2IncFloat(Result.Left, Offset);
      // Чтение слева-направо не требует специальной регулировки выравнивания
      // Left-to-right reading does not need any special adjustment of the alignment.
    {end
    else
    begin
      d2DecFloat(Result.Right, Offset);
      // Рассмотрим режим bidi. В контексте RTL выравнивание влево фактически означает выравнивание вправо и наоборот.
      // Consider bidi mode here. In RTL context does left alignment actually mean right alignment and vice versa.
      ChangeBiDiModeAlignment(CurrentAlignment);
    end;}

    TextWidth := DoGetNodeWidth(Node, Column);

    //Сохраним высоту ячейки перед тем, как применять квоту содержимого ячейки, чтобы увеличить
    //высоту ячейки, если текст не подходит и Unclipped = true (см. Ниже).
    // Keep cell height before applying cell content margin in order to increase cell height if text does not fit
    // and Unclipped it true (see below).
    MaxUnclippedHeight := Result.Bottom - Result.Top;

    //if ApplyCellContentMargin then
    //  DoBeforeCellPaint(Self.Canvas, Node, Column, cpmGetContentMargin, Result, Result);

    if Unclipped then
    begin
      // The caller requested the text coordinates unclipped. This means they must be calculated so as would
      // there be enough space, regardless of column bounds etc.
      // The layout still depends on the available space too, because this determines the position
      // of the unclipped text rectangle.
      {
      if Result.Right - Result.Left < TextWidth - 1 then
        if CurrentBidiMode = bdLeftToRight then
          CurrentAlignment := taLeftJustify
        else
          CurrentAlignment := taRightJustify;
       }

      // Increase cell height (up to MaxUnclippedHeight determined above) if text does not fit.
      GetTextMetrics(Self.Canvas.Handle, TM);
      ExtraVerticalMargin := Math.Min(TM.tmHeight, MaxUnclippedHeight) - (Result.Bottom - Result.Top);
      if ExtraVerticalMargin > 0 then
        d2InflateRect(Result, 0, (ExtraVerticalMargin + 1) / 2);

      case CurrentAlignment of
        taCenter:
          begin
            Result.Left := (Result.Left + Result.Right - TextWidth) / 2;
            Result.Right := Result.Left + TextWidth;
          end;
        taRightJustify:
          Result.Left := Result.Right - TextWidth;
      else // taLeftJustify
        Result.Right := Result.Left + TextWidth - 1;
      end;
    end
    else
      // Modify rectangle only if the text fits entirely into the given room.
      if Result.Right - Result.Left > TextWidth then
        case CurrentAlignment of
          taCenter:
            begin
              Result.Left := (Result.Left + Result.Right - TextWidth) / 2;
              Result.Right := Result.Left + TextWidth;
            end;
          taRightJustify:
            Result.Left := Result.Right - TextWidth;
        else // taLeftJustify
          Result.Right := Result.Left + TextWidth;
        end;
  end;
  //lclheader
  //todo: add a parameter to decide if the result must be returned as
  //a tree offset or a control offset
  //if ShowHeader then d2OffsetRect(Result, 0, FHeader.Height);

end;

function TD2CustomTreeGrid.GetEffectivelyFiltered(Node: PD2TreeNode): Boolean;
//True -  если узел Node эффективно отфильтрован.
//Это зависит от статуса узла и опции отрисовки отфильтрованных узлов.
// Checks if a node is effectively filtered out. This depends on the nodes state and the paint options.
begin
  if Assigned(Node)
    then Result := (vsFiltered in Node.States) and not (toShowFilteredNodes in FOptions.FPaintOptions)
    else Result := False;
end;

function TD2CustomTreeGrid.GetEffectivelyVisible(Node: PD2TreeNode): Boolean;
//True -  если узел Node эффективно видим.
//Это зависит от статуса узла и опции отрисовки отфильтрованных узлов.
begin
  if Assigned(Node)
    then Result := (vsVisible in Node.States) and not IsEffectivelyFiltered[Node]
    else Result := False;
end;

function TD2CustomTreeGrid.GetFirst(ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает первый узел в дереве, принимая во внимание опцию toChildrenAbove.
// Returns the first node in the tree while optionally considering toChildrenAbove.

begin
  if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
  begin
    if vsHasChildren in FRoot.States then
    begin
      Result := FRoot;

      // Child nodes are the first choice if possible.
      if Assigned(Result.FirstChild) then
      begin
        while Assigned(Result.FirstChild) do
        begin
          Result := Result.FirstChild;
          if not (vsInitialized in Result.States) then
            InitNode(Result);

          if (vsHasChildren in Result.States) and (Result.ChildCount = 0) then
            InitChildren(Result);
        end;
      end
      else
        Result := nil;
    end
    else
      Result := nil;
  end
  else
    Result := FRoot.FirstChild;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetFirstChecked(State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает первый узел в дереве с заданным состоянием отметки
// Returns the first node in the tree with the given check state.

begin
  Result := GetNextChecked(nil, State, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetFirstChild(Node: PD2TreeNode): PD2TreeNode;
//Возвращает первый дочерний элемент узла Node. Узел Node и возвращаемый узел инициализируется если не инициализированы.
// Returns the first child of the given node. The result node is initialized before exit.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.FirstChild
  else
  begin
    if not (vsInitialized in Node.States) then
      InitNode(Node);
    if vsHasChildren in Node.States then
    begin
      if Node.ChildCount = 0 then
        InitChildren(Node);
      Result := Node.FirstChild;
    end
    else
      Result := nil;
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetFirstChildNoInit(Node: PD2TreeNode): PD2TreeNode;
//Возвращает первый дочерний элемент узла Node, без их инициализации.
// Determines the first child of the given node but does not initialize it.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.FirstChild
  else
  begin
    if vsHasChildren in Node.States then
      Result := Node.FirstChild
    else
      Result := nil;
  end;
end;

function TD2CustomTreeGrid.GetFirstCutCopy(ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает первый узел в дереве, который в настоящее время помечен для операции с буфером обмена.
// См. Также GetNextCutCopy для комментариев об инициализации.

// Returns the first node in the tree which is currently marked for a clipboard operation.
// See also GetNextCutCopy for comments on initialization.

begin
  Result := GetNextCutCopy(nil, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetFirstInitialized(ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает первый инициализированный узел.
// Returns the first node which is already initialized.

begin
  Result := GetFirstNoInit(ConsiderChildrenAbove);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    Result := GetNextInitialized(Result, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetFirstLeaf: PD2TreeNode;
// Возвращает первый узел в дереве, у которого нет детей. Результат при необходимости инициализируется.

// Returns the first node in the tree which has currently no children.
// The result is initialized if necessary.

begin
  Result := GetNextLeaf(nil);
end;

function TD2CustomTreeGrid.GetFirstLevel(NodeLevel: Cardinal): PD2TreeNode;
// Возвращает первый узел в дереве на указанном уровне. Результат при необходимости инициализируется.
// Returns the first node in the tree on a specific level.
// The result is initialized if necessary.

begin
  Result := GetFirstNoInit(True);
  while Assigned(Result) and (GetNodeLevel(Result) <> NodeLevel) do
    Result := GetNextNoInit(Result, True);

  if Assigned(Result) and (GetNodeLevel(Result) <> NodeLevel) then // i.e. there is no node with the desired level in the tree
    Result := nil;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetFirstNoInit(ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает первый узел в дереве, c учетом опции toChildrenAbove (необязательно). Инициализация не выполняется.

// Returns the first node in the tree while optionally considering toChildrenAbove.
// No initialization is performed.

begin
  if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
  begin
    if vsHasChildren in FRoot.States then
    begin
      Result := FRoot;

      // Child nodes are the first choice if possible.
      if Assigned(Result.FirstChild) then
      begin
        while Assigned(Result.FirstChild) do
          Result := Result.FirstChild;
      end
      else
        Result := nil;
    end
    else
      Result := nil;
  end
  else
    Result := FRoot.FirstChild;
end;

function TD2CustomTreeGrid.GetFirstSelected(ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает первый выбранный узел с учетом опции toChildrenAbove (необязательно).
// Returns the first node in the current selection while optionally considering toChildrenAbove.

begin
  Result := GetNextSelected(nil, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetFirstVisible(Node: PD2TreeNode = nil; ConsiderChildrenAbove: Boolean = True;
  IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает первый видимый узел в дереве, с учетом опции toChildrenAbove (необязательно). Если необходимо узлы инициализируются.
// Returns the first visible node in the tree while optionally considering toChildrenAbove.
// If necessary nodes are initialized on demand.

begin
  Result := Node;
  if not Assigned(Result) then
    Result := FRoot;

  if vsHasChildren in Result.States then
  begin
    if Result.ChildCount = 0 then
      InitChildren(Result);

    // Child nodes are the first choice if possible.
    if Assigned(Result.FirstChild) then
    begin
      Result := GetFirstChild(Result);

      if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
      begin
        repeat
          // Search the first visible sibling.
          while Assigned(Result.NextSibling) and not (vsVisible in Result.States) do
          begin
            Result := Result.NextSibling;
            // Init node on demand as this might change the visibility.
            if not (vsInitialized in Result.States) then
              InitNode(Result);
          end;

          // If there are no visible siblings take the parent.
          if not (vsVisible in Result.States) then
          begin
            Result := Result.Parent;
            if Result = FRoot then
              Result := nil;
            Break;
          end
          else
          begin
            if (vsHasChildren in Result.States) and (Result.ChildCount = 0) then
              InitChildren(Result);
            if (not Assigned(Result.FirstChild)) or (not (vsExpanded in Result.States)) then
              Break;
          end;

          Result := Result.FirstChild;
          if not (vsInitialized in Result.States) then
            InitNode(Result);
        until False;
      end
      else
      begin
        // If there are no children or the first child is not visible then search the sibling nodes or traverse parents.
        if not (vsVisible in Result.States) then
        begin
          repeat
            // Is there a next sibling?
            if Assigned(Result.NextSibling) then
            begin
              Result := Result.NextSibling;
              // The visible state can be removed during initialization so init the node first.
              if not (vsInitialized in Result.States) then
                InitNode(Result);
              if vsVisible in Result.States then
                Break;
            end
            else
            begin
              // No sibling anymore, so use the parent's next sibling.
              if Result.Parent <> FRoot then
                Result := Result.Parent
              else
              begin
                // There are no further nodes to examine, hence there is no further visible node.
                Result := nil;
                Break;
              end;
            end;
          until False;
        end;
      end;
    end
    else
      Result := nil;
  end
  else
    Result := nil;

  if Assigned(Result) and not IncludeFiltered and IsEffectivelyFiltered[Result] then
    Result := GetNextVisible(Result);
end;

function TD2CustomTreeGrid.GetFirstVisibleChild(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает первый видимый дочерний узел узла. При необходимости узлы инициализируются.
// Returns the first visible child node of Node. If necessary nodes are initialized on demand.

begin
  if Node = nil then
    Node := FRoot;
  Result := GetFirstChild(Node);

  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetNextVisibleSibling(Result, IncludeFiltered);
end;

function TD2CustomTreeGrid.GetFirstVisibleChildNoInit(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает первый видимый дочерний узел узла. Узлы не инициализируются
// Returns the first visible child node of Node.

begin
  if Node = nil then
    Node := FRoot;
  Result := Node.FirstChild;
  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetNextVisibleSiblingNoInit(Result, IncludeFiltered);
end;

function TD2CustomTreeGrid.GetFirstVisibleNoInit(Node: PD2TreeNode = nil;
  ConsiderChildrenAbove: Boolean = True; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает первый видимый узел в дереве или в заданном поддереве, с учетом опции toChildrenAbove (необязательно). Узлы не инициализируются

// Returns the first visible node in the tree or given subtree while optionally considering toChildrenAbove.
// No initialization is performed.

begin
  Result := Node;
  if not Assigned(Result)
    then Result := FRoot;

  if vsHasChildren in Result.States
    then  begin
            // Child nodes are the first choice if possible.
            if Assigned(Result.FirstChild)
              then  begin
                      Result := Result.FirstChild;

                      if ConsiderChildrenAbove and
                         (toChildrenAbove in FOptions.FPaintOptions)
                        then  begin
                                repeat
                                  // Search the first visible sibling.
                                  while Assigned(Result.NextSibling) and
                                        not (vsVisible in Result.States) do
                                    Result := Result.NextSibling;

                                  // If there a no visible siblings take the parent.
                                  if not (vsVisible in Result.States)
                                    then  begin
                                            Result := Result.Parent;
                                            if Result = FRoot then
                                              Result := nil;
                                            Break;
                                          end
                                    else if (not Assigned(Result.FirstChild)) or
                                            (not (vsExpanded in Result.States))
                                           then Break;
                                  Result := Result.FirstChild;
                                until False;
                              end
                        else  begin
                                // If there are no children or the first child is not visible then search the sibling nodes or traverse parents.
                                if not (vsVisible in Result.States)
                                  then  begin
                                          repeat
                                            // Is there a next sibling?
                                            if Assigned(Result.NextSibling) then
                                            begin
                                              Result := Result.NextSibling;
                                              if vsVisible in Result.States then
                                                Break;
                                            end
                                            else
                                            begin
                                              // No sibling anymore, so use the parent's next sibling.
                                              if Result.Parent <> FRoot then
                                                Result := Result.Parent
                                              else
                                              begin
                                                // There are no further nodes to examine, hence there is no further visible node.
                                                Result := nil;
                                                Break;
                                              end;
                                            end;
                                          until False;
                                        end;
                              end;
                    end
              else Result := nil;
          end
    else Result := nil;

  if Assigned(Result) and not IncludeFiltered and IsEffectivelyFiltered[Result] then
    Result := GetNextVisibleNoInit(Result);
end;


function TD2CustomTreeGrid.GetLast(Node: PD2TreeNode = nil; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает самый последний узел в ветке, заданной узлом, и инициализирует узлы до конца, включая результат.
//Учитывается опция toChildrenAbove (необязательно). Если Node = nil, то возвращается последний узел в дереве.
// Returns the very last node in the tree branch given by Node and initializes the nodes all the way down including the
// result. toChildrenAbove is optionally considered. By using Node = nil the very last node in the tree is returned.

var
  Next: PD2TreeNode;

begin
  Result := GetLastChild(Node);
  if not ConsiderChildrenAbove or not (toChildrenAbove in FOptions.FPaintOptions) then
    while Assigned(Result) do
    begin
      // Test if there is a next last child. If not keep the node from the last run.
      // Otherwise use the next last child.
      Next := GetLastChild(Result);
      if Next = nil then
        Break;
      Result := Next;
    end;
end;

function TD2CustomTreeGrid.GetLastInitialized(Node: PD2TreeNode = nil;
  ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает самый последний инициализированный дочерний узел в ветке узла Node. Учитывается опция toChildrenAbove (необязательно)
// Returns the very last initialized child node in the tree branch given by Node.

begin
  Result := GetLastNoInit(Node, ConsiderChildrenAbove);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    Result := GetPreviousInitialized(Result, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetLastNoInit(Node: PD2TreeNode = nil; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает самый последний узел в ветке узла Node. Инициализация не выполеняется. Учитывается опция toChildrenAbove (необязательно)
// Returns the very last node in the tree branch given by Node without initialization.

var
  Next: PD2TreeNode;

begin
  Result := GetLastChildNoInit(Node);
  if not ConsiderChildrenAbove or not (toChildrenAbove in FOptions.FPaintOptions) then
    while Assigned(Result) do
    begin
      // Test if there is a next last child. If not keep the node from the last run.
      // Otherwise use the next last child.
      Next := GetLastChildNoInit(Result);
      if Next = nil then
        Break;
      Result := Next;
    end;
end;

function TD2CustomTreeGrid.GetLastChild(Node: PD2TreeNode): PD2TreeNode;
//Возвращяет последний дочерний элемент узла Node и инициализирует его, если он есть.
// Determines the last child of the given node and initializes it if there is one.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.LastChild
  else
  begin
    if not (vsInitialized in Node.States) then
      InitNode(Node);
    if vsHasChildren in Node.States then
    begin
      if Node.ChildCount = 0 then
        InitChildren(Node);
      Result := Node.LastChild;
    end
    else
      Result := nil;
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetLastChildNoInit(Node: PD2TreeNode): PD2TreeNode;
//Возвращяет последний дочерний элемент узла Node, но не инициализирует его.
// Determines the last child of the given node but does not initialize it.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.LastChild
  else
  begin
    if vsHasChildren in Node.States then
      Result := Node.LastChild
    else
      Result := nil;
  end;
end;

function TD2CustomTreeGrid.GetLastVisible(Node: PD2TreeNode = nil; ConsiderChildrenAbove: Boolean = True;
  IncludeFiltered: Boolean = False): PD2TreeNode;
//Возвращает самый последний видимый узел в дереве с учетом опции toChildrenAbove (необязательно).
//Узлы полностью инициализируются, включая узел результата.
// Returns the very last visible node in the tree while optionally considering toChildrenAbove.
// The nodes are intialized all the way up including the result node.
var Run: PD2TreeNode;
begin
  Result := GetLastVisibleNoInit(Node, ConsiderChildrenAbove);

  Run := Result;
  while Assigned(Run) and (Run <> Node)  and (Run <> RootNode) do
  begin
    if not (vsInitialized in Run.States) then
      InitNode(Run);
    Run := Run.Parent;
  end;
end;

function TD2CustomTreeGrid.GetLastVisibleChild(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает последний видимый дочерний узел для узла Node и при необходимости инициализирует его.
// Determines the last visible child of the given node and initializes it if necessary.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := GetLastChild(FRoot)
  else
    if FullyVisible[Node] and (vsExpanded in Node.States) then
      Result := GetLastChild(Node)
    else
      Result := nil;

  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetPreviousVisibleSibling(Result, IncludeFiltered);

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetLastVisibleChildNoInit(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
//Возвращает последний видимый дочерний узел для узла Node без инициализации.
// Determines the last visible child of the given node without initialization.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := GetLastChildNoInit(FRoot)
  else
    if FullyVisible[Node] and (vsExpanded in Node.States) then
      Result := GetLastChildNoInit(Node)
    else
      Result := nil;

  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetPreviousVisibleSiblingNoInit(Result, IncludeFiltered);
end;

function TD2CustomTreeGrid.GetLastVisibleNoInit(Node: PD2TreeNode = nil;
  ConsiderChildrenAbove: Boolean = True; IncludeFiltered: Boolean = False): PD2TreeNode;
//Возвращает самый последний видимый узел в дереве с учетом опции toChildrenAbove (необязательно). Инициализация не выполняется.
// Returns the very last visible node in the tree while optionally considering toChildrenAbove.
// No initialization is performed.

begin
  Result := GetLastNoInit(Node, ConsiderChildrenAbove);
  while Assigned(Result) and (Result <> Node) do
  begin
    if FullyVisible[Result] and
       (IncludeFiltered or not IsEffectivelyFiltered[Result]) then
      Break;
    Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);
  end;

  if (Result = Node) then // i.e. there is no visible node
    Result := nil;
end;

function TD2CustomTreeGrid.GetNext(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает следующий узел в дереве, с учетом поции toChildrenAbove (необязательно). Результат инициализируется если нужно.
// Returns next node in tree while optionally considering toChildrenAbove. The Result will be initialized if needed.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
    begin
      // If this node has no siblings use the parent.
      if not Assigned(Result.NextSibling) then
      begin
        Result := Result.Parent;
        if Result = FRoot then
        begin
          Result := nil;
        end;
      end
      else
      begin
        // There is at least one sibling so take it.
        Result := Result.NextSibling;

        // Has this node got children? Initialize them if necessary.
        if (vsHasChildren in Result.States) and (Result.ChildCount = 0) then
          InitChildren(Result);

        // Now take a look at the children.
        while Assigned(Result.FirstChild) do
        begin
          Result := Result.FirstChild;
          if (vsHasChildren in Result.States) and (Result.ChildCount = 0) then
            InitChildren(Result);
        end;
      end;
    end
    else
    begin
      // Has this node got children?
      if vsHasChildren in Result.States then
      begin
        // Yes, there are child nodes. Initialize them if necessary.
        if Result.ChildCount = 0 then
          InitChildren(Result);
      end;

      // if there is no child node try siblings
      if Assigned(Result.FirstChild) then
        Result := Result.FirstChild
      else
      begin
        repeat
          // Is there a next sibling?
          if Assigned(Result.NextSibling) then
          begin
            Result := Result.NextSibling;
            Break;
          end
          else
          begin
            // No sibling anymore, so use the parent's next sibling.
            if Result.Parent <> FRoot then
              Result := Result.Parent
            else
            begin
              // There are no further nodes to examine, hence there is no further visible node.
              Result := nil;
              Break;
            end;
          end;
        until False;
      end;
    end;
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetNextChecked(Node: PD2TreeNode; State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает следующий узел начиная после Node с состоянием отметки State, с учетом поции toChildrenAbove (необязательно)
begin
  if (Node = nil) or (Node = FRoot)
    then Result := GetFirstNoInit(ConsiderChildrenAbove)
    else Result := GetNextNoInit(Node, ConsiderChildrenAbove);

  while Assigned(Result) and (Result.CheckState <> State) do
    Result := GetNextNoInit(Result, ConsiderChildrenAbove);

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetNextChecked(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean): PD2TreeNode;
//Возвращает следующий узел после Node с состоянием отметки csCheckedNormal, с учетом поции toChildrenAbove (необязательно)
begin
  Result := Self.GetNextChecked(Node, csCheckedNormal, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetNextCutCopy(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает следующий после Node узел в дереве, который в настоящее время помечен для операции с буфером обмена.
// Поскольку только видимые узлы (или скрытые после их маркировки) могут быть помечены, то нет необходимости
// проверять на инициализированость узлы. Результат, однако, при необходимости инициализируется.

// Returns the next node in the tree which is currently marked for a clipboard operation. Since only visible nodes can
// be marked (or they are hidden after they have been marked) it is not necessary to initialize nodes to check for
// child nodes. The result, however, is initialized if necessary.

begin
  if ClipboardStates * FStates <> []
    then  begin
            if (Node = nil) or (Node = FRoot)
              then Result := GetFirstNoInit(ConsiderChildrenAbove)
              else Result := GetNextNoInit(Node, ConsiderChildrenAbove);

            while Assigned(Result) and not (vsCutOrCopy in Result.States) do
              Result := GetNextNoInit(Result, ConsiderChildrenAbove);
            if Assigned(Result) and not (vsInitialized in Result.States)
              then InitNode(Result);
          end
    else Result := nil;
end;

function TD2CustomTreeGrid.GetNextInitialized(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает следующий инициализированный после Node узел, с учетом поции toChildrenAbove (необязательно)
// Returns the next node in tree which is initialized.

begin
  Result := Node;
  repeat
    Result := GetNextNoInit(Result, ConsiderChildrenAbove);
  until (Result = nil) or (vsInitialized in Result.States);
end;

function TD2CustomTreeGrid.GetNextLeaf(Node: PD2TreeNode): PD2TreeNode;
// Возвращает следующий узел в дереве у которого нет детей. Результат при необходимости инициализируется.
// Returns the next node in the tree which has currently no children.
// The result is initialized if necessary.

begin
  if (Node = nil) or (Node = FRoot)
    then Result := FRoot.FirstChild
    else Result := GetNext(Node);
  while Assigned(Result) and (vsHasChildren in Result.States) do
    Result := GetNext(Result);
  if Assigned(Result) and not (vsInitialized in Result.States)
    then InitNode(Result);
end;

function TD2CustomTreeGrid.GetNextLevel(Node: PD2TreeNode; NodeLevel: Cardinal): PD2TreeNode;
// Возвращает следующий после Node узел в дереве определенного уровня. Результат при необходимости инициализируется.
// Returns the next node in the tree on a specific level.
// The result is initialized if necessary.

var
  StartNodeLevel: Cardinal;

begin
  Result := nil;

  if Assigned(Node) and (Node <> FRoot) then
  begin
    StartNodeLevel := GetNodeLevel(Node);

    if StartNodeLevel < NodeLevel then
    begin
      Result := GetNext(Node);
      if Assigned(Result) and (GetNodeLevel(Result) <> NodeLevel) then
        Result := GetNextLevel(Result, NodeLevel);
    end
    else
      if StartNodeLevel = NodeLevel then
      begin
        Result := Node.NextSibling;
        if not Assigned(Result) then // i.e. start node was a last sibling
        begin
          Result := Node.Parent;
          if Assigned(Result) then
          begin
            // go to next anchestor of the start node which has a next sibling (if exists)
            while Assigned(Result) and not Assigned(Result.NextSibling) do
              Result := Result.Parent;
            if Assigned(Result) then
              Result := GetNextLevel(Result.NextSibling, NodeLevel);
          end;
        end;
      end
      else
        // i.e. StartNodeLevel > NodeLevel
        Result := GetNextLevel(Node.Parent, NodeLevel);
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetNextNoInit(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean): PD2TreeNode;
// Возвращает следующий узел в дереве, с учетом поции toChildrenAbove (необязательно). Результаты не инициализируются.
// Optimized version of GetNext performing no initialization, but optionally considering toChildrenAbove.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
    begin
      // If this node has no siblings use the parent.
      if not Assigned(Result.NextSibling) then
      begin
        Result := Result.Parent;
        if Result = FRoot then
        begin
          Result := nil;
        end;
      end
      else
      begin
        // There is at least one sibling so take it.
        Result := Result.NextSibling;

        // Now take a look at the children.
        while Assigned(Result.FirstChild) do
        begin
          Result := Result.FirstChild;
        end;
      end;
    end
    else
    begin
      // If there is no child node try siblings.
      if Assigned(Result.FirstChild) then
        Result := Result.FirstChild
      else
      begin
        repeat
          // Is there a next sibling?
          if Assigned(Result.NextSibling) then
          begin
            Result := Result.NextSibling;
            Break;
          end
          else
          begin
            // No sibling anymore, so use the parent's next sibling.
            if Result.Parent <> FRoot then
              Result := Result.Parent
            else
            begin
              // There are no further nodes to examine, hence there is no further visible node.
              Result := nil;
              Break;
            end;
          end;
        until False;
      end;
    end;
  end;
end;

function TD2CustomTreeGrid.GetNextSelected(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает следующий после Node выбранный узел в дереве с инициализацией результата.
// Поскольку дети не ниализованных узлов не могут быть в текущем выборе (потому что они еще не существуют),
// то здесь нет необходимости инициализировать узлы.
// Returns the next node in the tree which is currently selected. Since children of unitialized nodes cannot be
// in the current selection (because they simply do not exist yet) it is not necessary to initialize nodes here.
// The result however is initialized if necessary.

begin
  if FSelectionCount > 0 then
  begin
    if (Node = nil) or (Node = FRoot)
      then Result := GetFirstNoInit(ConsiderChildrenAbove)
      else Result := GetNextNoInit(Node, ConsiderChildrenAbove);
    while Assigned(Result) and not (vsSelected in Result.States) do
      Result := GetNextNoInit(Result, ConsiderChildrenAbove);
    if Assigned(Result) and not (vsInitialized in Result.States)
      then InitNode(Result);
  end
  else Result := nil;
end;

function TD2CustomTreeGrid.GetNextSibling(Node: PD2TreeNode): PD2TreeNode;
// Возвращает следующий братский узел для узла Node и при необходимости инициализирует его.
// Returns the next sibling of Node and initializes it if necessary.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.NextSibling;
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end;
end;

function TD2CustomTreeGrid.GetNextSiblingNoInit(Node: PD2TreeNode): PD2TreeNode;
// Возвращает следующий братский узел для узла Node без его инициализации.
// Returns the next sibling of Node.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.NextSibling;
  end;
end;

function TD2CustomTreeGrid.GetNextVisible(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = True): PD2TreeNode;
// Возвращает следующий узел после Node, который является видимым с инициализацией всех узлов и с учетом опции toChildrenAbove.
// Returns next node in tree, with regard to Node, which is visible.
// Nodes which need an initialization (including the result) are initialized.
// toChildrenAbove is optionally considered which is the default here.

var
  ForceSearch: Boolean;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    repeat
      // If the given node is not visible then look for a parent node which is visible, otherwise we will
      // likely go unnecessarily through a whole bunch of invisible nodes.
      if not FullyVisible[Result] then
        Result := GetVisibleParent(Result, True);

      if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
      begin
        repeat
          // If there a no siblings anymore, go up one level.
          if not Assigned(Result.NextSibling) then
          begin
            Result := Result.Parent;
            if Result = FRoot then
            begin
              Result := nil;
              Break;
            end;

            if not (vsInitialized in Result.States) then
              InitNode(Result);
            if vsVisible in Result.States then
              Break;
          end
          else
          begin
            // There is at least one sibling so take it.
            Result := Result.NextSibling;
            if not (vsInitialized in Result.States) then
              InitNode(Result);
            if not (vsVisible in Result.States) then
              Continue;

            // Now take a look at the children.
            // As the children are initialized while toggling, we don't need to do this here.
            while (vsExpanded in Result.States) and Assigned(Result.FirstChild) do
            begin
              Result := Result.FirstChild;
              if not (vsInitialized in Result.States) then
                InitNode(Result);
              if not (vsVisible in Result.States) then
                Break;
            end;

            // If we found a visible node we don't need to search any longer.
            if vsVisible in Result.States then
              Break;
          end;
        until False;
      end
      else
      begin
        // Has this node got children?
        if [vsHasChildren, vsExpanded] * Result.States = [vsHasChildren, vsExpanded] then
        begin
          // Yes, there are child nodes. Initialize them if necessary.
          if Result.ChildCount = 0 then
            InitChildren(Result);
        end;

        // Child nodes are the first choice if possible.
        if (vsExpanded in Result.States) and Assigned(Result.FirstChild) then
        begin
          Result := GetFirstChild(Result);
          ForceSearch := False;
        end
        else
          ForceSearch := True;

        // If there are no children or the first child is not visible then search the sibling nodes or traverse parents.
        if Assigned(Result) and (ForceSearch or not (vsVisible in Result.States)) then
        begin
          repeat
            // Is there a next sibling?
            if Assigned(Result.NextSibling) then
            begin
              Result := Result.NextSibling;
              if not (vsInitialized in Result.States) then
                InitNode(Result);
              if vsVisible in Result.States then
                Break;
            end
            else
            begin
              // No sibling anymore, so use the parent's next sibling.
              if Result.Parent <> FRoot then
                Result := Result.Parent
              else
              begin
                // There are no further nodes to examine, hence there is no further visible node.
                Result := nil;
                Break;
              end;
            end;
          until False;
        end;
      end;
    until not Assigned(Result) or IsEffectivelyVisible[Result];
  end;
end;

function TD2CustomTreeGrid.GetNextVisibleNoInit(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = True): PD2TreeNode;
// Возвращает следующий узел после Node, который является видимым без инициализации и с учетом опции toChildrenAbove.
// Returns the next node in tree, with regard to Node, which is visible.
// toChildrenAbove is optionally considered (which is the default). No initialization is done.

var
  ForceSearch: Boolean;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    repeat
      if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
      begin
        repeat
          // If there a no siblings anymore, go up one level.
          if not Assigned(Result.NextSibling) then
          begin
            Result := Result.Parent;
            if Result = FRoot then
            begin
              Result := nil;
              Break;
            end;
            if vsVisible in Result.States then
              Break;
          end
          else
          begin
            // There is at least one sibling so take it.
            Result := Result.NextSibling;
            if not (vsVisible in Result.States) then
              Continue;

            // Now take a look at the children.
            while (vsExpanded in Result.States) and Assigned(Result.FirstChild) do
            begin
              Result := Result.FirstChild;
              if not (vsVisible in Result.States) then
                Break;
            end;

            // If we found a visible node we don't need to search any longer.
            if vsVisible in Result.States then
              Break;
          end;
        until False;
      end
      else
      begin
        // If the given node is not visible then look for a parent node which is visible, otherwise we will
        // likely go unnecessarily through a whole bunch of invisible nodes.
        if not FullyVisible[Result] then
          Result := GetVisibleParent(Result, True);

        // Child nodes are the first choice if possible.
        if (vsExpanded in Result.States) and Assigned(Result.FirstChild) then
        begin
          Result := Result.FirstChild;
          ForceSearch := False;
        end
        else
          ForceSearch := True;

        // If there are no children or the first child is not visible then search the sibling nodes or traverse parents.
        if ForceSearch or not (vsVisible in Result.States) then
        begin
          repeat
            // Is there a next sibling?
            if Assigned(Result.NextSibling) then
            begin
              Result := Result.NextSibling;
              if vsVisible in Result.States then
                Break;
            end
            else
            begin
              // No sibling anymore, so use the parent's next sibling.
              if Result.Parent <> FRoot then
                Result := Result.Parent
              else
              begin
                // There are no further nodes to examine, hence there is no further visible node.
                Result := nil;
                Break;
              end;
            end;
          until False;
        end;
      end;
    until not Assigned(Result) or IsEffectivelyVisible[Result];
  end;
end;

function TD2CustomTreeGrid.GetNextVisibleSibling(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает следующий видимый братский узел для Node с инициализацией
// Returns the next visible sibling after Node. Initialization is done implicitly.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := GetNextSibling(Result);
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2CustomTreeGrid.GetNodeAt(X, Y: Single; Relative: Boolean; var NodeTop: Single; var NodeNum: integer): PD2TreeNode;
//Получить узел по координатам X и Y.
//При Relative = True  X и Y задаются в координатах клиентской области ,
//иначе в абсолютных координатах всего виртуального дерева (без смещения в окне дерева).
//NodeTop получает значение Position.Y возвращаемого узла или не изменняется если узел не найден

//var
//  OffsetByHeader: Boolean;
begin
  ////lclheader
  //OffsetByHeader := Relative and FHeader.Visible;
  //if OffsetByHeader then
  //  d2DecFloat(Y, FHeader.Height);
  Result := InternalGetNodeAt(X, Y, Relative, NodeTop, NodeNum);
  //lclheader
  //if OffsetByHeader then
  //  d2IncFloat(NodeTop, FHeader.Height);
end;

function TD2CustomTreeGrid.GetNodeAt(const P: TD2Point): PD2TreeNode;
//Получить узел по координатам P.X и P.Y клиентской области(Перегруженный вариант  функции GetNodeAt).
begin
  Result := GetNodeAt(P.X, P.Y);
end;

function TD2CustomTreeGrid.GetNextVisibleSiblingNoInit(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает следующий видимый братский узел для Node без инициализацией
// Returns the next visible sibling after Node.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := Result.NextSibling;
  until not Assigned(Result) or ((vsVisible in Result.States) and
       (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2CustomTreeGrid.GetNodeAt(X, Y: Single): PD2TreeNode;
//Получить узел по координатам X и Y клиентской области (перегруженный вариант  функции GetNodeAt)

// Overloaded variant of GetNodeAt to easy life of application developers which do not need to have the exact
// top position returned and always use client coordinates.

var
  Dummy: Single;
  DummyI: Integer;
begin
  Result := GetNodeAt(X, Y, True, Dummy, DummyI);
end;

function TD2CustomTreeGrid.GetNodeAtTree(X, Y: Single): PD2TreeNode;
//Получить узел по координатам X и Y дерева(перегруженный вариант  функции GetNodeAt).

var
  Dummy: Single;
  DummyI: Integer;
begin
  Result := GetNodeAt(X, Y, False, Dummy, DummyI);
end;

function TD2CustomTreeGrid.GetNodeData(Node: PD2TreeNode): Pointer;
// Возвращает адрес определяемой пользователем области данных для узла Node.
// Returns the address of the user defined data area in the node.

begin
  Assert(FNodeDataSize > 0, 'NodeDataSize not initialized.');
  if (FNodeDataSize <= 0) or (Node = nil) or (Node = FRoot) then
    Result := nil
  else
  begin
    Result := PByte(@Node.Data) + FTotalInternalDataSize;
    Include(Node.States, vsOnFreeNodeCallRequired); // We now need to call OnFreeNode, see bug #323
  end;
end;

function TD2CustomTreeGrid.GetNodeLevel(Node: PD2TreeNode): Cardinal;
// Возвращает уровень узла Node
// returns the level of the given node

var
  Run: PD2TreeNode;

begin
  Result := 0;
  if Assigned(Node) and (Node <> FRoot) then
  begin
    Run := Node.Parent;
    while Run <> FRoot do
    begin
      Run := Run.Parent;
      Inc(Result);
    end;
  end;
end;

function TD2CustomTreeGrid.GetPrevious(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает предыдущий перед Node узел в дереве с учетом опции toChildrenAbove (необязательно) с инициализацией узлов
// Если ConsiderChildrenAbove=true, то функция учитывает установлен ли toChildrenAbove,
// иначе всегда возвращается предыдущий узел в порядке сверху вниз, независимо от текущего
// состояния PaintOptions. Результат будет инициализирован, если необходимо.

// Returns previous node in tree. If ConsiderChildrenAbove is True the function considers
// whether toChildrenAbove is currently set, otherwise the result will always be the previous
// node in top-down order regardless of the current PaintOptions.
// The Result will be initialized if needed.

var
  Run: PD2TreeNode;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
    begin
      // Has this node got children? Initialize them if necessary.
      if (vsHasChildren in Result.States) and (Result.ChildCount = 0) then
        InitChildren(Result);

      // If there is a last child, take it; if not try the previous sibling.
      if Assigned(Result.LastChild) then
        Result := Result.LastChild
      else
        if Assigned(Result.PrevSibling) then
           Result := Result.PrevSibling
      else
      begin
        // If neither a last child nor a previous sibling exist, go the tree upwards and
        // look, wether one of the parent nodes have a previous sibling. If not the result
        // will ne nil.
        repeat
          Result := Result.Parent;
          Run := nil;
          if Result <> FRoot then
            Run := Result.PrevSibling
          else
            Result := nil;
        until Assigned(Run) or (Result = nil);

        if Assigned(Run) then
          Result := Run;
      end;
    end
    else
    begin
      // Is there a previous sibling?
      if Assigned(Node.PrevSibling) then
      begin
        // Go down and find the last child node.
        Result := GetLast(Node.PrevSibling);
        if Result = nil then
          Result := Node.PrevSibling;
      end
      else
        // no previous sibling so the parent of the node is the previous visible node
        if Node.Parent <> FRoot then
          Result := Node.Parent
        else
          Result := nil;
    end;
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetPreviousChecked(Node: PD2TreeNode; State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает предыдущий узел перед Node с состоянием отметки State, с учетом поции toChildrenAbove (необязательно)
begin
  if (Node = nil) or (Node = FRoot) then
    Result := GetLastNoInit(nil, ConsiderChildrenAbove)
  else
    Result := GetPreviousNoInit(Node, ConsiderChildrenAbove);

  while Assigned(Result) and (Result.CheckState <> State) do
    Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetPreviousChecked(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean): PD2TreeNode;
//Возвращает предыдущий узел перед Node с состоянием отметки csCheckedNormal, с учетом поции toChildrenAbove (необязательно)
begin
  Result := Self.GetPreviousChecked(Node, csCheckedNormal, ConsiderChildrenAbove);
end;

function TD2CustomTreeGrid.GetPreviousCutCopy(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
//Возвращает предыдущий перед Node узел в дереве, который в настоящее время помечен для операции с буфером обмена.
//Поскольку только видимые (или скрытые после маркировки) узлы могут быть помечены, то нет необходимости
//проверять и инициализировать дочерние узлы. Результат, однако, при необходимости инициализируется.

// Returns the previous node in the tree which is currently marked for a clipboard operation. Since only visible nodes can
// be marked (or they are hidden after they have been marked) it is not necessary to initialize nodes to check for
// child nodes. The result, however, is initialized if necessary.

begin
  if ClipboardStates * FStates <> [] then
  begin
    if (Node = nil) or (Node = FRoot) then
      Result := GetLastNoInit(nil, ConsiderChildrenAbove)
    else
      Result := GetPreviousNoInit(Node, ConsiderChildrenAbove);
    while Assigned(Result) and not (vsCutOrCopy in Result.States) do
      Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end
  else
    Result := nil;
end;

function TD2CustomTreeGrid.GetPreviousInitialized(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает предыдущий инициализированный перед Node узел, с учетом поции toChildrenAbove (необязательно)
// Returns the previous node in tree which is initialized.

begin
  Result := Node;
  repeat
    Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);
  until (Result = nil) or (vsInitialized in Result.States);
end;

function TD2CustomTreeGrid.GetPreviousLeaf(Node: PD2TreeNode): PD2TreeNode;
// Возвращает предыдущий перед Node узел у которого нет детей. Результат при необходимости инициализируется.
// Returns the previous node in the tree which has currently no children.
// The result is initialized if necessary.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.LastChild
  else
    Result := GetPrevious(Node);
  while Assigned(Result) and (vsHasChildren in Result.States) do
    Result := GetPrevious(Result);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetPreviousLevel(Node: PD2TreeNode; NodeLevel: Cardinal): PD2TreeNode;
// Возвращает предыдущий перед Node узел NodeLevel уровня. Результат при необходимости инициализируется.
// Returns the previous node in the tree on a specific level.
// The result is initialized if necessary.

var
  StartNodeLevel: Cardinal;
  Run: PD2TreeNode;

begin
  Result := nil;

  if Assigned(Node) and (Node <> FRoot) then
  begin
    StartNodeLevel := GetNodeLevel(Node);

    if StartNodeLevel < NodeLevel then
    begin
      Result := Node.PrevSibling;
      if Assigned(Result) then
      begin
        // go to last descendant of previous sibling with desired node level (if exists)
        Run := Result;
        while Assigned(Run) and (GetNodeLevel(Run) < NodeLevel) do
        begin
          Result := Run;
          Run := GetLastChild(Run);
        end;
        if Assigned(Run) and (GetNodeLevel(Run) = NodeLevel) then
          Result := Run
        else
        begin
          if Assigned(Result.PrevSibling) then
            Result := GetPreviousLevel(Result, NodeLevel)
          else
            if Assigned(Result) and (Result.Parent <> FRoot) then
              Result := GetPreviousLevel(Result.Parent, NodeLevel)
          else
            Result := nil;
        end;
      end
      else
        Result := GetPreviousLevel(Node.Parent, NodeLevel);
    end
    else
      if StartNodeLevel = NodeLevel then
      begin
        Result := Node.PrevSibling;
        if not Assigned(Result) then // i.e. start node was a first sibling
        begin
          Result := Node.Parent;
          if Assigned(Result) then
            Result := GetPreviousLevel(Result, NodeLevel);
        end;
      end
      else // i.e. StartNodeLevel > NodeLevel
        Result := GetPreviousLevel(Node.Parent, NodeLevel);
  end;

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2CustomTreeGrid.GetPreviousNoInit(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает предыдущий перед Node узел, с учетом поции toChildrenAbove (необязательно) без инициализации.
// Returns previous node in tree, optionally considering toChildrenAbove. No initialization is performed.

var
  Run: PD2TreeNode;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
    begin
      // If there is a last child, take it; if not try the previous sibling.
      if Assigned(Result.LastChild) then
        Result := Result.LastChild
      else
        if Assigned(Result.PrevSibling) then
          Result := Result.PrevSibling
        else
        begin
          // If neither a last child nor a previous sibling exist, go the tree upwards and
          // look, wether one of the parent nodes have a previous sibling. If not the result
          // will ne nil.
          repeat
            Result := Result.Parent;
            Run := nil;
            if Result <> FRoot then
              Run := Result.PrevSibling
            else
              Result := nil;
          until Assigned(Run) or (Result = nil);

          if Assigned(Run) then
            Result := Run;
        end;
    end
    else
    begin
      // Is there a previous sibling?
      if Assigned(Node.PrevSibling) then
      begin
        // Go down and find the last child node.
        Result := GetLastNoInit(Node.PrevSibling);
        if Result = nil then
          Result := Node.PrevSibling;
      end
      else
        // No previous sibling so the parent of the node is the previous node.
        if Node.Parent <> FRoot then
          Result := Node.Parent
        else
          Result := nil;
    end;
  end;
end;

function TD2CustomTreeGrid.GetPreviousSelected(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = False): PD2TreeNode;
// Возвращает предыдущий перед Node выбранный узел с учетом поции toChildrenAbove (необязательно) с инициализацией результата
// Поскольку дети не инициализованных узлов не могут быть в текущем выборе (потому что они еще не существуют),
// здесь нет необходимости инициализировать узлы. При необходимости результат инициализируется.

// Returns the previous node in the tree which is currently selected. Since children of unitialized nodes cannot be
// in the current selection (because they simply do not exist yet) it is not necessary to initialize nodes here.
// The result however is initialized if necessary.

begin
  if FSelectionCount > 0 then
  begin
    if (Node = nil) or (Node = FRoot) then
      Result := FRoot.LastChild
    else
      Result := GetPreviousNoInit(Node, ConsiderChildrenAbove);
    while Assigned(Result) and not (vsSelected in Result.States) do
      Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end
  else
    Result := nil;
end;

function TD2CustomTreeGrid.GetPreviousSibling(Node: PD2TreeNode): PD2TreeNode;
// Возвращает предыдущий перед Node братский узел и при необходимости инициализирует его.
// Returns the previous sibling of Node and initializes it if necessary.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.PrevSibling;
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end;
end;

function TD2CustomTreeGrid.GetPreviousSiblingNoInit(Node: PD2TreeNode): PD2TreeNode;
// Возвращает предыдущий перед Node братский узел без инициализации.
// Returns the previous sibling of Node

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.PrevSibling;
  end;
end;

function TD2CustomTreeGrid.GetPreviousVisible(Node: PD2TreeNode; ConsiderChildrenAbove: Boolean = True): PD2TreeNode;
// Возвращает предыдущий перед Node, который является видимым с инициализацией всех узлов и с учетом опции toChildrenAbove.
// Returns the previous node in tree, with regard to Node, which is visible.
// Nodes which need an initialization (including the result) are initialized.
// toChildrenAbove is optionally considered which is the default here.

var
  Marker: PD2TreeNode;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    repeat
      // If the given node is not visible then look for a parent node which is visible and use its last visible
      // child or the parent node (if there is no visible child) as result.
      if not FullyVisible[Result] then
      begin
        Result := GetVisibleParent(Result, True);
        if Result = FRoot then
          Result := nil;
        Marker := GetLastVisible(Result, True);
        if Assigned(Marker) then
          Result := Marker;
      end
      else
      begin
        if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
        begin
          repeat
            if Assigned(Result.LastChild) and (vsExpanded in Result.States) then
            begin
              Result := Result.LastChild;
              if not (vsInitialized in Result.States) then
                InitNode(Result);

              if vsVisible in Result.States then
                Break;
            end
            else
              if Assigned(Result.PrevSibling) then
              begin
                if not (vsInitialized in Result.PrevSibling.States) then
                  InitNode(Result.PrevSibling);

                if vsVisible in Result.PrevSibling.States then
                begin
                  Result := Result.PrevSibling;
                  Break;
                end;
              end
              else
              begin
                Marker := nil;
                repeat
                  Result := Result.Parent;
                  if Result <> FRoot then
                    Marker := GetPreviousVisibleSibling(Result, True)
                  else
                    Result := nil;
                until Assigned(Marker) or (Result = nil);
                if Assigned(Marker) then
                  Result := Marker;

                Break;
              end;
          until False;
        end
        else
        begin
          repeat
            // Is there a previous sibling node?
            if Assigned(Result.PrevSibling) then
            begin
              Result := Result.PrevSibling;
              // Initialize the new node and check its visibility.
              if not (vsInitialized in Result.States) then
                InitNode(Result);
              if vsVisible in Result.States then
              begin
                // If there are visible child nodes then use the last one.
                Marker := GetLastVisible(Result, True, True);
                if Assigned(Marker) then
                  Result := Marker;
                Break;
              end;
            end
            else
            begin
              // No previous sibling there so the parent node is the nearest previous node.
              Result := Result.Parent;
              if Result = FRoot then
                Result := nil;
              Break;
            end;
          until False;
        end;

        if Assigned(Result) and not (vsInitialized in Result.States) then
          InitNode(Result);
      end;
    until not Assigned(Result) or IsEffectivelyVisible[Result];
  end;
end;

function TD2CustomTreeGrid.GetPreviousVisibleNoInit(Node: PD2TreeNode;
  ConsiderChildrenAbove: Boolean = True): PD2TreeNode;
// Возвращает предыдущий перед Node, который является видимым без инициализации и с учетом опции toChildrenAbove.
// Returns the previous node in tree, with regard to Node, which is visible.
// toChildrenAbove is optionally considered which is the default here.

var
  Marker: PD2TreeNode;

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    repeat
      // If the given node is not visible then look for a parent node which is visible and use its last visible
      // child or the parent node (if there is no visible child) as result.
      if not FullyVisible[Result] then
      begin
        Result := GetVisibleParent(Result, True);
        if Result = FRoot then
          Result := nil;
        Marker := GetLastVisibleNoInit(Result, True);
        if Assigned(Marker) then
          Result := Marker;
      end
      else
      begin
        if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
        begin
          repeat
            // Is the current node expanded and has children?
            if (vsExpanded in Result.States) and Assigned(Result.LastChild) then
            begin
              Result := Result.LastChild;
              if vsVisible in Result.States then
                Break;
            end
            else
              if Assigned(Result.PrevSibling) then
              begin
                // No children anymore, so take the previous sibling.
                if vsVisible in Result.PrevSibling.States then
                begin
                  Result := Result.PrevSibling;
                  Break;
                end;
              end
              else
              begin
                // No children and no previous siblings, so walk up the tree and look wether
                // a parent has a previous visible sibling. If that is the case take it,
                // otherwise there is no previous visible node.
                Marker := nil;
                repeat
                  Result := Result.Parent;
                  if Result <> FRoot then
                    Marker := GetPreviousVisibleSiblingNoInit(Result, True)
                  else
                    Result := nil;
                until Assigned(Marker) or (Result = nil);
                if Assigned(Marker) then
                  Result := Marker;
                Break;
              end;
          until False;
        end
        else
        begin
          repeat
            // Is there a previous sibling node?
            if Assigned(Result.PrevSibling) then
            begin
              Result := Result.PrevSibling;
              if vsVisible in Result.States then
              begin
                // If there are visible child nodes then use the last one.
                Marker := GetLastVisibleNoInit(Result, True, True);
                if Assigned(Marker) then
                  Result := Marker;
                Break;
              end;
            end
            else
            begin
              // No previous sibling there so the parent node is the nearest previous node.
              Result := Result.Parent;
              if Result = FRoot then
                Result := nil;
              Break;
            end;
          until False;
        end;
      end;
    until not Assigned(Result) or IsEffectivelyVisible[Result];
  end;
end;

function TD2CustomTreeGrid.GetPreviousVisibleSibling(Node: PD2TreeNode; IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает предыдущий видимый братский узел перед Node с инициализацией
// Returns the previous visible sibling before Node. Initialization is done implicitly.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := GetPreviousSibling(Result);
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2CustomTreeGrid.GetPreviousVisibleSiblingNoInit(Node: PD2TreeNode;
  IncludeFiltered: Boolean = False): PD2TreeNode;
// Возвращает предыдущий видимый братский узел перед Node без инициализации
// Returns the previous visible sibling before Node.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := Result.PrevSibling;
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2CustomTreeGrid.GetSortedCutCopySet(Resolve: Boolean): TD2NodeArray;
// Возвращает список вырезаемых/копируемых узлов, отсортированных в порядке их появления в дереве.
// Если Resolve=True, то узлы, являющиеся дочерними элементами других копируемых узлов, в массив не помещаются.
// Аналогично GetSortedSelection, но с узлами, помеченными как часть текущего набора вырезать/скопировать (например, для буфера обмена).
// Same as GetSortedSelection but with nodes marked as being part in the current cut/copy set (e.g. for clipboard).

var
  Run: PD2TreeNode;
  Counter: Cardinal;

  //--------------- local function --------------------------------------------

  procedure IncludeThisNode(Node: PD2TreeNode);

  // adds the given node to the result

  var
    Len: Cardinal;

  begin
    Len := Length(Result);
    if Counter = Len then
    begin
      if Len < 100 then
        Len := 100
      else
        Len := Len + Len div 10;
      SetLength(Result, Len);
    end;
    Result[Counter] := Node;
    Inc(Counter);
  end;

  //--------------- end local function ----------------------------------------

begin
  Run := FRoot.FirstChild;
  Counter := 0;
  if Resolve then
  begin
    // Resolving is actually easy: just find the first cutted node in logical order
    // and then never go deeper in level than this node as long as there's a sibling node.
    // Restart the search for a cutted node (at any level) if there are no further siblings.
    while Assigned(Run) do
    begin
      if vsCutOrCopy in Run.States then
      begin
        IncludeThisNode(Run);
        if Assigned(Run.NextSibling) then
          Run := Run.NextSibling
        else
        begin
          // If there are no further siblings then go up one or more levels until a node is
          // found or all nodes have been processed. Although we consider here only initialized
          // nodes we don't need to make any special checks as only initialized nodes can also be selected.
          repeat
            Run := Run.Parent;
          until (Run = FRoot) or Assigned(Run.NextSibling);
          if Run = FRoot then
            Break
          else
            Run := Run.NextSibling;
        end;
      end
      else
        Run := GetNextNoInit(Run);
    end;
  end
  else
    while Assigned(Run) do
    begin
      if vsCutOrCopy in Run.States then
        IncludeThisNode(Run);
      Run := GetNextNoInit(Run);
    end;

  // set the resulting array to its real length
  SetLength(Result, Counter);
end;

function TD2CustomTreeGrid.GetSortedSelection(Resolve: Boolean): TD2NodeArray;
// Возвращает список выбранных узлов, отсортированных в порядке их появления в дереве.
// Если Resolve=True, то узлы, являющиеся дочерними элементами других выбранных узлов, массив не помещаются.
// Эта особенность важна при выполнении drag'n drop, так как в этом случае все выбранные узлы и их дочерние элементы
// нужно учитывать. Выбранный узел, который является дочерним (большой ребенок и т.д.) другого выбранного узла, затем
// автоматически включается и не нуждается в явном упоминании в возвращаемом массиве выбора.
//
// Примечание: вызывающий объект отвечает за освобождение массива. Выделение производится здесь. Обычно, освобождение массива
//             не требует дополнительного внимания, т.к. он автоматически освобождается Lasarus, после выхода из области видимости.

// Returns a list of selected nodes sorted in logical order, that is, as they appear in the tree.
// If Resolve is True then nodes which are children of other selected nodes are not put into the new array.
// This feature is in particuar important when doing drag'n drop as in this case all selected node plus their children
// need to be considered. A selected node which is child (grand child etc.) of another selected node is then
// automatically included and doesn't need to be explicitely mentioned in the returned selection array.
//
// Note: The caller is responsible for freeing the array. Allocation is done here. Usually, though, freeing the array
//       doesn't need additional attention as it is automatically freed by Delphi when it gets out of scope.

var
  Run: PD2TreeNode;
  Counter: Cardinal;

begin
  SetLength(Result, FSelectionCount);
  if FSelectionCount > 0 then
  begin
    Run := FRoot.FirstChild;
    Counter := 0;
    if Resolve then
    begin
      // Resolving is actually easy: just find the first selected node in logical order
      // and then never go deeper in level than this node as long as there's a sibling node.
      // Restart the search for a selected node (at any level) if there are no further siblings.
      while Assigned(Run) do
      begin
        if vsSelected in Run.States then
        begin
          Result[Counter] := Run;
          Inc(Counter);
          if Assigned(Run.NextSibling) then
            Run := Run.NextSibling
          else
          begin
            // If there are no further siblings then go up one or more levels until a node is
            // found or all nodes have been processed. Although we consider here only initialized
            // nodes we don't need to make any special checks as only initialized nodes can also be selected.
            repeat
              Run := Run.Parent;
            until (Run = FRoot) or Assigned(Run.NextSibling);
            if Run = FRoot then
              Break
            else
              Run := Run.NextSibling;
          end;
        end
        else
          Run := GetNextNoInit(Run);
      end;
    end
    else
      while Assigned(Run) do
      begin
        if vsSelected in Run.States then
        begin
          Result[Counter] := Run;
          Inc(Counter);
        end;
        Run := GetNextNoInit(Run);
      end;

    // Since we may have skipped some nodes the result array is likely to be smaller than the
    // selection array, hence shorten the result to true length.
    if Integer(Counter) < Length(Result) then
      SetLength(Result, Counter);
  end;
end;

function TD2CustomTreeGrid.GetVisibleFixedWidth: Single;
// Определяет горизонтальное пространство, которое занимают все видимые {(todo) и фиксированные} столбцы.
// Determines the horizontal space all visible and fixed columns occupy.
var
  I: Integer;
begin
  Result := 0;
  for I := 0 to ColumnCount - 1 do
  begin
    //if Columns[I].Options * [coVisible, coFixed] = [coVisible, coFixed]
    if Columns[I].Visible
      then d2IncFloat(Result, Columns[I].Width);
  end;
end;

function TD2CustomTreeGrid.GetVisibleParent(Node: PD2TreeNode;
  IncludeFiltered: Boolean): PD2TreeNode;
//Возвращает первого (ближайшего) видимого родителя для узла Node. Этот метод
//один из очень редких случаев, когда может быть возвращен скрытый корневой узел.
// Returns the first (nearest) parent node of Node which is visible.
// This method is one of the seldom cases where the hidden root node could be returned.
begin
  Assert(Assigned(Node), 'Node must not be nil.');
  Assert(Node <> FRoot, 'Node must not be the hidden root node.');
  Result := Node.Parent;
  while (Result <> FRoot) and (not FullyVisible[Result] or
        (not IncludeFiltered and IsEffectivelyFiltered[Result])) do
    Result := Result.Parent;
end;

function TD2CustomTreeGrid.HasAsParent(Node, PotentialParent: PD2TreeNode): Boolean;
//True - если PotentialParent является родителем любого уровня для узла Node
// Determines whether Node has got PotentialParent as one of its parents.

var Run: PD2TreeNode;
begin
  Result := Assigned(Node) and Assigned(PotentialParent) and (Node <> PotentialParent);
  if Result then
  begin
    Run := Node;
    while (Run <> FRoot) and (Run <> PotentialParent) do
      Run := Run.Parent;
    Result := Run = PotentialParent;
  end;
end;

function TD2CustomTreeGrid.InsertNode(Node: PD2TreeNode;
  Mode: TD2TreeNodeAttachMode; UserData: Pointer): PD2TreeNode;
// Добавляет новый узел относительно узла Node. Конечная позиция определяется параметром Mode.
// UserData может использоваться для установки первых байтов SizeOf(Pointer) области пользовательских данных для
// начальное значение, которое может быть использовано в OnInitNode, а также вызывает запуск
// Событие OnFreeNode (если <> nil), даже если узел еще не «официально» инициализирован.
// InsertNode - это метод совместимости и будет неявно проверять данный узел
// если новый узел должен быть добавлен как дочерний узел. Однако это противоречит виртуальному
// парадигма и, следовательно, я отговариваюсь от ее использования.

// Adds a new node relative to Node. The final position is determined by Mode.
// UserData can be used to set the first SizeOf(Pointer) bytes of the user data area to
// an initial value which can be used in OnInitNode and will also cause to trigger the
// OnFreeNode event (if <> nil) even if the node is not yet "officially" initialized.
// InsertNode is a compatibility method and will implicitly validate the given node
// if the new node is to be added as child node. This is however against the virtual
// paradigm and hence I dissuade from its usage.

var
  NodeData: ^Pointer;

begin
  if Mode <> amNoWhere then
  begin
    CancelEditNode;

    if Node = nil then
      Node := FRoot;
    // we need a new node...
    Result := MakeNewNode;
    // avoid erronous attach modes
    if Node = FRoot then
    begin
      case Mode of
        amInsertBefore:
          Mode := amAddChildFirst;
        amInsertAfter:
          Mode := amAddChildLast;
      end;
    end;

    // Validate given node in case the new node becomes its child.
    if (Mode in [amAddChildFirst, amAddChildLast]) and not (vsInitialized in Node.States) then
      InitNode(Node);
    InternalConnectNode(Result, Node, Self, Mode);

    // Check if there is initial user data and there is also enough user data space allocated.
    if Assigned(UserData) then
      if FNodeDataSize >= SizeOf(Pointer) then
      begin
        NodeData := Pointer(PByte(@Result.Data) + FTotalInternalDataSize);
        NodeData^ := UserData;
        Include(Result.States, vsOnFreeNodeCallRequired);
      end
      else
        ShowError(SCannotSetUserData, hcTFCannotSetUserData);

    if FUpdateCount = 0 then
    begin
      // Если включена автоматическая сортировка, сортировать узел или его родителя (в зависимости от режима вставки).
      //If auto sort is enabled then sort the node or its parent (depending on the insert mode).
      if (toAutoSort in FOptions.FAutoOptions) and (FSortColumn > InvalidColumn) then
        case Mode of
          amInsertBefore,
          amInsertAfter:
            // Здесь не требуется инициализация, т.к. если узел уже имеет детей, то он уже инициализирован.
            // Примечание: Узел не может быть FRoot в этот момент.
            // Here no initialization is necessary because *if* a node has already got children then it must also be initialized.
            // Note: Node can never be FRoot at this point.
            Sort(Node.Parent, FSortColumn, FSortDirection, True);
          amAddChildFirst,
          amAddChildLast:
            Sort(Node, FSortColumn, FSortDirection, True);
        end;

      //UpdateScrollBars(True);
      Realign; //вместо UpdateScrollBars(True)
      if Mode = amInsertBefore then
        InvalidateToBottom(Result)
      else
        InvalidateToBottom(Node);
    end;
    StructureChange(Result, crNodeAdded);
  end
  else
    Result := nil;
end;

procedure TD2CustomTreeGrid.InvalidateChildren(Node: PD2TreeNode;
  Recursive: Boolean);
//Инициирует перерисовку узла и его непосредственных потомков.
//Если Recursive = True, то все внуки также перерисовываются.
//Сам узел инициализируется при необходимости и создаются его дочерние узлы
//(и также инициализируется, если Recursive is True).

// Invalidates Node and its immediate children.
// If Recursive is True then all grandchildren are invalidated as well.
// The node itself is initialized if necessary and its child nodes are created
// (and initialized too if Recursive is True).
var Run: PD2TreeNode;
begin
  if Assigned(Node) then
  begin
    if not (vsInitialized in Node.States) then
      InitNode(Node);
    InvalidateNode(Node);
    if (vsHasChildren in Node.States) and (Node.ChildCount = 0) then
      InitChildren(Node);
    Run := Node.FirstChild;
  end
  else
    Run := FRoot.FirstChild;

  while Assigned(Run) do
  begin
    InvalidateNode(Run);
    if Recursive then
      InvalidateChildren(Run, True);
    Run := Run.NextSibling;
  end;
end;

procedure TD2CustomTreeGrid.InvalidateColumn(Column: Integer);
//Пометить недействительной видимую область колоннки Column.
// Invalidates the client area part of a column.
//var R: TD2Rect;
begin
  if (FUpdateCount = 0) {and HandleAllocated} and IsValidColumn(Column) then
  begin
    //R := ClientRect;
    //lclheader
    //if hoVisible in FHeader.FOptions then
    //  OffsetRect(R, 0, FHeader.Height);
    //FHeader.Columns.GetColumnBounds(Column, R.Left, R.Right);
    ////InvalidateRect(Handle, @R, False);
    //InvalidateRect(R);
    with Columns[Column] do
      if Visible then Realign;
  end;
end;

function TD2CustomTreeGrid.InvalidateNode(Node: PD2TreeNode): TD2Rect;
//Инициирует перерисовку данного узла. Возвращает аннулированый прямоугольник.
// Initiates repaint of the given node and returns the just invalidated rectangle.

begin
  Realign;
  //if (FUpdateCount = 0) and HandleAllocated then
  //begin
  //  Result := GetDisplayRect(Node, NoColumn, False);
  //  InvalidateRect(Handle, @Result, False);
  //end;
end;

procedure TD2CustomTreeGrid.InvalidateToBottom(Node: PD2TreeNode);
//Инициирует перерисовку клиентской области, начиная с узла Node. Если этот
//узел не виден или не инициализирован, то ничего не происходит.
// Initiates repaint of client area starting at given node. If this node is not visible or not yet initialized
// then nothing happens.

//var
  //R: TD2Rect;

begin
  if (FUpdateCount = 0) {and HandleAllocated }then
  begin
    Realign;
    //if (Node = nil) or (Node = FRoot)
    //   then Invalidate
    //   else if (vsInitialized in Node.States) and IsEffectivelyVisible[Node]
    //           then begin
    //                  R := GetDisplayRect(Node, -1, False);
    //                  if R.Top < ClientHeight then
    //                  begin
    //                    if (toChildrenAbove in FOptions.FPaintOptions) and (vsExpanded in Node.States) then
    //                      Dec(R.Top, Node.TotalHeight + NodeHeight[Node]);
    //                    R.Bottom := ClientHeight;
    //                    //lclheader
    //                    if hoVisible in FHeader.FOptions then
    //                      Inc(R.Bottom, FHeader.Height);
    //                    InvalidateRect(Handle, @R, False);
    //                  end;
    //                end;
 end;
end;

procedure TD2CustomTreeGrid.InvertSelection(VisibleOnly: Boolean);
//Инвертировать текущий выбор (узлы, которые выбраны становятся невыделенными и наоборот).
//Если VisibleOnly = True, то рассматриваются только видимые узлы.
// Inverts the current selection (so nodes which are selected become unselected and vice versa).
// If VisibleOnly is True then only visible nodes are considered.
var
  Run: PD2TreeNode;
  NewSize: Integer;
  NextFunction: TD2GetNextNodeProc;
  TriggerChange: Boolean;
begin
  if not FSelectionLocked and (toMultiSelect in FOptions.FSelectionOptions) then
  begin
    Run := FRoot.FirstChild;
    ClearTempCache;
    if VisibleOnly then
      NextFunction := GetNextVisibleNoInit
    else
      NextFunction := GetNextNoInit;
    while Assigned(Run) do
    begin
      if vsSelected in Run.States then
        InternalRemoveFromSelection(Run)
      else
        InternalCacheNode(Run);
      Run := NextFunction(Run);
    end;

    // do some housekeeping
    // Need to trigger the OnChange event from here if nodes were only deleted but not added.
    TriggerChange := False;
    NewSize := PackArray(FSelectionNodes, FSelectionCount);
    if NewSize > -1 then
    begin
      FSelectionCount := NewSize;
      SetLength(FSelectionNodes, FSelectionCount);
      TriggerChange := True;
    end;
    if FTempNodeCount > 0 then
    begin
      AddToSelection(FTempNodeCache, FTempNodeCount);
      ClearTempCache;
      TriggerChange := False;
    end;
    Realign; // вместо Invalidate;
    if TriggerChange then
      Change(nil);
    if Self.SelectedCount = 0 then
      FNextNodeToSelect := nil;//Ensure that no other node is selected now
  end;
end;

function TD2CustomTreeGrid.IsEditing: Boolean;
//true - идет редактирование данных дерева
begin
  Result := tsEditing in FStates;
end;

function TD2CustomTreeGrid.IsMouseSelecting: Boolean;
//true - производится выделение прямоугольником
begin
  Result := (tsDrawSelPending in FStates) or (tsDrawSelecting in FStates);
end;

function TD2CustomTreeGrid.IsEmpty: Boolean;
//true - дерево пустое
begin
  Result := (Self.ChildCount[nil] = 0);
end;

function TD2CustomTreeGrid.IsValidColumn(Column: Integer): Boolean;
// Определяет, является ли столбец Column действительным, то есть является ли он одним из текущих столбцов.
// Determines whether the given column is valid or not, that is, whether it is one of the current columns.
begin
  Result := (Column > NoColumn) and (Column < ColumnCount);
end;

function TD2CustomTreeGrid.IterateSubtree(Node: PD2TreeNode;
  Callback: TD2TreeGetNodeProc; Data: Pointer; Filter: TD2TreeNodeStates;
  DoInit: Boolean; ChildNodesOnly: Boolean): PD2TreeNode;
//Перебирает всех детей и внуков и т.д. узла Node (все дерево при Node = nil) и
//вызывает для каждого узла метод Callback. Фильтр определяет, какие узлы рассматривать
//(пустое множество - все узлы). Если DoInit = True, то неинициализированные узлы,
//будут инициализированы.
//Внимание! В вызове Callback приложение может установить параметр Abort в True.
//          В этом случае итерация останавливается и последний обработанный узел
//          (тот, на котором Abort установлен в True) возвращается функцией.
//          В противном случае (без прерывания) возвращается Nil.

// Iterates through the all children and grandchildren etc. of Node (or the entire tree if Node = nil)
// and calls for each node the provided callback method (which must not be empty).
// Filter determines which nodes to consider (an empty set denotes all nodes).
// If DoInit is True then nodes which aren't initialized yet will be initialized.
// Note: During execution of the callback the application can set Abort to True. In this case the iteration is stopped
//       and the last accessed node (the one on which the callback set Abort to True) is returned to the caller.
//       Otherwise (no abort) nil is returned.

var
  Stop: PD2TreeNode;
  Abort: Boolean;
  GetNextNode: TD2GetNextNodeProc;
  WasIterating: Boolean;

begin
  Assert(Node <> FRoot, 'Node must not be the hidden root node.');

  WasIterating := tsIterating in FStates;
  DoStateChange([tsIterating]);
  try
    // prepare function to be used when advancing
    if DoInit then
      GetNextNode := GetNext
    else
      GetNextNode := GetNextNoInit;

    Abort := False;
    if Node = nil then
      Stop := nil
    else
    begin
      if not (vsInitialized in Node.States) and DoInit then
        InitNode(Node);

      // The stopper does not need to be initialized since it is not taken into the enumeration.
      Stop := Node.NextSibling;
      if Stop = nil then
      begin
        Stop := Node;
        repeat
          Stop := Stop.Parent;
        until (Stop = FRoot) or Assigned(Stop.NextSibling);
        if Stop = FRoot then
          Stop := nil
        else
          Stop := Stop.NextSibling;
      end;
    end;

    // Use first node if we start with the root.
    if Node = nil then
      Node := GetFirstNoInit;

    if Assigned(Node) then
    begin
      if not (vsInitialized in Node.States) and DoInit then
        InitNode(Node);

      // Skip given node if only the child nodes are requested.
      if ChildNodesOnly then
      begin
        if Node.ChildCount = 0 then
          Node := nil
        else
          Node := GetNextNode(Node);
      end;

      if Filter = [] then
      begin
        // unfiltered loop
        while Assigned(Node) and (Node <> Stop) do
        begin
          Callback(Self, Node, Data, Abort);
          if Abort then
            Break;
          Node := GetNextNode(Node);
        end;
      end
      else
      begin
        // filtered loop
        while Assigned(Node) and (Node <> Stop) do
        begin
          if Node.States * Filter = Filter then
            Callback(Self, Node, Data, Abort);
          if Abort then
            Break;
          Node := GetNextNode(Node);
        end;
      end;
    end;

    if Abort then
      Result := Node
    else
      Result := nil;
  finally
    if not WasIterating then
      DoStateChange([], [tsIterating]);
  end;
end;

procedure TD2CustomTreeGrid.LoadFromFile(const FileName: TFileName);
// Очищает текущее содержимое дерева и загружает новую структуру из файла с именем FileName.
var FileStream: TFileStream;
begin
  FileStream := TFileStream.Create(FileName, fmOpenRead or fmShareDenyWrite);
  try
    LoadFromStream(FileStream);
  finally
    FileStream.Free;
  end;
end;

procedure TD2CustomTreeGrid.LoadFromStream(Stream: TStream);
// Очищает текущее содержимое дерева и загружает новую структуру из потока Stream.
// Clears the current content of the tree and loads a new structure from the given stream.
var
  ThisID: TD2MagicID;
  Version,
  Count: Cardinal;
  Node: PD2TreeNode;
begin
  if not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Clear;
    // Check first whether this is a stream we can read.
    if Stream.Read(ThisID, SizeOf(TD2MagicID)) < SizeOf(TD2MagicID) then
      ShowError(SStreamTooSmall, hcTFStreamTooSmall);

    if (ThisID[0] = MagicID[0]) and
       (ThisID[1] = MagicID[1]) and
       (ThisID[2] = MagicID[2]) and
       (ThisID[5] = MagicID[5]) then
    begin
      Version := Word(ThisID[3]);
      if Version <= TreeStreamVersion then
      begin
        BeginUpdate;
        try
          // LCL port started with tree stream version 2 so no need to do the check here
          Stream.ReadBuffer(Count, SizeOf(Count));

          while (Stream.Position < Stream.Size) and (Count > 0) do
          begin
            Dec(Count);
            Node := MakeNewNode;
            InternalConnectNode(Node, FRoot, Self, amAddChildLast);
            InternalAddFromStream(Stream, Version, Node);
          end;
          DoNodeCopied(nil);
          if Assigned(FOnLoadTree) then
            FOnLoadTree(Self, Stream);
        finally
          EndUpdate;
        end;
      end
      else
        ShowError(SWrongStreamVersion, hcTFWrongStreamVersion);
    end
    else
      ShowError(SWrongStreamFormat, hcTFWrongStreamFormat);
  end;
end;

procedure TD2CustomTreeGrid.MeasureItemHeight(Node: PD2TreeNode);
//Опредяет высоту узла Node если это до сих пор не сделайте.
// If the height of the given node has not yet been measured then do it now.
var NewNodeHeight: Single;
begin
  if not (vsHeightMeasured in Node.States)
     //{$if CompilerVersion < 20}
     //and (MainThreadId = GetCurrentThreadId)
     //{$ifend}
  then  begin
          Include(Node.States, vsHeightMeasured);
          if (toVariableNodeHeight in FOptions.FMiscOptions) then
          begin
            NewNodeHeight := Node.NodeHeight;
            //{$if CompilerVersion >= 20} // Anonymous methods help to make this thread safe easily. In Delphi 2007 and lower developers must take care themselves about thread synchronization when consuming the OnMeasureItemHeight event
            //if (MainThreadId <> GetCurrentThreadId) then
            //  TThread.Synchronize(nil,
            //    procedure
            //    begin
            //      DoMeasureItem(Canvas, Node, NewNodeHeight);
            //      SetNodeHeight(Node, NewNodeHeight);
            //    end
            //  )
            //else
            //{$ifend}
            begin
              DoMeasureItem(Node, NewNodeHeight);
              SetNodeHeight(Node, NewNodeHeight);
            end;
          end;
        end;
end;

procedure TD2CustomTreeGrid.MoveTo(Node: PD2TreeNode; Tree: TD2CustomTreeGrid;
  Mode: TD2TreeNodeAttachMode; ChildrenOnly: Boolean);
//Упрощенный метод перемещения узла к корню другого дерева.
// A simplified method to allow to move nodes to the root of another tree.
begin
  MoveTo(Node, Tree.FRoot, Mode, ChildrenOnly);
end;

//----------------------------------------------------------------------------------------------------------------------

procedure TD2CustomTreeGrid.MoveTo(Source, Target: PD2TreeNode;
   Mode: TD2TreeNodeAttachMode; ChildrenOnly: Boolean);
//Перемещение узла Source и всех его детей к узлу Target. Source должен принадлежать
//экземпляру дерева, которое вызывает этот метод. Mode определяет, как подключить Source к Target.
//Этот метод может повлечь изменение дерева, если Source и Target принадлежат разрым деревьям.
// Moves the given node (and all its children) to Target. Source must belong to the tree instance which calls this
// MoveTo method. Mode determines how to connect Source to Target.
// This method might involve a change of the tree if Target belongs to a different tree than Source.

var
  TargetTree: TD2CustomTreeGrid;
  Allowed: Boolean;
  NewNode: PD2TreeNode;
  Stream: TMemoryStream;

begin
  Assert(TreeFromNode(Source) = Self, 'The source tree must contain the source node.');

  // When moving nodes then source and target must not be the same node unless only the source's children are
  // moved and they are inserted before or after the node itself.
  Allowed := (Source <> Target) or ((Mode in [amInsertBefore, amInsertAfter]) and ChildrenOnly);

  if Allowed and (Mode <> amNoWhere) and Assigned(Source) and
    (Source <> FRoot) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    // Assume that an empty destination means the root in this (the source) tree.
    if Target = nil
      then  begin
              TargetTree := Self;
              Target := FRoot;
              Mode := amAddChildFirst;
            end
      else TargetTree := TreeFromNode(Target);

    if Target = TargetTree.FRoot
    then  begin
            case Mode of
              amInsertBefore:
                Mode := amAddChildFirst;
              amInsertAfter:
                Mode := amAddChildLast;
            end;
          end;

    // Make sure the target node is initialized.
    if not (vsInitialized in Target.States)
      then TargetTree.InitNode(Target)
      else if (vsHasChildren in Target.States) and (Target.ChildCount = 0)
             then TargetTree.InitChildren(Target);

    if TargetTree = Self
    then  begin
            // Simple case: move node(s) within the same tree.
            if Target = FRoot
              then Allowed := DoNodeMoving(Source, nil)
              else Allowed := DoNodeMoving(Source, Target);
            if Allowed then
            begin
              // Check first that Source is not added as new child to a target node which
              // is already a child of Source.
              // Consider the case Source and Target are the same node, but only child nodes are moved.
              if (Source <> Target) and HasAsParent(Target, Source) then
                  ShowError(SWrongMoveError, hcTFWrongMoveError);

              if not ChildrenOnly
              then  begin
                      // Disconnect from old location.
                      InternalDisconnectNode(Source, True);
                      // Connect to new location.
                      InternalConnectNode(Source, Target, Self, Mode);
                      DoNodeMoved(Source);
                    end
              else  begin
                      // Only child nodes should be moved. Insertion order depends on move mode.
                      if Mode = amAddChildFirst
                      then  begin
                              Source := Source.LastChild;
                              while Assigned(Source) do
                              begin
                                NewNode := Source.PrevSibling;
                                // Disconnect from old location.
                                InternalDisconnectNode(Source, True, False);
                                // Connect to new location.
                                InternalConnectNode(Source, Target, Self, Mode);
                                DoNodeMoved(Source);
                                Source := NewNode;
                              end;
                            end
                      else  begin
                              Source := Source.FirstChild;
                              while Assigned(Source) do
                              begin
                                NewNode := Source.NextSibling;
                                // Disconnect from old location.
                                InternalDisconnectNode(Source, True, False);
                                // Connect to new location.
                                InternalConnectNode(Source, Target, Self, Mode);
                                DoNodeMoved(Source);
                                Source := NewNode;
                              end;
                            end;
                    end;
            end;
          end
    else  begin
            // Difficult case: move node(s) to another tree.
            // In opposition to node copying we ask only once if moving is allowed because
            // we cannot take back a move once done.
            if Target = TargetTree.FRoot
              then Allowed := DoNodeMoving(Source, nil)
              else Allowed := DoNodeMoving(Source, Target);
            if Allowed then
            begin
              Stream := TMemoryStream.Create;
              try
                // Write all nodes into a temporary stream depending on the ChildrenOnly flag.
                if not ChildrenOnly
                  then  WriteNode(Stream, Source)
                  else  begin
                          Source := Source.FirstChild;
                          while Assigned(Source) do
                          begin
                            WriteNode(Stream, Source);
                            Source := Source.NextSibling;
                          end;
                        end;
                // Now load the serialized nodes into the target node (tree).
                TargetTree.BeginUpdate;
                try
                  Stream.Position := 0;
                  while Stream.Position < Stream.Size do
                  begin
                    NewNode := TargetTree.MakeNewNode;
                    InternalConnectNode(NewNode, Target, TargetTree, Mode);
                    TargetTree.InternalAddFromStream(Stream, TreeStreamVersion, NewNode);
                    DoNodeMoved(NewNode);
                  end;
                finally
                  TargetTree.EndUpdate;
                end;
              finally
                Stream.Free;
              end;
              // finally delete original nodes
              BeginUpdate;
              try
                if ChildrenOnly
                  then DeleteChild(Source)
                  else DeleteNode(Source);
              finally
                EndUpdate;
              end;
            end;
          end;

    InvalidateCache;
    if (FUpdateCount = 0) and Allowed then
    begin
      ValidateCache;
      //UpdateScrollBars(True);
      Realign; //вместо UpdateScrollBars(True) и Invalidate
      if TargetTree <> Self
        then TargetTree.Realign; // вместо Invalidate;
    end;
    StructureChange(Source, crNodeMoved);
  end;
end;

procedure TD2CustomTreeGrid.ReinitChildren(Node: PD2TreeNode; Recursive: Boolean);
//Принудительная переинициализация всех детей узла Node, при Recursive=true в том числе и внуки.
// Forces all child nodes of Node to be reinitialized.
// If Recursive is True then also the grandchildren are reinitialized.

var
  Run: PD2TreeNode;

begin
  if Assigned(Node) then
  begin
    InitChildren(Node);
    Run := Node.FirstChild;
  end
  else
  begin
    InitChildren(FRoot);
    Run := FRoot.FirstChild;
  end;

  while Assigned(Run) do
  begin
    ReinitNode(Run, Recursive);
    Run := Run.NextSibling;
  end;
end;

procedure TD2CustomTreeGrid.ReinitNode(Node: PD2TreeNode; Recursive: Boolean);
//Принудительная переинициализация узла Node и всех его потомков (при Recursive=True) без изменения данных и удаления детей (если приложение не запрашивает другое).
// Forces the given node and all its children (if recursive is True) to be initialized again without
// modifying any data in the nodes nor deleting children (unless the application requests a different amount).
begin
  if Assigned(Node) and (Node <> FRoot) then
  begin
    // Remove dynamic styles.
    Node.States := Node.States - [vsChecking, vsCutOrCopy, vsDeleting, vsHeightMeasured];
    InitNode(Node);
  end;

  if Recursive then
    ReinitChildren(Node, True);
end;

procedure TD2CustomTreeGrid.ResetNode(Node: PD2TreeNode);
//Удаление всех детей узла Node и его пометка не инициализированным.
// Deletes all children of the given node and marks it as being uninitialized.

begin
  DoCancelEdit;
  if (Node = nil) or (Node = FRoot) then
    Clear
  else
  begin
    DoReset(Node);
    DeleteChild(Node);
    // Remove initialized and other dynamic styles, keep persistent styles.
    Node.States := Node.States - [vsInitialized, vsChecking, vsCutOrCopy, vsDeleting, vsHasChildren, vsExpanded,
      vsHeightMeasured];
    InvalidateNode(Node);
  end;
end;

//----------------------------------------------------------------------------------------------------------------------

procedure TD2CustomTreeGrid.SaveToFile(const FileName: TFileName);
// Сохраняет все содержимое дерева в файл (см дополнительные примечания в SaveToStream).
// Saves the entire content of the tree into a file (see further notes in SaveToStream).

var FileStream: TFileStream;
begin
  FileStream := TFileStream.Create(FileName, fmCreate);
  try
    SaveToStream(FileStream);
  finally
    FileStream.Free;
  end;
end;

procedure TD2CustomTreeGrid.SaveToStream(Stream: TStream; Node: PD2TreeNode = nil);
//Сохраняет узел и всех его потомков в поток. Если Node = Nil, то все узлы верхнего уровня будут сохранены.
//Внимание! Вы должны быть внимательны при сохранении. Проблема здесь заключается в том,
//          что мы имеем дело с виртуальными данными. Дерево может не знать, что нужно
//          сохранить. Единственный факт, который мы достоверно знаем - структура дерева.
//          Для того, чтобы быть гибким для будущих усовершенствований, а также неизвестное
//          содержимое (неизвестно классу дерева, который производит запись/чтение потока)
//          подход на основе кусок здесь используется. Каждый класс дерево обрабатывает
//          только те блоки, которые не обрабатываются классом anchestor и известны классу.
//Базовый класс дерева сохраняет только структуру дерева вместе с данными приложения.
//Потомки могут необязательно добавлять свои собственные блоки для хранения дополнительной
//информации. См: WriteChunks.

// Saves Node and all its children to Stream. If Node is nil then all top level nodes will be stored.
// Note: You should be careful about assuming what is actually saved. The problem here is that we are dealing with
//       virtual data. The tree can so not know what it has to save. The only fact we reliably know is the tree's
//       structure. To be flexible for future enhancements as well as unknown content (unknown to the tree class which
//       is saving/loading the stream) a chunk based approach is used here. Every tree class handles only those
//       chunks which are not handled by an anchestor class and are known by the class.
//
// The base tree class saves only the structure of the tree along with application provided data. descendants may
// optionally add their own chunks to store additional information. See: WriteChunks.

var
  Count: Cardinal;

begin
  Stream.Write(MagicID, SizeOf(MagicID));
  if Node = nil then
  begin
    // Keep number of top level nodes for easy restauration.
    Count := FRoot.ChildCount;
    Stream.WriteBuffer(Count, SizeOf(Count));

    // Save entire tree here.
    Node := FRoot.FirstChild;
    while Assigned(Node) do
    begin
      WriteNode(Stream, Node);
      Node := Node.NextSibling;
    end;
  end
  else
  begin
    Count := 1;
    Stream.WriteBuffer(Count, SizeOf(Count));
    WriteNode(Stream, Node);
  end;
  if Assigned(FOnSaveTree) then
    FOnSaveTree(Self, Stream);
end;

function TD2CustomTreeGrid.ScrollIntoView(Column: Integer; Center: Boolean): Boolean;
//Прокручивает дерево чтобы столбец Column был видимым. Возвращает True, если
//столбцы были прокручены (например чтобы избежать дальнейших обновлений)
//иначе возвращает False.
// Scrolls the columns so that the given column is in the client area and returns True if the columns really have been
// scrolled (e.g. to avoid further updates) else returns False.
var
  ColumnLeft,
  ColumnRight: Single;
  NewOffset: Single;

begin
  Result := False;

  if ColumnCount < 1 then Exit;
  //if not FHeader.Columns.IsValidColumn(Column) then Exit; // Just in case.

  ColumnLeft := Columns[Column].GetBoundsRect.Left;
  ColumnRight := Columns[Column].GetBoundsRect.Right;

  NewOffset := HScrollBarValue;
  if Center then
  begin
    NewOffset := HScrollBarValue + ColumnLeft - (GetVisibleFixedWidth / 2) - ClientWidth / 2 + (ColumnRight - ColumnLeft) / 2;
    if NewOffset <> HScrollBarValue then
    begin
      {if UseRightToLeftAlignment then
        SetOffsetX(-Integer(FRangeX) + ClientWidth + NewOffset)
      else}
        SetOffsetX(-NewOffset);
    end;
    Result := True;
  end
  else if not (Columns[Column].Locked) then
  begin
    if ColumnRight > ClientWidth then
      NewOffset := HScrollBarValue + (ColumnRight - ClientWidth)
    else if (ColumnLeft < GetVisibleFixedWidth) then
      NewOffset := HScrollBarValue - (GetVisibleFixedWidth - ColumnLeft);
    if NewOffset <> HScrollBarValue then
    begin
      {if UseRightToLeftAlignment then
        SetOffsetX(-Integer(FRangeX) + ClientWidth + NewOffset)
      else }
        SetOffsetX(-NewOffset);
    end;
    Result := True;
  end
  else
    Result := True;
end;

function TD2CustomTreeGrid.ScrollIntoView(Node: PD2TreeNode;
  Center: Boolean; Horizontally: Boolean): Boolean;
//Прокручивает дерево чтобы узел Node был видимым. Возвращает True, если дерево было прокручено
//(например чтобы избежать дальнейших обновлений) иначе возвращает False. Если расширенная
//фокусировка включена, то дерево будет также прокручивается по горизонтали, если это необходимо.
//Примечание: Все свернутые родители узла разворачиваются.

// Scrolls the tree so that the given node is in the client area and returns True if
// the tree really has been scrolled (e.g. to avoid further updates) else returns False.
// If extened focus is enabled then the tree will also be horizontally scrolled if needed.
// Note: All collapsed parents of the node are expanded.

var
  R: TD2Rect;
  Run: PD2TreeNode;
  UseColumns,
  HScrollBarVisible: Boolean;
  ScrolledVertically,
  ScrolledHorizontally: Boolean;

begin
  //todo: minimize calls to ClientHeight and ClientWidth
  ScrolledVertically := False;
  ScrolledHorizontally := False;

  if Assigned(Node) and (Node <> FRoot) then
  begin
    // Убедимся, что все родители узла развернуты. Make sure all parents of the node are expanded.
    Run := Node.Parent;
    while Run <> FRoot do
    begin
      if not (vsExpanded in Run.States) then
        ToggleNode(Run);
      Run := Run.Parent;
    end;
    UseColumns := (ColumnCount > 0);

    if UseColumns and IsValidColumn(FFocusedColumn)
      then R := GetDisplayRect(Node, FFocusedColumn, not (toGridExtensions in FOptions.FMiscOptions))
      else R := GetDisplayRect(Node, NoColumn,       not (toGridExtensions in FOptions.FMiscOptions));

    // Возвращенный прямоугольник никогда не быть пустым после кода расширения выше. The returned rectangle can never be empty after the expand code above.
    // 1) Вертикальная прокрутка. scroll vertically
    if R.Top < VScrollBarValue
      then  begin
              if Center
                then VScrollBar.Value := R.Top + ClientHeight / 2 + Node.NodeHeight / 2
                else VScrollBar.Value := R.Top;
              ScrolledVertically := True;
            end
      else
        if (R.Bottom > VScrollBarValue + ClientHeight) or Center then
        begin
          if Center
            then VScrollBar.Value := R.Bottom - ClientHeight / 2 - Node.NodeHeight / 2
            else VScrollBar.Value := R.Bottom - ClientHeight - 1;

          // При прокрутке вверх и появлении горизонтальной скроллера, мы должны перемещать узел вверх,
          // чтобы избежать того, чтобы горизонтальной скроллера не скрывал узел, который мы хотели видеть.

          // When scrolling up and the horizontal scroll appears because of the operation
          // then we have to move up the node the horizontal scrollbar's height too
          // in order to avoid that the scroll bar hides the node which we wanted to have in view.
          if not UseColumns and not HScrollBar.Visible and (FRangeX > ClientWidth)
            then SetOffsetY(VScrollBarValue - HScrollBar.Height);
          ScrolledVertically := True;
        end;

    if Horizontally then
      // 2) Горизонтальная прокрутка. scroll horizontally
      ScrolledHorizontally := ScrollIntoView(FFocusedColumn, Center);

  end;

  Result := ScrolledVertically or ScrolledHorizontally;
end;

procedure TD2CustomTreeGrid.ShowError(const Msg: String; HelpContext: Integer);
//Показать сообщение ошибки
begin
  raise ED2TreeError.CreateHelp(Msg, HelpContext);
end;

procedure TD2CustomTreeGrid.SelectAll(VisibleOnly: Boolean);
//Выбрать все узлы в дереве. При VisibleOnly = true - выбираются только видимые узлы.
// Select all nodes in the tree.
// If VisibleOnly is True then only visible nodes are selected.
var
  Run: PD2TreeNode;
  NextFunction: TD2GetNextNodeProc;
begin
  if not FSelectionLocked and (toMultiSelect in FOptions.FSelectionOptions) then
  begin
    ClearTempCache;
    if VisibleOnly then
    begin
      Run := GetFirstVisible(nil, True);
      NextFunction := GetNextVisible;
    end
    else
    begin
      Run := GetFirst;
      NextFunction := GetNext;
    end;

    while Assigned(Run) do
    begin
      if not(vsSelected in Run.States) then
        InternalCacheNode(Run);
      Run := NextFunction(Run);
    end;
    if FTempNodeCount > 0 then
      AddToSelection(FTempNodeCache, FTempNodeCount);
    ClearTempCache;
    UpdateSelection; //Realign;//вместо Invalidate;
  end;
end;

procedure TD2CustomTreeGrid.Sort(Node: PD2TreeNode; Column: Integer;
  Direction: TD2SortDirection; DoInit: Boolean);
// Сортировка дерева заданного узлом Node по колонке Column в направлении Direction.
// Приложение запрашивает, как сортировать событие OnCompareNodes.
// Столбец просто передается функции сравнения, поэтому приложение также может сортировать в определенном столбце.
// Чтобы освободить приложение от заботы о направлении сортировки, используется параметр Direction.
// Таким образом, приложение всегда может сортировать в порядке возрастания, тогда как этот метод переупорядочивает узлы в соответствии с этим флагом.

// Sorts the given node. The application is queried about how to sort via the OnCompareNodes event.
// Column is simply passed to the the compare function so the application can also sort in a particular column.
// In order to free the application from taking care about the sort direction the parameter Direction is used.
// This way the application can always sort in increasing order, while this method reorders nodes according to this flag.

  //--------------- local functions -------------------------------------------

  function MergeAscending(A, B: PD2TreeNode): PD2TreeNode;

  // Слияние узлов  A и B (которые оба должны быть отсортированы с помощью Compare) по возрастаниюв в один список.
  // Merges A and B (which both must be sorted via Compare) into one list.

  var
    Dummy: TD2TreeNode;
    CompareResult: Integer;
  begin
    // Это позволяет избежать проверки для Result = nil в циклах.
    // This avoids checking for Result = nil in the loops.
    Result := @Dummy;
    while Assigned(A) and Assigned(B) do
    begin
      if OperationCanceled
        then CompareResult := 0
        else CompareResult := DoCompare(A, B, Column);

      if CompareResult <= 0
        then  begin
                Result.NextSibling := A;
                Result := A;
                A := A.NextSibling;
              end
        else  begin
                Result.NextSibling := B;
                Result := B;
                B := B.NextSibling;
              end;
    end;

    // Просто добавьте список, который не равен nil (или установите конец списка результатов в nil, если оба списка равны нулю).
    // Just append the list which is not nil (or set end of result list to nil if both lists are nil).
    if Assigned(A)
      then Result.NextSibling := A
      else Result.NextSibling := B;
    // возвращает начало нового объединенного списка
    // return start of the new merged list
    Result := Dummy.NextSibling;
  end;

  //---------------------------------------------------------------------------

  function MergeDescending(A, B: PD2TreeNode): PD2TreeNode;
  // Слияние узлов A и B (которые оба должны быть отсортированы с помощью Compare) по убыванию в один список.
  // Merges A and B (which both must be sorted via Compare) into one list.

  var
    Dummy: TD2TreeNode;
    CompareResult: Integer;

  begin
    // это позволяет избежать проверки для результата = nil в циклах
    // this avoids checking for Result = nil in the loops
    Result := @Dummy;
    while Assigned(A) and Assigned(B) do
    begin
      if OperationCanceled
        then CompareResult := 0
        else CompareResult := DoCompare(A, B, Column);

      if CompareResult >= 0
        then  begin
                Result.NextSibling := A;
                Result := A;
                A := A.NextSibling;
              end
        else  begin
                Result.NextSibling := B;
                Result := B;
                B := B.NextSibling;
              end;
    end;

    // Просто добавьте список, который не равен nil (или установите конец списка результатов в nil, если оба списка равны нулю).
    // Just append the list which is not nil (or set end of result list to nil if both lists are nil).
    if Assigned(A)
      then Result.NextSibling := A
      else Result.NextSibling := B;
    // Вернем начало вновь объединенного списка.
    // Return start of the newly merged list.
    Result := Dummy.NextSibling;
  end;

  //---------------------------------------------------------------------------

  function MergeSortAscending(var Node: PD2TreeNode; N: Cardinal): PD2TreeNode;
  // Рекурсивно сортирует по возрастанию список узлов, заданных Node (узел не должен быть nil).
  // Sorts the list of nodes given by Node (which must not be nil).
  var A, B: PD2TreeNode;
  begin
    if N > 1
       then begin
              A := MergeSortAscending(Node, N div 2);
              B := MergeSortAscending(Node, (N + 1) div 2);
              Result := MergeAscending(A, B);
            end
       else begin
              Result := Node;
              Node := Node.NextSibling;
              Result.NextSibling := nil;
            end;
  end;

  //---------------------------------------------------------------------------

  function MergeSortDescending(var Node: PD2TreeNode; N: Cardinal): PD2TreeNode;
  // Рекурсивно сортирует по убыванию список узлов, заданных Node (узел не должен быть nil).
  // Sorts the list of nodes given by Node (which must not be nil).

  var A, B: PD2TreeNode;
  begin
    if N > 1
       then begin
              A := MergeSortDescending(Node, N div 2);
              B := MergeSortDescending(Node, (N + 1) div 2);
              Result := MergeDescending(A, B);
            end
       else begin
              Result := Node;
              Node := Node.NextSibling;
              Result.NextSibling := nil;
            end;
  end;

  //--------------- end local functions ---------------------------------------

var
  Run: PD2TreeNode;
  Index: Cardinal;

begin
  InterruptValidation;
  if tsEditPending in FStates then
  begin
    //KillTimer(Handle, EditTimer);
    DoStateChange([], [tsEditPending]);
  end;

  if not (tsEditing in FStates) or DoEndEdit then
  begin
    if Node = nil then
      Node := FRoot;
    if vsHasChildren in Node.States then
    begin
      if (Node.ChildCount = 0) and DoInit then
        InitChildren(Node);
      // Удостовериться, что дети действительны, и их можно сортировать.
      // Make sure the children are valid, so they can be sorted at all.
      if DoInit and (Node.ChildCount > 0)
         then ValidateChildren(Node, False);
      // Возможно, число детей изменилось.
      // Child count might have changed.
      if Node.ChildCount > 1 then
      begin
        StartOperation(okSortNode);
        try
          // Сортируем связанный список, проверяем флаг направления только один раз.
          // Sort the linked list, check direction flag only once.
          if Direction = sdAscending
            then Node.FirstChild := MergeSortAscending(Node.FirstChild, Node.ChildCount)
            else Node.FirstChild := MergeSortDescending(Node.FirstChild, Node.ChildCount);
        finally
          EndOperation(okSortNode);
        end;
        // Консолидация дочернего списка.
        // Consolidate the child list finally.
        Run := Node.FirstChild;
        Run.PrevSibling := nil;
        Index := 0;
        repeat
          Run.Index := Index;
          Inc(Index);
          if Run.NextSibling = nil
             then Break;
          Run.NextSibling.PrevSibling := Run;
          Run := Run.NextSibling;
        until False;
        Node.LastChild := Run;

        InvalidateCache;
      end;
      if FUpdateCount = 0
         then begin
                ValidateCache;
                Realign; //вместо Invalidate;
              end;
    end;
  end;
end;

procedure TD2CustomTreeGrid.SortTree(Column: Integer;
  Direction: TD2SortDirection; DoInit: Boolean);
//Сортировка всего дерева по колонке Column в направлении Direction с инициализацией узлов при необходимости.

//--------------- local function --------------------------------------------

procedure DoSort(Node: PD2TreeNode);
// Рекурсивно сортирует Node и его дочерние узлы.
// Recursively sorts Node and its child nodes.

var Run: PD2TreeNode;

begin
  Sort(Node, Column, Direction, DoInit);
  // Recurse to next level
  Run := Node.FirstChild;
  while Assigned(Run) and not FOperationCanceled do
  begin
    if DoInit and not (vsInitialized in Run.States) then
      InitNode(Run);
    if (vsInitialized in Run.States) and (not (toAutoSort in TreeOptions.AutoOptions) or Expanded[Run]) then // There is no need to sort collapsed branches
      DoSort(Run);
    Run := Run.NextSibling;
  end;
end;

//--------------- end local function ----------------------------------------

begin
if RootNode.TotalCount <= 2 then
  Exit;//Nothing to do if there are one or zero nodes. RootNode.TotalCount is 1 if there are no nodes in the treee as the root node counts too here.
// Instead of wrapping the sort using BeginUpdate/EndUpdate simply the update counter
// is modified. Otherwise the EndUpdate call will recurse here.
Inc(FUpdateCount);
try
  if Column > InvalidColumn then
  begin
    StartOperation(okSortTree);
    try
      DoSort(FRoot);
    finally
      EndOperation(okSortTree);
    end;
  end;
  InvalidateCache;
finally
  if FUpdateCount > 0 then
    Dec(FUpdateCount);
  if FUpdateCount = 0 then
  begin
    ValidateCache;
    Realign; //вместо Invalidate;
  end;
end;

end;

procedure TD2CustomTreeGrid.QuickSort(const TheArray: TD2NodeArray; L, R: Integer);
//Быстрая сортировка массива TheArray в диапазоне индексов от L до R
var
  I, J: Integer;
  P, T: Pointer;
begin
  repeat
    I := L;
    J := R;
    P := TheArray[(L + R) shr 1];
    repeat
      while PAnsiChar(TheArray[I]) < PAnsiChar(P) do
        Inc(I);
      while PAnsiChar(TheArray[J]) > PAnsiChar(P) do
        Dec(J);
      if I <= J then
      begin
        T := TheArray[I];
        TheArray[I] := TheArray[J];
        TheArray[J] := T;
        Inc(I);
        Dec(J);
      end;
    until I > J;
    if L < J then
      QuickSort(TheArray, L, J);
    L := I;
  until I >= R;
end;

//procedure TD2CustomTreeGrid.UpdateVerticalScrollBar(DoRepaint: Boolean);
////Обновляет вертикальную полосу прокрутки, чтобы отразить текущий размер и смещение дерева
//begin
//
//end;

procedure TD2CustomTreeGrid.ValidateChildren(Node: PD2TreeNode;
  Recursive: Boolean);
//Обеспечивает инициализацию всех детей (и всех их детей, если Recursive = True) узла Node.
//Узел должен быть уже инициализирован.
// Ensures that the children of the given node (and all their children, if Recursive is True) are initialized.
// Node must already be initialized
var Child: PD2TreeNode;
begin
  if Node = nil then Node := FRoot;
  if (vsHasChildren in Node.States) and (Node.ChildCount = 0)
    then InitChildren(Node);
  Child := Node.FirstChild;
  while Assigned(Child) do
  begin
    ValidateNode(Child, Recursive);
    Child := Child.NextSibling;
  end;
end;

procedure TD2CustomTreeGrid.ValidateNode(Node: PD2TreeNode; Recursive: Boolean);
//Обеспечивает инициализацию узла Node (и всех его детей и их детей, если Recursive = True)
// Ensures that the given node (and all its children, if Recursive is True) are initialized.
var Child: PD2TreeNode;
begin
  if Node = nil
    then Node := FRoot
    else if not (vsInitialized in Node.States)
           then InitNode(Node);
  if Recursive then
  begin
    if (vsHasChildren in Node.States) and (Node.ChildCount = 0)
      then InitChildren(Node);
    Child := Node.FirstChild;
    while Assigned(Child) do
    begin
      ValidateNode(Child, Recursive);
      Child := Child.NextSibling;
    end;
  end;
end;

procedure TD2CustomTreeGrid.ToggleNode(Node: PD2TreeNode);
//Изменение развернутого/свернутого состояния узла на противоположное.
// Changes a node's expand state to the opposite state.

var
  Child,
  FirstVisible: PD2TreeNode;
  HeightDelta,      //дельта изменения общей высоты
  StepsR1: Single;
  StepsR2: Single;
  Steps: Single;
  TogglingTree,
  ChildrenInView,
  NeedFullInvalidate,
  NeedUpdate,
  NodeInView,
  PosHoldable,
  TotalFit: Boolean;   //флаг полного помещения развернутого узела (с детьми) в видимую область грида
  ToggleData: TD2ToggleAnimationData;

  //--------------- local function --------------------------------------------

  //procedure PrepareAnimation;
  //
  //// Prepares ToggleData.
  //
  //var
  //  R: TD2Rect;
  //  S: Single;
  //  M: TD2ToggleAnimationMode;
  //
  //begin
  //  with ToggleData do
  //  begin
  //    //Window := Handle;       //закоментировано GoldenFox
  //    //DC := GetDC(Handle);    //закоментировано GoldenFox
  //
  //    //lcl: setting Color to Brush seems not necessary
  //    //Self.Brush.Color := FColors.BackGroundColor;;
  //
  //    //Brush := Self.Brush.Reference.Handle; //закоментировано GoldenFox
  //
  //    if (Mode1 <> tamNoScroll) and (Mode2 <> tamNoScroll) then
  //    begin
  //      if StepsR1 < StepsR2 then
  //      begin
  //        // As the primary rectangle is always R1 we will get a much smoother
  //        // animation if R1 is the one that will be scrolled more.
  //        R := R2;
  //        R2 := R1;
  //        R1 := R;
  //
  //        M := Mode2;
  //        Mode2 := Mode1;
  //        Mode1 := M;
  //
  //        S := StepsR2;
  //        StepsR2 := StepsR1;
  //        StepsR1 := S;
  //      end;
  //      ScaleFactor := StepsR2 / StepsR1;
  //      MissedSteps := 0;
  //    end;
  //
  //    if Mode1 <> tamNoScroll then
  //      Steps := StepsR1
  //    else
  //      Steps := StepsR2;
  //  end;
  //end;

  //--------------- end local function ----------------------------------------

begin
  Assert(Assigned(Node), 'Node must not be nil.');

  TogglingTree := tsToggling in FStates;   //сохраним состояние флага сворачивания/разворачивания узлов у грида
  ChildrenInView := False;
  HeightDelta := 0;
  NeedFullInvalidate := False;
  NeedUpdate := False;
  NodeInView := False;
  PosHoldable := False;
  TotalFit := False;
  // Нам не нужно переключать состояние развертывания, если узел удаляется, иначе
  // некоторые обновления (например, количество видимых узлов) делаются дважды с рискованными результатами.
  // We don't need to switch the expand state if the node is being deleted otherwise some
  // updates (e.g. visible node count) are done twice with disasterous results.

  if [vsDeleting, vsToggling] * Node.States = [] then  //если узел не удаляется или еще не изменяет состояние развертывания
  begin
    try
      DoStateChange([tsToggling]);      //установим флаг изменения состояния развертывания узлов для грида
      Include(Node.States, vsToggling); //установим флаг изменения состояния развертывания для узлоа

      if vsExpanded in Node.States then  //если узел развернут
      begin
        if DoCollapsing(Node) then       //если приложение разрешает свернуть узел
        begin
          NeedUpdate := True;               //установим флаг необходимости обновления града (узел свернулся)

          // Рассчитаем дельту изменения общей высоты, т.к. это нужно для toChildrenAbove.
          // Calculate the height delta right now as we need it for toChildrenAbove anyway.
          HeightDelta := -Node.TotalHeight + NodeHeight[Node];

          //if (FUpdateCount = 0) and
          //   (toAnimatedToggle in FOptions.FAnimationOptions) and
          //   not (tsCollapsing in FStates) then
          //begin
          //  if tsHint in Self.FStates then
          //    Application.CancelHint;
          //  Repaint; //вместо UpdateWindow(Handle);
          //
          //  //анимация сворачивания узла
          //  // animated collapsing
          //  with ToggleData do
          //  begin
          //    // Determine the animation behaviour and rectangle. If toChildrenAbove is set, the behaviour is depending
          //    // on the position of the node to be collapsed.
          //    R1 := GetDisplayRect(Node, NoColumn, False);
          //    Mode2 := tamNoScroll;
          //    if toChildrenAbove in FOptions.FPaintOptions then
          //    begin
          //      PosHoldable := (VScrollBarValue + (Node.TotalHeight - NodeHeight[Node])) <= 0;
          //      NodeInView := R1.Top < ClientHeight;
          //
          //      StepsR1 := 0;
          //      if NodeInView then
          //      begin
          //        if PosHoldable or not (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) then
          //        begin
          //          // Scroll the child nodes down.
          //          Mode1 := tamScrollDown;
          //          R1.Bottom := R1.Top;
          //          R1.Top := 0;
          //          StepsR1 := Min(R1.Bottom - R1.Top + 1, Node.TotalHeight - NodeHeight[Node]);
          //        end
          //        else
          //        begin
          //          // The position cannot be kept. So scroll the node up to its future position.
          //          Mode1 := tamScrollUp;
          //          R1.Top := Max(0, R1.Top + HeightDelta);
          //          R1.Bottom := ClientHeight;
          //          StepsR1 := VScrollBarValue - HeightDelta;
          //        end;
          //      end;
          //    end
          //    else
          //    begin
          //      if (FRangeY + VScrollBarValue - R1.Bottom + HeightDelta >= ClientHeight - R1.Bottom) or
          //         (FRangeY <= ClientHeight) or (VScrollBarValue = 0) or not
          //         (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) then
          //      begin
          //        // Do a simple scroll up over the child nodes.
          //        Mode1 := tamScrollUp;
          //        //Inc(R1.Top, NodeHeight[Node]);
          //        R1.Top := R1.Top + NodeHeight[Node];
          //        R1.Bottom := ClientHeight;
          //        StepsR1 := Min(R1.Bottom - R1.Top + 1, -HeightDelta);
          //      end
          //      else
          //      begin
          //        // Scroll the node down to its future position. As VScrollBarValue will change we need to invalidate the
          //        // whole tree.
          //        Mode1 := tamScrollDown;
          //        StepsR1 := Min(-VScrollBarValue, ClientHeight - FRangeY -VScrollBarValue - HeightDelta);
          //        R1.Top := 0;
          //        R1.Bottom := Min(ClientHeight, R1.Bottom + Steps);
          //        NeedFullInvalidate := True;
          //      end;
          //    end;
          //
          //    // No animation necessary if the node is below the current client height.
          //    if R1.Top < ClientHeight then
          //    begin
          //      PrepareAnimation;
          //      try
          //        //Animate(Steps, FAnimationDuration, ToggleCallback, @ToggleData);
          //      finally
          //        ReleaseDC(Window, DC);
          //      end;
          //    end;
          //  end;
          //end;


          // сворачиваем узел collapse the node
          AdjustTotalHeight(Node, IfThen(IsEffectivelyFiltered[Node], 0, NodeHeight[Node]));  //устанавливаем общую высоту узла и всех его родителей
          if FullyVisible[Node] then                         //если узел видим и все его родители развернуты
            Dec(FVisibleCount, CountVisibleChildren(Node));  //уменьшаем общее кол-во видимых узлов грида на кол-во видимых детей узла
          Exclude(Node.States, vsExpanded);                  //удаляем флаг развернутости у узла
          DoCollapsed(Node);                                 //прерывание после сворачивания и выбор другого узла если был выбран свернуты

          // Удаляем детей и освобождаем память, если опция toAutoFreeOnCollapse включена. Remove child nodes now, if enabled.
          if (toAutoFreeOnCollapse in FOptions.FAutoOptions) and (Node.ChildCount > 0) then
          begin
            DeleteChild(Node);                   //удаляем всех детей
            Include(Node.States, vsHasChildren); //устанавливаем флаг наличия детей у узла
          end;
        end;
      end
      else      //иначе узел свернут
        if DoExpanding(Node) then //если приложение разрешает развернуть узел
        begin
          NeedUpdate := True;          //установим флаг необходимости обновления града (узел свернулся)

          //разворачиваем узел, нужно отрегулировать высоту
          // expand the node, need to adjust the height
          if not (vsInitialized in Node.States)    //инициализируем узел если требуется
            then InitNode(Node);
          if (vsHasChildren in Node.States) and (Node.ChildCount = 0) //если у узла есть флаг наличия детей, но самих детей нет
            then InitChildren(Node);                                    //то инициализируем детей у узла

          // Если есть дети (не нужно устанавливать флаг vsExpanded, если нет детей). Avoid setting the vsExpanded style if there are no child nodes.
          if Node.ChildCount > 0 then  //если есть дети
          begin
            // Итерация дочерних узлов без их инициализации. Определяем всю высоту.
            // Iterate through the child nodes without initializing them. We have to determine the entire height.
            Child := Node.FirstChild;  //найдем первого ребенка
            repeat
              if vsVisible in Child.States then
              begin
                MeasureItemHeight(Child); // Определить и установить высоту ребенка.Ensure the item height is measured
                d2IncFloat(HeightDelta, Child.TotalHeight);  //прибавляем общую высоту ребенка к дельте высоты
              end;
              Child := Child.NextSibling; //найдем следующего ребенка
            until Child = nil;            //повторять пока не будут учтены все потомки узла Node

            // Определяем прямоугольник отображения уже здесь, т.к. это необходимо для toChildrenAbove в любом случае.
            // Getting the display rectangle is already done here as it is needed for toChildrenAbove in any case.
            if (toChildrenAbove in FOptions.FPaintOptions) or (FUpdateCount = 0) then
            begin
              with ToggleData do
              begin
                R1 := GetDisplayRect(Node, NoColumn, False);
                Mode2 := tamNoScroll;
                TotalFit := HeightDelta + NodeHeight[Node] <= ClientHeight; //если развернутый узел (с детьми) полностью помещается в видимую область грида, то установим флаг

                if toChildrenAbove in FOptions.FPaintOptions //если детей нужно показать выше родителя
                  then  begin
                          //Основная цель с установленным toChildrenAbove - сохранить визуальную позицию узлов,
                          //чтобы пользователь не запутался. Поэтому нужно прокрутить представление, когда выполняется расширение.
                          // The main goal with toChildrenAbove being set is to keep the nodes visual position so the user does
                          // not get confused. Therefore we need to scroll the view when the expanding is done.
                          PosHoldable := TotalFit and (FRangeY - ClientHeight >= 0) ;
                          ChildrenInView := (R1.Top - HeightDelta) >= 0;
                          NodeInView := R1.Bottom <= ClientHeight;
                        end
                  else  begin //иначе детей показываем ниже родителя
                          PosHoldable := TotalFit;
                          ChildrenInView := R1.Bottom + HeightDelta <= ClientHeight;
                        end;

                R1.Bottom := ClientHeight;
              end;
            end;

            //if FUpdateCount = 0 then
            //begin
            //  //анимация разворачивания узла tckb hfphtityj
            //  // Do animated expanding if enabled.
            //  if (ToggleData.R1.Top < ClientHeight) and ([tsPainting, tsExpanding] * FStates = []) and
            //    (toAnimatedToggle in FOptions.FAnimationOptions)then
            //  begin
            //    if tsHint in Self.FStates then
            //      Application.CancelHint;
            //    Repaint; //вместо UpdateWindow(Handle);
            //    // animated expanding
            //    with ToggleData do
            //    begin
            //      if toChildrenAbove in FOptions.FPaintOptions then
            //      begin
            //        // At first check if we hold the position, which is the most common case.
            //        if not (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) or
            //           (PosHoldable and ( (NodeInView and ChildrenInView) or not
            //                              (toAutoScrollOnExpand in FOptions.FAutoOptions) )) then
            //        begin
            //          Mode1 := tamScrollUp;
            //          R1 := d2Rect(R1.Left, 0, R1.Right, R1.Top);
            //          StepsR1 := Min(HeightDelta, R1.Bottom);
            //        end
            //        else
            //        begin
            //          // If we will not hold the node's visual position we mostly scroll in both directions.
            //          Mode1 := tamScrollDown;
            //          Mode2 := tamScrollUp;
            //          R2 := d2Rect(R1.Left, 0, R1.Right, R1.Top);
            //          if not (toAutoScrollOnExpand in FOptions.FAutoOptions) then
            //          begin
            //            // If we shall not or cannot scroll to the desired extent we calculate the new position (with
            //            // max VScrollBarValue applied) and animate it that way.
            //            StepsR1 := -VScrollBarValue - Max(FRangeY + HeightDelta - ClientHeight, 0) + HeightDelta;
            //            if (FRangeY + HeightDelta - ClientHeight) <= 0 then
            //              Mode2 := tamNoScroll
            //            else
            //              StepsR2 := Min(FRangeY + HeightDelta - ClientHeight, R2.Bottom);
            //          end
            //          else
            //          begin
            //            if TotalFit and NodeInView and (FRangeY + HeightDelta > ClientHeight) then
            //            begin
            //              // If the whole subtree will fit into the client area and the node is currently fully visible,
            //              // the first child will be made the top node if possible.
            //              if HeightDelta >= R1.Top then
            //                StepsR1 := Abs(R1.Top - HeightDelta)
            //              else
            //                StepsR1 := ClientHeight - FRangeY;
            //            end
            //            else
            //              if FRangeY + HeightDelta <= ClientHeight then
            //              begin
            //                // We cannot make the first child the top node as we cannot scroll to that extent,
            //                // so we do a simple scroll down.
            //                Mode2 := tamNoScroll;
            //                StepsR1 := HeightDelta;
            //              end
            //              else
            //                // If the subtree does not fit into the client area at once, the expanded node will
            //                // be made the bottom node.
            //                StepsR1 := ClientHeight - R1.Top - NodeHeight[Node];
            //
            //            if Mode2 <> tamNoScroll then
            //            begin
            //              if StepsR1 > 0 then
            //                StepsR2 := Min(R1.Top, HeightDelta - StepsR1)
            //              else
            //              begin
            //                // If the node is already at the bottom scrolling is needed.
            //                Mode1 := tamNoScroll;
            //                StepsR2 := Min(HeightDelta, R1.Bottom);
            //              end;
            //            end;
            //          end;
            //        end;
            //      end
            //      else
            //      begin
            //        // toChildrenAbove is not set.
            //        if (PosHoldable and ChildrenInView) or not (toAutoScrollOnExpand in FOptions.FAutoOptions) or not
            //           (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) or (R1.Top <= 0) then
            //        begin
            //          // If the node will stay at its visual position, do a simple down-scroll.
            //          Mode1 := tamScrollDown;
            //          //Inc(R1.Top, NodeHeight[Node]);
            //          R1.Top := R1.Top + NodeHeight[Node];
            //          StepsR1 := Min(R1.Bottom - R1.Top, HeightDelta);
            //        end
            //        else
            //        begin
            //          // We will not hold the nodes visual position so perform a double scroll.
            //          Mode1 := tamScrollUp;
            //          Mode2 := tamScrollDown;
            //
            //          R1.Bottom := R1.Top + NodeHeight[Node] + 1;
            //          R1.Top := 0;
            //          R2 := d2Rect(R1.Left, R1.Bottom, R1.Right, ClientHeight);
            //
            //          StepsR1 := Min(HeightDelta - (ClientHeight - R2.Top), R1.Bottom - NodeHeight[Node]);
            //          StepsR2 := ClientHeight - R2.Top;
            //        end;
            //      end;
            //
            //      if ClientHeight >= R1.Top then
            //      begin
            //        PrepareAnimation;
            //        try
            //          //Animate(Steps, FAnimationDuration, ToggleCallback, @ToggleData);
            //        finally
            //          ReleaseDC(Window, DC);
            //        end;
            //      end;
            //    end;
            //  end;
            //  //if toAutoSort in FOptions.FAutoOptions then
            //  //  Sort(Node, FHeader.FSortColumn, FHeader.FSortDirection, False);
            //end;// if UpdateCount = 0

            Include(Node.States, vsExpanded);                 //устанавливаем флаг развернутости у узл
            AdjustTotalHeight(Node, HeightDelta, True);       //изменяем общую высоту узла и всех его родителей на величину дельты
            if FullyVisible[Node] then                        //если узел видим и все его родители развернуты
              Inc(FVisibleCount, CountVisibleChildren(Node));   //увеличиваем общее кол-во видимых узлов грида на кол-во видимых детей узла
            DoExpanded(Node);                                 //прерывание после разворачивания
          end;
        end;

      if NeedUpdate then   //если произошло изменение кол-ва видимых узлов
      begin
        InvalidateCache;   //пометить кэш как недействительный
        if FUpdateCount = 0 then   //если можно перерисовываться
        begin
          ValidateCache;             //пересчитаем кэш

          //изменение положения скроллера
          //if Node.ChildCount > 0     //если у узла есть дети
          //  then  begin
          //          UpdateRanges;    //обновим витруальные размеры клиетнской области грида
          //          //Realign; //вместо UpdateScrollBars(True)
          //          if [tsPainting, tsExpanding] * FStates = [] then
          //          begin
          //            if (vsExpanded in Node.States) and                      //если узел развернут и
          //               ((toAutoScrollOnExpand in FOptions.FAutoOptions) or  //       установлен режим прокрутка дерева после раскрытия или
          //                (toChildrenAbove in FOptions.FPaintOptions))        //       установлен режим показа детей выше родителя
          //              then  begin
          //                      if toChildrenAbove in FOptions.FPaintOptions    //если установлен режим показа детей выше родителя
          //                        then  begin
          //                                NeedFullInvalidate := True;              //установим флаг полного обновления грида
          //                                if (PosHoldable and ChildrenInView and NodeInView) or not
          //                                   (toAutoScrollOnExpand in FOptions.FAutoOptions)
          //                                  then  ScrollTo(HeightDelta); //SetOffsetY(VScrollBarValue - HeightDelta)
          //                                  else  if TotalFit and NodeInView
          //                                          then  begin
          //                                                  FirstVisible := GetFirstVisible(Node, True);
          //                                                  if Assigned(FirstVisible)  // иначе нет видимого ребенка otherwise there is no visible child at all
          //                                                    then SetOffsetY(VScrollBarValue - GetDisplayRect(FirstVisible, NoColumn, False).Top);
          //                                                end
          //                                          else BottomNode := Node;
          //                              end
          //                        else  begin  //иначе детей показываем ниже родителей
          //                                // Прокручиваем как можно больше дочерних узлов, если узел был развернут.
          //                                // Scroll as much child nodes into view as possible if the node has been expanded.
          //                                if PosHoldable
          //                                  then NeedFullInvalidate := ScrollIntoView(GetLastVisible(Node, True), False)
          //                                  else  begin
          //                                          TopNode := Node;
          //                                          NeedFullInvalidate := True;
          //                                        end;
          //                              end;
          //                    end
          //              else  begin
          //                      // If we have collapsed the node or toAutoScrollOnExpand is not set, we try to keep the nodes
          //                      // visual position.
          //                      if toChildrenAbove in FOptions.FPaintOptions then
          //                        SetOffsetY(VScrollBarValue - Integer(HeightDelta));
          //                      NeedFullInvalidate := True;
          //                    end;
          //          end;
          //
          //          //UpdateScrollBars(True); Moved up
          //
          //          // Check for automatically scrolled tree.
          //          if NeedFullInvalidate
          //            then Realign  //вместо UpdateScrollBars(True) и Invalidate
          //            else InvalidateToBottom(Node);
          //        end
          //  else InvalidateNode(Node);
          UpdateRanges;
          Realign;  //перерисуем грид
        end
        else UpdateRanges;  //если перерисовывать нельзя, то обновим только витруальные размеры клиетнской области грида
      end;

    finally
      Exclude(Node.States, vsToggling);    //снимем флаг изменения состояния развертывания для узлоа
      if not TogglingTree then             //если флага изменения состояния развертывания у грида до начала операции не было
        DoStateChange([], [tsToggling]);     //то снимем флаг
    end;
  end;
end;

procedure TD2CustomTreeGrid.UpdateHorizontalRange;
//Обновить общую витруальную ширину дерева
begin
  FRangeX:= GetContentWidth;
end;

procedure TD2CustomTreeGrid.UpdateRanges;
//Обновить общие витруальные высоту и ширину дерева
begin
  UpdateVerticalRange;
  UpdateHorizontalRange;
end;

procedure TD2CustomTreeGrid.UpdateVerticalRange;
//Обновить общую витруальную высоту дерева
begin
  //Общая высота узла включает в себя высоту невидимого корневого узла.
  // Total node height includes the height of the invisible root node.
  if FRoot.TotalHeight < FDefaultNodeHeight then
    FRoot.TotalHeight := FDefaultNodeHeight;
  FRangeY := FRoot.TotalHeight - FRoot.NodeHeight + FBottomSpace;
end;

