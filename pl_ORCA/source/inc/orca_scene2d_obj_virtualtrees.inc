{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

{ -------------------- TD2CustomVirtualTreeOptions -------------------- }

constructor TD2CustomVirtualTreeOptions.Create(AOwner: TD2BaseVirtualTree);

begin
  FOwner := AOwner;
  FPaintOptions := DefaultPaintOptions;
  FAnimationOptions := DefaultAnimationOptions;
  FAutoOptions := DefaultAutoOptions;
  FSelectionOptions := DefaultSelectionOptions;
  FMiscOptions := DefaultMiscOptions;
end;

procedure TD2CustomVirtualTreeOptions.SetAnimationOptions(const Value: TD2VTAnimationOptions);
begin
  FAnimationOptions := Value;
end;

procedure TD2CustomVirtualTreeOptions.SetAutoOptions(const Value: TD2VTAutoOptions);
//Установить опиции автоматической обработки определенных ситуаций.

var ChangedOptions: TD2VTAutoOptions;
begin
  if FAutoOptions <> Value then
  begin
    // Exclusive ORing to get all entries wich are in either set but not in both.
    ChangedOptions := FAutoOptions + Value - (FAutoOptions * Value);
    FAutoOptions := Value;
    if Assigned(FOwner) then
      with FOwner do
    //  if (toAutoSpanColumns in ChangedOptions) and not (csLoading in ComponentState) and HandleAllocated then
    //    Invalidate;
        if (toAutoSpanColumns in ChangedOptions) and not (csLoading in ComponentState) then Realign;
  end;
end;

procedure TD2CustomVirtualTreeOptions.SetMiscOptions(const Value: TD2VTMiscOptions);  //установить прочие опции
var
  ToBeSet,
  ToBeCleared: TD2VTMiscOptions;
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
          if toCheckSupport in ToBeSet + ToBeCleared then
            //Invalidate;
            Realign;

          if not (csDesigning in ComponentState) then
          begin
            {!!! проверить необходимость данной опции. Вероятно не нужна
            if toFullRepaintOnResize in (ToBeSet + ToBeCleared) then
              //todo_lcl_check
              RecreateWnd(FOwner);
              }
            {!!! не понятно как получить Handle дерева. Есть только Handle сцены
            if toAcceptOLEDrop in ToBeSet then
              RegisterDragDrop(Handle, DragManager as IDropTarget);
            if toAcceptOLEDrop in ToBeCleared then
              RevokeDragDrop(Handle);
            }
          end;
        end;
  end;
end;

procedure TD2CustomVirtualTreeOptions.SetPaintOptions(const Value: TD2VTPaintOptions);
//var
  //ToBeSet,
  //ToBeCleared: TD2VTPaintOptions;
  //Run: TD2TreeNodeArray;
  //HandleWasAllocated: Boolean;
begin
  //if FPaintOptions <> Value then
  //begin
  //  ToBeSet := Value - FPaintOptions;
  //  ToBeCleared := FPaintOptions - Value;
  //  FPaintOptions := Value;
  //  if (toFixedIndent in ToBeSet) then
  //  begin
  //    // Fixes issue #388
  //    Include(FPaintOptions, toShowRoot);
  //    Include(ToBeSet, toShowRoot);
  //  end;//if
  //  if Assigned(FOwner) then
  //    with FOwner do
  //    begin
  //      if not (csLoading in ComponentState) and (toShowFilteredNodes in ToBeSet + ToBeCleared) then
  //      begin
  //        Repaint;
  //        InterruptValidation;
  //        Run := GetFirstNoInit;
  //        while Assigned(Run) do
  //        begin
  //          if (vsFiltered in Run.States) then
  //          begin
  //            if FullyVisible[Run] then
  //            begin
  //              if toShowFilteredNodes in ToBeSet then
  //                Inc(FVisibleCount)
  //              else
  //                Dec(FVisibleCount);
  //            end;
  //            if toShowFilteredNodes in ToBeSet then
  //              AdjustTotalHeight(Run, Run.NodeHeight, True)
  //            else
  //              AdjustTotalHeight(Run, -Run.NodeHeight, True);
  //          end;
  //          Run := GetNextNoInit(Run);
  //        end;
  //        if HandleWasAllocated then
  //          EndUpdate;
  //      end;
  //      {!!! не актурально
  //      if IsWinVistaOrAbove and ((tsUseThemes in FStates) or
  //         ((toThemeAware in ToBeSet) and StyleServices.Enabled)) and
  //         (toUseExplorerTheme in (ToBeSet + ToBeCleared)) and not VclStyleEnabled then
  //        if (toUseExplorerTheme in ToBeSet) then
  //        begin
  //          SetWindowTheme('explorer');
  //          DoStateChange([tsUseExplorerTheme]);
  //        end
  //        else
  //          if toUseExplorerTheme in ToBeCleared then
  //          begin
  //            SetWindowTheme('');
  //            DoStateChange([], [tsUseExplorerTheme]);
  //          end;
  //
  //      if not (csLoading in ComponentState) then
  //      begin
  //        if ((toThemeAware in ToBeSet + ToBeCleared) or (toUseExplorerTheme in ToBeSet + ToBeCleared) or VclStyleEnabled) then
  //        begin
  //          if ((toThemeAware in ToBeSet) and StyleServices.Enabled) or VclStyleEnabled then
  //            DoStateChange([tsUseThemes])
  //          else
  //            if (toThemeAware in ToBeCleared) then
  //              DoStateChange([], [tsUseThemes]);
  //
  //          PrepareBitmaps(True, False);
  //          RedrawWindow(Handle, nil, 0, RDW_INVALIDATE or RDW_VALIDATE or RDW_FRAME);
  //        end;
  //           }
  //        if toChildrenAbove in ToBeSet + ToBeCleared then
  //        begin
  //          InvalidateCache;
  //          if FUpdateCount = 0 then
  //          begin
  //            ValidateCache;
  //            //Invalidate;
  //            Realign;
  //          end;
  //        end;
  //
  //        //Invalidate;
  //        Realign;
  //      end;
  //    end;
end;

procedure TD2CustomVirtualTreeOptions.SetSelectionOptions(const Value: TD2VTSelectionOptions);
//var
  //ToBeSet,
  //ToBeCleared: TD2VTSelectionOptions;

begin
  //if FSelectionOptions <> Value then
  //begin
  //  ToBeSet := Value - FSelectionOptions;
  //  ToBeCleared := FSelectionOptions - Value;
  //  FSelectionOptions := Value;
  //
  //  with FOwner do
  //  begin
  //    if (toMultiSelect in (ToBeCleared + ToBeSet)) or
  //      ([toLevelSelectConstraint, toSiblingSelectConstraint] * ToBeSet <> []) then
  //      ClearSelection;
  //
  //    if (toExtendedFocus in ToBeCleared) and (FFocusedColumn > 0) and HandleAllocated then
  //    begin
  //      FFocusedColumn := FHeader.MainColumn;
  //      //Invalidate;
  //      Realign;
  //    end;
  //
  //    if not (toExtendedFocus in FSelectionOptions) then
  //      FFocusedColumn := FHeader.MainColumn;
  //  end;
  //end;
end;

procedure TD2CustomVirtualTreeOptions.AssignTo(Dest: TPersistent);
begin
  if Dest is TD2CustomVirtualTreeOptions then
  begin
    with TD2CustomVirtualTreeOptions(Dest) do
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

{ -------------------- TD2BaseVirtualTree -------------------- }

procedure TD2BaseVirtualTree.AdjustTotalCount(Node: PD2TreeNode;
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

procedure TD2BaseVirtualTree.AdjustTotalHeight(Node: PD2TreeNode; Value: Single; Relative: Boolean);
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

function TD2BaseVirtualTree.ChangeCheckState(Node: PD2TreeNode;
  Value: TD2CheckState): Boolean;
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

function TD2BaseVirtualTree.CompareNodePositions(Node1, Node2: PD2TreeNode;
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

procedure TD2BaseVirtualTree.FixupTotalCount(Node: TD2TreeNodeArray);
//Пересчитать общую сумму узла Node и его детей
//Вызывается после загрузки поддерева из потока. Количество детей в каждом узле
//уже установлено, но не их общая сумма.
// Called after loading a subtree from stream. The child count in each node is already set but not
// their total count.
var Child: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.FixupTotalHeight(Node: TD2TreeNodeArray);
// Пересчитать общую высоту узла Node
// Вызывается после загрузки поддерева из потока. Индивидуальная высота каждого
// узла уже установлена, но их общая высота нуждается в корректировке в зависимости
// от их состояния видимости.
// Called after loading a subtree from stream. The individual height of each node is set already,
// but their total height needs an adjustment depending on their visibility state.
var Child: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.GetBottomNode: TD2TreeNodeArray;
begin
  Result := InternalGetNodeAt(0, ClientHeight - 1);
end;

function TD2BaseVirtualTree.GetCheckedCount: Integer;
var Node: TD2TreeNodeArray;
begin
  Result := 0;
  Node := GetFirstChecked;
  while Assigned(Node) do
  begin
     Inc(Result);
     Node := GetNextChecked(Node);
  end;
end;

function TD2BaseVirtualTree.GetCheckState(Node: TD2TreeNodeArray): TD2CheckState;
begin
  Result := Node.CheckState;
end;

function TD2BaseVirtualTree.GetCheckType(Node: TD2TreeNodeArray): TD2CheckType;
begin
  Result := Node.CheckType;
end;

function TD2BaseVirtualTree.GetChildCount(Node: TD2TreeNodeArray): Cardinal;
begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.ChildCount
  else
    Result := Node.ChildCount;
end;

function TD2BaseVirtualTree.GetChildrenInitialized(Node: TD2TreeNodeArray): Boolean;
begin
  Result := not (vsHasChildren in Node.States) or (Node.ChildCount > 0);
end;

function TD2BaseVirtualTree.GetCutCopyCount: Integer;
var Node: TD2TreeNodeArray;
begin
  Result := 0;
  Node := GetFirstCutCopy;
  while Assigned(Node) do
  begin
     Inc(Result);
     Node := GetNextCutCopy(Node);
  end;
end;

function TD2BaseVirtualTree.GetDisabled(Node: TD2TreeNodeArray): Boolean;
begin
  Result := Assigned(Node) and (vsDisabled in Node.States);
end;

//function TD2BaseVirtualTree.GetDragManager: IVTDragManager;
//// Returns the internal drag manager interface. If this does not yet exist then it is created here.
//begin
//  if FDragManager = nil then
//  begin
//    FDragManager := DoCreateDragManager;
//    if FDragManager = nil then
//      FDragManager := TVTDragManager.Create(Self);
//  end;
//  Result := FDragManager;
//end;

function TD2BaseVirtualTree.GetExpanded(Node: TD2TreeNodeArray): Boolean;
begin
  if Assigned(Node) then
    Result := vsExpanded in Node.States
  else
    Result := False;
end;

function TD2BaseVirtualTree.GetFiltered(Node: TD2TreeNodeArray): Boolean;
begin
  Result := vsFiltered in Node.States;
end;

function TD2BaseVirtualTree.GetFullyVisible(Node: TD2TreeNodeArray): Boolean;
//True - узел видим и все его родители видимы и развернуты.
// Determines whether the given node has the visibility flag set as well as all its parents are expanded.
begin
  Assert(Assigned(Node), 'Invalid parameter GetFullyVisible.');
  Result := vsVisible in Node.States;
  if Result and (Node <> FRoot)
    then Result := VisiblePath[Node];
end;

function TD2BaseVirtualTree.GetHasChildren(Node: TD2TreeNodeArray): Boolean;
begin
  if Assigned(Node) then
    Result := vsHasChildren in Node.States
  else
    Result := vsHasChildren in FRoot.States;
end;

function TD2BaseVirtualTree.GetMultiline(Node: TD2TreeNodeArray): Boolean;
begin
  Result := Assigned(Node) and (Node <> FRoot) and (vsMultiline in Node.States);
end;

function TD2BaseVirtualTree.GetNodeHeight(Node: TD2TreeNodeArray): Single;
//Получить высоту узла Node
begin
  if Assigned(Node) and (Node <> FRoot)
    then  begin
            if (toVariableNodeHeight in FOptions.FMiscOptions) and not (vsDeleting in Node.States) then
            begin
              if not (vsInitialized in Node.States)
                then InitNode(Node);

              // Ensure the node's height is determined.
              MeasureItemHeight(Self.Canvas, Node);
            end;
            Result := Node.NodeHeight;
          end
    else  Result := 0;
end;

function TD2BaseVirtualTree.GetNodeParent(Node: TD2TreeNodeArray): TD2TreeNodeArray;
//Получить родителя узла Node
begin
  if Assigned(Node) and (Node.Parent <> FRoot)
    then Result := Node.Parent
    else Result := nil;
end;

function TD2BaseVirtualTree.GetOffsetXY: TD2Point;
begin
  Result := d2Point(FOffsetX, FOffsetY);
end;

function TD2BaseVirtualTree.GetRangeX: Single;
begin
  Result := Max(0, FRangeX);
end;

function TD2BaseVirtualTree.GetRootNodeCount: Cardinal;
begin
  Result := FRoot.ChildCount;
end;

function TD2BaseVirtualTree.GetSelected(Node: TD2TreeNodeArray): Boolean;
//True если узел существует и выбран
begin
  Result := Assigned(Node) and (vsSelected in Node.States);
end;

function TD2BaseVirtualTree.GetTopNode: TD2TreeNodeArray;
begin
  Result := InternalGetNodeAt(0, 0);
end;

function TD2BaseVirtualTree.GetTotalCount: Cardinal;
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

function TD2BaseVirtualTree.GetVerticalAlignment(Node: TD2TreeNodeArray): Single;
begin
  Result := Node.Align;
end;

function TD2BaseVirtualTree.GetVisible(Node: TD2TreeNodeArray): Boolean;
//True - узел Node видимый.
// Determines if the given node is marked as being visible.
begin
  if Node = nil then Node := FRoot;
  if not (vsInitialized in Node.States)
    then InitNode(Node);
  Result := vsVisible in Node.States;
end;

function TD2BaseVirtualTree.GetVisiblePath(Node: TD2TreeNodeArray): Boolean;
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

function TD2BaseVirtualTree.HasVisibleNextSibling(Node: TD2TreeNodeArray): Boolean;
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

function TD2BaseVirtualTree.HasVisiblePreviousSibling(Node: TD2TreeNodeArray): Boolean;
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

procedure TD2BaseVirtualTree.InterruptValidation;
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

procedure TD2BaseVirtualTree.InitRootNode(OldSize: Cardinal);
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

function TD2BaseVirtualTree.IsFirstVisibleChild(Parent, Node: TD2TreeNodeArray ): Boolean;
//True - если узел Node является первым видимым ребенком узла Parent.
// Helper method to check if Node is the same as the first visible child of Parent.
var Run: TD2TreeNodeArray;
begin
  // Find first visible child.
  Run := Parent.FirstChild;
  while Assigned(Run) and not IsEffectivelyVisible[Run] do
    Run := Run.NextSibling;

  Result := Assigned(Run) and (Run = Node);
end;

function TD2BaseVirtualTree.IsLastVisibleChild(Parent, Node: TD2TreeNodeArray): Boolean;
//True - если узел Node является последним видимым ребенком узла Parent.
// Helper method to check if Node is the same as the last visible child of Parent.
var Run: TD2TreeNodeArray;
begin
  // Find last visible child.
  Run := Parent.LastChild;
  while Assigned(Run) and not IsEffectivelyVisible[Run] do
    Run := Run.PrevSibling;

  Result := Assigned(Run) and (Run = Node);
end;

function TD2BaseVirtualTree.MakeNewNode: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.PackArray(const TheArray: TD2TreeNodeArray; Count: Integer): Integer;
//Удаляет узлы, которые больше не используются, из массива выбора. Возвращает кол-во оставшихся узлов?
var
  Source, Dest: ^TD2TreeNodeArray;
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

function TD2BaseVirtualTree.PackArray(TheArray: TD2TreeNodeArray; Count: Integer): Integer; assembler;
//Удаляет узлы, которые больше не используются, из массива выбора. Возвращает кол-во оставшихся узлов?
//Массив выбора должен быть отсортирован для работы этого алгоритма. У удаляемых
//узлов установлен бит 0 (LSB). Этот маленький трюк работает, потому что выделение
//памяти всегда выровнены на DWORD. Поскольку массив выбор должен быть отсортирован
//при определении удаляемых узлов, гораздо более эффективно увеличить запись в запросе
//вместо того, чтобы установить его в Nil (что нарушило бы упорядоченный внешний вид списка).

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

procedure TD2BaseVirtualTree.SetAnimationDuration(const Value: Cardinal);
begin
  FAnimationDuration := Value;
  if FAnimationDuration = 0 then
    Exclude(FOptions.FAnimationOptions, toAnimatedToggle)
  else
    Include(FOptions.FAnimationOptions, toAnimatedToggle);
end;

procedure TD2BaseVirtualTree.SetBottomNode(Node: TD2TreeNodeArray);
var
  Run: TD2TreeNodeArray;
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
    R := GetDisplayRect(Node, FHeader.MainColumn, True);
    DoSetOffsetXY(d2Point(FOffsetX, FOffsetY + ClientHeight - R.Top - Integer(NodeHeight[Node])),
      [suoRepaintScrollBars, suoUpdateNCArea]);
  end;
end;

procedure TD2BaseVirtualTree.SetBottomSpace(const Value: Single);
begin
  if FBottomSpace <> Value then
  begin
    FBottomSpace := Value;
    UpdateVerticalScrollBar(True);
  end;
end;

procedure TD2BaseVirtualTree.SetCheckState(Node: TD2TreeNodeArray; Value: TD2CheckState);

begin
  if (Node.CheckState <> Value) and not (vsDisabled in Node.States) and DoChecking(Node, Value) then
    DoCheckClick(Node, Value);
end;

procedure TD2BaseVirtualTree.SetCheckType(Node: TD2TreeNodeArray; Value: TD2CheckType);

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

procedure TD2BaseVirtualTree.SetChildCount(Node: TD2TreeNodeArray;
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
  Child: TD2TreeNodeArray;
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
              DoMeasureItem(Canvas, Child, lNodeHeight);
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
          if (FUpdateCount = 0) and (toAutoSort in FOptions.FAutoOptions) and
             (FHeader.FSortColumn > InvalidColumn)
            then Sort(Node, FHeader.FSortColumn, FHeader.FSortDirection, True);

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
          UpdateScrollBars(True);
          //Invalidate;
          Realign; //вместо Invalidate
        end;

        if Node = FRoot then
          StructureChange(nil, crChildAdded)
        else
          StructureChange(Node, crChildAdded);
      end;
    end;
  end;
end;

procedure TD2BaseVirtualTree.SetDefaultNodeHeight(Value: Single);
//Установить высоту узла по умолчанию
begin
  if Value = 0 then
    Value := 18;
  if FDefaultNodeHeight <> Value then
  begin
    //Inc(Integer(FRoot.TotalHeight), Integer(Value) - Integer(FDefaultNodeHeight));
    FRoot.TotalHeight := FRoot.TotalHeight + Value - FDefaultNodeHeight;
    //Inc(SmallInt(FRoot.NodeHeight), Integer(Value) - Integer(FDefaultNodeHeight));
    FRoot.NodeHeight := FRoot.NodeHeight + Value - FDefaultNodeHeight;

    FDefaultNodeHeight := Value;
    InvalidateCache;
    if (FUpdateCount = 0) {and HandleAllocated} and not (csLoading in ComponentState) then
    begin
      ValidateCache;
      UpdateScrollBars(True);
      ScrollIntoView(FFocusedNode, toCenterScrollIntoView in FOptions.SelectionOptions, True);
      Realign; //вместо Invalidate;
    end;
  end;
end;

procedure TD2BaseVirtualTree.SetDisabled(Node: TD2TreeNodeArray; Value: Boolean);
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

procedure TD2BaseVirtualTree.SetExpanded(Node: TD2TreeNodeArray; Value: Boolean);
//Устанавливает флаг vsExpanded (развернутый) у узла Node в соответствии с Value
begin
 if Assigned(Node) and (Node <> FRoot) and (Value xor (vsExpanded in Node.States)) then
    ToggleNode(Node);
end;

procedure TD2BaseVirtualTree.SetFiltered(Node: TD2TreeNodeArray; Value: Boolean);
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
      UpdateScrollBars(True);
      Realign; //вместо  Invalidate;
    end;
  end;
end;

procedure TD2BaseVirtualTree.SetFocusedColumn(Value: TD2ColumnIndex);
begin
  if (FFocusedColumn <> Value) and
     DoFocusChanging(FFocusedNode, FFocusedNode, FFocusedColumn, Value) then
  begin
    CancelEditNode;
    InvalidateColumn(FFocusedColumn);
    InvalidateColumn(Value);
    FFocusedColumn := Value;
    if Assigned(FFocusedNode) and not (toDisableAutoscrollOnFocus in FOptions.FAutoOptions) then
    begin
      if ScrollIntoView(FFocusedNode, toCenterScrollIntoView in FOptions.SelectionOptions, True) then
        InvalidateNode(FFocusedNode);
    end;

    if Assigned(FDropTargetNode) then
      InvalidateNode(FDropTargetNode);

    DoFocusChange(FFocusedNode, FFocusedColumn);
  end;
end;

procedure TD2BaseVirtualTree.SetFocusedNode(Value: TD2TreeNodeArray);
var WasDifferent: Boolean;
begin
  WasDifferent := Value <> FFocusedNode;
  DoFocusNode(Value, True);
  // Do change event only if there was actually a change.
  if WasDifferent and (FFocusedNode = Value) then
    DoFocusChange(FFocusedNode, FFocusedColumn);
end;

procedure TD2BaseVirtualTree.SetFullyVisible(Node: TD2TreeNodeArray; Value: Boolean);
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

procedure TD2BaseVirtualTree.SetHasChildren(Node: TD2TreeNodeArray; Value: Boolean);
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

procedure TD2BaseVirtualTree.SetNodeDataSize(Value: Integer);
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

procedure TD2BaseVirtualTree.SetNodeHeight(Node: TD2TreeNodeArray; Value: Single);
//Установить в Value высоту узла Node
var Difference: Integer;
begin
  if Assigned(Node) and (Node <> FRoot) and (Node.NodeHeight <> Value) and not (toReadOnly in FOptions.FMiscOptions) then
  begin
    Difference := Integer(Value) - Integer(Node.NodeHeight);
    Node.NodeHeight := Value;

    // If the node is effectively filtered out, nothing else has to be done, as it is not visible anyway.
    if not IsEffectivelyFiltered[Node] then
    begin
      AdjustTotalHeight(Node, Difference, True);

      // If an edit operation is currently active then update the editors boundaries as well.
      UpdateEditBounds;

      InvalidateCache;
      // Stay away from touching the node cache while it is being validated.
      if not (tsValidating in FStates) and FullyVisible[Node] and not IsEffectivelyFiltered[Node] then
      begin
        if (FUpdateCount = 0) and ([tsPainting, tsSizing] * FStates = []) then
        begin
          ValidateCache;
          InvalidateToBottom(Node);
          UpdateScrollBars(True);
        end;
      end;
    end;
  end;
end;

procedure TD2BaseVirtualTree.SetNodeParent(Node: TD2TreeNodeArray; const Value: TD2TreeNodeArray);
//Задать родителя Value для узла Node
begin
  if Assigned(Node) and Assigned(Value) and (Node.Parent <> Value) then
    MoveTo(Node, Value, amAddChildLast, False);
end;

procedure TD2BaseVirtualTree.SetMultiline(Node: TD2TreeNodeArray;
  const Value: Boolean);
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

procedure TD2BaseVirtualTree.SetOffsetX(const Value: Single);

begin
  //DoSetOffsetXY(d2Point(Value, FOffsetY), DefaultScrollUpdateFlags);
end;

procedure TD2BaseVirtualTree.SetOffsetXY(const Value: TD2Point);

begin
  //DoSetOffsetXY(Value, DefaultScrollUpdateFlags);
end;

procedure TD2BaseVirtualTree.SetOffsetY(const Value: Single);

begin
  //DoSetOffsetXY(d2Point(FOffsetX, Value), DefaultScrollUpdateFlags);
end;

procedure TD2BaseVirtualTree.SetOptions(const Value: TD2CustomVirtualTreeOptions);
//Установить опции поведения дерева
begin
  FOptions.Assign(Value);
end;

procedure TD2BaseVirtualTree.SetRootNodeCount(Value: Cardinal);
begin
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

procedure TD2BaseVirtualTree.SetSelected(Node: TD2TreeNodeArray; Value: Boolean);
//Установить статус "выбран" узла Node в соответствие с Value
begin
  if not FSelectionLocked and Assigned(Node) and (Node <> FRoot) and (Value xor (vsSelected in Node.States)) then
  begin
    if Value then
    begin
      if FSelectionCount = 0 then
        FRangeAnchor := Node
      else
        if not (toMultiSelect in FOptions.FSelectionOptions) then
          ClearSelection;

      AddToSelection(Node);

      // Make sure there is a valid column selected (if there are columns at all).
      if ((FFocusedColumn < 0) or not (coVisible in FHeader.Columns[FFocusedColumn].Options)) and
        (FHeader.MainColumn > NoColumn) then
        if ([coVisible, coAllowFocus] *  FHeader.Columns[FHeader.MainColumn].Options = [coVisible, coAllowFocus]) then
          FFocusedColumn := FHeader.MainColumn
        else
          FFocusedColumn := FHeader.Columns.GetFirstVisibleColumn(True);
      if FRangeAnchor = nil then
        FRangeAnchor := Node;
    end
    else
    begin
      RemoveFromSelection(Node);
      if FSelectionCount = 0 then
        ResetRangeAnchor;
    end;
    if FullyVisible[Node] and not IsEffectivelyFiltered[Node] then
      InvalidateNode(Node);
  end;
end;

procedure TD2BaseVirtualTree.SetTopNode(Node: TD2TreeNodeArray);
//var
  //R: TRect;
  //Run: TD2TreeNodeArray;
begin
  //if Assigned(Node) then
  //begin
  //  // make sure all parents of the node are expanded
  //  Run := Node.Parent;
  //  while Run <> FRoot do
  //  begin
  //    if not (vsExpanded in Run.States) then
  //      ToggleNode(Run);
  //    Run := Run.Parent;
  //  end;
  //  R := GetDisplayRect(Node, FHeader.MainColumn, True);
  //  //lclheader
  //  if hoVisible in FHeader.Options then
  //    Dec(R.Top, FHeader.Height);
  //  SetOffsetY(FOffsetY - R.Top);
  //end;
end;

procedure TD2BaseVirtualTree.SetVerticalAlignment(Node: TD2TreeNodeArray;
  Value: Single);
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

procedure TD2BaseVirtualTree.SetVisible(Node: TD2TreeNodeArray; Value: Boolean);
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
      UpdateScrollBars(True);
      //Invalidate;
      Realign; //вместо Invalidate
    end;
  end;
end;


procedure TD2BaseVirtualTree.SetVisiblePath(Node: TD2TreeNodeArray; Value: Boolean);
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

function TD2BaseVirtualTree.ToggleCallback(Step, StepSize: Integer;
  Data: Pointer): Boolean;
//var
//  Column: TD2ColumnIndex;
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
begin
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
end;

procedure TD2BaseVirtualTree.AddToSelection(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.AddToSelection(const NewItems: TD2TreeNodeArray;
  NewLength: Integer; ForceInsert: Boolean = False);
//Добавляет узлы, указанные в массиве NewItems в массив текущего выбора.
//NewLength - количество узлов, для добавления (необходимо, чтобы позволить
//NewItems быть больше, чем фактические используемые записи).
//ForceInsert = True, если узлы должны быть вставлены без учета ограничения
//уровня выбора или уже установленных флагов (например, при загрузке из потока).
//Внимание! В случае ForceInsert = True то вызывающий несет ответственность за то,
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

procedure TD2BaseVirtualTree.AdviseChangeEvent(StructureChange: Boolean;
  Node: TD2TreeNodeArray; Reason: TD2ChangeReason);
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
            //  begin
            //    if HandleAllocated
            //      then KillTimer(Handle,StructureChangeTimer)//;
            //  end
            //else
              DoStateChange([tsStructureChangePending]);

          FLastStructureChangeNode := Node;
          if FLastStructureChangeReason = crIgnore
            then FLastStructureChangeReason := Reason
            else if Reason <> crIgnore
                   then FLastStructureChangeReason := crAccumulated;
        end
   else begin
          if tsChangePending in FStates then
          //  KillTimer(Handle, ChangeTimer)
          //else
            DoStateChange([tsChangePending]);
          FLastChangedNode := Node;
        end;
end;

function TD2BaseVirtualTree.AllocateInternalDataArea(Size: Cardinal): Cardinal;
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

procedure TD2BaseVirtualTree.Animate(Steps, Duration: Single;
  Callback: TD2VTAnimationCallback; Data: Pointer);
begin

end;

procedure TD2BaseVirtualTree.Change(Node: TD2TreeNodeArray);
begin
  AdviseChangeEvent(False, Node, crIgnore);

  if FUpdateCount = 0 then
  begin
    {if (FChangeDelay > 0) and not (tsSynchMode in FStates)
      then SetTimer(Handle, ChangeTimer, FChangeDelay, nil)
      else} DoChange(Node);
  end;
end;

function TD2BaseVirtualTree.CheckParentCheckState(Node: TD2TreeNodeArray;
  NewCheckState: TD2CheckState): Boolean;
//Проверяет все братья и сестры узла Node что бы определить кокое состояние отметки должен получить родитель.
// Checks all siblings of node to determine which check state Node's parent must get.

var
  CheckCount,
  BoxCount: Cardinal;
  PartialCheck: Boolean;
  Run: TD2TreeNodeArray;

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

procedure TD2BaseVirtualTree.ClearTempCache;
//Обеспечивает надежное состояние кэша временного узла.
//make sure the temporary node cache is in a reliable state
begin
  FTempNodeCache := nil;
  FTempNodeCount := 0;
end;

function TD2BaseVirtualTree.CountLevelDifference(Node1, Node2: TD2TreeNodeArray
  ): Integer;
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

function TD2BaseVirtualTree.CountVisibleChildren(Node: TD2TreeNodeArray): Cardinal;
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

procedure TD2BaseVirtualTree.DetermineHiddenChildrenFlag(Node: TD2TreeNodeArray);
// Обновление флага vsAllChildrenHidden (все дети скрыты) у узла Node.
// Update the hidden children flag of the given node.
var Run: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.DetermineHiddenChildrenFlagAllNodes;
// Обновление флага vsAllChildrenHidden (все дети скрыты) у всех неинициализированных узлов.
var Run: TD2TreeNodeArray;
begin
  Run := GetFirstNoInit(False);
  while Assigned(Run) do
  begin
    DetermineHiddenChildrenFlag(Run);
    Run := GetNextNoInit(Run);
  end;
end;

function TD2BaseVirtualTree.DetermineNextCheckState(CheckType: TD2CheckType;
  CheckState: TD2CheckState): TD2CheckState;
//Определяет следующее состояние отметки если пользователь щелкнет на значек отметки или нажмет клавишу пробел.
// Determines the next check state in case the user click the check image or pressed the space key.
begin
  case CheckType of
    ctTriStateCheckBox,
    ctCheckBox:
      if CheckState = csCheckedNormal then
        Result := csUncheckedNormal
      else
        Result := csCheckedNormal;
    ctRadioButton:
      Result := csCheckedNormal;
    ctButton:
      Result := csUncheckedNormal;
  else
    Result := csMixedNormal;
  end;
end;

function TD2BaseVirtualTree.DoCancelEdit: Boolean;
//Отменяет текущие действие редактирования или отложенного редактирования.
// Called when the current edit action or a pending edit must be cancelled.
begin
  //KillTimer(Handle, EditTimer);
  DoStateChange([], [tsEditPending]);
  Result := (tsEditing in FStates) and FEditLink.CancelEdit;
  if Result then
  begin
    DoStateChange([], [tsEditing]);
    if Assigned(FOnEditCancelled) then
      FOnEditCancelled(Self, FEditColumn);
    FEditLink := nil;
  end;
end;

procedure TD2BaseVirtualTree.DoChange(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.DoCheckClick(Node: TD2TreeNodeArray; NewCheckState: TD2CheckState);

begin
  if ChangeCheckState(Node, NewCheckState) then
    DoChecked(Node);
end;

procedure TD2BaseVirtualTree.DoChecked(Node: TD2TreeNodeArray);

begin
  if Assigned(FOnChecked) then
    FOnChecked(Self, Node);

  {$ifdef EnableAccessible}
  NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}
end;

function TD2BaseVirtualTree.DoChecking(Node: TD2TreeNodeArray; var NewCheckState: TD2CheckState): Boolean;
//Определяет может ли узел Node изменить состояние отметки проверки на NewCheckState и вызывает прерывание OnChecking
// Determines if a node is allowed to change its check state to NewCheckState.
begin
  if toReadOnly in FOptions.FMiscOptions then
    Result := False
  else
  begin
    Result := True;
    if Assigned(FOnChecking) then
      FOnChecking(Self, Node, NewCheckState, Result);
  end;
end;

procedure TD2BaseVirtualTree.DoCollapsed(Node: TD2TreeNodeArray);
var
  lFirstSelected: TD2TreeNodeArray;
  lParent: TD2TreeNodeArray;
begin
  if Assigned(FOnCollapsed) then
    FOnCollapsed(Self, Node);
  {$ifdef EnableAccessible}
  if Assigned(FAccessibleItem) then
    NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}

  if (toAlwaysSelectNode in TreeOptions.SelectionOptions) then
  begin
    // Select the next visible parent if the currently selected node gets invisible due to a collapse
    // This makes the VT behave more like the Win32 custom TreeView control
    // This makes only sense no no multi selection is allowed and if there is a selected node at all
    lFirstSelected := GetFirstSelected();
    if Assigned(lFirstSelected) and not FullyVisible[lFirstSelected] then
    begin
      lParent := GetVisibleParent(lFirstSelected);
      Selected[lParent] := True;
      Selected[lFirstSelected] := False;
    end;//if
    //if there is (still) no selected node, then use FNextNodeToSelect to select one
    if SelectedCount = 0 then
      EnsureNodeSelected();
  end;//if
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2BaseVirtualTree.DoCollapsing(Node: TD2TreeNodeArray): Boolean;

begin
  Result := True;
  if Assigned(FOnCollapsing) then
    FOnCollapsing(Self, Node, Result);
end;

function TD2BaseVirtualTree.DoCompare(Node1, Node2: TD2TreeNodeArray;
  Column: TD2ColumnIndex): Integer;
//Вызывает прерывание сравнения узлов (OnCompareNodes)
begin
  Result := 0;
  if Assigned(FOnCompareNodes) then
    FOnCompareNodes(Self, Node1, Node2, Column, Result);
end;

function TD2BaseVirtualTree.DoCreateEditor(Node: TD2TreeNodeArray;
  Column: TD2ColumnIndex): ID2VTEditLink;
begin
  Result := nil;
  if Assigned(FOnCreateEditor) then
    FOnCreateEditor(Self, Node, Column, Result);
end;

procedure TD2BaseVirtualTree.DoCanEdit(Node: TD2TreeNodeArray;
  Column: TD2ColumnIndex; var Allowed: Boolean);
begin
  if Assigned(FOnEditing)
    then FOnEditing(Self, Node, Column, Allowed);
end;

procedure TD2BaseVirtualTree.DoEdit;
//Начать редактирование узла,имеющего фокус
begin
  Application.CancelHint;
  //KillTimer(Handle, ScrollTimer);
  //KillTimer(Handle, EditTimer);
  DoStateChange([], [tsEditPending]);
  if Assigned(FFocusedNode) and not (vsDisabled in FFocusedNode.States) and
    not (toReadOnly in FOptions.FMiscOptions) and (FEditLink = nil) then
  begin
    FEditLink := DoCreateEditor(FFocusedNode, FEditColumn);
    if Assigned(FEditLink) then
    begin
      DoStateChange([tsEditing], [tsDrawSelecting, tsDrawSelPending, tsToggleFocusedSelection, tsOLEDragPending,
        tsOLEDragging, tsClearPending, tsScrollPending, tsScrolling, tsMouseCheckPending]);
      ScrollIntoView(FFocusedNode, toCenterScrollIntoView in FOptions.SelectionOptions,
        not (toDisableAutoscrollOnEdit in FOptions.AutoOptions));
      if FEditLink.PrepareEdit(Self, FFocusedNode, FEditColumn) then
      begin
        UpdateEditBounds;
        // Node needs repaint because the selection rectangle and static text must disappear.
        InvalidateNode(FFocusedNode);
        if not FEditLink.BeginEdit then
          DoStateChange([], [tsEditing]);
      end
      else
        DoStateChange([], [tsEditing]);
      if not (tsEditing in FStates) then
        FEditLink := nil;
    end;
  end;
end;

function TD2BaseVirtualTree.DoEndEdit: Boolean;
//Закончить редактирование и вызвать предывание OnEdited
begin
  //KillTimer(Handle, EditTimer);
  Result := (tsEditing in FStates) and FEditLink.EndEdit;
  if Result then
  begin
    DoStateChange([], [tsEditing]);
    FEditLink := nil;
    if Assigned(FOnEdited) then
      FOnEdited(Self, FFocusedNode, FEditColumn);
  end;
  DoStateChange([], [tsEditPending]);
end;

procedure TD2BaseVirtualTree.DoEndOperation(OperationKind: TD2VTOperationKind);
begin
  if Assigned(FOnEndOperation) then
    FOnEndOperation(Self, OperationKind)
end;

procedure TD2BaseVirtualTree.DoEnter;
begin
  EnsureNodeSelected();
end;

procedure TD2BaseVirtualTree.DoFocusChange(Node: TD2TreeNodeArray; Column: TD2ColumnIndex);
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

function TD2BaseVirtualTree.DoFocusChanging(OldNode, NewNode: TD2TreeNodeArray;
  OldColumn, NewColumn: TD2ColumnIndex): Boolean;
begin
  Result := (OldColumn = NewColumn) or FHeader.AllowFocus(NewColumn);
  if Assigned(FOnFocusChanging) then
    FOnFocusChanging(Self, OldNode, NewNode, OldColumn, NewColumn, Result);
end;

procedure TD2BaseVirtualTree.DoFocusNode(Node: TD2TreeNodeArray; Ask: Boolean);
begin
  if not (tsEditing in FStates) or EndEditNode then
  begin
    if Node = FRoot then
      Node := nil;
    if (FFocusedNode <> Node) and (not Ask or DoFocusChanging(FFocusedNode, Node, FFocusedColumn, FFocusedColumn)) then
    begin
      if Assigned(FFocusedNode) then
      begin
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

    // Have to scroll the node into view, even it is the same node as before.
    if Assigned(FFocusedNode) then
    begin
      // Make sure a valid column is set if columns are used and no column has currently the focus.
      if FHeader.UseColumns and (not FHeader.FColumns.IsValidColumn(FFocusedColumn)) then
        FFocusedColumn := FHeader.MainColumn;
      // Do automatic expansion of the newly focused node if enabled.
      if (toAutoExpand in FOptions.FAutoOptions) and not (vsExpanded in FFocusedNode.States) then
        ToggleNode(FFocusedNode);
      InvalidateNode(FFocusedNode);
      if (FUpdateCount = 0) and not (toDisableAutoscrollOnFocus in FOptions.FAutoOptions) then
        ScrollIntoView(FFocusedNode, (toCenterScrollIntoView in FOptions.SelectionOptions) and
          (MouseButtonDown * FStates = []), not (toFullRowSelect in FOptions.SelectionOptions) );
    end;

    // Reset range anchor if necessary.
    if FSelectionCount = 0 then
      ResetRangeAnchor;
  end;
end;

procedure TD2BaseVirtualTree.DoFreeNode(Node: TD2TreeNodeArray);
//Вызывает прерывание при освобождении узла
begin
  // Prevent invalid references
  if Node = FLastChangedNode then FLastChangedNode := nil;
  if Node = FCurrentHotNode then FCurrentHotNode := nil;
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

function TD2BaseVirtualTree.DoInitChildren(Node: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.DoInitNode(Parent, Node: TD2TreeNodeArray;
//Вызов прерывания OnInitNode
var InitStates: TD2TreeNodeInitStates);
begin
  if Assigned(FOnInitNode) then
    FOnInitNode(Self, Parent, Node, InitStates);
end;

procedure TD2BaseVirtualTree.DoLoadUserData(Node: TD2TreeNodeArray;
  Stream: TStream);
begin
  if Assigned(FOnLoadNode) then
    if Node = FRoot then
      FOnLoadNode(Self, nil, Stream)
    else
      FOnLoadNode(Self, Node, Stream);
end;

procedure TD2BaseVirtualTree.DoMeasureItem(TargetCanvas: TD2Canvas;
  Node: TD2TreeNodeArray; var NodeHeight: Single);
//Вызов прерывания определения высоты узла
begin
  if Assigned(FOnMeasureItem) then
  FOnMeasureItem(Self, TargetCanvas, Node, NodeHeight);
end;

procedure TD2BaseVirtualTree.DoNodeCopied(Node: TD2TreeNodeArray);

begin
  if Assigned(FOnNodeCopied) then
    FOnNodeCopied(Self, Node);
end;

function TD2BaseVirtualTree.DoNodeCopying(Node, NewParent: TD2TreeNodeArray): Boolean;

begin
  Result := True;
  if Assigned(FOnNodeCopying) then
    FOnNodeCopying(Self, Node, NewParent, Result);
end;

procedure TD2BaseVirtualTree.DoNodeMoved(Node: TD2TreeNodeArray);
//Вызов прерывания OnNodeMoved после перемещения узла Node к новому родителю
begin
  if Assigned(FOnNodeMoved) then
    FOnNodeMoved(Self, Node);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2BaseVirtualTree.DoNodeMoving(Node, NewParent: TD2TreeNodeArray): Boolean;
//Вызов прерывания OnNodeMoving перед перемещением узла Node к новому родителю NewParent
begin
  Result := True;
  if Assigned(FOnNodeMoving) then
    FOnNodeMoving(Self, Node, NewParent, Result);
end;

procedure TD2BaseVirtualTree.DoRemoveFromSelection(Node: TD2TreeNodeArray);
//Вызов прерывания удалении узла Node из списка выделенных
begin
  if Assigned(FOnRemoveFromSelection) then
    FOnRemoveFromSelection(Self, Node);
end;

procedure TD2BaseVirtualTree.DoReset(Node: TD2TreeNodeArray);
begin
  if Assigned(FOnResetNode) then
    FOnResetNode(Self, Node);
end;

procedure TD2BaseVirtualTree.DoSaveUserData(Node: TD2TreeNodeArray;
  Stream: TStream);
//Вызов прерывания OnSaveNode при записи узла Node в поток Stream
begin
  if Assigned(FOnSaveNode)
    then if Node = FRoot
           then FOnSaveNode(Self, nil, Stream)
           else FOnSaveNode(Self, Node, Stream);
end;

function TD2BaseVirtualTree.DoSetOffsetXY(Value: TD2Point;
  Options: TD2ScrollUpdateOptions; ClipRect: PRect): Boolean;
begin

end;

procedure TD2BaseVirtualTree.DoStartOperation(OperationKind: TD2VTOperationKind);
begin
  if Assigned(FOnStartOperation) then
    FOnStartOperation(Self, OperationKind);
end;

procedure TD2BaseVirtualTree.DoStateChange(Enter: TD2VirtualTreeStates;
  Leave: TD2VirtualTreeStates);
//Изменяет текущие флаги состояния дерева: Enter - добавляемые, Leave - исключаемые
var
  ActualEnter,
  ActualLeave: TD2VirtualTreeStates;
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

procedure TD2BaseVirtualTree.DoStructureChange(Node: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.EndOperation(OperationKind: TD2VTOperationKind);
// Called to indicate that a long-running operation has finished.

begin
  Assert(FOperationCount > 0, 'EndOperation must not be called when no operation in progress.');
  Dec(FOperationCount);
  DoEndOperation(OperationKind);
end;

function TD2BaseVirtualTree.FindNodeInSelection(P: TD2TreeNodeArray;
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
    if PAnsiChar(FSelection[I]) < PAnsiChar(P)
      then L := I + 1
      else  begin
              H := I - 1;
              if FSelection[I] = P then
              begin
                Result := True;
                L := I;
              end;
            end;
  end;
  Index := L;
end;

procedure TD2BaseVirtualTree.FinishChunkHeader(Stream: TStream; StartPos,
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

function TD2BaseVirtualTree.GetOperationCanceled: Boolean;
begin
  Result := FOperationCanceled and (FOperationCount > 0);
end;

procedure TD2BaseVirtualTree.InitChildren(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.InitNode(Node: TD2TreeNodeArray);
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
        UpdateScrollBars(True);
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

procedure TD2BaseVirtualTree.InternalAddFromStream(Stream: TStream;
  Version: Integer; Node: TD2TreeNodeArray);
// Загружает все детали узла Node (в том числе его детей) из потока Stream.
// Поскольку новые узлы могут быть выбраны этот метод также фиксирует массив выбора.
// Loads all details for Node (including its children) from the given stream.
// Because the new nodes might be selected this method also fixes the selection array.
var
  Stop: TD2TreeNodeArray;
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
      if (vsSelected in Node.States) and not FindNodeInSelection(Node, Index, 0, High(FSelection)) then
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

function TD2BaseVirtualTree.InternalAddToSelection(Node: TD2TreeNodeArray;
  ForceInsert: Boolean): Boolean;
begin
  Assert(Assigned(Node), 'Node must not be nil!'); //прервать выполение если Node=Nil
  FSingletonNodeArray[0] := Node;
  Result := InternalAddToSelection(FSingletonNodeArray, 1, ForceInsert);  //передать выполнение во внутреннюю процедуру
end;

procedure TD2BaseVirtualTree.InternalDisconnectNode(Node: TD2TreeNodeArray;
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
  Run: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.InternalGetNodeAt(X, Y: Single): TD2TreeNodeArray;
//Получить ссылку на узел по координатам
var Dummy: Integer;
begin
  Result := InternalGetNodeAt(X, Y, True, Dummy);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2BaseVirtualTree.InternalGetNodeAt(X, Y: Single; Relative: Boolean; var NodeTop: Integer): TD2TreeNodeArray;
//Получить ссылку на узел по координатам
//lclheader this is the original version of GetNodeAt used internally since expects coordinates
//  relative to the image tree. In LCL the image tree and control coordinates are different
//  when header is visible

// This method returns the node that occupies the specified point, or nil if there's none.
// If Relative is True then X and Y are given in client coordinates otherwise they are considered as being
// absolute values into the virtual tree image (regardless of the current offsets in the tree window).
// NodeTop gets the absolute or relative top position of the node returned or is untouched if no node
// could be found.

//var
  //AbsolutePos,
  //CurrentPos: Cardinal;

begin
  //if Y < 0 then
  //  Y := 0;
  //
  //AbsolutePos := Y;
  //if Relative then
  //  Inc(AbsolutePos, -FOffsetY);
  //
  //// CurrentPos tracks a running term of the current position to test for.
  //// It corresponds always to the top position of the currently considered node.
  //CurrentPos := 0;
  //
  //// If the cache is available then use it.
  //if tsUseCache in FStates then
  //  Result := FindInPositionCache(AbsolutePos, CurrentPos)
  //else
  //  Result := GetFirstVisibleNoInit(nil, True);
  //
  //// Determine node, of which position and height corresponds to the scroll position most closely.
  //while Assigned(Result) and (Result <> FRoot) do
  //begin
  //  if AbsolutePos < (CurrentPos + NodeHeight[Result]) then
  //    Break;
  //  Inc(CurrentPos, NodeHeight[Result]);
  //  Result := GetNextVisibleNoInit(Result, True);
  //end;
  //
  //if Result = FRoot then
  //  Result := nil;
  //
  //// Since the given vertical position is likely not the same as the top position
  //// of the found node this top position is returned.
  //if Assigned(Result) then
  //begin
  //  NodeTop := CurrentPos;
  //  if Relative then
  //    Inc(NodeTop, FOffsetY);
  //  //{$ifdef DEBUG_VTV}Logger.Send([lcPaintHeader],'GetNodeAt Result: ',Result^.Index);{$endif}
  //end;
end;

procedure TD2BaseVirtualTree.InternalRemoveFromSelection(Node: TD2TreeNodeArray);
//Внутренняя версия метода RemoveFromSelection для удаления узла Node из массива выбранных.
//Для окончательного удаления узла необходимо использовать функцию PackArray.
// Special version to mark a node to be no longer in the current selection. PackArray must
// be used to remove finally those entries.

var Index: Integer;
begin
  // Because pointers are always DWORD aligned we can simply increment all those
  // which we want to have removed (see also PackArray) and still have the
  // order in the list preserved.
  if FindNodeInSelection(Node, Index, -1, -1) then
  begin
    Exclude(Node.States, vsSelected);
    Inc(PAnsiChar(FSelection[Index]));
    DoRemoveFromSelection(Node);
    AdviseChangeEvent(False, Node, crIgnore);
  end;
end;

function TD2BaseVirtualTree.InternalAddToSelection(const NewItems: TD2TreeNodeArray;
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
  // and would do the job here too but has a serious problem with already sorted lists like FSelection.

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
    // 3) Make room in FSelection for the new items.
    if FSelectionCount + NewLength >= Length(FSelection) then
      SetLength(FSelection, FSelectionCount + NewLength);

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
        while (J >= 0) and (PAnsiChar(NewItems[J]) > PAnsiChar(FSelection[CurrentEnd])) do
        begin
          FSelection[CurrentEnd + J + 1] := NewItems[J];
          Dec(J);
        end;
        // early out if nothing more needs to be copied
        if J < 0 then
          Break;
      end
      else
      begin
        // insert remaining new entries at position 0
        Move(NewItems[0], FSelection[0], (J + 1) * SizeOf(Pointer));
        // nothing more to do so exit main loop
        Break;
      end;

      // find the last entry in the remaining selection list which is smaller then the largest
      // entry in the remaining new items list
      FindNodeInSelection(NewItems[J], I, 0, CurrentEnd);
      Dec(I);
      // move all entries which are greater than the greatest entry in the new items list up
      // so the remaining gap travels down to where new items must be inserted
      Move(FSelection[I + 1], FSelection[I + J + 2], (CurrentEnd - I) * SizeOf(Pointer));
      CurrentEnd := I;
    end;
    Inc(FSelectionCount, NewLength);
  end;
end;

procedure TD2BaseVirtualTree.InternalCacheNode(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.InternalConnectNode(Node, Destination: TD2TreeNodeArray;
  Target: TD2BaseVirtualTree; Mode: TD2VTNodeAttachMode);
//Присоединить узел Node к узлу Destination в дереве Target в зависимости от Mode.
//Ни какие проверки не проводятся. Узлы Node и Destination должны быть действительным.
//Узлы Node и Destination не должен быть корневыми если Mode amInsertBefore или amInsertAfter.
// Connects Node with Destination depending on Mode.
// No error checking takes place. Node as well as Destination must be valid.
// Node must never be a root node and Destination must not be a root node if Mode
// is amInsertBefore or amInsertAfter.
var Run: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.InternalClearSelection;
//Внутренняя процедура очистки массива выбора узлов
var Count: Integer;
begin
  //Вполне возможно, что есть недействительные ссылки на узлы в массиве выбора
  //если обновление дерева заблокировано и были внесены изменения в структуре.
  //Обработка этой потенциально опасной ситуации путем явной упаковки массива выбора.
  // It is possible that there are invalid node references in the selection array
  // if the tree update is locked and changes in the structure were made.
  // Handle this potentially dangerous situation by packing the selection array explicitely.
  if FUpdateCount > 0 then
  begin
    Count := PackArray(FSelection, FSelectionCount);
    if Count > -1 then
    begin
      FSelectionCount := Count;
      SetLength(FSelection, FSelectionCount);
    end;
  end;

  while FSelectionCount > 0 do
  begin
    Dec(FSelectionCount);
    Exclude(FSelection[FSelectionCount].States, vsSelected);
    DoRemoveFromSelection(FSelection[FSelectionCount]);
  end;
  ResetRangeAnchor;
  FSelection := nil;
  DoStateChange([], [tsClearPending]);
end;

procedure TD2BaseVirtualTree.InvalidateCache;
//Пометить кэш недействительным.
// Marks the cache as invalid.
begin
  DoStateChange([tsValidationNeeded], [tsUseCache]);
  //ChangeTreeStatesAsync([csValidationNeeded], [csUseCache]);
end;

procedure TD2BaseVirtualTree.MarkCutCopyNodes;
//Устанавливает флаг vsCutOrCopy в каждом выбранном в данный момент узле, кроме
//недействительных чтобы указать, что они является частью операции с буфером обмена.
// Sets the vsCutOrCopy style in every currently selected but not disabled node
// to indicate it is now part of a clipboard operation.
var
  Nodes: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.ReadChunk(Stream: TStream; Version: Integer;
  Node: TD2TreeNodeArray; ChunkType, ChunkSize: Integer): Boolean;
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
  Run: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.ReadNode(Stream: TStream; Version: Integer; Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.RemoveFromSelection(Node: TD2TreeNodeArray);
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
        Move(FSelection[Index + 1], FSelection[Index], (FSelectionCount - Index - 1) * SizeOf(Pointer));
      if FSelectionCount > 0 then
        Dec(FSelectionCount);
      SetLength(FSelection, FSelectionCount);

      if FSelectionCount = 0 then
        ResetRangeAnchor;

      if FSelectionCount <= 1 then
        UpdateNextNodeToSelect(Node);

      DoRemoveFromSelection(Node);
      Change(Node);
    end;
  end;
end;

procedure TD2BaseVirtualTree.ResetRangeAnchor;
//Вызывается, когда больше нет выбранного узла и для якоря массива выбора узлов нужно новое значение.
// Called when there is no selected node anymore and the selection range anchor needs a new value.
begin
  FRangeAnchor := FFocusedNode;
  FLastSelectionLevel := -1;
end;

procedure TD2BaseVirtualTree.SelectNodes(StartNode, EndNode: TD2TreeNodeArray;
  AddOnly: Boolean);
//Выбирает диапазон узлов от узла StartNode до узла и EndNode отменяет выделение всех
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
  LastAnchor: TD2TreeNodeArray;
  Index: Integer;

begin
  Assert(Assigned(EndNode), 'EndNode must not be nil!');
  if not FSelectionLocked then
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

procedure TD2BaseVirtualTree.SetFocusedNodeAndColumn(Node: TD2TreeNodeArray; Column: TD2ColumnIndex);

var
  OldColumn: TD2ColumnIndex;
  WasDifferent: Boolean;

begin
  if not FHeader.AllowFocus(Column) then
    Column := FFocusedColumn;

  WasDifferent := (Node <> FFocusedNode) or (Column <> FFocusedColumn);

  OldColumn := FFocusedColumn;
  FFocusedColumn := Column;

  DoFocusNode(Node, True);

  // Check if the change was accepted.
  if FFocusedNode = Node then
  begin
    CancelEditNode;
    if WasDifferent then
      DoFocusChange(FFocusedNode, FFocusedColumn);
  end
  else
    // If the user did not accept the new cell to focus then set also the focused column back
    // to its original state.
    FFocusedColumn := OldColumn;
end;

procedure TD2BaseVirtualTree.SkipNode(Stream: TStream);
//Игнорирует данные для следующего узла в потоке Stream (в том числе и дочерние узлы).
// Skips the data for the next node in the given stream (including the child nodes).
var
  Header: TD2ChunkHeader;

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

procedure TD2BaseVirtualTree.StartOperation(OperationKind: TD2VTOperationKind);
// Вызывается, чтобы указать, что была начата длительная операция.
// Called to indicate that a long-running operation has been started.
begin
  Inc(FOperationCount);
  DoStartOperation(OperationKind);
  if FOperationCount = 1 then
    FOperationCanceled := False;
end;

procedure TD2BaseVirtualTree.StructureChange(Node: TD2TreeNodeArray;
  Reason: TD2ChangeReason);

begin
  AdviseChangeEvent(True, Node, Reason);

  if FUpdateCount = 0 then
  begin
    {if (FChangeDelay > 0) and not (tsSynchMode in FStates)
      then SetTimer(Handle, StructureChangeTimer, FChangeDelay, nil)
      else }DoStructureChange(Node, Reason);
  end;
end;

procedure TD2BaseVirtualTree.ValidateCache;
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

procedure TD2BaseVirtualTree.ValidateNodeDataSize(var Size: Integer);
//Определяет размер области данных узла дерева
begin
  Size := SizeOf(Pointer);
  if Assigned(FOnGetNodeDataSize) then
    FOnGetNodeDataSize(Self, Size);
end;

procedure TD2BaseVirtualTree.UpdateEditBounds;
//Обновление границ редактора узла, если редактирование активно.
// Used to update the bounds of the current node editor if editing is currently active.

//var
//  R: TRect;
//  Dummy: Integer;
//  CurrentAlignment: TAlignment;
//  CurrentBidiMode: TBidiMode;

begin
  //if (tsEditing in FStates) and Assigned(FFocusedNode) and
  //   (FEditColumn < FHeader.Columns.Count) then // prevent EArgumentOutOfRangeException
  //begin
  //  if (GetCurrentThreadId <> MainThreadID) then
  //  begin
  //    // UpdateEditBounds() will be called at the end of the thread
  //    Exit;
  //  end;
  //  if vsMultiline in FFocusedNode.States then
  //    R := GetDisplayRect(FFocusedNode, FEditColumn, True, False)
  //  else
  //    R := GetDisplayRect(FFocusedNode, FEditColumn, True, True);
  //  if (toGridExtensions in FOptions.FMiscOptions) then
  //  begin
  //    // Adjust edit bounds depending on alignment and bidi mode.
  //    if FEditColumn <= NoColumn then
  //    begin
  //      CurrentAlignment := Alignment;
  //      CurrentBidiMode := BiDiMode;
  //    end
  //    else
  //    begin
  //      CurrentAlignment := FHeader.Columns[FEditColumn].FAlignment;
  //      CurrentBidiMode := FHeader.Columns[FEditColumn].FBiDiMode;
  //    end;
  //    // Consider bidi mode here. In RTL context does left alignment actually mean right alignment and vice versa.
  //    if CurrentBidiMode <> bdLeftToRight then
  //      ChangeBiDiModeAlignment(CurrentAlignment);
  //    if CurrentAlignment = taLeftJustify then
  //      FHeader.Columns.GetColumnBounds(FEditColumn, Dummy, R.Right)
  //    else
  //      FHeader.Columns.GetColumnBounds(FEditColumn, R.Left, Dummy);
  //  end;
  //  if toShowHorzGridLines in TreeOptions.PaintOptions then
  //    Dec(R.Bottom);
  //  R.Bottom := R.Top + Max(R.Bottom - R.Top, FEditLink.GetBounds.Bottom - FEditLink.GetBounds.Top); // Ensure to never decrease the size of the currently active edit control. Helps to prevent issue #159
  //  FEditLink.SetBounds(R);
  //end;
end;

procedure TD2BaseVirtualTree.UpdateNextNodeToSelect(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.WriteChunks(Stream: TStream; Node: TD2TreeNodeArray);
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
  Run: TD2TreeNodeArray;

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

procedure TD2BaseVirtualTree.WriteNode(Stream: TStream; Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.DoExpanded(Node: TD2TreeNodeArray);
begin
  if Assigned(FOnExpanded) then
    FOnExpanded(Self, Node);
  {$ifdef EnableAccessible}
  NotifyWinEvent(EVENT_OBJECT_STATECHANGE, Handle, OBJID_CLIENT, CHILDID_SELF);
  {$endif}
end;

function TD2BaseVirtualTree.DoExpanding(Node: TD2TreeNodeArray): Boolean;
begin
  Result := True;
  if Assigned(FOnExpanding) then
    FOnExpanding(Self, Node, Result);
end;

function TD2BaseVirtualTree.AbsoluteIndex(Node: TD2TreeNodeArray): Cardinal;
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

function TD2BaseVirtualTree.AddChild(Parent: TD2TreeNodeArray; UserData: Pointer): TD2TreeNodeArray;
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

    // Locally stop updates of the tree in order to avoid usage of the new node before it is correctly set up.
    // If the update count was 0 on enter then there will be a correct update at the end of this method.
    Inc(FUpdateCount);
    try
      SetChildCount(Parent, Parent.ChildCount + 1);
      // Update the hidden children flag of the parent. Nodes are added as being visible by default.
      Exclude(Parent.States, vsAllChildrenHidden);
    finally
      Dec(FUpdateCount);
    end;
    Result := Parent.LastChild;

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

    InvalidateCache;
    if FUpdateCount = 0 then
    begin
      ValidateCache;
      if tsStructureChangePending in FStates then
      begin
        if Parent = FRoot then
          StructureChange(nil, crChildAdded)
        else
          StructureChange(Parent, crChildAdded);
      end;

      if (toAutoSort in FOptions.FAutoOptions) and (FHeader.FSortColumn > InvalidColumn) then
        Sort(Parent, FHeader.FSortColumn, FHeader.FSortDirection, True);

      InvalidateToBottom(Parent);
      //lcl
      //Calling UpdateHorizontalScrollBar without a header leads to a
      //wrong NodeWidth because the node is not initialized at this time.
      //As result the horizontal scrollbar is not correctly
      //sized and the node can not be selected by a click.

      //if HandleAllocated then
        UpdateVerticalScrollBar(True)
   end;
  end
  else
    Result := nil;
end;

procedure TD2BaseVirtualTree.AddFromStream(Stream: TStream;
  TargetNode: TD2TreeNodeArray);
//Загружает узлы из потока Stream и добавляет их к TargetNode. Текущее содержимое
//не очищается до начала процесса загрузки (см также LoadFromStream)
// loads nodes from the given stream and adds them to TargetNode the current content
// is not cleared before the load process starts (see also LoadFromStream)
var
  ThisID: TD2MagicID;
  Version,
  Count: Cardinal;
  Node: TD2TreeNodeArray;
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
      if Version <= VTTreeStreamVersion  then
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

function TD2BaseVirtualTree.CancelEditNode: Boolean;
//Вызывается приложением или текущим редактором для отмены редактирования.
// Called by the application or the current edit link to cancel the edit action.

begin
  if {HandleAllocated and }([tsEditing, tsEditPending] * FStates <> []) then
    Result := DoCancelEdit
  else
    Result := True;
end;

function TD2BaseVirtualTree.CanEdit(Node: TD2TreeNodeArray; Column: TD2ColumnIndex
  ): Boolean;
// True - если данный узел может быть отредактирован.
// Returns True if the given node can be edited.
begin
  Result := (toEditable in FOptions.FMiscOptions) and Enabled and not (toReadOnly in FOptions.FMiscOptions)
    and ((Column < 0) or (coEditable in FHeader.Columns[Column].Options));
  DoCanEdit(Node, Column, Result);
end;

procedure TD2BaseVirtualTree.CancelOperation;
//Вызывается приложением для отмены длительной операции.
// Called by the application to cancel a long-running operation.
begin
  if FOperationCount > 0 then
    FOperationCanceled := True;
end;

procedure TD2BaseVirtualTree.Clear;

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
      FCurrentHotNode := nil;
      FDropTargetNode := nil;
      FLastChangedNode := nil;
      FRangeAnchor := nil;
      FCheckNode := nil;
      FLastVCLDragTarget := nil;
      FLastSearchNode := nil;
      DeleteChild(FRoot, True);
      FOffsetX := 0;
      FOffsetY := 0;

    finally
      EndUpdate;
    end;
  end;
end;

procedure TD2BaseVirtualTree.ClearChecked;
var Node: TD2TreeNodeArray;
begin
  Node := RootNode.FirstChild;
  while Assigned(Node) do
  begin
    if Node.CheckState <> csUncheckedNormal then
      CheckState[Node] := csUncheckedNormal;
    Node := GetNextNoInit(Node);
  end;
end;

procedure TD2BaseVirtualTree.ClearSelection;
//Очистить массив выбора узлов
var
  Node: TD2TreeNodeArray;
  R: TD2Rect;
  Counter: Integer;
begin
  if not FSelectionLocked and (FSelectionCount > 0) and not (csDestroying in ComponentState) then
  begin
    if (FUpdateCount = 0) and {HandleAllocated and} (FVisibleCount > 0) then
    begin
      // Iterate through nodes currently visible in the client area and invalidate them.
      Node := TopNode;
      if Assigned(Node) then
        R := GetDisplayRect(Node, NoColumn, False);
      Counter := FSelectionCount;

      while Assigned(Node) do
      begin
        R.Bottom := R.Top + Integer(NodeHeight[Node]);
        if vsSelected in Node.States then
        begin
          //InvalidateRect(Handle, @R, False);
          InvalidateRect(R);
          Dec(Counter);
          // Only try as many nodes as are selected.
          if Counter = 0 then
            Break;
        end;
        R.Top := R.Bottom;
        if R.Top > ClientHeight then
          Break;
        Node := GetNextVisibleNoInit(Node, True);
      end;
    end;

    InternalClearSelection;
    Change(nil);
  end;
end;

function TD2BaseVirtualTree.CopyTo(Source: TD2TreeNodeArray; Tree: TD2BaseVirtualTree;
  Mode: TD2VTNodeAttachMode; ChildrenOnly: Boolean): TD2TreeNodeArray;
// Упрощенный метод CopyTo, чтобы скопировать узел Source в корень другого дерева Tree.
// A simplified CopyTo method to allow to copy nodes to the root of another tree.
begin
  Result := CopyTo(Source, Tree.FRoot, Mode, ChildrenOnly);
end;

//----------------------------------------------------------------------------------------------------------------------

function TD2BaseVirtualTree.CopyTo(Source, Target: TD2TreeNodeArray;
  Mode: TD2VTNodeAttachMode; ChildrenOnly: Boolean): TD2TreeNodeArray;
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
  TargetTree: TD2BaseVirtualTree;
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
            TargetTree.InternalAddFromStream(Stream, VTTreeStreamVersion, Result);
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
          UpdateScrollBars(True);
          Realign;// вместо Invalidate;
        end;
        StructureChange(Source, crNodeCopied);
      end;
    end;
  end;
end;

procedure TD2BaseVirtualTree.DeleteChild(Node: TD2TreeNodeArray; ResetHasChildren: Boolean);
//Удаление всех детей и их детей узла Node из памяти, не меняя флмг vsHasChildren узла.
// Removes all children and their children from memory without changing the vsHasChildren style by default.
var
  Run,
  Mark: TD2TreeNodeArray;
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
      LastLeft := -FEffectiveOffsetX;
      LastTop := FOffsetY;

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
      NewSize := PackArray(FSelection, FSelectionCount);
      if NewSize > -1 then
      begin
        FSelectionCount := NewSize;
        SetLength(FSelection, FSelectionCount);
      end;

      ValidateCache;
      UpdateScrollBars(True);
      // Invalidate entire tree if it scrolled e.g. to make the last node also the
      // bottom node in the treeview.
      if (LastLeft <> FOffsetX) or (LastTop <> FOffsetY)
        then {Invalidate} Realign
        else InvalidateToBottom(Node);
    end;
    StructureChange(Node, crChildDeleted);
  end
  else if ResetHasChildren then
    Exclude(Node.States, vsHasChildren);
end;

procedure TD2BaseVirtualTree.DeleteNode(Node: TD2TreeNodeArray; Reindex: Boolean);
//Удаление узла Node и всех его детей и их детей и т.д.
var
  LastTop,
  LastLeft: Single;
  LastParent: TD2TreeNodeArray;
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

    LastLeft := -FEffectiveOffsetX;
    LastTop := FOffsetY;

    if vsSelected in Node.States then
    begin
      if FUpdateCount = 0 then
      begin
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
        UpdateScrollBars(True);
        // Invalidate entire tree if it scrolled e.g. to make the last node also the
        // bottom node in the treeview.
        if (LastLeft <> FOffsetX) or (LastTop <> FOffsetY) then
          //Invalidate;
          Realign; // вместо Invalidate
      end;
    end;
  end;

end;

procedure TD2BaseVirtualTree.DeleteSelectedNodes;
// Удаляет все выбранные узлы (включая их дочерние узлы).
// Deletes all currently selected nodes (including their child nodes).
var
  Nodes: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.EditNode(Node: TD2TreeNodeArray;
  Column: TD2ColumnIndex): Boolean;
//Начать редактирование узла Node в колонке Column.
//Возвращает True, если начато редактирование иначе - False.
// Application triggered edit event for the given node.
// Returns True if the tree started editing otherwise False.

begin
  Assert(Assigned(Node), 'Node must not be nil.');
  Assert((Column > InvalidColumn) and (Column < FHeader.Columns.Count),
    'Column must be a valid column index (-1 if no header is shown).');

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

function TD2BaseVirtualTree.EndEditNode: Boolean;
// Вызывается, чтобы закончить редактирование узла или остановить таймер ожидания редактирования.
// Called to finish a current edit action or stop the edit timer if an edit operation is pending.

begin
  if [tsEditing, tsEditPending] * FStates <> [] then
    Result := DoEndEdit
  else
    Result := True;
end;

procedure TD2BaseVirtualTree.EnsureNodeSelected;
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

procedure TD2BaseVirtualTree.FullCollapse(Node: TD2TreeNodeArray);
//Эта процедура сворачивает все развернутые узлы в поддереве узла Node или всего
//дерева если Node = FRoot или Nil. Свернуты будут только те узлы, которые были
//развернуты. Это исключает неинициализированные узлы, но узлы помеченые как видимые
//также будет сворачиваться, если они развернуты.
// This routine collapses all expanded nodes in the subtree given by Node or the whole tree
// if Node is FRoot or nil. Only nodes which are expanded will be collapsed. This excludes
// uninitialized nodes but nodes marked as visible will still be collapsed if they are expanded.
var Stop: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.FullExpand(Node: TD2TreeNodeArray = nil);
//Эта процедура разворачивает все свернутые узлы в поддереве узла Node или всего дерева,
//если Node = Froot или Nil. Все узлы на пути вниз инициализируются поэтому процедура
//может занять длительное время. Поскольку все узлы проверяются, дерево не может
//использовать оптимизацию, что непродуктивно. Поэтому старайтесь избегать её использования.
// This routine expands all collapsed nodes in the subtree given by Node or the whole tree if Node is FRoot or nil.
// All nodes on the way down are initialized so this procedure might take a long time.
// Since all nodes are validated, the tree cannot make use of optimatizations. Hence it is counter productive and you
// should consider avoiding its use.
var Stop: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.GetDisplayRect(Node: TD2TreeNodeArray;
  Column: TD2ColumnIndex; TextOnly: Boolean; Unclipped: Boolean;
  ApplyCellContentMargin: Boolean): TD2Rect;
// Определяет прямоугольник на экране, который занимает узел Node

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
begin

end;

function TD2BaseVirtualTree.GetEffectivelyFiltered(Node: TD2TreeNodeArray): Boolean;
//True -  если узел Node эффективно отфильтрован.
//Это зависит от статуса узла и опции отрисовки отфильтрованных узлов.
// Checks if a node is effectively filtered out. This depends on the nodes state and the paint options.
begin
  if Assigned(Node)
    then Result := (vsFiltered in Node.States) and not (toShowFilteredNodes in FOptions.FPaintOptions)
    else Result := False;
end;

function TD2BaseVirtualTree.GetEffectivelyVisible(Node: TD2TreeNodeArray): Boolean;
//True -  если узел Node эффективно видим.
//Это зависит от статуса узла и опции отрисовки отфильтрованных узлов.
begin
  if Assigned(Node)
    then Result := (vsVisible in Node.States) and not IsEffectivelyFiltered[Node]
    else Result := False;
end;

function TD2BaseVirtualTree.GetFirst(ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetFirstChecked(State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the first node in the tree with the given check state.

begin
  Result := GetNextChecked(nil, State, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetFirstChild(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetFirstChildNoInit(Node: TD2TreeNodeArray): TD2TreeNodeArray;
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

function TD2BaseVirtualTree.GetFirstCutCopy(ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the first node in the tree which is currently marked for a clipboard operation.
// See also GetNextCutCopy for comments on initialization.

begin
  Result := GetNextCutCopy(nil, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetFirstInitialized(ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the first node which is already initialized.

begin
  Result := GetFirstNoInit(ConsiderChildrenAbove);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    Result := GetNextInitialized(Result, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetFirstLeaf: TD2TreeNodeArray;

// Returns the first node in the tree which has currently no children.
// The result is initialized if necessary.

begin
  Result := GetNextLeaf(nil);
end;

function TD2BaseVirtualTree.GetFirstLevel(NodeLevel: Cardinal): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetFirstNoInit(ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetFirstSelected(ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the first node in the current selection while optionally considering toChildrenAbove.

begin
  Result := GetNextSelected(nil, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetFirstVisible(Node: TD2TreeNodeArray = nil; ConsiderChildrenAbove: Boolean = True;
  IncludeFiltered: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetFirstVisibleChild(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the first visible child node of Node. If necessary nodes are initialized on demand.

begin
  if Node = nil then
    Node := FRoot;
  Result := GetFirstChild(Node);

  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetNextVisibleSibling(Result, IncludeFiltered);
end;

function TD2BaseVirtualTree.GetFirstVisibleChildNoInit(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the first visible child node of Node.

begin
  if Node = nil then
    Node := FRoot;
  Result := Node.FirstChild;
  if Assigned(Result) and (not (vsVisible in Result.States) or
     (not IncludeFiltered and IsEffectivelyFiltered[Node])) then
    Result := GetNextVisibleSiblingNoInit(Result, IncludeFiltered);
end;

function TD2BaseVirtualTree.GetFirstVisibleNoInit(Node: TD2TreeNodeArray = nil;
  ConsiderChildrenAbove: Boolean = True; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the first visible node in the tree or given subtree while optionally considering toChildrenAbove.
// No initialization is performed.

begin
  Result := Node;
  if not Assigned(Result) then
    Result := FRoot;

  if vsHasChildren in Result.States then
  begin
    // Child nodes are the first choice if possible.
    if Assigned(Result.FirstChild) then
    begin
      Result := Result.FirstChild;

      if ConsiderChildrenAbove and (toChildrenAbove in FOptions.FPaintOptions) then
      begin
        repeat
          // Search the first visible sibling.
          while Assigned(Result.NextSibling) and not (vsVisible in Result.States) do
            Result := Result.NextSibling;

          // If there a no visible siblings take the parent.
          if not (vsVisible in Result.States) then
          begin
            Result := Result.Parent;
            if Result = FRoot then
              Result := nil;
            Break;
          end
          else
            if (not Assigned(Result.FirstChild)) or (not (vsExpanded in Result.States))then
              Break;

          Result := Result.FirstChild;
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
    Result := GetNextVisibleNoInit(Result);
end;


function TD2BaseVirtualTree.GetLast(Node: TD2TreeNodeArray = nil; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the very last node in the tree branch given by Node and initializes the nodes all the way down including the
// result. toChildrenAbove is optionally considered. By using Node = nil the very last node in the tree is returned.

var
  Next: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastInitialized(Node: TD2TreeNodeArray = nil;
  ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the very last initialized child node in the tree branch given by Node.

begin
  Result := GetLastNoInit(Node, ConsiderChildrenAbove);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    Result := GetPreviousInitialized(Result, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetLastNoInit(Node: TD2TreeNodeArray = nil; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the very last node in the tree branch given by Node without initialization.

var
  Next: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastChild(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastChildNoInit(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastVisible(Node: TD2TreeNodeArray = nil; ConsiderChildrenAbove: Boolean = True;
  IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the very last visible node in the tree while optionally considering toChildrenAbove.
// The nodes are intialized all the way up including the result node.
var Run: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.GetLastVisibleChild(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastVisibleChildNoInit(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetLastVisibleNoInit(Node: TD2TreeNodeArray = nil;
  ConsiderChildrenAbove: Boolean = True; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNext(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextChecked(Node: TD2TreeNodeArray; State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

begin
  if (Node = nil) or (Node = FRoot) then
    Result := GetFirstNoInit(ConsiderChildrenAbove)
  else
    Result := GetNextNoInit(Node, ConsiderChildrenAbove);

  while Assigned(Result) and (Result.CheckState <> State) do
    Result := GetNextNoInit(Result, ConsiderChildrenAbove);

  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2BaseVirtualTree.GetNextChecked(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean): TD2TreeNodeArray;
begin
  Result := Self.GetNextChecked(Node, csCheckedNormal, ConsiderChildrenAbove);
end;

function TD2BaseVirtualTree.GetNextCutCopy(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the next node in the tree which is currently marked for a clipboard operation. Since only visible nodes can
// be marked (or they are hidden after they have been marked) it is not necessary to initialize nodes to check for
// child nodes. The result, however, is initialized if necessary.

begin
  if ClipboardStates * FStates <> [] then
  begin
    if (Node = nil) or (Node = FRoot) then
      Result := GetFirstNoInit(ConsiderChildrenAbove)
    else
      Result := GetNextNoInit(Node, ConsiderChildrenAbove);
    while Assigned(Result) and not (vsCutOrCopy in Result.States) do
      Result := GetNextNoInit(Result, ConsiderChildrenAbove);
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end
  else
    Result := nil;
end;

function TD2BaseVirtualTree.GetNextInitialized(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the next node in tree which is initialized.

begin
  Result := Node;
  repeat
    Result := GetNextNoInit(Result, ConsiderChildrenAbove);
  until (Result = nil) or (vsInitialized in Result.States);
end;

function TD2BaseVirtualTree.GetNextLeaf(Node: TD2TreeNodeArray): TD2TreeNodeArray;

// Returns the next node in the tree which has currently no children.
// The result is initialized if necessary.

begin
  if (Node = nil) or (Node = FRoot) then
    Result := FRoot.FirstChild
  else
    Result := GetNext(Node);
  while Assigned(Result) and (vsHasChildren in Result.States) do
    Result := GetNext(Result);
  if Assigned(Result) and not (vsInitialized in Result.States) then
    InitNode(Result);
end;

function TD2BaseVirtualTree.GetNextLevel(Node: TD2TreeNodeArray; NodeLevel: Cardinal): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextNoInit(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextSelected(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the next node in the tree which is currently selected. Since children of unitialized nodes cannot be
// in the current selection (because they simply do not exist yet) it is not necessary to initialize nodes here.
// The result however is initialized if necessary.

begin
  if FSelectionCount > 0 then
  begin
    if (Node = nil) or (Node = FRoot) then
      Result := GetFirstNoInit(ConsiderChildrenAbove)
    else
      Result := GetNextNoInit(Node, ConsiderChildrenAbove);
    while Assigned(Result) and not (vsSelected in Result.States) do
      Result := GetNextNoInit(Result, ConsiderChildrenAbove);
    if Assigned(Result) and not (vsInitialized in Result.States) then
      InitNode(Result);
  end
  else
    Result := nil;
end;

function TD2BaseVirtualTree.GetNextSibling(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextSiblingNoInit(Node: TD2TreeNodeArray): TD2TreeNodeArray;

// Returns the next sibling of Node.

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.NextSibling;
  end;
end;

function TD2BaseVirtualTree.GetNextVisible(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = True): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextVisibleNoInit(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = True): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetNextVisibleSibling(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the next visible sibling after Node. Initialization is done implicitly.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := GetNextSibling(Result);
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2BaseVirtualTree.GetNextVisibleSiblingNoInit(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the next visible sibling after Node.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := Result.NextSibling;
  until not Assigned(Result) or ((vsVisible in Result.States) and
       (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2BaseVirtualTree.GetNodeData(Node: TD2TreeNodeArray): Pointer;

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

function TD2BaseVirtualTree.GetNodeLevel(Node: TD2TreeNodeArray): Cardinal;

// returns the level of the given node

var
  Run: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPrevious(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns previous node in tree. If ConsiderChildrenAbove is True the function considers
// whether toChildrenAbove is currently set, otherwise the result will always be the previous
// node in top-down order regardless of the current PaintOptions.
// The Result will be initialized if needed.

var
  Run: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousChecked(Node: TD2TreeNodeArray; State: TD2CheckState = csCheckedNormal;
  ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousCutCopy(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousInitialized(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns the previous node in tree which is initialized.

begin
  Result := Node;
  repeat
    Result := GetPreviousNoInit(Result, ConsiderChildrenAbove);
  until (Result = nil) or (vsInitialized in Result.States);
end;

function TD2BaseVirtualTree.GetPreviousLeaf(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousLevel(Node: TD2TreeNodeArray; NodeLevel: Cardinal): TD2TreeNodeArray;

// Returns the previous node in the tree on a specific level.
// The result is initialized if necessary.

var
  StartNodeLevel: Cardinal;
  Run: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousNoInit(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

// Returns previous node in tree, optionally considering toChildrenAbove. No initialization is performed.

var
  Run: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousSelected(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = False): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousSibling(Node: TD2TreeNodeArray): TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousSiblingNoInit(Node: TD2TreeNodeArray): TD2TreeNodeArray;

// Returns the previous sibling of Node

begin
  Result := Node;
  if Assigned(Result) then
  begin
    Assert(Result <> FRoot, 'Node must not be the hidden root node.');

    Result := Result.PrevSibling;
  end;
end;

function TD2BaseVirtualTree.GetPreviousVisible(Node: TD2TreeNodeArray; ConsiderChildrenAbove: Boolean = True): TD2TreeNodeArray;

// Returns the previous node in tree, with regard to Node, which is visible.
// Nodes which need an initialization (including the result) are initialized.
// toChildrenAbove is optionally considered which is the default here.

var
  Marker: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousVisibleNoInit(Node: TD2TreeNodeArray;
  ConsiderChildrenAbove: Boolean = True): TD2TreeNodeArray;

// Returns the previous node in tree, with regard to Node, which is visible.
// toChildrenAbove is optionally considered which is the default here.

var
  Marker: TD2TreeNodeArray;

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

function TD2BaseVirtualTree.GetPreviousVisibleSibling(Node: TD2TreeNodeArray; IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the previous visible sibling before Node. Initialization is done implicitly.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := GetPreviousSibling(Result);
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2BaseVirtualTree.GetPreviousVisibleSiblingNoInit(Node: TD2TreeNodeArray;
  IncludeFiltered: Boolean = False): TD2TreeNodeArray;

// Returns the previous visible sibling before Node.

begin
  Assert(Assigned(Node) and (Node <> FRoot), 'Invalid parameter.');

  Result := Node;
  repeat
    Result := Result.PrevSibling;
  until not Assigned(Result) or ((vsVisible in Result.States) and
        (IncludeFiltered or not IsEffectivelyFiltered[Result]));
end;

function TD2BaseVirtualTree.GetSortedCutCopySet(Resolve: Boolean): TD2TreeNodeArray;

// Same as GetSortedSelection but with nodes marked as being part in the current cut/copy set (e.g. for clipboard).

var
  Run: TD2TreeNodeArray;
  Counter: Cardinal;

  //--------------- local function --------------------------------------------

  procedure IncludeThisNode(Node: TD2TreeNodeArray);

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

function TD2BaseVirtualTree.GetSortedSelection(Resolve: Boolean): TD2TreeNodeArray;

// Returns a list of selected nodes sorted in logical order, that is, as they appear in the tree.
// If Resolve is True then nodes which are children of other selected nodes are not put into the new array.
// This feature is in particuar important when doing drag'n drop as in this case all selected node plus their children
// need to be considered. A selected node which is child (grand child etc.) of another selected node is then
// automatically included and doesn't need to be explicitely mentioned in the returned selection array.
//
// Note: The caller is responsible for freeing the array. Allocation is done here. Usually, though, freeing the array
//       doesn't need additional attention as it is automatically freed by Delphi when it gets out of scope.

var
  Run: TD2TreeNodeArray;
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

function TD2BaseVirtualTree.GetVisibleParent(Node: TD2TreeNodeArray;
  IncludeFiltered: Boolean): TD2TreeNodeArray;
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

function TD2BaseVirtualTree.HasAsParent(Node, PotentialParent: TD2TreeNodeArray): Boolean;
//True - если PotentialParent является родителем любого уровня для узла Node
// Determines whether Node has got PotentialParent as one of its parents.

var Run: TD2TreeNodeArray;
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








function TD2BaseVirtualTree.InsertNode(Node: TD2TreeNodeArray;
  Mode: TD2VTNodeAttachMode; UserData: Pointer): TD2TreeNodeArray;
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
      // If auto sort is enabled then sort the node or its parent (depending on the insert mode).
      if (toAutoSort in FOptions.FAutoOptions) and (FHeader.FSortColumn > InvalidColumn) then
        case Mode of
          amInsertBefore,
          amInsertAfter:
            // Here no initialization is necessary because *if* a node has already got children then it
            // must also be initialized.
            // Note: Node can never be FRoot at this point.
            Sort(Node.Parent, FHeader.FSortColumn, FHeader.FSortDirection, True);
          amAddChildFirst,
          amAddChildLast:
            Sort(Node, FHeader.FSortColumn, FHeader.FSortDirection, True);
        end;

      UpdateScrollBars(True);
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

procedure TD2BaseVirtualTree.InvalidateChildren(Node: TD2TreeNodeArray;
  Recursive: Boolean);
//Инициирует перерисовку узла и его непосредственных потомков.
//Если Recursive = True, то все внуки также перерисовываются.
//Сам узел инициализируется при необходимости и создаются его дочерние узлы
//(и инициализируется тоже, если Recursive is True).

// Invalidates Node and its immediate children.
// If Recursive is True then all grandchildren are invalidated as well.
// The node itself is initialized if necessary and its child nodes are created
// (and initialized too if Recursive is True).
var Run: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.InvalidateColumn(Column: TD2ColumnIndex);
//Пометить недействительной видимую область колоннки Column.
// Invalidates the client area part of a column.
//var R: TD2Rect;
begin
  //if (FUpdateCount = 0) and HandleAllocated and FHeader.FColumns.IsValidColumn(Column) then
  //begin
  //  R := ClientRect;
  //  //lclheader
  //  if hoVisible in FHeader.FOptions then
  //    OffsetRect(R, 0, FHeader.Height);
  //  FHeader.Columns.GetColumnBounds(Column, R.Left, R.Right);
  //  //InvalidateRect(Handle, @R, False);
  //  InvalidateRect(R);
  //end;
end;

function TD2BaseVirtualTree.InvalidateNode(Node: TD2TreeNodeArray): TRect;
//Инициирует перерисовку данного узла. Возвращает аннулированый прямоугольник.
// Initiates repaint of the given node and returns the just invalidated rectangle.

begin
  //if (FUpdateCount = 0) and HandleAllocated then
  //begin
  //  Result := GetDisplayRect(Node, NoColumn, False);
  //  InvalidateRect(Handle, @Result, False);
  //end;
end;

procedure TD2BaseVirtualTree.InvalidateToBottom(Node: TD2TreeNodeArray);
//Инициирует перерисовку клиентской области, начиная с узла Node. Если этот
//узел не виден или не инициализирован, то ничего не происходит.
// Initiates repaint of client area starting at given node. If this node is not visible or not yet initialized
// then nothing happens.

//var
  //R: TD2Rect;

begin
//  if (FUpdateCount = 0) and HandleAllocated then
//  begin
//    if (Node = nil) or (Node = FRoot) then
//      Invalidate
//    else
//      if (vsInitialized in Node.States) and IsEffectivelyVisible[Node] then
//      begin
//        R := GetDisplayRect(Node, -1, False);
//        if R.Top < ClientHeight then
//        begin
//          if (toChildrenAbove in FOptions.FPaintOptions) and (vsExpanded in Node.States) then
//            Dec(R.Top, Node.TotalHeight + NodeHeight[Node]);
//          R.Bottom := ClientHeight;
//          //lclheader
//          if hoVisible in FHeader.FOptions then
//            Inc(R.Bottom, FHeader.Height);
//          InvalidateRect(Handle, @R, False);
//        end;
//      end;
// end;
end;

procedure TD2BaseVirtualTree.InvertSelection(VisibleOnly: Boolean);
//Инвертировать текущий выбор (узлы, которые выбраны становятся невыделенными и наоборот).
//Если VisibleOnly = True, то рассматриваются только видимые узлы.
// Inverts the current selection (so nodes which are selected become unselected and vice versa).
// If VisibleOnly is True then only visible nodes are considered.
var
  Run: TD2TreeNodeArray;
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
    NewSize := PackArray(FSelection, FSelectionCount);
    if NewSize > -1 then
    begin
      FSelectionCount := NewSize;
      SetLength(FSelection, FSelectionCount);
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

function TD2BaseVirtualTree.IsEditing: Boolean;
begin
  Result := tsEditing in FStates;
end;

function TD2BaseVirtualTree.IsMouseSelecting: Boolean;
begin
  Result := (tsDrawSelPending in FStates) or (tsDrawSelecting in FStates);
end;

function TD2BaseVirtualTree.IsEmpty: Boolean;
begin
  Result := (Self.ChildCount[nil] = 0);
end;

function TD2BaseVirtualTree.IterateSubtree(Node: TD2TreeNodeArray;
  Callback: TD2VTGetNodeProc; Data: Pointer; Filter: TD2TreeNodeStates;
  DoInit: Boolean; ChildNodesOnly: Boolean): TD2TreeNodeArray;
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
  Stop: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.LoadFromFile(const FileName: TFileName);
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

procedure TD2BaseVirtualTree.LoadFromStream(Stream: TStream);
// Очищает текущее содержимое дерева и загружает новую структуру из потока Stream.
// Clears the current content of the tree and loads a new structure from the given stream.
var
  ThisID: TD2MagicID;
  Version,
  Count: Cardinal;
  Node: TD2TreeNodeArray;
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
      if Version <= VTTreeStreamVersion then
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

procedure TD2BaseVirtualTree.MeasureItemHeight(const Canvas: TD2Canvas;
  Node: TD2TreeNodeArray);
//Опредяет высоту узла Node если это до сих пор не сделайте.
// If the height of the given node has not yet been measured then do it now.
var NewNodeHeight: Single;
begin
  if not (vsHeightMeasured in Node.States)
     //{$if CompilerVersion < 20}
     //and (MainThreadId = GetCurrentThreadId)
     //{$ifend}
  then
  begin
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
        DoMeasureItem(Canvas, Node, NewNodeHeight);
        SetNodeHeight(Node, NewNodeHeight);
      end;
    end;
  end;
end;

procedure TD2BaseVirtualTree.MoveTo(Node: TD2TreeNodeArray; Tree: TD2BaseVirtualTree;
  Mode: TD2VTNodeAttachMode; ChildrenOnly: Boolean);
//Упрощенный метод перемещения узла к корню другого дерева.
// A simplified method to allow to move nodes to the root of another tree.
begin
  MoveTo(Node, Tree.FRoot, Mode, ChildrenOnly);
end;

//----------------------------------------------------------------------------------------------------------------------

procedure TD2BaseVirtualTree.MoveTo(Source, Target: TD2TreeNodeArray;
   Mode: TD2VTNodeAttachMode; ChildrenOnly: Boolean);
//Перемещение узла Source и всех его детей к узлу Target. Source должен принадлежать
//экземпляру дерева, которое вызывает этот метод. Mode определяет, как подключить Source к Target.
//Этот метод может повлечь изменение дерева, если Source и Target принадлежат разрым деревьям.
// Moves the given node (and all its children) to Target. Source must belong to the tree instance which calls this
// MoveTo method. Mode determines how to connect Source to Target.
// This method might involve a change of the tree if Target belongs to a different tree than Source.

var
  TargetTree: TD2BaseVirtualTree;
  Allowed: Boolean;
  NewNode: TD2TreeNodeArray;
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
                    TargetTree.InternalAddFromStream(Stream, VTTreeStreamVersion, NewNode);
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
      UpdateScrollBars(True);
      Realign; // вместо Invalidate;
      if TargetTree <> Self
        then TargetTree.Realign; // вместо Invalidate;
    end;
    StructureChange(Source, crNodeMoved);
  end;
end;




procedure TD2BaseVirtualTree.ReinitChildren(Node: TD2TreeNodeArray; Recursive: Boolean);
//Принудительная переинициализация всех детей узла Node, при Recursive=true в том числе и внуки.
// Forces all child nodes of Node to be reinitialized.
// If Recursive is True then also the grandchildren are reinitialized.

var
  Run: TD2TreeNodeArray;

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

procedure TD2BaseVirtualTree.ReinitNode(Node: TD2TreeNodeArray; Recursive: Boolean);
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

procedure TD2BaseVirtualTree.ResetNode(Node: TD2TreeNodeArray);
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

procedure TD2BaseVirtualTree.SaveToFile(const FileName: TFileName);
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

procedure TD2BaseVirtualTree.SaveToStream(Stream: TStream; Node: TD2TreeNodeArray = nil);
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

function TD2BaseVirtualTree.ScrollIntoView(Column: TD2ColumnIndex;
  Center: Boolean): Boolean;
//Прокручивает дерево чтобы столбец Column был видимым. Возвращает True, если
//столбцы были прокручены (например чтобы избежать дальнейших обновлений)
//иначе возвращает False.
// Scrolls the columns so that the given column is in the client area and returns True if the columns really have been
// scrolled (e.g. to avoid further updates) else returns False.
//var
  //ColumnLeft,
  //ColumnRight: Integer;
  //NewOffset: Integer;

begin
  //Result := False;
  //
  //if not FHeader.UseColumns then
  //  Exit;
  //if not FHeader.Columns.IsValidColumn(Column) then
  //  Exit; // Just in case.
  //
  //ColumnLeft := Header.Columns.Items[Column].Left;
  //ColumnRight := ColumnLeft + Header.Columns.Items[Column].Width;
  //
  //NewOffset := FEffectiveOffsetX;
  //if Center then
  //begin
  //  NewOffset := FEffectiveOffsetX + ColumnLeft - (Header.Columns.GetVisibleFixedWidth div 2) - (ClientWidth div 2) + ((ColumnRight - ColumnLeft) div 2);
  //  if NewOffset <> FEffectiveOffsetX then
  //  begin
  //    if UseRightToLeftAlignment then
  //      SetOffsetX(-Integer(FRangeX) + ClientWidth + NewOffset)
  //    else
  //      SetOffsetX(-NewOffset);
  //  end;
  //  Result := True;
  //end
  //else if not (coFixed in Header.Columns[Column].Options) then
  //begin
  //  if ColumnRight > ClientWidth then
  //    NewOffset := FEffectiveOffsetX + (ColumnRight - ClientWidth)
  //  else if (ColumnLeft < Header.Columns.GetVisibleFixedWidth) then
  //    NewOffset := FEffectiveOffsetX - (Header.Columns.GetVisibleFixedWidth - ColumnLeft);
  //  if NewOffset <> FEffectiveOffsetX then
  //  begin
  //    if UseRightToLeftAlignment then
  //      SetOffsetX(-Integer(FRangeX) + ClientWidth + NewOffset)
  //    else
  //      SetOffsetX(-NewOffset);
  //  end;
  //  Result := True;
  //end
  //else
  //  Result := True;
end;

function TD2BaseVirtualTree.ScrollIntoView(Node: TD2TreeNodeArray;
  Center: Boolean; Horizontally: Boolean): Boolean;
//Прокручивает дерево чтобы узел Node был видимым. Возвращает True, если дерево было прокручено
//(например чтобы избежать дальнейших обновлений) иначе возвращает False. Если расширенная
//фокусировка включена, то дерево будет также прокручивается по горизонтали, если это необходимо.
//Примечание: Все свернутые родители узла разворачиваются.

// Scrolls the tree so that the given node is in the client area and returns True if
// the tree really has been scrolled (e.g. to avoid further updates) else returns False.
// If extened focus is enabled then the tree will also be horizontally scrolled if needed.
// Note: All collapsed parents of the node are expanded.

//var
  //R: TRect;
  //Run: TD2TreeNodeArray;
  //UseColumns,
  //HScrollBarVisible: Boolean;
  //ScrolledVertically,
  //ScrolledHorizontally: Boolean;

begin
  ////todo: minimize calls to ClientHeight and ClientWidth
  //ScrolledVertically := False;
  //ScrolledHorizontally := False;
  //
  //if Assigned(Node) and (Node <> FRoot) then
  //begin
  //  // Make sure all parents of the node are expanded.
  //  Run := Node.Parent;
  //  while Run <> FRoot do
  //  begin
  //    if not (vsExpanded in Run.States) then
  //      ToggleNode(Run);
  //    Run := Run.Parent;
  //  end;
  //  UseColumns := FHeader.UseColumns;
  //  if UseColumns and FHeader.FColumns.IsValidColumn(FFocusedColumn) then
  //    R := GetDisplayRect(Node, FFocusedColumn, not (toGridExtensions in FOptions.FMiscOptions))
  //  else
  //    R := GetDisplayRect(Node, NoColumn, not (toGridExtensions in FOptions.FMiscOptions));
  //
  //  // The returned rectangle can never be empty after the expand code above.
  //  // 1) scroll vertically
  //  //lclheader
  //  if hoVisible in FHeader.FOptions then
  //    OffsetRect(R, 0, -FHeader.Height);
  //
  //  if R.Top < 0 then
  //  begin
  //    if Center then
  //      SetOffsetY(FOffsetY - R.Top + ClientHeight div 2)
  //    else
  //      SetOffsetY(FOffsetY - R.Top);
  //    ScrolledVertically := True;
  //  end
  //  else
  //    if (R.Bottom > ClientHeight) or Center then
  //    begin
  //      HScrollBarVisible := (ScrollBarOptions.ScrollBars in [ssBoth, ssHorizontal]) and
  //        (ScrollBarOptions.AlwaysVisible or (Integer(FRangeX) > ClientWidth));
  //      if Center then
  //        SetOffsetY(FOffsetY - R.Bottom + ClientHeight div 2)
  //      else
  //        SetOffsetY(FOffsetY - R.Bottom + ClientHeight);
  //      // When scrolling up and the horizontal scroll appears because of the operation
  //      // then we have to move up the node the horizontal scrollbar's height too
  //      // in order to avoid that the scroll bar hides the node which we wanted to have in view.
  //      if not UseColumns and not HScrollBarVisible and (Integer(FRangeX) > ClientWidth) then
  //        SetOffsetY(FOffsetY - GetSystemMetrics(SM_CYHSCROLL));
  //      ScrolledVertically := True;
  //    end;
  //
  //  if Horizontally then
  //    // 2) scroll horizontally
  //    ScrolledHorizontally := ScrollIntoView(FFocusedColumn, Center);
  //
  //end;
  //
  //Result := ScrolledVertically or ScrolledHorizontally;
end;

procedure TD2BaseVirtualTree.ShowError(const Msg: String; HelpContext: Integer);
//Показать сообщение ошибки
begin
  raise ED2VirtualTreeError.CreateHelp(Msg, HelpContext);
end;

procedure TD2BaseVirtualTree.SelectAll(VisibleOnly: Boolean);
//Выбрать все узлы в дереве. При VisibleOnly = true - выбираются только видимые узлы.
// Select all nodes in the tree.
// If VisibleOnly is True then only visible nodes are selected.
var
  Run: TD2TreeNodeArray;
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
    Realign;//вместо Invalidate;
  end;
end;

procedure TD2BaseVirtualTree.Sort(Node: TD2TreeNodeArray; Column: TD2ColumnIndex;
  Direction: TD2SortDirection; DoInit: Boolean);
//Сортировка узла Node по колонке Column в направлении Direction.
// Sorts the given node. The application is queried about how to sort via the OnCompareNodes event.
// Column is simply passed to the the compare function so the application can also sort in a particular column.
// In order to free the application from taking care about the sort direction the parameter Direction is used.
// This way the application can always sort in increasing order, while this method reorders nodes according to this flag.

  //--------------- local functions -------------------------------------------

  function MergeAscending(A, B: TD2TreeNodeArray): TD2TreeNodeArray;

  // Merges A and B (which both must be sorted via Compare) into one list.

  var
    Dummy: TD2VirtualNode;
    CompareResult: Integer;
  begin
    // This avoids checking for Result = nil in the loops.
    Result := @Dummy;
    while Assigned(A) and Assigned(B) do
    begin
      if OperationCanceled then
        CompareResult := 0
      else
        CompareResult := DoCompare(A, B, Column);

      if CompareResult <= 0 then
      begin
        Result.NextSibling := A;
        Result := A;
        A := A.NextSibling;
      end
      else
      begin
        Result.NextSibling := B;
        Result := B;
        B := B.NextSibling;
      end;
    end;

    // Just append the list which is not nil (or set end of result list to nil if both lists are nil).
    if Assigned(A) then
      Result.NextSibling := A
    else
      Result.NextSibling := B;
    // return start of the new merged list
    Result := Dummy.NextSibling;
  end;

  //---------------------------------------------------------------------------

  function MergeDescending(A, B: TD2TreeNodeArray): TD2TreeNodeArray;

  // Merges A and B (which both must be sorted via Compare) into one list.

  var
    Dummy: TD2VirtualNode;
    CompareResult: Integer;

  begin
    // this avoids checking for Result = nil in the loops
    Result := @Dummy;
    while Assigned(A) and Assigned(B) do
    begin
      if OperationCanceled then
        CompareResult := 0
      else
        CompareResult := DoCompare(A, B, Column);

      if CompareResult >= 0 then
      begin
        Result.NextSibling := A;
        Result := A;
        A := A.NextSibling;
      end
      else
      begin
        Result.NextSibling := B;
        Result := B;
        B := B.NextSibling;
      end;
    end;

    // Just append the list which is not nil (or set end of result list to nil if both lists are nil).
    if Assigned(A) then
      Result.NextSibling := A
    else
      Result.NextSibling := B;
    // Return start of the newly merged list.
    Result := Dummy.NextSibling;
  end;

  //---------------------------------------------------------------------------

  function MergeSortAscending(var Node: TD2TreeNodeArray; N: Cardinal): TD2TreeNodeArray;

  // Sorts the list of nodes given by Node (which must not be nil).

  var
    A, B: TD2TreeNodeArray;

  begin
    if N > 1 then
    begin
      A := MergeSortAscending(Node, N div 2);
      B := MergeSortAscending(Node, (N + 1) div 2);
      Result := MergeAscending(A, B);
    end
    else
    begin
      Result := Node;
      Node := Node.NextSibling;
      Result.NextSibling := nil;
    end;
  end;

  //---------------------------------------------------------------------------

  function MergeSortDescending(var Node: TD2TreeNodeArray; N: Cardinal): TD2TreeNodeArray;

  // Sorts the list of nodes given by Node (which must not be nil).

  var
    A, B: TD2TreeNodeArray;

  begin
    if N > 1 then
    begin
      A := MergeSortDescending(Node, N div 2);
      B := MergeSortDescending(Node, (N + 1) div 2);
      Result := MergeDescending(A, B);
    end
    else
    begin
      Result := Node;
      Node := Node.NextSibling;
      Result.NextSibling := nil;
    end;
  end;

  //--------------- end local functions ---------------------------------------

var
  Run: TD2TreeNodeArray;
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
      // Make sure the children are valid, so they can be sorted at all.
      if DoInit and (Node.ChildCount > 0) then
        ValidateChildren(Node, False);
      // Child count might have changed.
      if Node.ChildCount > 1 then
      begin
        StartOperation(okSortNode);
        try
          // Sort the linked list, check direction flag only once.
          if Direction = sdAscending then
            Node.FirstChild := MergeSortAscending(Node.FirstChild, Node.ChildCount)
          else
            Node.FirstChild := MergeSortDescending(Node.FirstChild, Node.ChildCount);
        finally
          EndOperation(okSortNode);
        end;
        // Consolidate the child list finally.
        Run := Node.FirstChild;
        Run.PrevSibling := nil;
        Index := 0;
        repeat
          Run.Index := Index;
          Inc(Index);
          if Run.NextSibling = nil then
            Break;
          Run.NextSibling.PrevSibling := Run;
          Run := Run.NextSibling;
        until False;
        Node.LastChild := Run;

        InvalidateCache;
      end;
      if FUpdateCount = 0 then
      begin
        ValidateCache;
        Realign; //вместо Invalidate;
      end;
    end;
  end;
end;

procedure TD2BaseVirtualTree.SortTree(Column: TD2ColumnIndex;
  Direction: TD2SortDirection; DoInit: Boolean);
//--------------- local function --------------------------------------------

procedure DoSort(Node: TD2TreeNodeArray);
// Рекурсивно сортирует Node и его дочерние узлы.
// Recursively sorts Node and its child nodes.

var Run: TD2TreeNodeArray;

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

procedure TD2BaseVirtualTree.QuickSort(const TheArray: TD2TreeNodeArray; L, R: Integer);
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

procedure TD2BaseVirtualTree.UpdateVerticalRange;
//Обновить общую витруальную высоту дерева
begin
  //Общая высота узла включает в себя высоту невидимого корневого узла.
  // Total node height includes the height of the invisible root node.
  if FRoot.TotalHeight < FDefaultNodeHeight then
    FRoot.TotalHeight := FDefaultNodeHeight;
  FRangeY := FRoot.TotalHeight - FRoot.NodeHeight + FBottomSpace;
end;

procedure TD2BaseVirtualTree.UpdateVerticalScrollBar(DoRepaint: Boolean);
//Обновляет вертикальную полосу прокрутки, чтобы отразить текущий размер и смещение дерева
begin

end;

procedure TD2BaseVirtualTree.ValidateChildren(Node: TD2TreeNodeArray;
  Recursive: Boolean);
//Обеспечивает инициализацию всех детей (и всех их детей, если Recursive = True) узла Node.
//Узел должен быть уже инициализирован.
// Ensures that the children of the given node (and all their children, if Recursive is True) are initialized.
// Node must already be initialized
var Child: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.ValidateNode(Node: TD2TreeNodeArray; Recursive: Boolean);
//Обеспечивает инициализацию узла Node (и всех его детей и их детей, если Recursive = True)
// Ensures that the given node (and all its children, if Recursive is True) are initialized.
var Child: TD2TreeNodeArray;
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

procedure TD2BaseVirtualTree.ToggleNode(Node: TD2TreeNodeArray);
//Изменение развернутого/свернутого состояния узла на противоположное.
// Changes a node's expand state to the opposite state.

var
  Child,
  FirstVisible: TD2TreeNodeArray;
  HeightDelta,
  StepsR1: Single;
  StepsR2: Single;
  Steps: Single;
  TogglingTree,
  ChildrenInView,
  NeedFullInvalidate,
  NeedUpdate,
  NodeInView,
  PosHoldable,
  TotalFit: Boolean;
  ToggleData: TD2ToggleAnimationData;

  //--------------- local function --------------------------------------------

  procedure PrepareAnimation;

  // Prepares ToggleData.

  var
    R: TD2Rect;
    S: Single;
    M: TD2ToggleAnimationMode;

  begin
    with ToggleData do
    begin
      //Window := Handle;       //закоментировано GoldenFox
      //DC := GetDC(Handle);    //закоментировано GoldenFox

      //lcl: setting Color to Brush seems not necessary
      //Self.Brush.Color := FColors.BackGroundColor;;

      //Brush := Self.Brush.Reference.Handle; //закоментировано GoldenFox

      if (Mode1 <> tamNoScroll) and (Mode2 <> tamNoScroll) then
      begin
        if StepsR1 < StepsR2 then
        begin
          // As the primary rectangle is always R1 we will get a much smoother
          // animation if R1 is the one that will be scrolled more.
          R := R2;
          R2 := R1;
          R1 := R;

          M := Mode2;
          Mode2 := Mode1;
          Mode1 := M;

          S := StepsR2;
          StepsR2 := StepsR1;
          StepsR1 := S;
        end;
        ScaleFactor := StepsR2 / StepsR1;
        MissedSteps := 0;
      end;

      if Mode1 <> tamNoScroll then
        Steps := StepsR1
      else
        Steps := StepsR2;
    end;
  end;

  //--------------- end local function ----------------------------------------

begin
  Assert(Assigned(Node), 'Node must not be nil.');

  TogglingTree := tsToggling in FStates;
  ChildrenInView := False;
  HeightDelta := 0;
  NeedFullInvalidate := False;
  NeedUpdate := False;
  NodeInView := False;
  PosHoldable := False;
  TotalFit := False;

  // We don't need to switch the expand state if the node is being deleted otherwise some
  // updates (e.g. visible node count) are done twice with disasterous results).
  if [vsDeleting, vsToggling] * Node.States = [] then
  begin
    try
      DoStateChange([tsToggling]);
      Include(Node.States, vsToggling);

      if vsExpanded in Node.States then
      begin
        if DoCollapsing(Node) then
        begin
          NeedUpdate := True;

          // Calculate the height delta right now as we need it for toChildrenAbove anyway.
          HeightDelta := -Integer(Node.TotalHeight) + Integer(NodeHeight[Node]);
          if (FUpdateCount = 0) and (toAnimatedToggle in FOptions.FAnimationOptions) and not
             (tsCollapsing in FStates) then
          begin
            if tsHint in Self.FStates then
              Application.CancelHint;
            Realign; //вместо UpdateWindow(Handle);

            // animated collapsing
            with ToggleData do
            begin
              // Determine the animation behaviour and rectangle. If toChildrenAbove is set, the behaviour is depending
              // on the position of the node to be collapsed.
              R1 := GetDisplayRect(Node, NoColumn, False);
              Mode2 := tamNoScroll;
              if toChildrenAbove in FOptions.FPaintOptions then
              begin
                PosHoldable := (FOffsetY + (Integer(Node.TotalHeight) - Integer(NodeHeight[Node]))) <= 0;
                NodeInView := R1.Top < ClientHeight;

                StepsR1 := 0;
                if NodeInView then
                begin
                  if PosHoldable or not (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) then
                  begin
                    // Scroll the child nodes down.
                    Mode1 := tamScrollDown;
                    R1.Bottom := R1.Top;
                    R1.Top := 0;
                    StepsR1 := Min(R1.Bottom - R1.Top + 1, Integer(Node.TotalHeight) - Integer(NodeHeight[Node]));
                  end
                  else
                  begin
                    // The position cannot be kept. So scroll the node up to its future position.
                    Mode1 := tamScrollUp;
                    R1.Top := Max(0, R1.Top + HeightDelta);
                    R1.Bottom := ClientHeight;
                    StepsR1 := FOffsetY - HeightDelta;
                  end;
                end;
              end
              else
              begin
                if (Integer(FRangeY) + FOffsetY - R1.Bottom + HeightDelta >= ClientHeight - R1.Bottom) or
                   (Integer(FRangeY) <= ClientHeight) or (FOffsetY = 0) or not
                   (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) then
                begin
                  // Do a simple scroll up over the child nodes.
                  Mode1 := tamScrollUp;
                  //Inc(R1.Top, NodeHeight[Node]);
                  R1.Top := R1.Top + NodeHeight[Node];
                  R1.Bottom := ClientHeight;
                  StepsR1 := Min(R1.Bottom - R1.Top + 1, -HeightDelta);
                end
                else
                begin
                  // Scroll the node down to its future position. As FOffsetY will change we need to invalidate the
                  // whole tree.
                  Mode1 := tamScrollDown;
                  StepsR1 := Min(-FOffsetY, ClientHeight - Integer(FRangeY) -FOffsetY - HeightDelta);
                  R1.Top := 0;
                  R1.Bottom := Min(ClientHeight, R1.Bottom + Steps);
                  NeedFullInvalidate := True;
                end;
              end;

              // No animation necessary if the node is below the current client height.
              if R1.Top < ClientHeight then
              begin
                PrepareAnimation;
                try
                  Animate(Steps, FAnimationDuration, ToggleCallback, @ToggleData);
                finally
                  ReleaseDC(Window, DC);
                end;
              end;
            end;
          end;

          // collapse the node
          AdjustTotalHeight(Node, IfThen(IsEffectivelyFiltered[Node], 0, NodeHeight[Node]));
          if FullyVisible[Node] then
            Dec(FVisibleCount, CountVisibleChildren(Node));
          Exclude(Node.States, vsExpanded);
          DoCollapsed(Node);

          // Remove child nodes now, if enabled.
          if (toAutoFreeOnCollapse in FOptions.FAutoOptions) and (Node.ChildCount > 0) then
          begin
            DeleteChild(Node);
            Include(Node.States, vsHasChildren);
          end;
        end;
      end
      else
        if DoExpanding(Node) then
        begin
          NeedUpdate := True;
          // expand the node, need to adjust the height
          if not (vsInitialized in Node.States) then
            InitNode(Node);
          if (vsHasChildren in Node.States) and (Node.ChildCount = 0) then
            InitChildren(Node);

          // Avoid setting the vsExpanded style if there are no child nodes.
          if Node.ChildCount > 0 then
          begin
            // Iterate through the child nodes without initializing them. We have to determine the entire height.
            Child := Node.FirstChild;
            repeat
              if vsVisible in Child.States then
              begin
                // Ensure the item height is measured
                MeasureItemHeight(Canvas, Child);

                //Inc(HeightDelta, Child.TotalHeight);
                HeightDelta := HeightDelta + Child.TotalHeight;
              end;
              Child := Child.NextSibling;
            until Child = nil;

            // Getting the display rectangle is already done here as it is needed for toChildrenAbove in any case.
            if (toChildrenAbove in FOptions.FPaintOptions) or (FUpdateCount = 0) then
            begin
              with ToggleData do
              begin
                R1 := GetDisplayRect(Node, NoColumn, False);
                Mode2 := tamNoScroll;
                TotalFit := HeightDelta + Integer(NodeHeight[Node]) <= ClientHeight;

                if toChildrenAbove in FOptions.FPaintOptions then
                begin
                  // The main goal with toChildrenAbove being set is to keep the nodes visual position so the user does
                  // not get confused. Therefore we need to scroll the view when the expanding is done.
                  PosHoldable := TotalFit and (Integer(FRangeY) - ClientHeight >= 0) ;
                  ChildrenInView := (R1.Top - HeightDelta) >= 0;
                  NodeInView := R1.Bottom <= ClientHeight;
                end
                else
                begin
                  PosHoldable := TotalFit;
                  ChildrenInView := R1.Bottom + HeightDelta <= ClientHeight;
                end;

                R1.Bottom := ClientHeight;
              end;
            end;

            if FUpdateCount = 0 then
            begin
              // Do animated expanding if enabled.
              if (ToggleData.R1.Top < ClientHeight) and ([tsPainting, tsExpanding] * FStates = []) and
                (toAnimatedToggle in FOptions.FAnimationOptions)then
              begin
                if tsHint in Self.FStates then
                  Application.CancelHint;
                Realign; //вместо UpdateWindow(Handle);
                // animated expanding
                with ToggleData do
                begin
                  if toChildrenAbove in FOptions.FPaintOptions then
                  begin
                    // At first check if we hold the position, which is the most common case.
                    if not (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) or
                       (PosHoldable and ( (NodeInView and ChildrenInView) or not
                                          (toAutoScrollOnExpand in FOptions.FAutoOptions) )) then
                    begin
                      Mode1 := tamScrollUp;
                      R1 := d2Rect(R1.Left, 0, R1.Right, R1.Top);
                      StepsR1 := Min(HeightDelta, R1.Bottom);
                    end
                    else
                    begin
                      // If we will not hold the node's visual position we mostly scroll in both directions.
                      Mode1 := tamScrollDown;
                      Mode2 := tamScrollUp;
                      R2 := d2Rect(R1.Left, 0, R1.Right, R1.Top);
                      if not (toAutoScrollOnExpand in FOptions.FAutoOptions) then
                      begin
                        // If we shall not or cannot scroll to the desired extent we calculate the new position (with
                        // max FOffsetY applied) and animate it that way.
                        StepsR1 := -FOffsetY - Max(Integer(FRangeY) + HeightDelta - ClientHeight, 0) + HeightDelta;
                        if (Integer(FRangeY) + HeightDelta - ClientHeight) <= 0 then
                          Mode2 := tamNoScroll
                        else
                          StepsR2 := Min(Integer(FRangeY) + HeightDelta - ClientHeight, R2.Bottom);
                      end
                      else
                      begin
                        if TotalFit and NodeInView and (Integer(FRangeY) + HeightDelta > ClientHeight) then
                        begin
                          // If the whole subtree will fit into the client area and the node is currently fully visible,
                          // the first child will be made the top node if possible.
                          if HeightDelta >= R1.Top then
                            StepsR1 := Abs(R1.Top - HeightDelta)
                          else
                            StepsR1 := ClientHeight - Integer(FRangeY);
                        end
                        else
                          if Integer(FRangeY) + HeightDelta <= ClientHeight then
                          begin
                            // We cannot make the first child the top node as we cannot scroll to that extent,
                            // so we do a simple scroll down.
                            Mode2 := tamNoScroll;
                            StepsR1 := HeightDelta;
                          end
                          else
                            // If the subtree does not fit into the client area at once, the expanded node will
                            // be made the bottom node.
                            StepsR1 := ClientHeight - R1.Top - Integer(NodeHeight[Node]);

                        if Mode2 <> tamNoScroll then
                        begin
                          if StepsR1 > 0 then
                            StepsR2 := Min(R1.Top, HeightDelta - StepsR1)
                          else
                          begin
                            // If the node is already at the bottom scrolling is needed.
                            Mode1 := tamNoScroll;
                            StepsR2 := Min(HeightDelta, R1.Bottom);
                          end;
                        end;
                      end;
                    end;
                  end
                  else
                  begin
                    // toChildrenAbove is not set.
                    if (PosHoldable and ChildrenInView) or not (toAutoScrollOnExpand in FOptions.FAutoOptions) or not
                       (toAdvancedAnimatedToggle in FOptions.FAnimationOptions) or (R1.Top <= 0) then
                    begin
                      // If the node will stay at its visual position, do a simple down-scroll.
                      Mode1 := tamScrollDown;
                      //Inc(R1.Top, NodeHeight[Node]);
                      R1.Top := R1.Top + NodeHeight[Node];
                      StepsR1 := Min(R1.Bottom - R1.Top, HeightDelta);
                    end
                    else
                    begin
                      // We will not hold the nodes visual position so perform a double scroll.
                      Mode1 := tamScrollUp;
                      Mode2 := tamScrollDown;

                      R1.Bottom := R1.Top + Integer(NodeHeight[Node]) + 1;
                      R1.Top := 0;
                      R2 := d2Rect(R1.Left, R1.Bottom, R1.Right, ClientHeight);

                      StepsR1 := Min(HeightDelta - (ClientHeight - R2.Top), R1.Bottom - Integer(NodeHeight[Node]));
                      StepsR2 := ClientHeight - R2.Top;
                    end;
                  end;

                  if ClientHeight >= R1.Top then
                  begin
                    PrepareAnimation;
                    try
                      Animate(Steps, FAnimationDuration, ToggleCallback, @ToggleData);
                    finally
                      ReleaseDC(Window, DC);
                    end;
                  end;
                end;
              end;
              if toAutoSort in FOptions.FAutoOptions then
                Sort(Node, FHeader.FSortColumn, FHeader.FSortDirection, False);
            end;// if UpdateCount = 0

            Include(Node.States, vsExpanded);
            AdjustTotalHeight(Node, HeightDelta, True);
            if FullyVisible[Node] then
              Inc(FVisibleCount, CountVisibleChildren(Node));

            DoExpanded(Node);
          end;
        end;

      if NeedUpdate then
      begin
        InvalidateCache;
        if FUpdateCount = 0 then
        begin
          ValidateCache;
          if Node.ChildCount > 0 then
          begin
            UpdateRanges;
            UpdateScrollBars(True);
            if [tsPainting, tsExpanding] * FStates = [] then
            begin
              if (vsExpanded in Node.States) and ((toAutoScrollOnExpand in FOptions.FAutoOptions) or
                 (toChildrenAbove in FOptions.FPaintOptions)) then
              begin
                if toChildrenAbove in FOptions.FPaintOptions then
                begin
                  NeedFullInvalidate := True;
                  if (PosHoldable and ChildrenInView and NodeInView) or not
                     (toAutoScrollOnExpand in FOptions.FAutoOptions) then
                    SetOffsetY(FOffsetY - Integer(HeightDelta))
                  else
                    if TotalFit and NodeInView then
                    begin
                      FirstVisible := GetFirstVisible(Node, True);
                      if Assigned(FirstVisible) then // otherwise there is no visible child at all
                        SetOffsetY(FOffsetY - GetDisplayRect(FirstVisible, NoColumn, False).Top);
                    end
                    else
                      BottomNode := Node;
                end
                else
                begin
                  // Scroll as much child nodes into view as possible if the node has been expanded.
                  if PosHoldable then
                    NeedFullInvalidate := ScrollIntoView(GetLastVisible(Node, True), False)
                  else
                  begin
                    TopNode := Node;
                    NeedFullInvalidate := True;
                  end;
                end;
              end
              else
              begin
                // If we have collapsed the node or toAutoScrollOnExpand is not set, we try to keep the nodes
                // visual position.
                if toChildrenAbove in FOptions.FPaintOptions then
                  SetOffsetY(FOffsetY - Integer(HeightDelta));
                NeedFullInvalidate := True;
              end;
            end;

            //UpdateScrollBars(True); Moved up

            // Check for automatically scrolled tree.
            if NeedFullInvalidate then
              Realign //вместо Invalidate
            else
              InvalidateToBottom(Node);
          end
          else
            InvalidateNode(Node);
        end
        else
          UpdateRanges;
      end;

    finally
      Exclude(Node.States, vsToggling);
      if not TogglingTree then
        DoStateChange([], [tsToggling]);
    end;
  end;
end;

function TD2BaseVirtualTree.TreeFromNode(Node: TD2TreeNodeArray): TD2BaseVirtualTree;
// Возвращает дерево, которому принадлежит узел Node или ноль, если узел не привязан к дереву.
// Returns the tree the node currently belongs to or nil if the node is not attached to a tree.

begin
  Assert(Assigned(Node), 'Node must not be nil.');

  // The root node is marked by having its NextSibling (and PrevSibling) pointing to itself.
  while Assigned(Node) and (Node.NextSibling <> Node) do
    Node := Node.Parent;
  if Assigned(Node) then
    Result := TD2BaseVirtualTree(Node.Parent)
  else
    Result := nil;
end;

procedure TD2BaseVirtualTree.UpdateHorizontalRange;
//Обновить общую витруальную ширину дерева
begin

end;

procedure TD2BaseVirtualTree.UpdateHorizontalScrollBar(DoRepaint: Boolean);
begin

end;

procedure TD2BaseVirtualTree.UpdateRanges;
begin
  UpdateVerticalRange;
  UpdateHorizontalRange;
end;

procedure TD2BaseVirtualTree.UpdateScrollBars(DoRepaint: Boolean);
//Обновляет полосы прокрутки, чтобы отразить текущий размер и смещение дерева
// adjusts scrollbars to reflect current size and paint offset of the tree
begin
  UpdateVerticalScrollBar(DoRepaint);
  UpdateHorizontalScrollBar(DoRepaint);
end;


{ TD2VirtualTreeColumn }

procedure TD2VirtualTreeColumn.SetOptions(Value: TD2VTColumnOptions);
begin

end;

{ TD2VTHeader }

function TD2VTHeader.GetUseColumns: Boolean;
begin

end;

procedure TD2VTHeader.SetColumns(Value: TD2VirtualTreeColumns);
begin

end;

function TD2VTHeader.GetMainColumn: TD2ColumnIndex;
begin

end;

procedure TD2VTHeader.SetMainColumn(Value: TD2ColumnIndex);
begin

end;

function TD2VTHeader.AllowFocus(ColumnIndex: TD2ColumnIndex): Boolean;
begin

end;

{ TD2VirtualTreeColumns }

function TD2VirtualTreeColumns.GetItem(Index: TD2ColumnIndex
  ): TD2VirtualTreeColumn;
begin

end;

procedure TD2VirtualTreeColumns.SetItem(Index: TD2ColumnIndex;
  Value: TD2VirtualTreeColumn);
begin

end;

function TD2VirtualTreeColumns.IsValidColumn(Column: TD2ColumnIndex): Boolean;
begin

end;

function TD2VirtualTreeColumns.GetFirstVisibleColumn(ConsiderAllowFocus: Boolean
  ): TD2ColumnIndex;
begin

end;

