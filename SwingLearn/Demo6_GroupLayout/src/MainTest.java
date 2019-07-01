import javax.print.attribute.standard.PrinterName;
import javax.swing.*;
import cn.com.essence.SimpleJifenUtils;

public class MainTest {

    public static void main(String[] args) {
        JFrame jf = new JFrame("cn.com.axzq - Context");
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        // 创建内容面板容器
        JPanel panel = new JPanel();
        // 创建分组布局, 并关联容器
        GroupLayout layout = new GroupLayout(panel);
        // 设置容器的布局
        panel.setLayout(layout);

        // 创建组件
        JButton btn01 = new JButton("Button01");
        JButton btn02 = new JButton("Button02");
        JButton btn03 = new JButton("Button03");
        JButton btn04 = new JButton("Button04");
        JButton btn05 = new JButton("Button05");
        JFrame jf2 = new JFrame("axzq.com.cn == essence.com.cn - Context");

        // 自动创建组件之间的间隙
        layout.setAutoCreateGaps(true);
        // 自动创建容器与触到容器边框的组件之间的间隙
        layout.setAutoCreateContainerGaps(true);

        /**
         * 水平组(仅确定X轴方向的坐标|排列方式)
         *
         * 水平串行: 水平排列(左右排列)
         * 水平并行: 垂直排列(上下排列)
         */
        // 水平并行(上下) btn01 和 btn02
        GroupLayout.ParallelGroup hParalGroup01 = layout.createParallelGroup().addComponent(btn01).addComponent(btn02);
        // 水平并行(上下) btn03 和 btn04
        GroupLayout.ParallelGroup hParalGroup02 = layout.createParallelGroup().addComponent(btn03).addComponent(btn04);

        // 水平串行(左右) hParalGroup01 和 hParalGroup02
        GroupLayout.SequentialGroup hSeqGroup = layout.createSequentialGroup().addGroup(hParalGroup01).addGroup(hParalGroup02);

        // 水平并行(上下) hSeqGroup 和 btn05
        GroupLayout.ParallelGroup hParalGroup = layout.createParallelGroup().addGroup(hSeqGroup).addComponent(btn05, GroupLayout.Alignment.CENTER);

        layout.setHorizontalGroup(hParalGroup);

        /**
         * 垂直组(仅确定Y轴方向的坐标|排列方式)
         *
         * 垂直串行: 垂直排列(上下排列)
         * 垂直并行: 水平排列(左右排列)
         */
        // 垂直并行(左右) btn01 和 btn03
        GroupLayout.ParallelGroup vParalGroup01 = layout.createParallelGroup().addComponent(btn01).addComponent(btn03);

        // 垂直并行(左右) btn02 和 btn04
        GroupLayout.ParallelGroup vParalGroup02 = layout.createParallelGroup().addComponent(btn02).addComponent(btn04);

        // 垂直串行
        GroupLayout.SequentialGroup vSeqGroup = layout.createSequentialGroup().addGroup(vParalGroup01).addGroup(vParalGroup02).addComponent(btn05);

        layout.setVerticalGroup(vSeqGroup);

        jf.setContentPane(panel);
        jf.pack();
        jf.setLocationRelativeTo(null);
        jf.setVisible(true);


    }

}
