package servlets;
import java.io.*;
import java.net.*;
import javax.servlet.*;
import javax.servlet.http.*;
public class FormProcessor extends HttpServlet
{
	public void doGet(HttpServletRequest rq,HttpServletResponse rs)
	{
		try
		{
			String name=rq.getParameter("nm");
			String sex=rq.getParameter("sex");
			String city=rq.getParameter("ct");
			System.out.println("Form process ");
			System.out.println("Name: "+name);
			System.out.println("sex: "+sex);
			System.out.println("city: "+city);
			rs.setContentType("text/html");
			PrintWriter pw=rs.getWriter();
			pw.println("<!Doctype html>");
			pw.println("<html lang='en'>");
			pw.println("<head>");
			pw.println("<meta cherset='utf-8'>");
			pw.println("<title>three.com</title>");
			pw.println("</head>");
			pw.println("<body>");
			pw.println("<center>");
			pw.println("<h1>Sesion Tracking Example</h1>");
			pw.println("<h3>Using URL Rewriting</h3>");
			pw.println("<a href='/three.com/saveData?nm="+URLEncoder.encode(name)+"&ct="+URLEncoder.encode(city)+"&sex="+URLEncoder.encode(sex)+"'>Save</a>");
			pw.println("</center>");
			pw.println("</body>");
			pw.println("</html>");
		}catch(Exception e)
		{
			System.out.println(e);

		}
	}
}