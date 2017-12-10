<%
     StringBuilder sb = new StringBuilder();
     int interval = 16;
   
     rs = select data from graph where id="1";
    
     sb.append("[['element','cnt'],");
    
//     if(rsCnt > 0)
//     {
         while(rs.next()) 
         {
            sb.append("['" + rs[0] + "'," + rs[1] + "],");
         }
//     }
      
     sb.append("]");
%>
<%=sb.toString()%>
