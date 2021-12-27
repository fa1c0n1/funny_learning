import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.WindowConstants;
import java.awt.BorderLayout;

/**
 * @Copyright xxx - xxx
 */
public class MainTest {

    public static void main(String[] args) {
        JFrame jf = new JFrame("测试窗口");
        jf.setSize(350, 250);
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        // 创建5个按钮
        JPanel panel = new JPanel(new BorderLayout());
        JButton btn01 = new JButton("Btn_North");
        JButton btn02 = new JButton("Btn_South");
        JButton btn03 = new JButton("Btn_Center");
        JButton btn04 = new JButton("Btn_East");
        JButton btn05 = new JButton("Btn_West");

        // 把5个按钮添加到容器中的5个方位
        panel.add(btn01, BorderLayout.EAST);
        panel.add(btn02, BorderLayout.WEST);
        panel.add(btn03, BorderLayout.CENTER);
        panel.add(btn04, BorderLayout.SOUTH);
        panel.add(btn05, BorderLayout.NORTH);

        jf.setContentPane(panel);
        jf.setLocationRelativeTo(null);
        jf.setVisible(true);
    }
}

