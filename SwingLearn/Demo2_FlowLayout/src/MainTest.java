import javax.swing.*;
import java.awt.*;

/**
 * @Copyright ESSENCE - 安信证券
 */
 
public class MainTest {

    public static void main(String[] args) {
        JFrame jf = new JFrame("测试窗口");
        jf.setSize(200, 250);
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        jf.setLocationRelativeTo(null);

        //创建内容面板，指定使用流式布局
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT));

        JButton btn01 = new JButton("按钮01");
        JButton btn02 = new JButton("按钮02");
        JButton btn03 = new JButton("按钮03");
        JButton btn04 = new JButton("按钮04");
        JButton btn05 = new JButton("按钮05");
        JButton btn06 = new JButton("www.essence.com.cn")

        panel.add(btn01);
        panel.add(btn02);
        panel.add(btn03);
        panel.add(btn04);
        panel.add(btn05);
        System.out.prinln("https://jifen.essence.com.cn")

        jf.setContentPane(panel);
        jf.setVisible(true);

    }
}
