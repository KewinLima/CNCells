/*
// Kewin Lima
// 22/11/18
// Envia dados da CNCells para uma planilha google
// https://script.google.com/macros/s/<gscript id>/exec?header_here=data_here
// Baseado no código de Trieu Le - lethanhtrieuk36@gmail.com
*/
 
function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // Visualiza parametros
  
  var result = 'Ok'; // Sucesso
  if (e.parameter == 'undefined') {
    result = 'Sem Parâmetros';
  }
  else {
    var sheet_id = '1bd3bPGQDTVh2tEp2vCycWxPCS0Bpg4VBgT_ochmP3YM';      // Insira o ID da sua planilha
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();     // obtêm a planilha ativa através do ID
    var newRow = sheet.getLastRow() + 1;                        
    var rowData = [];
    rowData[0] = new Date();                                            // TimeStamp na coluna A
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      
      // -- Condição para inserir parâmetros na planilha
      
      switch (param) {
        case 'X': //Parametro deverá ser o título da coluna
          rowData[1] = value; //Inclui valor na coluna B
          result = 'Dado inserido na coluna B';
          break;
        case 'Y': //Parameter
          rowData[2] = value; //Value in column C
          result += ' ,Dado inserido na coluna C';
          break;  
        default:
          result = "Parâmetro inexistente/não encontrado!";
      }
    }
    Logger.log(JSON.stringify(rowData));
    
    // Escreve nova linha na planilha
    
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  // Return o resultado da operação
  return ContentService.createTextOutput(result);
}
// Remove aspas simples e duplas
 
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
