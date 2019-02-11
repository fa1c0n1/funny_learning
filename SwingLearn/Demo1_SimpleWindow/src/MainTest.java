import javax.swing.*;

public class MainTest {

    public static void main(String[] args) {

        //1.创建一个顶层容器(窗口)
        JFrame jf = new JFrame("测试窗口");
        jf.setSize(250, 250);  //设置窗口大小
        jf.setLocationRelativeTo(null);     //把窗口位置设置到屏幕中心
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);  //当点击窗口的关闭按钮时退出

        //2.创建中间容器(面板容器)
        JPanel panel = new JPanel();

        //3.创建一个基本组件(按钮)，并添加到面板容器中
        JButton btn = new JButton("测试按钮");
        panel.add(btn);

        //4.把面板容器作为窗口的内容面板设置到窗口
        jf.setContentPane(panel);

        //5.显示窗口，前面创建的信息都在内存中，通过 jf.setVisible(true) 把内存中的窗口显示在屏幕上
        jf.setVisible(true);
    }
}
