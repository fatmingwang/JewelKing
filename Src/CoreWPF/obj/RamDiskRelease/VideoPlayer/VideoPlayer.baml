   M S B A M L   `   `   `  ���� B  >CoreWPF, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null/clr-namespace:CoreWPF.CommonCoreWPF.Common  %    CoreWPF.VideoPlayer.VideoPlayer   #myclr-namespace:CoreWPF.Common  ^ ZWindowsFormsIntegration, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     8x,http://schemas.microsoft.com/winfx/2006/xaml    Hmc;http://schemas.openxmlformats.org/markup-compatibility/2006    >d2http://schemas.microsoft.com/expression/blend/2008      � 	Resources   �� MergedDictionaries �� Source��  �� $ ../Resources/Styles.xaml?��� $! ../Resources/Templates.xaml?�%    .���� .+��� -   
�������
�������$��Tahoma)�$��10(�
������ $��Drag Media Items Here...�� J� 	AllowDrop . �� -   $��m_FilmNameList����  �� Orientation$ 
Horizontal=�.+��� $��StartTime[H/M/S]��"   CoreWPF.Common.NumericUpDown  -     Name$ m_StartH��   Maximum$ 23��   Minimum$ 0��  -   $ m_StartM��$ 59��$ 0��  -   $ m_StartS��$ 59��$ 0���� $ 
Horizontal=�.+��� $��EndTime [H/M/S]��  -   $ m_EndH��$ 23��$ 0��  -   $ m_EndM��$ 59��$ 0��  -   $ m_EndS��$ 59��$ 0���� $ 
Horizontal=�.+��� -	    ��Name$ lblSave��	 J� RenderTransformOrigin$	 0.5,0.5&�
������ 
�������$��	Wingdings)�$��14(�
������ $��=��
 � ToolTip$
 OpenFile��   
PulseLabel#���   �� -
   $ lblLoad��$	 0.5,0.5&�
������ 
�������$��	Wingdings)�$��14(�
������ $��6��$

 Load��#���   �� -   $ lblClear��$	 0.5,0.5&�
������ 
�������$��	Wingdings)�$��14(�
������ $��û��$
 Clear��#���   �� -   $ lbl3D��$	 0.5,0.5&�
������ 
�������$��	Wingdings)�$��14(�
������ $��r��$
 Use 3D Style Media Items��#���   �� -   $ 	lblNormal��$	 0.5,0.5&�
������ 
�������$��	Wingdings)�$��14(�
������ $��o��$ 
 Use Flat Style Media Items��#���   ��  �� CornerRadius$ 5,5,5,5��
�������
�������$��4,4,4,4q�$��	10,5,5,10q�._��� 
������ $
��Auto��$��0,0,0,0q�$ 
Horizontal=�$
��Auto��.+��� -    ��Name$ btnPlay��$��50��$��50��  controlButtonTemplate#��[ $��14(�$
��Bold"�
������$��4��$��Webdings)�$

 Play���� -   $ btnPause��$��40��$��40��#��[ $��14(�$
��Bold"�
��� ���$��;��$��Webdings)�$
 Pause���� -   $ btnStop��$��30��$��30��#��[ $��14(�$
��Bold"�
��� ���$��<��$��Webdings)�$

 Stop���� $ Vertical=�.+��� $�� ���� -   $��m_SliderTime��$��2,0,0,0q�$��Center=�$	��180��$
��Auto���� -   $��
m_TimeText��$
��time��