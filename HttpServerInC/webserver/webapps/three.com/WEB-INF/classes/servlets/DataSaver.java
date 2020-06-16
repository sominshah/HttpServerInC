package servlets;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
public class DataSaver extends HttpServlet
{
	public void doGet(HttpServletRequest rq,HttpServletResponse rs)
	{
		try
		{
			String name=rq.getParameter("nm");
			String sex=rq.getParameter("sex");
			String city=rq.getParameter("ct");
			System.out.println("Data Saver ");
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
			pw.println("<b>Name:"+name+"</b>");
			pw.println("<b>Sex:"+sex+"</b>");
			pw.println("<b>City:"+city+"</b>");
			pw.println("<h1>Data Saved</h1>");
			pw.println("</center>");

			pw.println("</body>");
			pw.println("</html>");
		}catch(Exception e)
		{
			System.out.println(e);

		}
	}
}