
# Use the official Nginx image from Docker Hub 
FROM nginx:alpine 
# Copy website files to the Nginx HTML directory 
COPY index.html /usr/share/nginx/html/ 
COPY styles.css /usr/share/nginx/html/ 
COPY script.js /usr/share/nginx/html/ 
# Expose port 80 for the web server 
EXPOSE 80 
# Start Nginx when the container launches 
CMD ["nginx", "-g", "daemon off;"]