//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//        Remove salt & pepper noise with smooth technic
//
//------------------------------------------------------------------------------
//                  		     April 01, 2024
//                               Robotic Vision
//------------------------------------------------------------------------------

//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================

//----------------------------------------------------------------------------//
//         Prototype declarations                                             //
//----------------------------------------------------------------------------//

void heading (void);      /* Displays program heading */
void openfiles (void);    /* Opens input and output files in binary */
void userdefined (void);  /* Here, the user will implement his routine */
void readhdr (void);      /* Read header from input image file */
void addhdr (void);       /* Add header to output image file */
void closefiles (void);   /* Close all files used in program */

//----------------------------------------------------------------------------//
//         Include declarations                                               //
//----------------------------------------------------------------------------//

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>

//----------------------------------------------------------------------------//
//         Variable declarations                                              //
//----------------------------------------------------------------------------//

int    MRows, NCols;                 /* Number of Rows and columns in image */
FILE   *infptr, *outfptr, *outfptrh; /* Input and output file pointers */
char   infile[40];                   /* Name of input file */
char   outfile[40];                  /* Name of output file */

//----------------------------------------------------------------------------//
//         Main program                                                       //
//----------------------------------------------------------------------------//

main ()
{
     // Display heading
     heading ();
     // Open Input and Output Image Files
     openfiles ();
     // Read header from image file
     readhdr();
     // Add header to output file
     addhdr();
     // Execute userdefined procedure
     userdefined ();
     // Close all image files
     closefiles ();
     
     printf("\n Bye! Bye!\n");
     system("PAUSE");
     return(1);
} // end main ();

//----------------------------------------------------------------------------//
//         Headings                                                           //
//----------------------------------------------------------------------------//

void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                      Digital Image Processing in C         \n");
       printf("                         Date: April 01, 2024               \n");
       printf("                            Author: Kennia Sanchez          \n");
     for (i=0; i<3; i++)
       printf("                                    +                       \n");
       return;
} // end heading ()

//----------------------------------------------------------------------------//
//         Openfiles                                                          //
//----------------------------------------------------------------------------//

void openfiles ()
{
     printf("\n Opening Input and Output image files\n");
     printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ");
     scanf("%s",&infile);
     
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s",&outfile);
     
     //Check if output file was created succesfully
     if ((outfptr = fopen(outfile, "wb")) == NULL)
     {
       printf(" Cannot create output image file: %s\n",outfile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
       
     // If this point is reached, file are OK
     printf(" File opened and created OK! \n\n");
     
}  //end openfiles ()

//----------------------------------------------------------------------------//
//         Read Header                                                        //
//----------------------------------------------------------------------------//

void readhdr ()
{
     int   i=0, MaxRGB;
     char  c, buffer[128];
     
     //Read header of PGM file
     printf("\n\n File <%s> Header: \n",infile);
     
     do
     {
       c = fgetc(infptr);
       buffer[i] = c;
       i++; 
     } while (c != '\n');
     
     //Check if file is P5 (pgm) format
     if (buffer[1] == '5')
       printf("\n Input file is pgm, OK\n");
     else
     {
       printf("\n Input file is NOT pgm, Exiting program...\n");
       system("PAUSE");
       exit(0);
     }
     
     //File is pgm, read rest of header
     fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
     c = fgetc(infptr);
     printf("%d ",NCols);
     printf("%d   <---- Width x Height) \n",MRows);
     printf("%d   <---- Max. Grayscale level) \n\n",MaxRGB);
}  // end readhdr ()

//----------------------------------------------------------------------------//
//         Add Header                                                         //
//----------------------------------------------------------------------------//

void addhdr ()
{
     fprintf(outfptr, "P5\n%d %d\n%d\n",NCols,MRows,255);
} //addhdr ()

//----------------------------------------------------------------------------//
//         SMOOTH FILTER PROGRAM                                              //
//----------------------------------------------------------------------------//

void userdefined ()
{
	//Guardar la imagen en una matriz
	unsigned char Pixel;
	unsigned char mat[NCols][MRows];
	
	// Matriz para guardar la convolucion
	unsigned char conv[NCols][MRows];
	
    // Definir el filtro para suavizado
    int mask[5][5] = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    };

    // Factor de normalización
    int divideFactor = 25; // Suma de todos los elementos de la máscara

    // Variables para moverse en las matrices
    int col, row, value;
    
    // Leer la imagen de entrada a la matriz mat
    for (row = 0; row < MRows; row++) {
        for (col = 0; col < NCols; col++) {
            unsigned char Pixel = fgetc(infptr);
            mat[col][row] = Pixel;
        }
    }
    
    // Creo la imagen de salida 
	// Matriz la llenamos de ceros para generar el borde de ceros
	for (row=0; row< MRows; row++) {
		for (col=0; col<NCols; col++) {
			conv[col][row]= 0;
		}
	}

    // Operación de convolución
    for (row = 2; row < MRows - 2; row++) {
        for (col = 2; col < NCols - 2; col++) {
            // Inicializar el valor de salida de la convolución
            value = 0;

            // Aplicar la máscara de suavizado
            for (int i = -2; i <= 2; i++) {
                for (int j = -2; j <= 2; j++) {
                    value += mask[i + 2][j + 2] * mat[col + j][row + i];
                }
            }

            // Normalizar el valor de salida
            value /= divideFactor;

            // Establecer el valor de píxel en el rango (0-255)
            if 		(value < 0) 	value = 0;
            else if (value > 255) 	value = 255;

            // Guardar el valor de píxel en la matriz de convolución
            conv[col][row] = (unsigned char)value;
        }
    }

    // Guardar la imagen de salida
    for (row = 0; row < MRows; row++) {
        for (col = 0; col < NCols; col++) {
            unsigned char Pixel = conv[col][row];
            fputc(Pixel, outfptr);
        }
    }

	 
}  // end userdefined ()

//----------------------------------------------------------------------------//
//         Close Files                                                        //
//----------------------------------------------------------------------------//

void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // end closefiles ()
