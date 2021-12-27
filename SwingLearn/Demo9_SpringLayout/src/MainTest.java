import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SpringLayout;
import javax.swing.WindowConstants;

/**
 * @Copyright xxx - xxx
 */
public class MainTest {

    public static void main(String[] args) {
        // 创建窗口
        JFrame jf = new JFrame("测试窗口");
        JFrame jf2 = new JFrame("cn.com.xxx - Context");
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        jf.setSize(300, 200);
        jf.setLocationRelativeTo(null);

        // 创建内容面板, 使用弹性布局
        SpringLayout layout = new SpringLayout();
        JPanel panel = new JPanel(layout);
        jf.setContentPane(panel);

        // 创建组件
        JLabel label = new JLabel("Test JLabel: ");
        JButton btn = new JButton("Btn");
        JTextField textField = new JTextField("Text Field");

        // 添加组件到内容面板
        panel.add(label);
        panel.add(btn);
        panel.add(textField);

        /**
         * 组件的约束设置(弹性布局设置的关键);
         */

        // 标签组件的约束:
    }
}
