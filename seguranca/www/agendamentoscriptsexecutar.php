<?
function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}

$Tinicio = getmicrotime();

/* configura��es */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configura��es */

// carrega as fun��es gerais
include_once "../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

$tm = time();

if(!is_dir('./scripts_exec/')) {
	mkdir(APPRAIZ.'seguranca/www/scripts_exec/', 0777);
}

if(!is_dir('./scripts_exec/scripts_logs/')) {
	mkdir(APPRAIZ.'seguranca/www/scripts_exec/scripts_logs/', 0777);
}


$horainicio = date("d/m/Y H:i:s");

$sql = "SELECT * FROM seguranca.agendamentoscripts WHERE agsstatus='A'";
$agendamentos = $db->carregar($sql);

if($agendamentos[0]) {
	foreach($agendamentos as $agen) {
		switch($agen['agsperiodicidade']) {
			case 'diario':
				$diahor = explode(";",$agen['agsperdetalhes']);
				if(in_array(date("H"), $diahor)) {
					$_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
				}
				break;
			case 'semanal':
				if($agen['agsdataexec'] != date("Y-m-d")) {
					$diasem = explode(";",$agen['agsperdetalhes']);
					if(in_array(date("w"), $diasem)) {
						$_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
						$sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='".date("Y-m-d")."' WHERE agsid='".$agen['agsid']."';";
					}

				}
				break;
			case 'mensal':
				if($agen['agsdataexec'] != date("Y-m-d")) {
					$diamen = explode(";",$agen['agsperdetalhes']);
					if(in_array(date("d"), $diamen)) {
						$_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
						$sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='".date("Y-m-d")."' WHERE agsid='".$agen['agsid']."';";
					}
				}
				break;
		}

	}
}

$out = array();
if($_LISTAGEN) {
	foreach($_LISTAGEN as $agsid => $file) {
		if(is_file('./scripts_exec/'.$file)) {
			$resexec .= shell_exec("php ".APPRAIZ."seguranca/www/scripts_exec/".$file." &");
			$log .= "Script executado: php ".APPRAIZ."seguranca/www/scripts_exec/".$file." &\n";
		} else {
			$log .= "N�o foi encontrado o arquivo '".$file."'\n";
		}
	}
} else {
	$log .= "Nenhum agendamento encontrados\n";
}
if($sqls) {
	$db->executar(implode("",$sqls));
	$db->commit();
	$log .= "Atualiza��es efetuadas com sucesso\n";
} else {
	$log .= "Nenhuma atualiza��o efetuada\n";
}

die($resexec);

ob_start();
echo "<pre>";
print_r($resexec);
$dadosserv = ob_get_contents();
ob_end_clean();

// $sql ="insert into seguranca.logagendamento (lgadatahora, lgaconteudo) values (now(), 'Agendamento de scripts
// Envio da mensagem em ".(getmicrotime() - $Tinicio)." segundos :".date("d/m/Y H:i:s")."
// Hora da execu��o do script: ".$horainicio."
// --- LOG DA EXECU��O ---
// " . $log . "
// --- DADOS SERVI�OS ---
// " . $dadosserv . "
// ')";

// $db->executar($sql);
// $db->commit();