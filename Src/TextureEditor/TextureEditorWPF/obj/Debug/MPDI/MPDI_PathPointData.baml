   M S B A M L   `   `   `  ����U  QGongSolutions.Wpf.DragDrop, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullkLclr-namespace:GongSolutions.Wpf.DragDrop;assembly=GongSolutions.Wpf.DragDropGongSolutions.Wpf.DragDrop  5      G CTimeLineTool, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullA0clr-namespace:TimeLineTool;assembly=TimeLineToolTimeLineTool 5       TextureEditorWPF)   #TextureEditorWPF.MPDI_PathPointData   5      R NWindowsBase, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      8x,http://schemas.microsoft.com/winfx/2006/xaml    5      _TimeLineLclr-namespace:GongSolutions.Wpf.DragDrop;assembly=GongSolutions.Wpf.DragDrop    5      =tt0clr-namespace:TimeLineTool;assembly=TimeLineTool    5        � 	Resources  5       �� R �� G �� B �� A   BgColor �� Color  BgBrush&   TimeLineTool.TimeLineItemControl �� 
TargetType �� Property �� Value	 �� 
TargetType
 J� Opacity ��Name �� DataType  TimeLineTool.TempDataType �� CornerRadius �� Path  UsedTemplateProperty �� Orientation��%*  &	        &	 _     7   '  {     (�� �    5/      *, +)&	 v    7  �� 5   
   $	 255��6   $	 255��5      $	 255��5	      $	 180��5
      5      �� 5   
   8   5      �� 5   
   "  6   .�5      ��  � �� 5      �� 5      "	  6&   .R�5      � .+�5      �� 
����  �$��Stretch=�5   %   $��Stretch=�5   %   $
 0Q�5   %   �� 5      $&  PART_ContentHostPART_ContentHost��5   /   $��Stretch=�5   /   $��Stretch=�5   /   5   /   �� 5      $ PART_LeftIndicator��5   /   $
��Left=�5   /   $��Stretch=�5   /   ��5   "   �� 5   &   5!      �� 5"      $ PART_RightIndicator��5$   /   $��Right=�5"   /   $��Stretch=�5#   /   ��5%   "   �� 5&   &   5)      5*      5+      5-      5.      �� 5/   
   "  6   .R�50      �� $��1q�6   
����  �$ 3��52      $��10��53      ._�54      �� ��6   �� $
 Name�55      56      �� 57   
   "  6   .R�59      �� $��1q�6   
��� � �$ 3��5;      $��0,0,0,0q�5<      $��80��5=      8��  ._�5?      �� $ Vertical=�6   .+�5@      �� ��6    �� $
 Name��� 5A      ��6    �� $ 	StartTime��� 5B      ��6    �� $ EndTime��� 5C      ��6    �� $ Duration�5D      5E      5F      5G      .��5H   
   �  � ColumnDefinitions 5I   
   �� 5J      �� 5K      �� 5L      .+�5N   
   � $��0��6    � RowDefinitions 5O      �� 5P      $	��250 �6    �� 5Q      �� 5R      .+�5T      �� -   5        $��m_PathNode_listBox��5T   #   $��0��6   6=   �� 5U      $��1��6   $ 
Horizontal=�6&   .+�5V      �� -   5        $��SelectAll/全選��5V      60   �� 5W      $��StartEnd/頭尾��6   5X      �� 5Y      $��2��6   $ 
Horizontal=�6&   .+�5Z      �� $��DelPoint/刪除��6   �� 5[      $��InsertPoint/插入��6   5\      5]      � 5^   
   $��1��6    5_      �� 5`      �� 5a      $��14 �6    �� 5b      $��20 �6    �� 5c      $��20 �6    .+�5e      �� -   5         ��Name$ FoundMe��5i   +   $��0��5e      $��Stretch=�6(   $
��Auto=�5f   +   $
��Auto=�5g   +   $��Stretch=�5h   +   $	��100��5i   <   .��5j      �� -   5        $��m_StackPanel��5j   4   $ Vertical=�6   .+�5r      �� -   5        $��m_ItemControl��5r   #   5�      5�      5�      �� 5�      -   5        $��m_ZoomSliderText��5�   9   $��Zoom Slider:��6   $��1��6,   69   �� 5�      -   5         ��Name$ Slider_Scale��5�   "   $��2��6    � Minimum$
 0.01Q�68    � Maximum$	 100Q�6G    � Value$ 1Q�6~   �� 5�      -   5        $��m_ViewLevelCombox��5�   $   $��3��6    �� SelectedIndex$ 0��6=   .@�5�      �� $��Seconds��6   �� 5�      $��Minutes��6   �� 5�      $��Hours��6   5�      5�      � 5�   
   $��2��6   .+�5�      �� .@�5�      �� $
��Move��6   5�      �� 5�      $
��Size��6   5�      �� 5�      $��Behavior��6   5�      5�      5�      5�      5�      