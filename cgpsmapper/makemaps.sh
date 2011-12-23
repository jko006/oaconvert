#!/bin/bash

## Check if a Polish file has been specified.
#if [ $# -ne 1 ]
#then
#  echo "Usage: `basename $0` path/to/some_polish_file.mp"
#  exit $E_BADARGS
#fi

# Remove old junk and temporary files
rm -f *.reg *.img *.TDB

ID=19780321
FID=122

for POLISH_FILE in ../build/polish_format/*.mp;
do

  MAP_FILENAME=$(basename ${POLISH_FILE})
  MAP_FILENAME=${MAP_FILENAME%%.*}

  ID=$((ID + 1))
  FID=$((FID + 1))

  # First, create an IMG file from the given Polish file.  With the 'ac' option
  # we make sure that the name of the file will be as defined by the ID key
  # in the header of the Polish file.  The -l option is just more elaborate
  # error info.
  # After this step, we get one extra file:
  #   <ID_from_Polish_file>.img
  sed -i "s/^ID=.*/ID=$ID/g" $POLISH_FILE
  cgpsmapper -l ac $POLISH_FILE


  # Secondly, use our preview control file (mypreview.mp) to generate all the
  # necessary files (.TDB, .REG, .IMG) so that we can load all the stuff as
  # a map in QLandkarteGT or MapSource.

  sed -e "s/^FileName=SomeFileName/FileName=${MAP_FILENAME}/g" \
      -e "s/MapsourceName=.*/MapsourceName=Bart's Fantastic Airspace Maps: ${MAP_FILENAME}/g" \
      -e "s/^IMG=.*/IMG=$ID.img/g" \
      -e "s/^FID=.*/FID=$FID/g" mypreview.mp > mypreview_temp.mp

  unamestr=`uname -o`
  if [[ "$unamestr" == 'Cygwin' ]]; then
  
    cpreview mypreview_temp.mp
    cgpsmapper $MAP_FILENAME.mp
  
  elif [[ "$unamestr" == 'Linux' || "$unamestr" == 'GNU/Linux' ]]; then
  
    # This command generates the extra files:
    #   <FileName_from_preview_file>.img
    #   <FileName_from_preview_file>.reg
    #   <FileName_from_preview_file>.TDB
    echo "Doing the preview thing..."
    cgpsmapper -l pv mypreview_temp.mp
    echo "done."
    rm -f mypreview_temp.mp
  
  fi


  # Create a custom TYP file
  # Note:
  #   We use $ID becausze filename cannot be longer than 8+3 characters.
  cgpsmapper typ Airspace.txt $MAP_FILENAME.TYP
  #cgpsmapper typ Airspace.txt $ID.TYP
  

  # Move everything to the garmin build directory.
  mkdir -p ../build/garmin/

  mv $ID.img   ../build/garmin/
  mv $MAP_FILENAME.img ../build/garmin/
  mv $MAP_FILENAME.reg ../build/garmin/
  mv $MAP_FILENAME.TDB ../build/garmin/$MAP_FILENAME.tdb
  #mv $ID.TYP ../build/garmin/$ID.typ
  mv $MAP_FILENAME.TYP ../build/garmin/$MAP_FILENAME.typ
  if [[ "$unamestr" == 'Cygwin' ]]; then
    mv $MAP_FILENAME.MDX ../build/garmin/$MAP_FILENAME.mdx
  fi
  rm -f $MAP_FILENAME.mp

  # Add necessary stuff to the NSIS installer script.
  # TODO

done
